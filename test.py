# TODO: stubs
# https://stackoverflow.com/questions/73879484/vscode-not-autocompleting-python-from-module-made-with-pybind11
	# stubgen:
		# PYTHONPATH=./kea/ stubgen -o ./kea/ -m bindings.test
		# where xxx is the submodule name

# # NOTE: need to add the built shader's folder to path, so that the read in the code finds it
# import sys
# from pathlib import Path
# sys.path.append(str(Path.cwd().resolve() / "build" / "shaders"))

# print(sys.path)

import kea
import kea.test

kea.test.shaderTest()