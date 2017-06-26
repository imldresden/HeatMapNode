# HeatMapNode

This project is a [libavg](https://www.libavg.de) plugin.
HeatMapNode is derived from RasterNode and can render a Heatmap based on single Points or a whole matrix.
We have tested it under Linux and Mac.
Also, there are no OS-specific features that we know of, so it should not be hard to get to run under windows.

## Install Instructions

__Dependencies:__
* cmake

__Build & Install:__

```
cd HeatMapNode/plugin
mkdir build && cd build
cmake ..
make -j9
make install # run as sudo if you´re not using virtualenv
cd ../..
python setup.py install # run as sudo if you´re not using virtualenv
```

## Usage

You can use the pythonwrapper class or the plugin directly.

### Wrapper

```
from HeatMapNode import HeatMapNode
...

heatmap_node = HeatMapNode(
    id="test",
    size=self.size,
    parent=self,
    colormap=cm,
    valuerangemin=1,
    valuerangemax=16,
    overlay_src="world.png"
    blur_level=1
)
```

Your can set those attributes on you needs and then call:

```
heatmap_node.set_data(data)
```

The _data_ attribute must be a single avg.Point2D or a list of lists (2D Array or Matrix like)
You find a example in heatmapnode_test.py.

### Directly

```
player.loadPlugin('heatmap')
...
heatmap_node = heatmap.HeatMapNode(
    parent=parent,
    size=size,
    viewportrangemin=(0,0),
    viewportrangemax=(1,1),
    mapsize=(10,10),
    valuerangemin=valuerangemin,
    valuerangemax=valuerangemax,
    colormap=colormap
)
```

You can then call:

```
heatmap_node.setMatrix(data)
OR
heatmap_node.setPosns(data)
```

If you are using the plugin directly, blurring does not work out of the box!
