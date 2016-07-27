#ifndef _heatmap_wrapper_H_
#define _heatmap_wrapper_H_

#include "HeatMapNode.h"

#include <wrapper/raw_constructor.hpp>
#include <wrapper/WrapHelper.h>

#include <iostream>

using namespace boost::python;
using namespace std;
using namespace avg;

char heatMapNodeName[] = "heatmapnode";

BOOST_PYTHON_MODULE(heatmap)
{
  class_<HeatMapNode, bases<avg::RasterNode>, boost::noncopyable>("HeatMapNode", no_init)
    .def("__init__", raw_constructor(createNode<heatMapNodeName>))
    .add_property("info", make_function(&HeatMapNode::getInfo,
        return_value_policy<copy_const_reference>()), &HeatMapNode::setInfo);
}

AVG_PLUGIN_API PyObject* registerPlugin()
{
  HeatMapNode::registerType();
  initheatmap();
  PyObject* pyHeatMapModule = PyImport_ImportModule("heatmap");
  return pyHeatMapModule;
// #if PY_MAJOR_VERSION < 3
//   initheatmap();
//   PyObject* pyHeatMapModule = PyImport_ImportModule("heatmap");
// #else
//   PyObject* pyHeatMapModule = PyInit_heatmap();
// #endif
}

#endif
