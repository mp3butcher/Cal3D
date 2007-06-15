#!BPY
# $Id$

"""
Name: 'Cal3D v10.0 (blendercal)'
Blender: 242
Group: 'Export'
Tip: 'Export armature/bone/mesh/action data to the Cal3D format.'
"""

import Blender
import blendercal
import blendercal.bcdata
import blendercal.bcconf
import blendercal.bcgui
import sys
import os

__version__ = blendercal.VERSION
__author__  = blendercal.AUTHOR
__email__   = blendercal.EMAIL
__url__     = blendercal.URL
__bpydoc__  = blendercal.DOC

# Our extremely simple export function; it has two entry points, one
# from the commandline and another from the GUI.
def Cal3DExport(filename):
	# Reset globals, in case the script is executed multiple times
	# from the gui
	blendercal.bcobject.Material.MATERIALS = {}
	blendercal.bcobject.Skeleton.ARMATURE  = None
	blendercal.bcobject.Bone.BONES         = {}

	skeldata = blendercal.bcdata.SkeletonData()
	meshdata = blendercal.bcdata.MeshData()
	animdata = blendercal.bcdata.AnimationData()
	
	blendercal.bcdata.ExportData(filename, skeldata, meshdata, animdata)

if __name__ == "__main__":
	# If the user wants to run in "batch" mode, assume that ParseArgs
	# will correctly set bcconf data and go.
	if blendercal.ParseArgs(sys.argv):
		Cal3DExport(blendercal.bcconf.FILENAME)

		Blender.Quit()
	
	# Otherwise, let the bccgui module take over.
	else:
		blendercal.bcgui.CALLBACK = Cal3DExport
		
		Blender.Draw.Register(
			blendercal.bcgui.InterfaceDraw,
			blendercal.bcgui.InterfaceEvent,
			blendercal.bcgui.InterfaceButton
		)
