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


#ifndef _HeatMapNode_H_
#define _HeatMapNode_H_

#include <api.h>

#include <base/GLMHelper.h>
#include <graphics/Color.h>
#include <player/RasterNode.h>

#include <boost/python/stl_iterator.hpp>
#include <boost/shared_ptr.hpp>

namespace avg {
    class MCTexture;
    typedef boost::shared_ptr<MCTexture> MCTexturePtr;
}

class HeatMapNode: public avg::RasterNode
{
    public:
                                  // libavg plugin methods
                                  static void  registerType();
                                               HeatMapNode(const avg::ArgList& args, const std::string& sPublisherName="Node");
                                      virtual  ~HeatMapNode();
                                 virtual void  connectDisplay();
                                 virtual void  connect(avg::CanvasPtr);
                                 virtual void  disconnect(bool bKill);
                                 virtual void  preRender(const avg::VertexArrayPtr&, bool, float);
                                 virtual void  render(avg::GLContext* pContext, const glm::mat4& transform);

                                         // node attribute methods
                                         void  setColorMap(const std::vector<std::string>& colormap);
              const std::vector<std::string>&  getColorMap() const;
                                         void  setOpacityMap(const std::vector<float>& opacitymap);
                    const std::vector<float>&  getOpacityMap() const;
                                         void  setValueRangeMin(float min);
                                        float  getValueRangeMin() const;
                                         void  setValueRangeMax(float max);
                                        float  getValueRangeMax() const;

                                         // node external methods
                                         void  setPosns(const std::vector<glm::vec2>& posns);
                                         void  setMatrix(const std::vector<std::vector<float> >& matrix);
    private:
                     // basics
                     avg::MCTexturePtr  m_pTex;
                                  bool  m_ShouldPrerender;

                             // member varibales
                             glm::vec2  m_ViewportRangeMin;
                             glm::vec2  m_ViewportRangeMax;
                             glm::vec2  m_MapSize;
                                 float  m_ValueRangeMin;
                                 float  m_ValueRangeMax;
              std::vector<std::string>  m_ColorStrs;
               std::vector<avg::Color>  m_Colors;
                    std::vector<float>  m_Opacities;
      std::vector<std::vector<float> >  m_Matrix;

                                  void  createColorMap();
                                  void  setupRender();
};

typedef boost::shared_ptr<HeatMapNode> HeatMapNodePtr;

// struct to be able to use vector<vector<T>> with python list of lists of T: [[T],[T]]
// taken from: http://stackoverflow.com/questions/15842126/feeding-a-python-list-into-a-function-taking-in-a-vector-with-boost-python
struct iterable_converter
{
  /// @note Registers converter from a python interable type to the
  ///       provided type.
  template <typename Container>
  iterable_converter&
  from_python()
  {
    boost::python::converter::registry::push_back(
      &iterable_converter::convertible,
      &iterable_converter::construct<Container>,
      boost::python::type_id<Container>());

    // Support chaining.
    return *this;
  }

  /// @brief Check if PyObject is iterable.
  static void* convertible(PyObject* object)
  {
    return PyObject_GetIter(object) ? object : NULL;
  }

  /// @brief Convert iterable PyObject to C++ container type.
  ///
  /// Container Concept requirements:
  ///
  ///   * Container::value_type is CopyConstructable.
  ///   * Container can be constructed and populated with two iterators.
  ///     I.e. Container(begin, end)
  template <typename Container>
  static void construct(
    PyObject* object,
    boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    namespace python = boost::python;
    // Object is a borrowed reference, so create a handle indicting it is
    // borrowed for proper reference counting.
    python::handle<> handle(python::borrowed(object));

    // Obtain a handle to the memory block that the converter has allocated
    // for the C++ type.
    typedef python::converter::rvalue_from_python_storage<Container>
                                                                storage_type;
    void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

    typedef python::stl_input_iterator<typename Container::value_type>
                                                                    iterator;

    // Allocate the C++ type into the converter's memory block, and assign
    // its handle to the converter's convertible variable.  The C++
    // container is populated by passing the begin and end iterators of
    // the python object to the container's constructor.
    new (storage) Container(
      iterator(python::object(handle)), // begin
      iterator());                      // end
    data->convertible = storage;
  }
};

#endif
