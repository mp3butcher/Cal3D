/*	-----------------------------------------------------------------------------  */
--	Script part to be inserted in the install script msrun.ms
--	Do not modify
/*	-----------------------------------------------------------------------------  */

global MZPsetupError=OFF
clearListener()

----------------------------------------------------------
fn recursMove source target =
----------------------------------------------------------
(
	local gf = getFiles (source+"*.*")
	for itm in gf do
	(
		local tgt = target + (filenameFromPath itm)
		if (doesFileExist tgt) then
		(
			if (getFileAttribute tgt #readOnly) then setFileAttribute tgt #readOnly OFF
			if not (deleteFile tgt) then
			(
				format "Error : The following file cannot be replaced : %\n" tgt
				MZPsetupError=ON
			)
		)
		if not (copyFile itm tgt) then
		(
			format "Error : The following file cannot be installed in the target directory %\n" tgt
			MZPsetupError=ON
		)
	)
	
	local gd = getDirectories (source+"*.*")
	local len = source.count + 1
	for itm in gd do
	(
		tgt = target + (substring itm len (itm.count - len) )
		if (doesFileExist tgt) then
		(
			if not (getFileAttribute tgt #directory) then
			(
				format "Error : The following directory could not be created (a file with the same name already exists)\n%\n" tgt
				MZPsetupError=ON
				continue
			)
		)
		else
		(
			makeDir tgt
		)
		recursMove itm (tgt+"\\")
	)
)

----------------------------------------------------------
fn myTreemove source target =
----------------------------------------------------------
(
	local realSource = sysInfo.tempDir + "\\"+source+"\\#"+target+"\\"
	local realTarget = getDir (target as name)
	if (realTarget[realTarget.count]!="\\") then realTarget+="\\"
	
	recursMove realSource realTarget
)

----------------------------------------------------------
fn bkslash str =
----------------------------------------------------------
(
	local dpath=copy str
	if dpath[dpath.count]!="\\" then dpath+="\\"
	return dpath
)


----------------------------------------------------------
fn SPReportErrors = 
----------------------------------------------------------
(
	if MZPsetupError then
	(
		MessageBox "Installation error(s)\n\nThe package installation could not be totally accomplished,\ncheck the listener output for a list of the errors that occured.\n" title:"MZP setup"
	)
)

/*	-----------------------------------------------------------------------------  */
/*	-----------------------------------------------------------------------------  */

myTreemove "mzp_jtskgadkbr" "scripts"
fileIn ((bkslash (getDir #scripts)) + "export_cal3D\\export_cal3D.ms")
SPReportErrors()

