#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "shader_test.h"

namespace py = pybind11;

// // Declarations of other module generator functions
// void moduleColor(py::module &);
// void moduleConfig(py::module &);
// void moduleQuery(py::module &);
// void moduleGeometry(py::module &);
// void moduleLog(py::module &);
// void moduleRadio(py::module &);
// void moduleMovement(py::module &);


// Bindings ====================================================================
/**
 * @brief Definition of the submodule generator function
 * 
 * @param parent_m 
 */
void moduleTest(py::module &parent_m) {
	// Define the submodule
	auto m = parent_m.def_submodule("test");

	// Bindings ----------------------------------------------------------------
	// shaderTest
	m.def("shaderTest", &shaderTest);
}

/**
 * @brief The main python module that collects all submodules
 * 
 */
PYBIND11_MODULE(Bindings, m) {
	moduleTest(m);
}
