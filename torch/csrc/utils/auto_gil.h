#pragma once

// RAII structs to acquire and release Python's global interpreter lock (GIL)

#include <c10/util/Deprecated.h>
#include <torch/csrc/python_headers.h>

// Acquires the GIL on construction
struct C10_DEPRECATED_MESSAGE(
    "Use pybind11::gil_scoped_acquire instead") AutoGIL {
  AutoGIL() : gstate(PyGILState_Ensure()) {}
  ~AutoGIL() {
    PyGILState_Release(gstate);
  }

  PyGILState_STATE gstate;
};

// Releases the GIL on construction
struct C10_DEPRECATED_MESSAGE(
    "Use pybind11::gil_scoped_release instead") AutoNoGIL {
  AutoNoGIL() : save(PyEval_SaveThread()) {}
  ~AutoNoGIL() {
    PyEval_RestoreThread(save);
  }

  PyThreadState* save;
};

// Runs the function without the GIL
template <typename F>
C10_DEPRECATED inline void with_no_gil(F f) {
  AutoNoGIL no_gil;
  f();
}
