#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import avg, app, player

player.loadPlugin('heatmap')

class HeatMapDiv(app.MainDiv):
    def onInit(self):

        # text matrix 8x8
        # len(list[lists])  => cols
        # len(lists)        => row
        matrix = [
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
            [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0]
        ]

        cm = ["00b159","d11141","ffc425","2b91a9","f7c8f9","720707","403f3f","4dfffe"]

        self.blur_canvas = player.createCanvas(
            id="blur_test",
            autorender=True,
            multisamplesamples=1,
            size=(256,256)
        )
        self.blur_node = self.blur_canvas.getRootNode()

        self.heatmap_node = heatmap.HeatMapNode(
            parent=self.blur_node,
            size=(256,256),
            viewportrangemin=(0,0),
            viewportrangemax=(1,1),
            mapsize=(10,10),
            valuerangemin=0.0,
            valuerangemax=50.0,
            colormap=cm
        )
        self.heatmap_node.setMatrix(matrix)

        self.blur_FX = avg.BlurFXNode(0.3)
        self.heatmap_node.setEffect(self.blur_FX)
        #
        avg.ImageNode(
            parent=self,
            href="canvas:blur_test",
            pos=(0,0),
            size=self.size,
        )

        # print (self.heatmap_node.viewportrangemin, self.heatmap_node.viewportrangemax, self.heatmap_node.mapsize, self.heatmap_node.valuerangemin, self.heatmap_node.valuerangemax, self.heatmap_node.colormap)

resolution = avg.Point2D(1024,720)
resolutionStr = str(resolution.x) + ", " + str(resolution.y)
app.App().run(HeatMapDiv(), app_resolution=resolutionStr)
