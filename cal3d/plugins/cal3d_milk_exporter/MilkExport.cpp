//----------------------------------------------------------------------------//
// MilkExport.cpp                                                             //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "MilkExport.h"
#include "Exporter.h"
#include "MilkInterface.h"
#include "SelectionDialog.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMilkExport::CMilkExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMilkExport::~CMilkExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

int CMilkExport::GetType()
{
  return cMsPlugIn::eTypeExport;
}

const char *CMilkExport::GetTitle()
{
  return "Cal3D...";
}


int CMilkExport::Execute(msModel *pModel)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // check if the model is valid
  if(!pModel) return -1;

  // query the file type
	CSelectionDialog dlg;
  if(dlg.DoModal() != IDOK) return -1;

  // build the file type description
  std::string strType;
  std::string strExtension;
  switch(dlg.GetType())
  {
  case CSelectionDialog::TYPE_SKELETON:
    strType = "Cal3D Skeleton Files (*.csf)|*.csf|All Files (*.*)|*.*||";
    strExtension = ".csf";
    break;
  case CSelectionDialog::TYPE_ANIMATION:
    strType = "Cal3D Animation Files (*.caf)|*.caf|All Files (*.*)|*.*||";
    strExtension = ".caf";
    break;
  case CSelectionDialog::TYPE_MESH:
    strType = "Cal3D Mesh Files (*.cmf)|*.cmf|All Files (*.*)|*.*||";
    strExtension = ".cmf";
    break;
  case CSelectionDialog::TYPE_MATERIAL:
    strType = "Cal3D Material Files (*.crf)|*.crf|All Files (*.*)|*.*||";
    strExtension = ".crf";
    break;
  default:
    return -1;
    break;
  }

  // query the filename
  CFileDialog fileDlg(FALSE, NULL, NULL, OFN_EXPLORER, strType.c_str());
  if(fileDlg.DoModal() != IDOK) return -1;

  // get the full filename
  std::string strFilename;
  strFilename = fileDlg.GetPathName();

  // check if the filename is valid
  if(fileDlg.GetFileName().IsEmpty())
  {
    AfxMessageBox("Invalid filename", MB_OK | MB_ICONEXCLAMATION);
    return -1;
  }

  // add the default extension if necessary
  if(fileDlg.GetFileExt().IsEmpty())
  {
    strFilename += strExtension;
  }

	// create an export interface for milkshape
	CMilkInterface milkInterface;
	if(!milkInterface.Create(pModel))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	// create an exporter instance
	if(!theExporter.Create(&milkInterface))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

  // export the specific type
  switch(dlg.GetType())
  {
  case CSelectionDialog::TYPE_SKELETON:
	  if(!theExporter.ExportSkeleton(strFilename))
	  {
		  AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		  return -1;
	  }
    break;
  case CSelectionDialog::TYPE_ANIMATION:
	  if(!theExporter.ExportAnimation(strFilename))
	  {
		  AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		  return -1;
	  }
    break;
  case CSelectionDialog::TYPE_MESH:
	  if(!theExporter.ExportMesh(strFilename))
	  {
		  AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		  return -1;
	  }
    break;
  case CSelectionDialog::TYPE_MATERIAL:
	  if(!theExporter.ExportMaterial(strFilename))
	  {
		  AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		  return -1;
	  }
    break;
  default:
    return -1;
    break;
  }
	// export the skeleton

  return 0;
}

//----------------------------------------------------------------------------//
