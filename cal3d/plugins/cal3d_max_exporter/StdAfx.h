//----------------------------------------------------------------------------//
// StdAfx.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef STDAFX_H
#define STDAFX_H

//----------------------------------------------------------------------------//
// Defines                                                                    //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#define VC_EXTRALEAN
#pragma warning(disable : 4786)
#endif

#define WINVER 0x0400

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>

#include "max.h"
#include "maxscrpt/maxscrpt.h"
#include "bipexp.h"
#include "phyexp.h"
#include "decomp.h"

#if MAX_RELEASE >= 4000
#include "iskin.h"
#endif

#include "cal3d/cal3d.h"

#include "resource.h"

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
