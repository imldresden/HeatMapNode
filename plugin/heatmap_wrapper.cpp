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

  // Register interable conversions.
  iterable_converter()
    .from_python<std::vector<std::vector<float> > >()
    ;

  class_<HeatMapNode, bases<avg::RasterNode>, boost::noncopyable>("HeatMapNode", no_init)
    .def("__init__", raw_constructor(createNode<heatMapNodeName>))
    .add_property("viewportrangemin", make_function(&HeatMapNode::getViewportRangeMin, return_value_policy<copy_const_reference>()), &HeatMapNode::setViewportRangeMin)
    .add_property("viewportrangemax", make_function(&HeatMapNode::getViewportRangeMax, return_value_policy<copy_const_reference>()), &HeatMapNode::setViewportRangeMax)
    .add_property("mapsize", make_function(&HeatMapNode::getMapSize, return_value_policy<copy_const_reference>()), &HeatMapNode::setMapSize)
    .add_property("valuerangemin", make_function(&HeatMapNode::getValueRangeMin, return_value_policy<copy_const_reference>()), &HeatMapNode::setValueRangeMin)
    .add_property("valuerangemax", make_function(&HeatMapNode::getValueRangeMax, return_value_policy<copy_const_reference>()), &HeatMapNode::setValueRangeMax)
    .add_property("colormap", make_function(&HeatMapNode::getColorMap, return_value_policy<copy_const_reference>()), &HeatMapNode::setColorMap)
    .def("setPosns", &HeatMapNode::setPosns)
    .def("setMatrix", &HeatMapNode::setMatrix)
    ;
}

AVG_PLUGIN_API PyObject* registerPlugin()
{
  HeatMapNode::registerType();

#if PY_MAJOR_VERSION < 3
  initheatmap();
  PyObject* pyHeatMapModule = PyImport_ImportModule("heatmap");
#else
  PyObject* pyHeatMapModule = PyInit_heatmap();
#endif

  return pyHeatMapModule;
}

#endif
