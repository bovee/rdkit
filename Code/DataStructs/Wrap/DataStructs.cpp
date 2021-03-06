// $Id$
//
// Copyright (c) 2001-2006 greg Landrum and Rational Discovery LLC
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#define PY_ARRAY_UNIQUE_SYMBOL rddatastructs_array_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <boost/python.hpp>
#include <RDBoost/Wrap.h>
#include <DataStructs/BitVects.h>
#include <DataStructs/DiscreteValueVect.h>
#include "DataStructs.h"
#include <boost/python/numeric.hpp>
#include <numpy/npy_common.h>
#include <numpy/arrayobject.h>


namespace python = boost::python;

void wrap_SBV();
void wrap_EBV();
void wrap_BitOps();
void wrap_Utils();
void wrap_discreteValVect();
void wrap_sparseIntVect();

template <typename T>
void convertToNumpyArray(const T &v,python::object destArray){
  if (!PyArray_Check(destArray.ptr())) {
    throw_value_error("Expecting a Numeric array object");
  }
  PyArrayObject *destP=(PyArrayObject *)destArray.ptr();
  npy_intp ndims[1];
  ndims[0]=v.size();
  PyArray_Dims dims;
  dims.ptr=ndims;
  dims.len=1;
  PyArray_Resize(destP,&dims,0,NPY_ANYORDER);
  for(unsigned int i=0;i<v.size();++i){
    #if PY_MAJOR_VERSION >= 3
      PyObject *iItem = PyLong_FromLong(v[i]);
    #else
      PyObject *iItem = PyInt_FromLong(v[i]);
    #endif
    PyArray_SETITEM(destP,PyArray_GETPTR1(destP,i),iItem);
  }
}



BOOST_PYTHON_MODULE(cDataStructs)
{
  import_array();
  python::scope().attr("__doc__") =
    "Module containing an assortment of functionality for basic data structures.\n"
    "\n"
    "At the moment the data structures defined are:\n"
    "  Bit Vector classes (for storing signatures, fingerprints and the like:\n"
    "    - ExplicitBitVect: class for relatively small (10s of thousands of bits) or\n"
    "                       dense bit vectors.\n"
    "    - SparseBitVect:   class for large, sparse bit vectors\n"
    "  DiscreteValueVect:   class for storing vectors of integers\n"
    "  SparseIntVect:       class for storing sparse vectors of integers\n"
    ;
  
  python::register_exception_translator<IndexErrorException>(&translate_index_error);
  python::register_exception_translator<ValueErrorException>(&translate_value_error);

  wrap_Utils();
  wrap_SBV();
  wrap_EBV();
  wrap_BitOps();
  wrap_discreteValVect();
  wrap_sparseIntVect();

  python::def("ConvertToNumpyArray", (void (*)(const ExplicitBitVect &,python::object))convertToNumpyArray,
              (python::arg("bv"),python::arg("destArray")));
  python::def("ConvertToNumpyArray", (void (*)(const RDKit::DiscreteValueVect &,python::object))convertToNumpyArray,
              (python::arg("bv"),python::arg("destArray")));

}
