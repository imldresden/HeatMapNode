#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import avg, app, player

player.loadPlugin('heatmap')

BLUR_CROP_VAL = 0.015

class HeatMapNode(avg.DivNode):

    def __init__(self, id=id, size=None, parent=None, colormap=None,valuerangemin=None, valuerangemax=None, overlay_src=None, blur_level=1, **kwargs):
        super(HeatMapNode, self).__init__(**kwargs)

        self.div_node_wrapper = avg.DivNode(
            parent=parent,
            size=size,
            crop=True
        )

        self.blur_canvas = player.createCanvas(
            id="blur_canvas_"+id,
            autorender=True,
            multisamplesamples=1,
            size=size*(1-BLUR_CROP_VAL)
        )
        self.blur_node = self.blur_canvas.getRootNode()

        self.heatmap_node = heatmap.HeatMapNode(
            parent=self.blur_node,
            size=size*(1+BLUR_CROP_VAL),
            viewportrangemin=(0,0),
            viewportrangemax=(1,1),
            mapsize=(10,10),
            valuerangemin=valuerangemin,
            valuerangemax=valuerangemax,
            colormap=colormap
        )

        self.blur_FX = avg.BlurFXNode(blur_level)
        self.heatmap_node.setEffect(self.blur_FX)

        self.img_node = avg.ImageNode(
            parent=self.div_node_wrapper,
            href="canvas:blur_canvas_"+id,
            pos=(-size.x*BLUR_CROP_VAL,-size.y*BLUR_CROP_VAL),
            size=size*(1+BLUR_CROP_VAL)
        )

        if overlay_src:
            self.overlay = avg.ImageNode(
                parent=self.div_node_wrapper,
                href=overlay_src,
                pos=(0,0),
                size=size
            )

    def set_colormap(self, cm):
        self.heatmap_node.colormap = cm

    def set_opacity_map(self, om):
        self.heatmap_node.opacitymap = om

    def set_data(self, data):
        if isinstance(data, list) and isinstance(data[0], list):
            self.heatmap_node.setMatrix(data)
        elif isinstance(data, avg.Point2D):
            self.heatmap_node.setPosns(data)
        else:
            raise TypeError("Type of data must be list of lists or avg.Point2D!")

    def set_value_range(self, min, max):
        self.heatmap_node.valuerangemin = min
        self.heatmap_node.valuerangemax = max
