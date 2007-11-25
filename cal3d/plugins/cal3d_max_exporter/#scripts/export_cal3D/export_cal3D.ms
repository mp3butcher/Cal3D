--	-------------------------------------------------------------------
--	CAL3D export
--	version : 1.04.00
--	Based on the 0.9.1 'maxscript-extended' version of the exporter for Max6
--
-- Copyright (C) 2004 Mekensleep
--
-- Mekensleep
-- 24 rue vieille du temple
-- 75004 Paris
--       licensing@mekensleep.com
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--
-- Authors:
--  Philippe Lamoureux <box@phlam.net>
--	-------------------------------------------------------------------

global cal3DexportR
try (destroyDialog cal3DexportR) catch()
global cal3DDataVersion = 1.0
global cal3DDataNodeName = "cal3D_data"

---------------------------------------------------------------------------
---------------------------------------------------------------------------
--	getStringKey / setStringKey
--	Same commands as GetINIsetting and setINIsetting, but works on string buffers
---------------------------------------------------------------------------
global setStringKey, getStringKey
---------------------------------------------------------------------------
fn setStringKey &sbuff section myKey myValue =
--	Writes a [section/]key in a string buffer
--	If myValue is an empty string, the key is erased, and the section too
--	  if there is no key anymore in the section
---------------------------------------------------------------------------
(
	if (myValue==undefined) then myValue=""
	if (section=="") or (myKey=="") then
	(
		MessageBox "setStringKey error : wrong data format"
		return()
	)

	local startKey = myKey+"="
	local lignes = filterstring sbuff "\r\n"
	local i, section_indexes = #(), bonne_section=0, bonne_ligne=0

	if (lignes.count != 0) then
	(
		--	List the sections
		for i=1 to lignes.count do
		(
			if (lignes[i]!="") and (lignes[i][1]=="[") and (lignes[i][lignes[i].count]=="]") then
				append section_indexes i
		)
		append section_indexes (lignes.count+1)

		--	Seek the right section
		for i=1 to (section_indexes.count-1) do
			if (lignes[section_indexes[i]]=="["+section+"]") then
				bonne_section = i
		--	If the section is OK, look for the key
		if (bonne_section!=0) then
		(
			for i=section_indexes[bonne_section] to (section_indexes[bonne_section+1]-1) do
			(
				if ( (substring lignes[i] 1 startKey.count)==startKey) then
					bonne_ligne = i
			)
		)
		--	else, append the section at the end (if myKey is not empty)
		else if (myValue !="") then
		(
			append lignes ("["+section+"]")
			bonne_ligne = lignes.count + 1
		)

		--	If the key is found, update its content
		if (bonne_ligne != 0) then
		(
			-- if myValue is empty, delete the key and maybe the section
			if (myValue=="") then
			(
				deleteItem lignes bonne_ligne
				--	If the number of keys in the section is below 2, delete the section
				if (section_indexes[bonne_section + 1] - section_indexes[bonne_section] - 1 < 2 ) then
					deleteItem lignes section_indexes[bonne_section]
			)
			else
				lignes[bonne_ligne] = myKey + "=" + myValue
		)
		--	else insert the key before the end of the section
		else
		(
			if (myValue!="") then
			(
				bonne_ligne = section_indexes[bonne_section+1]
				insertItem (myKey + "=" + myValue) lignes bonne_ligne
			)
		)
	)
	else
	(
		if (myValue!="") then
			lignes = #( ("["+section+"]"), (myKey + "=" + myValue) )
	)
	--	Re-create the string buffer
	sbuff = ""
	if (lignes.count != 0) then
	(
		for i in lignes do sbuff=sbuff+i+"\r\n"
	)
)



---------------------------------------------------------------------------
fn getStringKey sbuff section myKey =
--	Returns the value of a 'myKey' key in the 'section' section of a string buffer
--	Returns "" if the key or its section does not exist
---------------------------------------------------------------------------
(
	if (section=="") or (myKey=="") then
	(
		MessageBox "getStringKey error : wrong data format"
		return ""
	)
	local startSection = "["+section+"]"
	local startKey = myKey+"="
	local lignes = filterstring sbuff "\r\n"
	local i, section_indexes=#(), bonne_section=0, bonne_ligne=0

	if (lignes.count == 0) then return ""						--	S'il n'y a rien, renvoie ""
	--	Lists the sections
	for i=1 to lignes.count do
	(
		if (lignes[i]!="") and (lignes[i][1]=="[") and (lignes[i][lignes[i].count]=="]") then
			append section_indexes i
	)
	append section_indexes (lignes.count+1)
	--	Seek the right section
	for i=1 to (section_indexes.count-1) do
		if (lignes[section_indexes[i]]==startSection) then
			bonne_section = i
	if (bonne_section==0) then return ""						--	the section is not found, return ""
	for i=section_indexes[bonne_section] to (section_indexes[bonne_section+1]-1) do
	(
		if ( (substring lignes[i] 1 startKey.count)==startKey) then
			bonne_ligne = i
	)
	if (bonne_ligne == 0) then return ""						--	the key is not found, return ""
	return ( substring lignes[bonne_ligne] (1+startkey.count) -1 )	--	return the myKey value
)
---------------------------------------------------------------------------
---------------------------------------------------------------------------




--	-------------------------------------------------------------------
--	-------------------------------------------------------------------
rollout cal3DexportR "Cal3D Multi-export" width:464 height:400
(
	local aList=#()						--	Animations data array
	--	each element is a 5 items array
	--	#( <string>Animation_name, <int>start_frame, <int>end_frame, <bool>full_skeleton, <string>bones_NamedSelectionSet )
	local selectedAnimIdx = 0			--	Index of the selected animation
	local nssList = #()					--	Selection Sets Names

	--	-------------------------------------------------------------------
	fn backSlash dir =
	--	Ensures the given 'dir' directory path has a tailing '\' character
	--	Returns the (corrected) directory path string
	--	-------------------------------------------------------------------
	(
		if (dir[dir.count]!="\\") then dir = dir + "\\"
		return dir
	)
	local configFile = (backslash (getDir #plugcfg)) + "cal3DExport.cfg"	--	general config file path

	GroupBox grp1 "Meshes && Skeleton" pos:[8,8] width:216 height:192
	label lbl11 "Objects" pos:[16,36] width:40 height:16
	dropdownList ddlObjSet "" pos:[80,32] width:136 height:21
	checkbox chkIndexMat "Re-index materials" pos:[80,56] width:136 height:16 enabled:true checked:true
	checkbox chkLOD "LOD creation" pos:[80,72] width:136 height:16
	checkbox chkSprings "Spring system" pos:[32,88] width:96 height:16
	dropdownList ddlSprings "" pos:[80,104] width:136 height:21
	label lbl13 "Skel. root(s)" pos:[16,138] width:64 height:16
	dropdownList ddlSkelSet "" pos:[80,136] width:136 height:21
	label lbl9 "Max bones by vertex" pos:[48,160] width:104 height:16
	spinner spnMaxBones "" pos:[152,160] width:64 height:16 range:[1,999,4] type:#integer scale:1
	label lbl10 "Min weight threshold" pos:[48,176] width:104 height:16
	spinner spnMinThres "" pos:[152,176] width:64 height:16 range:[0.001,1,0.001] scale:0.01
	GroupBox grp4 "Cal3D mini-viewer" pos:[8,200] width:216 height:56
	checkbox chkCreateCFG "Create cal3D.cfg" pos:[24,216] width:112 height:16
	checkbox chkPreviewCFG "Preview cal3D.cfg" pos:[24,232] width:112 height:16
	GroupBox grp2 "Animations" pos:[232,8] width:224 height:248
	listbox lbxAnims "" pos:[256,48] width:160 height:8
	button btnAnimAdd "+" pos:[424,48] width:24 height:16 toolTip:"Add an animation entry"
	button btnAnimDel "-" pos:[424,72] width:24 height:16 toolTip:"Delete the selected animation entry"
	button btnAnimUp "up" pos:[424,120] width:24 height:16 toolTip:""
	button btnAnimDn "dn" pos:[424,144] width:24 height:16
	label lbl6 "Name" pos:[248,168] width:32 height:16
	edittext edtAnimName "" pos:[284,168] width:160 height:16
	label lbl7 "Start" pos:[254,189] width:24 height:16
	spinner spnAnimStart "" pos:[287,188] width:48 height:16 range:[0,10000,0] type:#integer scale:1
	button btnAnimRange "|< >|" pos:[342,190] width:26 height:14 toolTip:"Use the active segment for the start and end frames"
	label lbl12 "End" pos:[372,190] width:24 height:16
	spinner spnAnimEnd "" pos:[396,188] width:48 height:16 range:[0,10000,0] type:#integer scale:1
	radiobuttons rdoSkelType "" pos:[248,208] width:195 height:16 labels:#("Full skeleton", "Bones set") columns:2
	dropdownList ddlBonesSet "" pos:[288,224] width:157 height:21
	GroupBox grp3 "Configuration" pos:[8,264] width:448 height:96
	label lbl19 "File format" pos:[24,280] width:56 height:16
	radiobuttons rdoFormat "" pos:[96,280] width:118 height:16 enabled:true labels:#("Binary", "XML") columns:2
	checkbox chkTransform "Transform into OpenGL coord. system" pos:[24,296] width:208 height:16
	label lb210 "Scale factor" pos:[320,280] width:64 height:16
	spinner spnScale "" pos:[384,280] width:64 height:16 range:[0.001,100000,1]
	label lbl40 "Sampling rate (frames)" pos:[248,24] width:104 height:16
	spinner spnSRate "" pos:[368,24] width:48 height:16 range:[0,100,1] type:#integer scale:1
	button btnViewerPath "Viewer path" pos:[16,320] width:72 height:16
	edittext lblViewerPath "" pos:[88,320] width:360 height:16 enabled:false
	button btnExportDir "Export dir" pos:[16,336] width:72 height:16
	edittext lblExportDir "" pos:[88,336] width:360 height:16 enabled:false
	button btnSaveConfig "Save config" pos:[88,368] width:80 height:24
	button btnExport "Export" pos:[176,368] width:112 height:24
	button btnAbort "Cancel" pos:[296,368] width:80 height:24
	--	-------------------------------------------------------------------
	fn displayAnim num =
	--	Fills the fields of the currently selected (num) animation data
	--	Disbles the controls if there is no animation data (aList is an empty array)
	--	-------------------------------------------------------------------
	(
		if (num == 0) then
		(
			edtAnimName.enabled = spnAnimStart.enabled = spnAnimEnd.enabled = rdoSkelType.enabled = OFF
			ddlBonesSet.enabled = btnAnimDel.enabled = btnAnimUp.enabled = btnAnimDn.enabled = OFF
			btnAnimRange.enabled = OFF
			edtAnimName.text = ""
			spnAnimStart.value = 0
			spnAnimEnd.value = 100
			return()
		)
		else
		(
			edtAnimName.enabled = spnAnimStart.enabled = spnAnimEnd.enabled = rdoSkelType.enabled = ON
			btnAnimDel.enabled = btnAnimUp.enabled = btnAnimDn.enabled = btnAnimRange.enabled = ON
		)
		lbxAnims.selection = num
		edtAnimName.text = aList[num][1]
		spnAnimStart.value = aList[num][2]
		spnAnimEnd.value = aList[num][3]
		if aList[selectedAnimIdx][4] then
		(
			rdoSkelType.state = 2
			ddlBonesSet.enabled = ON
			nSet = findItem nssList aList[selectedAnimIdx][5]
			if (nSet != 0) then ddlBonesSet.selection = nSet
		)
		else
		(
			rdoSkelType.state = 1
			ddlBonesSet.enabled = OFF
			ddlBonesSet.selection = 1
		)
	)
	--	-------------------------------------------------------------------
	fn displayAnimList =
	--	Fills the animations list listbox
	--	-------------------------------------------------------------------
	(
		lbxAnims.items = for a in aList collect ((a[2] as string) + "-" + (a[3] as string) + "   " + a[1] )
	)
	--	-------------------------------------------------------------------
	fn getChildren nodlist &fullList =
	--	Lists (to the fullList array) every nodes and their descending
	--	hierarchies listed in the nodList array.
	--	nodList : Array of the root nodes
	--	fullList : empty array (filled with the result)
	--	-------------------------------------------------------------------
	(
		for n in nodList do
		(
			append fulllist n
			getChildren n.children &fullList
		)
	)
	--	-------------------------------------------------------------------
	fn getMatIndex mname =
	--	Returns as an integer the index written at the end of the given 'mname' string
	--	Used in the CAL3D Material names ("<material_name> [<index>]")
	--	-------------------------------------------------------------------
	(
		if mname[mname.count]!="]" then return -1
		idx = mname.count
		while (idx>0) and (mname[idx]!="[") do idx-=1
		index = substring mname (idx+1) (mname.count - idx - 1)
		return (index as integer)
	)

	--	-------------------------------------------------------------------
	fn setMatIndex mname index =
	--	Returns the 'mname' string ending with " [<index>]"
	--	Used in the CAL3D Material names ("<material_name> [<index>]")
	--	-------------------------------------------------------------------
	(
		idx = mname.count
		if mname[idx]=="]" then
		(
			while (idx>0) and (mname[idx]!="[") do idx-=1
			idx = amax #(1, idx-1)
			while (idx>0) and (mname[idx]==" ") do idx-=1
		)
		return ((substring mname 1 idx) + " [" + (index as string) + "]")
	)

	--	-------------------------------------------------------------------
	fn loadConfig =
	--	Loads the general Cal3D_Export script configuration
	--	-------------------------------------------------------------------
	(
		lblViewerPath.text = getINIsetting configFile "Directories" "viewerPath"
		tmp = getINIsetting configFile "Directories" "Format"
		if tmp=="" then tmp="1"
		rdoFormat.state = tmp as Integer
		chkTransform.checked = (getINIsetting configFile "Options" "Transform") == "true"
		tmp = (getINIsetting configFile "Options" "Scale") as Float
		spnScale.value = if (tmp != 0.0) then tmp else 1.0
		spnSRate.value = (getINIsetting configFile "Options" "SamplingRate") as integer
	)
	--	-------------------------------------------------------------------
	fn saveConfig =
	--	Saves the general Cal3D_Export script configuration
	--	-------------------------------------------------------------------
	(
		setINIsetting configFile "Directories" "viewerPath" lblViewerPath.text
		setINIsetting configFile "Directories" "Format" (rdoFormat.state as string)
		setINIsetting configFile "Options" "Transform" (chkTransform.checked as String)
		setINIsetting configFile "Options" "Scale" (spnScale.value as string)
		setINIsetting configFile "Options" "SamplingRate" (spnSRate.value as string)
	)
	--	-------------------------------------------------------------------
	fn loadExportParam =
	--	Loads the in-file Cal3D_Export local configuration :
	--	. Used Named-Selection-Sets (Objects, Skeleton roots)
	--	. Parameters
	--	. Animation entries : List of {Name / Start / End / Bones set} items
	--	Fills the aList array with the animation data
	--	-------------------------------------------------------------------
	(
		aList = #()

		lblExportDir.text = backslash (getDir #Export)
		cal3DDataNode = getNodeByName cal3DDataNodeName
		if (cal3DDataNode==undefined) then return #()
		sbuff = getUserPropBuffer cal3DDataNode
		if (sbuff=="") then return #()
		if ((getStringKey sbuff "general" "name") != "cal3D") then
		(
			MessageBox ("Error, wrong User Properties format placed in the node "+cal3DDataNodeName)
			return #()
		)
		ver = (getStringKey sbuff "general" "version") as float
		if (ver != cal3DDataVersion) then
		(
			-- Different cal3D data versions
		)
		else
		(
			tmp = getStringKey sbuff "general" "exportDir"
			if (tmp=="") then tmp=getDir #Export
			lblExportDir.text = backSlash tmp
			chkCreateCFG.checked = (getStringKey sbuff "general" "generateCFG")== "true"
			chkPreviewCFG.checked = (getStringKey sbuff "general" "LaunchPreview")== "true"

			--	Get the meshes set
			nssn = getStringKey sbuff "export" "meshSet"
			ddlObjSet.selection = if ((idx = findItem nssList nssn) != 0) then idx else 1
			chkIndexMat.checked = (getStringKey sbuff "export" "ReIndexMat")== "true"
			chkLOD.checked = (getStringKey sbuff "export" "LOD")== "true"
			chkSprings.checked = (getStringKey sbuff "export" "Springs")== "true"

			--	Get the springs set
			nssn = getStringKey sbuff "export" "springSet"
			ddlSprings.selection = if ((idx = findItem nssList nssn) != 0) then idx else 1

			--	Get the skeleton roots set
			nssn = getStringKey sbuff "export" "skelSet"
			ddlSkelSet.selection = if ((idx = findItem nssList nssn) != 0) then idx else 1

			--	Get the animations list
			num = (getStringKey sbuff "export" "anmNumber") as integer
			for i=1 to num do
			(
				anmName = getStringKey sbuff ("anim" + i as string) "Name"
				anmStart = (getStringKey sbuff ("anim" + i as string) "Start") as integer
				anmEnd = (getStringKey sbuff ("anim" + i as string) "End") as integer
				anmUseBSet = (getStringKey sbuff ("anim" + i as string) "useBonesSet") == "true"
				anmBSet = getStringKey sbuff ("anim" + i as string) "BonesSet"
				append aList #(anmName, anmStart, anmEnd, anmUseBSet, anmBSet)
			)

			--	Get the Max Bones parameters
			spnMaxBones.value = (getStringKey sbuff "general" "MaxBones") as integer
			spnMinThres.value = (getStringKey sbuff "general" "MinThreshold") as float
		)
	)
	--	-------------------------------------------------------------------
	fn saveExportParam =
	--	Saves the in-file Cal3D_Export local configuration
	--	Makes 3DSMax need to save the scene
	--	-------------------------------------------------------------------
	(
		sbuff = ""
		setStringKey &sbuff "general" "name" "cal3D"
		setStringKey &sbuff "general" "version" (cal3DDataVersion as string)
		setStringKey &sbuff "general" "exportDir" lblExportDir.text
		setStringKey &sbuff "general" "generateCFG" (chkCreateCFG.checked as string)
		setStringKey &sbuff "general" "LaunchPreview" (chkPreviewCFG.checked as string)
		setStringKey &sbuff "general" "MaxBones" (spnMaxBones.value as string)
		setStringKey &sbuff "general" "MinThreshold" (spnMinThres.value as string)

		setStringKey &sbuff "export" "meshSet" ddlObjSet.selected
		setStringKey &sbuff "export" "ReIndexMat" (chkIndexMat.checked as string)
		setStringKey &sbuff "export" "LOD" (chkLOD.checked as string)
		setStringKey &sbuff "export" "Springs" (chkSprings.checked as string)
		setStringKey &sbuff "export" "springSet" ddlSprings.selected

		setStringKey &sbuff "export" "skelSet" ddlSkelSet.selected
		setStringKey &sbuff "export" "anmNumber" (aList.count as string)
		for i=1 to aList.count do
		(
			setStringKey &sbuff ("anim" + i as string) "Name" aList[i][1]
			setStringKey &sbuff ("anim" + i as string) "Start" (aList[i][2] as string)
			setStringKey &sbuff ("anim" + i as string) "End" (aList[i][3] as string)
			setStringKey &sbuff ("anim" + i as string) "useBonesSet" (aList[i][4] as string)
			val = if aList[i][4] then aList[i][5] else ""
			setStringKey &sbuff ("anim"+i as string) "BonesSet" val
		)
		cal3DDataNode = getNodeByName cal3DDataNodeName
		if (cal3DDataNode==undefined) then
		(
			cal3DDataNode = point name:cal3DDataNodeName
			cal3DDataNode.isFrozen=true
		)
		setUserPropBuffer cal3DDataNode sbuff
		setSaveRequired ON
	)
	--	-------------------------------------------------------------------
	fn exportData =
	--	Export the skeleton / mesh / animations files
	--	Returns an empty string in case of succes
	--	Returns an error message string in case of failure
	--	-------------------------------------------------------------------
	(
		oldCommandPanelTaskMode = getCommandPanelTaskMode()

		--	Need the TimeSlider on frame 0
				oldAnimationRange = animationRange
				animationRange = interval 0f animationRange.end
				oldSliderTime = sliderTime
				sliderTime = 0f

		--	Export every object
		local expType = if rdoFormat.state==1 then "C" else "X"
		local exportDir = lblExportDir.text
		try ( objSet = for obj in selectionSets[ddlObjSet.selected] collect obj )
		catch ( return "The Named Selection Set that lists the objects is invalid" )
		if chkSprings.checked then
		(
			try ( for obj in selectionSets[ddlSprings.selected] where ((finditem objSet obj)==0) do append objSet obj )
			catch ( return "The Named Selection Set that lists the cloth objects is invalid" )
		)

		--	Skeleton
		skeletonfile = exportDir + "Skeleton."+expType+"SF"
		try ( SRootsSet = for obj in selectionSets[ddlSkelSet.selected] collect obj )
		catch ( return "The Named Selection Set that lists the skeleton roots is invalid" )

		-- CJF: turn on figure mode!
		SRootsSet[1].transform.controller.figuremode = true

		res = exportcalskel skeletonfile SRootsSet off transform:chkTransform.checked
		case res of
		(
			(-2) : return "An Exception occured in the ExportSkeleton plugin function"
			(-1) : return "ExportSkeleton didn't work"
			 1 : return ("Verify the path for the skeleton file :\n" + skeletonFile)
			 2 : return "The Named Selection Set that lists the skeleton roots is empty"
			 3 : return "The Named Selection Set that lists the skeleton roots contains invalid references"
		)

		--	Mat & Mesh
		num=0
		for obj in objSet do
		(
			--	Filename validity verification
			wrongChars = "\\/:*?\"<>|"
			for w=1 to wrongChars.count do
			(
				if (findString obj.name wrongChars[w] != undefined) then
					return ("The following object has a filename-related invalid name :	\n"+obj.name+"\nIt should not contain any of these characters "+wrongChars)
			)

			--	Re-index materials
			mat = obj.material
			case (classof mat) of
			(
				StandardMaterial:
				(
					if chkIndexMat.checked then
					(
						mat.name = setMatIndex mat.name num
						num+=1
						--	Export material
						materialFile = exportDir + obj.name + "." + expType + "RF"
						res = exportcalmat materialFile mat
					)
				)
				MultiMaterial:
				(
					if chkIndexMat.checked then
					(
						for i=1 to mat.materialList.count do
						(
							mat.materialList[i].name = setMatIndex mat.materialList[i].name num
							num+=1

							--	Export material
							materialFile = exportDir + obj.name + "_" + (i as String) + "." + expType + "RF"
							res = exportcalmat materialFile mat.materialList[i]
							if (res!=0) then exit
						)
					)
				)
				default:
				(
					return "The material assigned to "+obj.name+" is not a Standard- nor a Multi- material"
				)
			)
			case res of
			(
				(-2) : return "An Exception occured in the ExportMaterial plugin function"
				(-1) : return "ExportMaterial didn't work"
				 1 : return ("Verify the path for the material file : \n" + materialFile)
				 2 : return "No good material"	--	already checked before the export (will never appear)
				 3 : return "No good material"	--	already checked before the export (will never appear)
			)

			--	Export Mesh
			meshFile = exportDir + obj.name + "."+expType+"MF"

			if (getCommandPanelTaskMode() == #modify) then
				setCommandPanelTaskMode mode:#create

			isaCloth = chkSprings.checked and ((findItem selectionSets[ddlSprings.selected] obj)!=0)
			res = exportcalmesh meshfile skeletonfile obj spnMaxBones.value spnMinThres.value chkLOD.checked isaCloth  transform:chkTransform.checked
			case res of
			(
				(-2) : return "An Exception occured in the ExportMesh plugin function"
				(-1) : return "ExportMesh didn't work"
				 1 : return ("Verify the path for the mesh file : \n" + meshFile)
				 2 : return "The skeleton file is not defined"
				 3 : return ("The skeleton file is not a valid file : \n" + skeletonFile)
				 4 : return "The number of bones by vertex is null or negative"
				 5 : return "The min weight threshold is negative"
				 6 : return "The Names Selection Set for the objects cointains invalid mesh nodes"
				 7 : return "The Names Selection Set for the objects cointains invalid mesh nodes"
			)
		)

		-- CJF: turn off figure mode
		SRootsSet[1].transform.controller.figuremode = false

		--	Animations
		for anm in aList do
		(
			local bonesList
			animationFile = exportDir + anm[1] + "."+expType+"AF"
			if anm[4] then
			(
				bonesList = for obj in selectionSets[anm[5]] collect obj
			)
			else
			(
				bonesList = #()
				getChildren SRootsSet &bonesList
			)

			res = exportcalanim animationFile skeletonfile boneslist anm[2] anm[3] spnSRate.value frameRate transform:chkTransform.checked
			case res of
			(
				(-2) : return "An Exception occured in the ExportAnimation plugin function"
				(-1) : return "ExportAnimation didn't work"
				 1 : return ("Verify the path for the material file : \n" + meshFile)
				 2 : return "The skeleton file is not defined"
				 3 : return ("The skeleton file is not a valid file : \n" + skeletonFile)
				 4 : return "The Names Selection Set that lists the bones is empty"
				 5 : return "The Start frame is negative"
				 6 : return "The End frame is negative"
				 7 : return "The Start frame is greater than the End frame"
				 8 : return "The offset (sample rate) is negative"
				 9 : return "The framerate is negative"
				10 : return "The Names Selection Set that lists the bones contains invalid nodes"
			)
		)

		--	Set the environment back to its former state
		animationrange = oldAnimationRange
		sliderTime = oldSliderTime
		if (oldCommandPanelTaskMode != getCommandPanelTaskMode()) then
			setCommandPanelTaskMode mode:oldCommandPanelTaskMode
		return ""
	)
	--	-------------------------------------------------------------------
	fn exportDataCheck =
	--	Calls the exportData process
	--	If no error is returned, creates the cal3D.cfg file and launches the viewer
	--	-------------------------------------------------------------------
	(
		--	Export the data files and check for an error
		err = exportData()
		if (err != "") then
		(
			MessageBox ("Error during the export :\n" + err + "\n\nProcess aborted") title:"CAL3D Export"
			return()
		)
		--	Generate the cal3D.CFG file
		if chkCreateCFG.checked then
		(
			f = createFile (lblExportDir.text + "cal3d.cfg")
			if (f==undefined) then
			(
				MessageBox ("Cannot create the config file\n"+f) title:"CAL3D Export"
				return()
			)
			--	XML or Binary
			extens = if rdoFormat.state==1 then "C" else "X"

			--	Objects to be listed (meshes and clothes)
			nods = for o in selectionSets[ddlObjSet.selected] collect o
			if chkSprings.checked then
				for o in selectionSets[ddlSprings.selected] where ((finditem nods o)==0) do append nods o

			format "################################################\n" to:f
			format "#\n" to:f
			format "# Cal3d cfg File\n" to:f
			format "#\n" to:f
			format "################################################\n" to:f
			format "\n" to:f
			format "scale=%\n" spnScale.value to:f
			format "\n" to:f
			format "################# Skeleton #################\n" to:f
			format "skeleton=Skeleton.%SF\n" extens to:f
			format "\n" to:f
			format "################# Meshes #################\n" to:f
			for meshnod in nods do
				format "mesh=%.%MF\n" meshnod.name extens to:f
			format "\n" to:f
			format "################# Animations #################\n" to:f
			for anm in aList do
				format "animation=%.%AF\n" anm[1] extens to:f
			format "\n" to:f
			format "################# Materials #################\n" to:f
			for meshnod in nods do
			(
				mat = meshnod.material
				case (classof mat) of
				(
					StandardMaterial:
					(
						format "material=%.%RF\n" meshnod.name extens to:f
					)
					MultiMaterial:
					(
						for i=1 to mat.materialList.count do
						(
							format "material=%_%.%RF\n" meshnod.name i extens to:f
						)
					)
					default:
					(
						MessageBox ("Grumpfffff, no good material for "+meshnod.name)
					)
				)
			)
			format "\n" to:f
			close f
		)

		--	Preview the cal3D.CFG file
		if chkPreviewCFG.checked then
		(
			local f, viewBatch="c:\\cal3Dpre.bat"
			if ((f = createFile viewBatch)==undefined) then
			(
				MessageBox "Error : Can't create the batch file "+viewBatch+" to launch the mini-viewer" title:"Cal3D Export"
				return()
			)
			delimiter1 = ""
			delimiter2 = ""
			if ((findstring lblExportDir.text " ") != undefined) then delimiter1="\""
			if ((findstring lblViewerPath.text " ") != undefined) then delimiter2="\""
			format "%:\n" lblExportDir.text[1] to:f
			format "cd %%%\n" delimiter1 lblExportDir.text delimiter1 to:f
			format "%%% cal3d.cfg\n" delimiter2 lblViewerPath.text delimiter2 to:f
			close f
			DOSCommand viewBatch
			deleteFile viewBatch
		)
	)


	--	-------------------------------------------------------------------
	--	Interface events
	--	-------------------------------------------------------------------

	on cal3DexportR open do
	(
		--	Load genral configuration
		loadConfig()
		if not (doesFileExist lblViewerPath.text) then
		(
			lblViewerPath.text = ""
			chkPreviewCFG.enabled=FALSE
		)
		--	Named Selection Sets
		nssList = for i=1 to getNumNamedSelSets() collect (getNamedSelSetName i)
		ddlObjSet.items = nssList
		ddlSprings.items = nssList
		ddlSkelSet.items = nssList
		ddlBonesSet.items = nssList
		--	Animations list
		loadExportParam()
		lbxAnims.items = for a in aList collect a[1]
		displayAnimList()
		--	Show the 1st anim parameters
		selectedAnimIdx = amin #(1, aList.count)
		displayAnim selectedAnimIdx
	)
	on lbxAnims selected val do
	(
		displayAnim (selectedAnimIdx = val)
	)
	on lbxAnims doubleClicked val do
	(
		animationrange = interval aList[val][2] aList[val][3]
	)
	on btnAnimAdd pressed do
	(
		append aList #("Unnamed", animationrange.Start.frame as integer, animationrange.End.frame as integer, OFF, "")
		displayAnim (selectedAnimIdx = aList.count)
		displayAnimList()
	)
	on btnAnimDel pressed do
	(
		deleteItem aList selectedAnimIdx
		selectedAnimIdx = amin selectedAnimIdx aList.count
		displayAnim selectedAnimIdx
		displayAnimList()
	)
	on btnAnimUp pressed do
	(
		if (selectedAnimIdx <= 1) then return()
		elt = for i=1 to 5 collect aList[selectedAnimIdx][i]
		aList[selectedAnimIdx] = for i=1 to 5 collect aList[selectedAnimIdx-1][i]
		aList[selectedAnimIdx-1] = for i=1 to 5 collect elt[i]
		selectedAnimIdx -= 1
		displayAnim selectedAnimIdx
		displayAnimList()
	)
	on btnAnimDn pressed do
	(
		if (selectedAnimIdx >= aList.count) then return()
		elt = for i=1 to 5 collect aList[selectedAnimIdx][i]
		aList[selectedAnimIdx] = for i=1 to 5 collect aList[selectedAnimIdx+1][i]
		aList[selectedAnimIdx+1] = for i=1 to 5 collect elt[i]
		selectedAnimIdx += 1
		displayAnim selectedAnimIdx
		displayAnimList()
	)
	on edtAnimName changed val do
	(
		aList[selectedAnimIdx][1] = val
		displayAnimList()
	)
	on spnAnimStart changed val do
	(
		aList[selectedAnimIdx][2] = val
		displayAnimList()
	)
	on btnAnimRange pressed do
	(
		spnAnimStart.value = aList[selectedAnimIdx][2] = animationrange.start.frame as integer
		spnAnimEnd.value = aList[selectedAnimIdx][3] = animationrange.end.frame as integer
		displayAnimList()
	)
	on spnAnimEnd changed val do
	(
		aList[selectedAnimIdx][3] = val
		displayAnimList()
	)
	on rdoSkelType changed state do
	(
		aList[selectedAnimIdx][4] = (state==2)
		if ((findItem nssList aList[selectedAnimIdx][5])==0) then
			aList[selectedAnimIdx][5] = ddlBonesSet.items[1]
		displayAnim selectedAnimIdx
		displayAnimList()
	)
	on ddlBonesSet selected val do
	(
		aList[selectedAnimIdx][5] = ddlBonesSet.selected
	)
	on btnViewerPath pressed do
	(
		tmp = getOpenFilename filename:lblViewerPath.text types:"*.exe|*.exe|"
		if (tmp != undefined) then
			lblViewerPath.text = tmp
		chkPreviewCFG.enabled = doesFileExist lblViewerPath.text
	)
	on btnExportDir pressed do
	(
		tmp = getSavePath caption:"Export directory"
		if (tmp != undefined) then
		(
			lblExportDir.text = backSlash tmp
		)
	)
	on btnSaveConfig pressed do
	(
		saveConfig()
		saveExportParam()
		destroyDialog cal3DexportR
	)
	on btnExport pressed do
	(
		saveConfig()
		saveExportParam()
		exportDataCheck()
		destroyDialog cal3DexportR
	)
	on btnAbort pressed do
	(
		destroyDialog cal3DexportR
	)
)

--	-------------------------------------------------------------------
fn cal3DExportFull =
--	Checks the presence of Named Selection Sets before calling the interface
--	(Other pre-requisites are handled by the dialog window events)
--	-------------------------------------------------------------------
(
	--	Test the selection sets
	if (getNumNamedSelSets()==0) then
	(
		MessageBox "This utility works with Named Selection Sets.\nDefine Named Selection Sets that represent :	\n	. the objects to export,\n	. the skeleton root(s),\n	. the animated bones\nbefore using this tool."
		return()
	)
	--	Everything seems fine, continue
	createDialog cal3DexportR
)


--	-------------------------------------------------------------------
--	MacroScript definition
--	-------------------------------------------------------------------
macroScript exportCAL3D category:"File" buttonText:"CAL3D Export" toolTip:"CAL3D - Export the selected skinned objects" icon:#("Maintoolbar", 93)
(
	fname = scriptsPath + "export_cal3D\\export_cal3D.ms"
	if keyboard.shiftpressed then
		edit fname
	else
		fileIn fname
)


cal3DExportFull()

