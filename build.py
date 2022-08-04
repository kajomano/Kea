import os
import subprocess as sp

os.makedirs('./build', exist_ok = True)

sp.run([
    'cmake',
    '-S', './src',
    '-B', './build'
])

sp.run([
    'cmake',
    '--build', './build'
])