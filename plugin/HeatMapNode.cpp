#include "HeatMapNode.h"

#include <math.h>

#include <base/Exception.h>
#include <base/StringHelper.h>
#include <base/ScopeTimer.h>
#include <base/TimeSource.h>
#include <base/ThreadHelper.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/StandardShader.h>
#include <graphics/MCTexture.h>
#include <graphics/VertexArray.h>
#include <graphics/GLContextManager.h>
#include <graphics/Filterfill.h>
#include <graphics/Bitmap.h>
#include <graphics/FilterResizeBilinear.h>
#include <player/Player.h>
#include <player/Arg.h>
#include <player/TypeDefinition.h>
#include <player/TypeRegistry.h>
#include <player/OGLSurface.h>

#include <iostream>
#include <string>

using namespace boost;
using namespace std;
using namespace avg;

void HeatMapNode::registerType()
{
    vector<string> cm;
    TypeDefinition def = TypeDefinition("heatmapnode", "rasternode",  ExportedObject::buildObject<HeatMapNode>)
        .addArg( Arg<glm::vec2>("viewportrangemin", glm::vec2(0,0), false, offsetof(HeatMapNode, m_ViewportRangeMin)) )
        .addArg( Arg<glm::vec2>("viewportrangemax", glm::vec2(0,0), false, offsetof(HeatMapNode, m_ViewportRangeMax)) )
        .addArg( Arg<glm::vec2>("mapsize", glm::vec2(0,0), false, offsetof(HeatMapNode, m_MapSize)) )
        .addArg( Arg<float >("valuerangemin", 0.0, false, offsetof(HeatMapNode, m_ValueRangeMin)) )
        .addArg( Arg<float >("valuerangemax", 0.0, false, offsetof(HeatMapNode, m_ValueRangeMax)) )
        .addArg( Arg<vector<string> >("colormap", cm, false, offsetof(HeatMapNode, m_ColorMap)) )
        ;

    const char* allowedParentNodeNames[] = {"div", "canvas", "avg", 0};
    TypeRegistry::get()->registerType(def, allowedParentNodeNames);
}

HeatMapNode::HeatMapNode(const ArgList& args, const string& sPublisherName) : RasterNode(sPublisherName)
{
    args.setMembers(this);
    setSize(args.getArgVal<glm::vec2>("size"));
}

HeatMapNode::~HeatMapNode()
{
    cout << "HeatMapNode destruct" << endl;
}

void HeatMapNode::connect(CanvasPtr pCanvas)
{
    RasterNode::connect(pCanvas);
}

void HeatMapNode::connectDisplay()
{
    m_pTex = GLContextManager::get()->createTexture(m_Size, R8G8B8A8, getMipmap());
    getSurface()->create(R8G8B8A8, m_pTex);
    setupFX();
    RasterNode::connectDisplay();
}

void HeatMapNode::disconnect(bool bKill)
{
    RasterNode::disconnect(bKill);
}

static ProfilingZoneID PrerenderProfilingZone("HeatMapNode::prerender");
void HeatMapNode::preRender(const VertexArrayPtr& pVA, bool bIsParentActive, float parentEffectiveOpacity)
{
    ScopeTimer timer(PrerenderProfilingZone);
    AreaNode::preRender(pVA, bIsParentActive, parentEffectiveOpacity);

    BitmapPtr pBmp(new Bitmap(m_Size, R8G8B8A8));
    GLContextManager::get()->scheduleTexUpload(m_pTex, pBmp);
    scheduleFXRender();

    calcVertexArray(pVA);
}

static ProfilingZoneID RenderProfilingZone("HeatMapNode::render");
void HeatMapNode::render(GLContext* pContext, const glm::mat4& transform)
{
    ScopeTimer Timer(RenderProfilingZone);
    blt32(pContext, transform);
}

void HeatMapNode::setSize(const glm::vec2& pt)
{
    m_Size = pt;
}

const glm::vec2& HeatMapNode::getViewportRangeMin() const
{
    return m_ViewportRangeMin;
}

const glm::vec2& HeatMapNode::getViewportRangeMax() const
{
    return m_ViewportRangeMax;
}

void HeatMapNode::setViewportRangeMin(const glm::vec2& viewportrangemin)
{
    m_ViewportRangeMin = viewportrangemin;
}

void HeatMapNode::setViewportRangeMax(const glm::vec2& viewportrangemax)
{
    m_ViewportRangeMax = viewportrangemax;
}

const glm::vec2& HeatMapNode::getMapSize() const
{
    return m_MapSize;
}

void HeatMapNode::setMapSize(const glm::vec2& mapsize)
{
    m_MapSize = mapsize;
}

const float& HeatMapNode::getValueRangeMin() const
{
    return m_ValueRangeMin;
}

const float& HeatMapNode::getValueRangeMax() const
{
    return m_ValueRangeMax;
}


void HeatMapNode::setValueRangeMin(const float& valuerangemin)
{
    m_ValueRangeMin = valuerangemin;
}

void HeatMapNode::setValueRangeMax(const float& valuerangemax)
{
    m_ValueRangeMax = valuerangemax;
}

const vector<string>& HeatMapNode::getColorMap() const
{
    return m_ColorMap;
}

void HeatMapNode::setColorMap(const vector<string>& colormap)
{
    m_ColorMap = colormap;
}

void HeatMapNode::setPosns(const std::vector<glm::vec2>& posns)
{
  cout << "POSNS SET" << endl;
}

void HeatMapNode::setMatrix(const vector<vector<float> >& matrix)
{
    for (int i = 0; i < matrix.size(); i++) {
      for(int j = 0; j < matrix[i].size(); j++) {
        cout << matrix[i][j] << endl;
      }
    }
}
