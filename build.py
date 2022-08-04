import os
import subprocess as sp

os.makedirs('./build', exist_ok = True)

cmd_config = [
    'cmake',
    '-S', './src',
    '-B', './build'
]
# If on windows
if (os.name == 'nt'): 
    cmd_config += ['-DCMAKE_TOOLCHAIN_FILE=C:/Users/mkajo/Home/Raytracing/vcpkg/scripts/buildsystems/vcpkg.cmake']

# Configure
sp.run(cmd_config)

# Build
sp.run([
    'cmake',
    '--build', './build'
])