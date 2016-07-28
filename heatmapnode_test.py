#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import avg, app, player

player.loadPlugin('heatmap')

class HeatMapDiv(app.MainDiv):
    def onInit(self):
        self.matrix_node = heatmap.HeatMapNode(parent=self, size=self.size)

resolution = avg.Point2D(800,600)
resolutionStr = str(resolution.x) + ", " + str(resolution.y)
app.App().run(HeatMapDiv(), app_resolution=resolutionStr)
