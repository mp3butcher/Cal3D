//
// Copyright (C) 2004 Mekensleep
//
// Mekensleep
// 24 rue vieille du temple
// 75004 Paris
//       licensing@mekensleep.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

//----------------------------------------------------------------------------//
// Exporter.cpp                                                               //
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
#include "Exporter.h"
#include "MaxInterface.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseNode.h"
#include "SkeletonExportSheet.h"
#include "AnimationExportSheet.h"
#include "MeshExportSheet.h"
#include "MaterialExportSheet.h"
#include "MeshCandidate.h"
#include "SubmeshCandidate.h"
#include "VertexCandidate.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"
#include "MaxAnimationExport.h"


bool CMaxInterface::ExportAnimationFromMaxscriptCall(const std::string& strFilename, void* _AnimExportParams)
{
	if (!_AnimExportParams)
	{
		theExporter.SetLastError("_AnimExportParams pointer is null.", __FILE__, __LINE__);
		return false;
	}

	AnimExportParams*	param = reinterpret_cast<AnimExportParams*>(_AnimExportParams);


	// build a skeleton candidate
	CSkeletonCandidate skeletonCandidate;

	//Remove user interface
	/*// show export wizard sheet
	CAnimationExportSheet sheet("Cal3D Animation Export", GetMainWnd());
	sheet.SetSkeletonCandidate(&skeletonCandidate);
	sheet.SetAnimationTime(GetStartFrame(), GetEndFrame(), GetCurrentFrame(), GetFps());
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;
	*/

	//Following block replaces the user interface interactions
	{
		// create the skeleton candidate from the skeleton file
		if(! skeletonCandidate.CreateFromSkeletonFile(param->m_skeletonfilepath))
		{
			AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;
		}	

		//Set all bones in our array of nodes selected
		std::vector<CBoneCandidate *>& vectorBoneCandidate = skeletonCandidate.GetVectorBoneCandidate();

		int NumElemInTabMaxscript = param->m_tabbones.Count();

		// Select bone candidates that are in our array
		int idx = 0;
		const int numelems = vectorBoneCandidate.size();
		for (idx = 0;idx<numelems;idx++)
		{
			CBoneCandidate * bonecandidate = vectorBoneCandidate[idx];
			if (! bonecandidate)return false;

			//Deselect it
			bonecandidate->SetSelected(false);
			
			int j;
			for (j=0;j<NumElemInTabMaxscript;j++)
			{
				std::string bcname		= bonecandidate->GetNode()->GetName();
				std::string	bonename	= param->m_tabbones[j]->GetName();

				if (bcname == bonename)
				{
					//This bone candidate is in the array passed by maxscript, so select it.
					bonecandidate->SetSelected(true);
					break;
				}
			}
		}
	}

  CalCoreAnimationPtr coreAnimation = theExporter.ExtractAnimation(
      skeletonCandidate,
      param->m_startframe,
      param->m_endframe,
      param->m_frameoffset,
      GetFps(),
      param->m_framerate);
  if (!coreAnimation)
  {
    return false;
  }

  // save core animation to the file
	if(!CalSaver::saveCoreAnimation(strFilename, coreAnimation.get()))
	{
		theExporter.SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		return false;
	}

	return true;
}
