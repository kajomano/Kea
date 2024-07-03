# stubs:
# https://stackoverflow.com/questions/73879484/vscode-not-autocompleting-python-from-module-made-with-pybind11
	# stubgen:
		# PYTHONPATH=./kea/ stubgen -o ./kea/ -m bindings.test
		# where xxx is the submodule name

import kea
import kea.test

kea.test.shaderTest()