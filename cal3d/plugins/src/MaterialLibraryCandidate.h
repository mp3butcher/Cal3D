//----------------------------------------------------------------------------//
// MaterialLibraryCandidate.h                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MATERIAL_LIBRARY_CANDIDATE_H
#define MATERIAL_LIBRARY_CANDIDATE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMaterialCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaterialLibraryCandidate
{
// member variables
protected:
	std::vector<CMaterialCandidate *> m_vectorMaterialCandidate;
	CMaterialCandidate *m_pSelectedMaterialCandidate;

// constructors/destructor
public:
	CMaterialLibraryCandidate();
	virtual ~CMaterialLibraryCandidate();

// member functions
public:
	void Clear();
	bool CreateFromInterface();
	CMaterialCandidate *GetSelectedMaterialCandidate();
	std::vector<CMaterialCandidate *>& GetVectorMaterialCandidate();
	void SetSelectedMaterialCandidate(CMaterialCandidate *pSelectedMaterialCandidate);

	bool CreateFromInterfaceFromMaxscriptCall();
};

#endif

//----------------------------------------------------------------------------//
