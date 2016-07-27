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
    TypeDefinition def = TypeDefinition("heatmapnode", "rasternode",  ExportedObject::buildObject<HeatMapNode>)
        .addArg( Arg<UTF8String>("info", "", false, offsetof(HeatMapNode, m_Info)) );
    const char* allowedParentNodeNames[] = {"div", "canvas", "avg", 0};
    TypeRegistry::get()->registerType(def, allowedParentNodeNames);
}

/// Constructor
HeatMapNode::HeatMapNode(const ArgList& args, const string& sPublisherName) : RasterNode(sPublisherName)
{
    args.setMembers(this);
    // m_Info = std::string("Empty Info");
}

/// Destructor
HeatMapNode::~HeatMapNode()
{
  cout << "HeatMapNode destruct" << endl;
}

/// Connect
void HeatMapNode::connect(CanvasPtr pCanvas)
{
    RasterNode::connect(pCanvas);
}

/// ConnectDisplay
void HeatMapNode::connectDisplay()
{
    RasterNode::connectDisplay();
}

void HeatMapNode::disconnect(bool bKill)
{
    RasterNode::disconnect(bKill);
}

void HeatMapNode::preRender(const VertexArrayPtr& pVA, bool bIsParentActive, float parentEffectiveOpacity)
{
  AreaNode::preRender(pVA, bIsParentActive, parentEffectiveOpacity);
}

void HeatMapNode::render(GLContext* pContext, const glm::mat4& transform)
{
    blt32(pContext, transform);
    cout << "HeatMapNode rendered" << endl;
}

void HeatMapNode::setInfo(const avg::UTF8String& info)
{
    m_Info = info;
}

const avg::UTF8String& HeatMapNode::getInfo() const
{
    return m_Info;
}
