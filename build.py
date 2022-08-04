import os
import subprocess as sp
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

# If clean build
if args.clean:
    shutil.rmtree('./build', ignore_errors = True)

# Create build directory
os.makedirs('./build', exist_ok = True)

# Assemble the config and build commands
cmd_config = [
    'cmake',
    '-S', './src',
    '-B', './build'
]
cmd_build = [
    'cmake',
    '--build', './build'
]

# TODO: untested!
# If on linux
if (os.name == 'posix'):
    if args.release:
        cmd_config += ['-DCMAKE_BUILD_TYPE=Release']

# If on windows
elif (os.name == 'nt'):
    vcpkg_path = Path.cwd().parent / 'vcpkg/scripts/buildsystems/vcpkg.cmake'
    cmd_config += [f'-DCMAKE_TOOLCHAIN_FILE={vcpkg_path.absolute()}']

    if args.release:
        cmd_build += ['--config', 'Release']

else:
    raise Exception('Unknown operating system!')

# Configure and build
sp.run(cmd_config)
sp.run(cmd_build)