//
// Copyright (C) 2003 Thomas Cowell 
// 
// File: cal3dExportCmd.cpp
//
// MEL Command: cal3dExport
//
// Author: Maya SDK Wizard
//

// Includes everything needed to register a simple MEL command with Maya.
// 
#include <maya/MSimple.h>

// Use helper macro to register a command with Maya.  It creates and
// registers a command that does not support undo or redo.  The 
// created class derives off of MPxCommand.
//
DeclareSimpleCommand( cal3dExport, "Thomas Cowell", "4.5");

MStatus cal3dExport::doIt( const MArgList& args )
//
//	Description:
//		implements the MEL cal3dExport command.
//
//	Arguments:
//		args - the argument list that was passes to the command from MEL
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement.
//
{
	MStatus stat = MS::kSuccess;

	// Since this class is derived off of MPxCommand, you can use the 
	// inherited methods to return values and set error messages
	//
	setResult( "cal3dExport command executed!\n" );

	return stat;
}
