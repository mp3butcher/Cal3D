-----------------------------------------------
Cal3D_Export
Version 1.02.00
Copyright (C) 2004 Mekensleep

Mekensleep
24 rue vieille du temple
75004 Paris
      licensing@mekensleep.com

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

Authors:
 Philippe Lamoureux <box@phlam.net>
-----------------------------------------------

Complete rewrite of the previous version.
Based on Named Selection Sets to clearly define the objects, skeleton
roots and used bones that are to be exported. Allows the definition of
all the animations contained in the scene for an automated export


Installation :
--------------
Drag'n'Drop the .MZP file onto a 3DSMax viewport : The tool is now
installed and run once. A new macroscript "Cal3D Export" is defined in the
"File" macroscript category : Add this macroscript to a toolbar of your
choice to run the script.


Interface :
-----------
*** "Mesh & Squeleton" group
"Objects" : Specify the Named Selection Set containing the skinned meshe(s)
to be exported.
"Re-index materials" : The materials are renamed with a tailing
[<index>] refering to the order of the exported meshes.
"LOD creation" : Same as the manual export interface option
"Spring system" : Same as the manual export interface option

"Skel. root(s)" : Specify the Named Selection Set containing the skeleton
root(s) of the hierarch-y/-ies to be exported. The whole children tree is
exported.
"Max bones by vertex" : Same as the manual export interface option
"Min weight threshold" : Same as the manual export interface option

*** "Cal3D mini-viewer" group
"Create ALL.CFG" : Creates a configuration file for the mini-viewer,
if the export is successful
"Preview ALL.CFG" : Launches the mini-viewer if the export is successful

*** "Animations" group
"Sampling rate" : Same as the manual export interface "Offset" option
"Animations" listbox : Lists the defined animation segments to be exported.
Each animation segment is defined by a name (used in the filename), a start
and end frame, and a Named Selection Set containing the bones which animation
is exported.
Double-clicking an animation entry applies the start and end values to the
3DSMax time slider range.
"+" / "-" : Adds / deletes an animation entry
"up" / "dn" : Changes the place of the selected animation in the listbox

*** "Configuration" group
"File format" : Will export as .C?F or .X?F, depending on the chosen
option
"Viewer path" : Path of the CAL3D mini-viewer
"Export Dir" : Directory to export the files to

"Save config"
Save the parameters and closes the dialog box
"Export"
Export the files, save the parameters and closes the dialog box
"Abort"
Don't save anything and close the dialog box


Notes :
-------
The skeleton file is saved in a unique file called skeleton.CSF

The script uses 2 methods to save the different parameters defined in the
interface : an external file for the global parameters (viewer path, file
format and sampling rate) and the User Properties of an additional scene node
for the other, more character-specific, parameters.
This scene node is a (frozen) Point helper called "cal3D_data". This allows
you to merge a character and its animation data into another scene, by merging
this helper too.
Each time the parameters are saved, the 'Save is required' state of the scene
is activated, to remind you to save the scene so as to keep the fresh
parameters.

Implements a workaround that avoids the bug described at
http://sourceforge.net/tracker/index.php?func=detail&aid=555376&group_id=26281&atid=386671
