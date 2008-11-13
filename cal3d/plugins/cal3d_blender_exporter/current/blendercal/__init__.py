# -*- indent-tabs-mode: t -*-
# $Id$

import bcconf
import Blender.Mathutils

VERSION   = "0.0.1"
AUTHOR    = "Jeremy Moles, Palle Raabjerg, Jean-Baptiste Lamy"
EMAIL     = "jeremy@emperorlinux.com, palle@user-friendly.dk"
URL       = "http://gna.org/projects/cal3d"
ROT90X    = Blender.Mathutils.RotationMatrix(90, 4, "x")
IROT90X   = Blender.Mathutils.RotationMatrix(90, 4, "x").invert()
VECTOR2GL = lambda v: bcconf.EXPORTGL and ROT90X * v or v
MATRIX2GL = lambda m: bcconf.EXPORTGL and ROT90X * m * IROT90X or m

DOC = """
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

NOTE:

This code (well, just the bcobject code, though it it also highly modified),
is based loosely on the original code by Jiba in blender2cal3d.py. Many thanks
to him for his initial work on paving the way! Other than that, most of this
code is entirely original by Jeremy Moles and Palle Raabjerg, with a bit of
influence from Loic Dachary, especially in the bcdata.AnimationData().
"""

# A decorator to catch any exception semi-gracefully. Sometiem later
# this will create a Blender panel to show the exception.
def exception(function):
    def exceptionDecorator(*args, **kargs):
        try:
            return function(*args, **kargs)

        except Exception, e:
            print "Exception in", function.func_name, "- error was:", e

    return exceptionDecorator

# A function that will parse the passed-in sequences and set the appropriate
# values in bcconf.
@exception
def ParseArgs(parse):
    args     = []
    strip    = lambda s: s.rstrip().lstrip().replace("\t", "").replace("\n", "")
    str2bool = lambda s: s.lower() == "true" or s == "1"

    for arg in parse:
        arg = strip(arg)
    
        if "--blendercal" in arg:
            args = arg[13 : ].split(";")

    for arg in args:
        if "=" in arg:
            a, v = arg.split("=")
            a    = strip(a).upper()
            v    = strip(v)

            print ">>", a, "=", v

            {
                "FILENAME":    lambda: setattr(bcconf, a, v),
                "EXPORTGL":    lambda: setattr(bcconf, a, str2bool(v)),
                "FLOATPRE":    lambda: setattr(bcconf, a, int(v)),
                "XMLINDENT":   lambda: setattr(bcconf, a, int(v)),
                "ANIMFPS":     lambda: setattr(bcconf, a, float(v)),
                "AUTHOR":      lambda: setattr(bcconf, a, v),
                "PREPEND":     lambda: setattr(bcconf, a, v),
                "LOD":         lambda: setattr(bcconf, a, str2bool(v)),
                "EXTRADATA":   lambda: setattr(bcconf, a, str2bool(v)),
                "SUBMESHMODE": lambda: setattr(bcconf, a, v),
            }[a]()

    # Return args; since this will be False by default, we'll use this
    # to determine if the user passed a --blendercal argument.
    return args
