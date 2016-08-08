#include "HeatMapNode.h"

#include <base/ScopeTimer.h>
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
    m_ShouldPrerender = false;
    createColorRange(m_ValueRangeMin, m_ValueRangeMax);
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
                avg::Pixel32 c;
                std::map<float, avg::Pixel32>::iterator low, prev;
                double pos = m_Matrix[y][x];
                low = m_ColorMapping.lower_bound(pos);
                if (low == m_ColorMapping.end()) {
                    // nothing found
                } else if (low == m_ColorMapping.begin()) {
                    c = m_ColorMapping[low->first];
                } else {
                    prev = low;
                    --prev;
                    if ((pos - prev->first) < (low->first - pos)) {
                        c = m_ColorMapping[prev->first];
                    } else {
                        c = m_ColorMapping[low->first];
                    }
                }
                *pPixel = c;
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

const vector<string>& HeatMapNode::getColorMap() const
{
    return m_ColorMap;
}

void HeatMapNode::setColorMap(const vector<string>& colormap)
{
    m_ColorMap = colormap;
    createColorRange(m_ValueRangeMin, m_ValueRangeMax);
    m_ShouldPrerender = true;
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
            if (m_Matrix[matPos.y][matPos.x] < m_ValueRangeMax-1) {
                m_Matrix[matPos.y][matPos.x]++;
            }
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

void HeatMapNode::createColorRange(const float& min, const float& max)
{
    float _min;
    float _max;
    float goes_over_zero = 0;
    float is_nonchanging_range = 0;
    if (min < 0)
        _min = -min;
    else
        _min = min;

    if (max < 0)
        _max = -max;
    else
        _max = max;

    if (min < 0 && max > 0)
        goes_over_zero = 1;
    else
        is_nonchanging_range = 1;

    float range_size = _min + _max - is_nonchanging_range;
    float range_steps = range_size / (m_ColorMap.size() - goes_over_zero);

    for (int i=0; i < m_ColorMap.size(); ++i)
    {
      float v = max - (i*range_steps);
      m_ColorMapping[v] = Color(m_ColorMap.at( (m_ColorMap.size()-1) - i));
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
