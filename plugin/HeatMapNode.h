
#ifndef _HeatMapNode_H_
#define _HeatMapNode_H_

#include <api.h>

#include <base/GLMHelper.h>
#include <base/UTF8String.h>
#include <base/GLMHelper.h>
#include <player/RasterNode.h>

#include <boost/shared_ptr.hpp>
#include <string>

namespace avg {
    class MCTexture;
    typedef boost::shared_ptr<MCTexture> MCTexturePtr;
}

class HeatMapNode: public avg::RasterNode
{
  public:
             static void  registerType();
                          HeatMapNode(const avg::ArgList& args, const std::string& sPublisherName="Node");
                 virtual  ~HeatMapNode();
            virtual void   connectDisplay();
            virtual void   connect(avg::CanvasPtr pCanvas);
            virtual void   disconnect(bool bKill);
            virtual void   preRender(const avg::VertexArrayPtr& pVA, bool bIsParentActive, float   parentEffectiveOpacity);
            virtual void  render(avg::GLContext* pContext, const glm::mat4& transform);

                    void  setInfo(const avg::UTF8String& info);
  const avg::UTF8String&  getInfo() const;

  private:
         avg::UTF8String  m_Info;
       avg::MCTexturePtr  m_pTex;
                    bool  m_bNewSurface;
              //       viewportrange:
              //    glm::vec2 m_ViewportMin
              //    glm::vec2 m_ViewportMax
              //  mapsize
              //    glm::vec2 m_MapSize
              //  valuerange
              //    m_ValueMin
              //    m_ValueMax
              //  colormap
              //    vector<avg::Pixel32> m_ColorMap
};

typedef boost::shared_ptr<HeatMapNode> HeatMapNodePtr;

#endif
