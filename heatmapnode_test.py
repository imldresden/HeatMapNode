#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import avg, app, player
import large_data as LD

player.loadPlugin('heatmap')

class HeatMapDiv(app.MainDiv):
    def onInit(self):

        # text matrix 8x8
        # len(list[lists])  => cols
        # len(lists)        => row
        # matrix = [
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0],
        #     [1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0]
        # ]

        cm = [
            "040ED8","2050FF","4196FF","6DC1FF",
            "86D9FF","9CEEFF","AFF5FF","CEFFFF",
            "FFFE47","FFEB00","FFC400","FF9000",
            "FF4800","FF0000","D50000","9E0000"
        ]

        self.blur_canvas = player.createCanvas(
            id="blur_test",
            autorender=True,
            multisamplesamples=1,
            size=(1920,1080)
        )
        self.blur_node = self.blur_canvas.getRootNode()

        self.heatmap_node = heatmap.HeatMapNode(
            parent=self.blur_node,
            size=(1920,1080),
            viewportrangemin=(0,0),
            viewportrangemax=(1,1),
            mapsize=(10,10),
            valuerangemin=-8.9,
            valuerangemax=8.3,
            colormap=cm
        )
        self.heatmap_node.setMatrix(LD._matrix)

        self.blur_FX = avg.BlurFXNode(1)
        self.heatmap_node.setEffect(self.blur_FX)
        #
        avg.ImageNode(
            parent=self,
            href="canvas:blur_test",
            pos=(0,0),
            size=self.size
        )

        avg.ImageNode(
            parent=self,
            href="world.png",
            pos=(0,0),
            size=self.size
        )

        print self.heatmap_node.colormap

resolution = avg.Point2D(1920,1080)
resolutionStr = str(resolution.x) + ", " + str(resolution.y)
app.App().run(HeatMapDiv(), app_resolution=resolutionStr)
