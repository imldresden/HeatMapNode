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

#include "HeatMapNode.h"

#include <base/ScopeTimer.h>
#include <base/Exception.h>
#include <graphics/VertexArray.h>
#include <graphics/GLContextManager.h>
#include <graphics/Bitmap.h>
#include <graphics/Color.h>
#include <graphics/MCTexture.h>
#include <player/Arg.h>
#include <player/TypeDefinition.h>
#include <player/TypeRegistry.h>
#include <player/OGLSurface.h>

using namespace boost;
using namespace std;
using namespace avg;

void HeatMapNode::registerType()
{
    vector<string> cm;
    vector<float> om;
    TypeDefinition def = TypeDefinition("heatmapnode", "rasternode",  ExportedObject::buildObject<HeatMapNode>)
        .addArg( Arg<glm::vec2>("viewportrangemin", glm::vec2(0,0), false, offsetof(HeatMapNode, m_ViewportRangeMin)) )
        .addArg( Arg<glm::vec2>("viewportrangemax", glm::vec2(0,0), false, offsetof(HeatMapNode, m_ViewportRangeMax)) )
        .addArg( Arg<glm::vec2>("mapsize", glm::vec2(0,0), false, offsetof(HeatMapNode, m_MapSize)) )
        .addArg( Arg<float >("valuerangemin", 0.0, false, offsetof(HeatMapNode, m_ValueRangeMin)) )
        .addArg( Arg<float >("valuerangemax", 0.0, false, offsetof(HeatMapNode, m_ValueRangeMax)) )
        .addArg( Arg<vector<string> >("colormap", cm, false, offsetof(HeatMapNode, m_ColorStrs)) )
        .addArg( Arg<vector<float> >("opacitymap", om, false, offsetof(HeatMapNode, m_Opacities)) )
        ;

    const char* allowedParentNodeNames[] = {"div", "canvas", "avg", 0};
    TypeRegistry::get()->registerType(def, allowedParentNodeNames);
}

HeatMapNode::HeatMapNode(const ArgList& args, const string& sPublisherName) : RasterNode(sPublisherName)
{
    args.setMembers(this);
    m_ShouldPrerender = false;
    createColorMap();
}

HeatMapNode::~HeatMapNode() {}

void HeatMapNode::connect(CanvasPtr pCanvas)
{
    RasterNode::connect(pCanvas);
}

void HeatMapNode::connectDisplay()
{
    if (m_Matrix.size() != 0) {
        setupRender();
    }
    RasterNode::connectDisplay();
}

void HeatMapNode::disconnect(bool bKill)
{
    RasterNode::disconnect(bKill);
}

glm::ivec2 getMatrixSize(vector<vector<float> > matrix)
{
    int ysize = matrix.size();
    int xsize;
    if (ysize > 0) {
        xsize = matrix.front().size();
    } else {
        xsize = 0;
    }
    return glm::ivec2(xsize, ysize);
}

static ProfilingZoneID PrerenderProfilingZone("HeatMapNode::prerender");
void HeatMapNode::preRender(const VertexArrayPtr& pVA, bool bIsParentActive, float parentEffectiveOpacity)
{
    AreaNode::preRender(pVA, bIsParentActive, parentEffectiveOpacity);

    if (m_pTex && m_ShouldPrerender)
    {
        ScopeTimer timer(PrerenderProfilingZone);

        BitmapPtr pBmp(new Bitmap(getMatrixSize(m_Matrix), R8G8B8A8));
        int Stride = pBmp->getStride()/pBmp->getBytesPerPixel();
        IntPoint size = pBmp->getSize();
        Pixel32 * pLine = (Pixel32*)(pBmp->getPixels());
        Pixel32 * pPixel;
        for (int y=0; y<size.y; ++y) {
            pPixel = pLine;
            for (int x=0; x<size.x; ++x) {
                float val = m_Matrix[y][x];
                val = max(m_ValueRangeMin, min(val, m_ValueRangeMax));
                float posInColors = ((val - m_ValueRangeMin)/(m_ValueRangeMax - m_ValueRangeMin)) * (m_Colors.size()-1);
                int i = int(posInColors);
                float ratio = posInColors - i;
                Color c = Color::mix(m_Colors[i], m_Colors[i+1], 1-ratio);

                *pPixel = c;
                if (!m_Opacities.empty()) {
                    pPixel->setA((m_Opacities[i]*(1-ratio) + m_Opacities[i+1]*ratio)*255);
                }
                pPixel++;
            }
            pLine += Stride;
        }
        GLContextManager::get()->scheduleTexUpload(m_pTex, pBmp);
        scheduleFXRender();
        calcVertexArray(pVA);

        m_ShouldPrerender = false;
    }
}

static ProfilingZoneID RenderProfilingZone("HeatMapNode::render");
void HeatMapNode::render(GLContext* pContext, const glm::mat4& transform)
{
    if (m_pTex) {
        ScopeTimer Timer(RenderProfilingZone);
        blt32(pContext, transform);
    }
}

void HeatMapNode::setColorMap(const vector<string>& colormap)
{
    m_ColorStrs = colormap;
    m_Opacities.clear();
    createColorMap();
    m_ShouldPrerender = true;
}

const vector<string>& HeatMapNode::getColorMap() const
{
    return m_ColorStrs;
}

void HeatMapNode::setOpacityMap(const vector<float>& opacitymap)
{
    m_Opacities = opacitymap;
    createColorMap();
    m_ShouldPrerender = true;
}

const vector<float>& HeatMapNode::getOpacityMap() const
{
    return m_Opacities;
}

void HeatMapNode::setValueRangeMin(float min)
{
    m_ValueRangeMin = min;
    m_ShouldPrerender = true;
}

float HeatMapNode::getValueRangeMin() const
{
    return m_ValueRangeMin;
}

void HeatMapNode::setValueRangeMax(float max)
{
    m_ValueRangeMax = max;
    m_ShouldPrerender = true;
}

float HeatMapNode::getValueRangeMax() const
{
    return m_ValueRangeMax;
}

void HeatMapNode::setPosns(const std::vector<glm::vec2>& posns)
{
    m_Matrix.clear();
    for (int y=0; y<m_MapSize.y; ++y) {
        m_Matrix.push_back(vector<float>(m_MapSize.x, 0.0f));
    }
    glm::vec2 viewportExtent = m_ViewportRangeMax - m_ViewportRangeMin;
    for (auto pos: posns) {
        glm::vec2 matPos = ((pos - m_ViewportRangeMin)/viewportExtent) * m_MapSize;
        if (matPos.y >= 0 && matPos.y < m_MapSize.y && matPos.x >= 0 && matPos.x < m_MapSize.x) {
            m_Matrix[matPos.y][matPos.x]++;
        }
    }
    if (getState() == NS_CANRENDER && !m_pTex) {
        setupRender();
    }
    m_ShouldPrerender = true;
}

void HeatMapNode::setMatrix(const vector<vector<float> >& matrix)
{
    glm::ivec2 oldSize = getMatrixSize(m_Matrix);
    m_Matrix = matrix;
    if (getState() == NS_CANRENDER) {
        glm::ivec2 size = getMatrixSize(m_Matrix);
        // Create new texture if it doesn't exist yet or the size has changed.
        if (!m_pTex || (m_pTex && oldSize != size)) {
            setupRender();
        }
    }
    m_ShouldPrerender = true;
}

void HeatMapNode::createColorMap()
{
    if (!m_Opacities.empty() && !(m_ColorStrs.size() == m_Opacities.size())) {
        throw avg::Exception(AVG_ERR_OUT_OF_RANGE, "colormap and opacitymap must have the same size.");
    }
    m_Colors.clear();
    for (auto colorStr: m_ColorStrs) {
        m_Colors.push_back(colorStr);
    }
}

void HeatMapNode::setupRender()
{
    glm::ivec2 size = getMatrixSize(m_Matrix);
    m_pTex = GLContextManager::get()->createTexture(size, R8G8B8A8, getMipmap());
    getSurface()->create(R8G8B8A8, m_pTex);
    newSurface();
    setupFX();

}
