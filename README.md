# HeatMapNode

The HeatMapNode is a libavg plugin.
It is derived from RasterNode and can render a Heatmap based on single Points or a whole matrix.

## Installation Unix

__Dependencies:__
* cmake

__Build & Install:__

```
git clone git@git.imld.de:lucas.recknagel/HeatMapNode.git
cd HeatMapNode
mkdir build && cd build
cmake ..
make -j9
make install # run as sudo if you´re not in a virtual env
cd ..
python setup.py install # run as sudo if you´re not in a virtual env
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
