import os
import subprocess
from pathlib import Path
import argparse
import shutil

# Parse arguments
parser = argparse.ArgumentParser(description = 'Build the Kea raytracer.')
parser.add_argument(
    '--clean',
    action  = 'store_const',
    const   = True, 
    default = False,
    help    = 'clean build'
)
parser.add_argument(
    '--release',
    action  = 'store_const',
    const   = True, 
    default = False,
    help    = 'build in release mode'
)
args = parser.parse_args()

# Project root
root = Path.cwd().resolve()

# If clean build
if args.clean:
    shutil.rmtree(root / 'build', ignore_errors = True)

# Create build directory
os.makedirs(root / 'build', exist_ok = True)

# Assemble the config and build commands
cmd_config = [
    'cmake',
    '-S', str(root / 'src'),
    '-B', str(root / 'build')
]
cmd_build = [
    'cmake',
    '--build', str(root / 'build')
]

# TODO: untested!
# If on linux
if (os.name == 'posix'):
    if args.release:
        cmd_config += ['-DCMAKE_BUILD_TYPE=Release']

# If on windows
elif (os.name == 'nt'):
    vcpkg_path = root.parent / 'vcpkg' / 'scripts' / 'buildsystems' / 'vcpkg.cmake'
    cmd_config += [f'-DCMAKE_TOOLCHAIN_FILE={vcpkg_path.absolute()}']

    if args.release:
        cmd_build += ['--config', 'Release']

else:
    raise Exception('Unknown operating system!')

# Configure and build
subprocess.run(cmd_config)
subprocess.run(cmd_build)

# # TODO: something is not great here, don't know which file to move
# # NOTE: for some reason the install target just wouldn't run in CMAKE, so i do
# # this manually with python
# # Move binaries to the correct places
# shutil.move(
#     root / 'build' / 'bindings' / 'Debug' / "bindings.cp311-win_amd64.pyd",
#     root / 'kea'
# )