// HeatMapNode
// Copyright (C) 2017 Interactive Media Lab Dresden
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

  // Register interable conversions for vector of vector of float.
  iterable_converter()
      .from_python<std::vector<std::vector<float> > >()
      ;

  class_<HeatMapNode, bases<avg::RasterNode>, boost::noncopyable>("HeatMapNode", no_init)
      .def("__init__", raw_constructor(createNode<heatMapNodeName>))
      .add_property(
              "colormap",
              make_function(&HeatMapNode::getColorMap, return_value_policy<copy_const_reference>()),
              &HeatMapNode::setColorMap
              )
      .add_property(
              "opacitymap",
              make_function(&HeatMapNode::getOpacityMap, return_value_policy<copy_const_reference>()),
              &HeatMapNode::setOpacityMap
              )
      .add_property(
              "valuerangemin",
              &HeatMapNode::getValueRangeMin,
              &HeatMapNode::setValueRangeMin
              )
      .add_property(
              "valuerangemax",
              &HeatMapNode::getValueRangeMax,
              &HeatMapNode::setValueRangeMax
              )
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
