# $Id$

import Blender
import os
import blendercal
import bcconf

CALLBACK = lambda *a, **k: None
UI       = [None] * 11
IMAGE    = Blender.Image.Load(os.path.join(Blender.Get("scriptsdir"), "blendercal", "logo.png"))

(
	EV_AUTHOR,
	EV_PREPEND,
	EV_EXPORTGL,
	EV_LOD,
	EV_EXTRADATA,
	EV_XMLINDENT,
	EV_FLOATPRE,
	EV_ANIMFPS,
	EV_VIEW,
	EV_CANCEL,
	EV_WRITE
) = range(11)

# We do some forced type coersion here JUST IN CASE the user messes it up. :)
# I usually don't do stuff like this (since the python "way" is to let exceptions
# propagate), but we really don't want to have weird behavior here.
EXPORTS = [
	str(bcconf.AUTHOR),
	str(bcconf.PREPEND),
	int(bcconf.EXPORTGL),
	int(bcconf.LOD),
	int(bcconf.EXTRADATA),
	int(bcconf.XMLINDENT),
	int(bcconf.FLOATPRE),
	int(bcconf.ANIMFPS)
]

# Easy way to create and manage a progress bar. bar_steps is the number of times the 
# bar itself will be updated during the progress. This is to avoid a horrendous amount of
# bar updates, which would create a fantastic amount of overhead on machines without
# hardware rendering. Start by calling Progress.setup, and then call Progress.increment
# for each time any progress is made.
class Progress(object):
	def __init__(self, bar_steps):
		object.__init__(self)

		self.bar_steps = bar_steps

	def setup(self, total_steps, pstring):
		self.total_steps = total_steps
		self.pstring = pstring
		self.stepsize = float(self.total_steps)/float(self.bar_steps)
		self.progress = 0
		self.multiplier = 0
		self.steplimit = 0
		self.update()

	def increment(self, amount = 1):
		self.progress += amount

		if self.progress >= self.steplimit:
			self.multiplier += 1
			self.steplimit = self.stepsize * self.multiplier
			self.update()

	# TODO: Palle, whoa. :) I'll have to break this up a bit.
	def update(self):
		Blender.Window.DrawProgressBar(
			float(self.progress)/float(self.total_steps),
			self.pstring + ": " + str(int((float(self.progress)/float(self.total_steps))*100)) + "%"
		)

def InterfaceDraw():
	Blender.BGL.glClear(Blender.BGL.GL_COLOR_BUFFER_BIT)
	Blender.BGL.glEnable(Blender.BGL.GL_BLEND)
	Blender.BGL.glBlendFunc(
		Blender.BGL.GL_SRC_ALPHA,
		Blender.BGL.GL_ONE_MINUS_SRC_ALPHA
	)

	UI[0] = Blender.Draw.String(
		"Author's Name: ",
		EV_AUTHOR,
		5, 25, 400, 20,
		EXPORTS[0],
		255,
		"The author's name; goes into $MODEL.cfg."
	)
	
	UI[1] = Blender.Draw.String(
		"Prepend Name: ",
		EV_PREPEND,
		5, 45, 400, 20,
		EXPORTS[1],
		255,
		"Prepend all model files with this string."
	)
	
	UI[2] = Blender.Draw.Toggle(
		"Export OpenGL",
		EV_EXPORTGL,
		5, 65, 140, 20,
		EXPORTS[2],
		"Change the up direction to Y; useful for some engines."
	)
	
	UI[3] = Blender.Draw.Toggle(
		"Calculate LODS",
		EV_LOD,
		5, 85, 140, 20,
		EXPORTS[3],
		"Calculate Cal3D Level Of Detail steps."
	)
	
	UI[4] = Blender.Draw.Toggle(
		"Extra Data",
		EV_EXTRADATA,
		5, 105, 140, 20,
		EXPORTS[4],
		"Add date, time, etc. into $MODEL.cfg."
	)
	
	UI[5] = Blender.Draw.Number(
		"XML Indent:      ",
		EV_XMLINDENT,
		5, 125, 200, 20,
		EXPORTS[5],
		0,
		10,
		"Use this level of indentation in the XML files."
	)
	
	UI[6] = Blender.Draw.Number(
		"Float Precision: ",
		EV_FLOATPRE,
		5, 145, 200, 20,
		EXPORTS[6],
		3,
		20,
		"Use this level of float/double precision (significant digits)."
	)
	
	UI[7] = Blender.Draw.Number(
		"Animation FPS:   ",
		EV_ANIMFPS,
		5, 165, 200, 20,
		EXPORTS[7],
		1,
		100,
		"Frames Per Second"
	)
	
	UI[8] = Blender.Draw.PushButton(
		"View Stats",
		EV_VIEW,
		5, 5, 200, 20,
		"View the objects and groups the exporter will be aware of."
	)
	
	UI[9] = Blender.Draw.PushButton(
		"Quit",
		EV_CANCEL,
		205, 5, 100, 20,
		"Oh nos!"
	)
	
	UI[10] = Blender.Draw.PushButton(
		"WRITE",
		EV_WRITE,
		305, 5, 100, 20,
		"Weee!"
	)

	Blender.Draw.Image(IMAGE, 145, 65)

	Blender.BGL.glDisable(Blender.BGL.GL_BLEND)

def InterfaceEvent(event, value):
	if event == Blender.Draw.ESCKEY:
		Blender.Draw.Exit()

def InterfaceButton(event):
	def __setVal(e):
		EXPORTS[e] = UI[e].val
	
	def __setBool(e):
		EXPORTS[e] = not EXPORTS[e]

	def __doView(e):
		pass
	
	def __doCancel(e):
		Blender.Draw.Exit()
	
	def __doWrite(e):
		blendercal.ParseArgs(["""
			--blendercal=
			AUTHOR    = %s;
			PREPEND   = %s;
			XMLINDENT = %s;
			FLOATPRE  = %s;
			ANIMFPS   = %g;
			EXPORTGL  = %s;
			LOD       = %s;
			EXTRADATA = %s;
		""" % (
			EXPORTS[EV_AUTHOR],
			EXPORTS[EV_PREPEND],
			EXPORTS[EV_XMLINDENT],
			EXPORTS[EV_FLOATPRE],
			EXPORTS[EV_ANIMFPS],
			EXPORTS[EV_EXPORTGL],
			EXPORTS[EV_LOD],
			EXPORTS[EV_EXTRADATA]
		)])
	
		Blender.Window.FileSelector(CALLBACK, "Export Cal3D", "model.cfg")

		Blender.Draw.Exit()

	{
		EV_AUTHOR:     __setVal,
		EV_PREPEND:    __setVal,
		EV_XMLINDENT:  __setVal,
		EV_FLOATPRE:   __setVal,
		EV_ANIMFPS:    __setVal,
		EV_EXPORTGL:   __setBool,
		EV_LOD:        __setBool,
		EV_EXTRADATA:  __setBool,
		EV_VIEW:       __doView,
		EV_CANCEL:     __doCancel,
		EV_WRITE:      __doWrite
	}[event](event)
