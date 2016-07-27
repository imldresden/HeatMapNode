# HeatMapNode

### Design Ideas

```c++
HeatMapNode: public RasterNode

constructor:
    viewportrange:
      glm::vec2 m_ViewportMin
      glm::vec2 m_ViewportMax
    mapsize
      glm::vec2 m_MapSize
    valuerange
      m_ValueMin
      m_ValueMax
    colormap
      vector<avg::Pixel32> m_ColorMap


setPosns(const vector<glm::vec2> posns)
setMap(const avg::BitmapPtr pBmp)
```

### Usefull Links

* https://www.libavg.de/site/projects/libavg/wiki/CodingConventions
* https://www.libavg.de/site/projects/libavg/wiki/NodeSubclassing
* https://www.libavg.de/site/projects/libavg/wiki/Plugins
