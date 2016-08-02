#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import avg, app, player

player.loadPlugin('heatmap')

class HeatMapDiv(app.MainDiv):
    def onInit(self):

        matrix = [[1.0,2.0,3.0,4.0],[5.0,6.0,7.0,8.0],[9.0,10.0,11.0,12.0]]

        cm = ["00b159","d11141"]

        self.heatmap_node = heatmap.HeatMapNode(
            parent=self,
            size=self.size,
            viewportrangemin=(0,0),
            viewportrangemax=(1,1),
            mapsize=(10,10),
            valuerangemin=0.0,
            valuerangemax=50.0,
            colormap=cm
        )
        self.heatmap_node.setMatrix(matrix)
        print (self.heatmap_node.viewportrangemin, self.heatmap_node.viewportrangemax, self.heatmap_node.mapsize, self.heatmap_node.valuerangemin, self.heatmap_node.valuerangemax, self.heatmap_node.colormap)

resolution = avg.Point2D(800,600)
resolutionStr = str(resolution.x) + ", " + str(resolution.y)
app.App().run(HeatMapDiv(), app_resolution=resolutionStr)
