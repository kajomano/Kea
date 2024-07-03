import os
import subprocess
from pathlib import Path
import argparse
import shutil

# Parse arguments
parser = argparse.ArgumentParser(description = 'Build the Kea raytracer.')
parser.add_argument(
    '-c', '--clean',
    action  = 'store_true',
    help    = 'clean build'
)
parser.add_argument(
    '-r', '--release',
    action  = 'store_true',
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
    '--build', str(root / 'build'),
    '--target', 'install'
]

# Release switch
if args.release:
	cmd_build += ['--config', 'Release']

# Configure and build
subprocess.run(cmd_config)
subprocess.run(cmd_build)
