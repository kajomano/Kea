import os
import subprocess as sp
from pathlib import Path

os.makedirs('./build', exist_ok = True)

cmd_config = [
    'cmake',
    '-S', './src',
    '-B', './build'
]
# If on windows
if (os.name == 'nt'):
    vcpkg_path = Path.cwd().parent / 'vcpkg/scripts/buildsystems/vcpkg.cmake'
    cmd_config += [f'-DCMAKE_TOOLCHAIN_FILE={vcpkg_path.absolute()}']

# Configure
sp.run(cmd_config)

# Build
sp.run([
    'cmake',
    '--build', './build'
])