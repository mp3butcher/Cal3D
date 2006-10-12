/*===========================================================================*\
 | 
 |  FILE:	wM3.h
 |			Weighted Morpher for MAX R3
 |			Main header file
 | 
 |  AUTH:   Harry Denholm
 |			Copyright(c) Kinetix 1999
 |			All Rights Reserved.
 |
 |  HIST:	Started 22-5-98
 | 
\*===========================================================================*/


#ifndef __MORPHR3__H
#define __MORPHR3__H

#define MorphExport __declspec( dllexport )


#include <Max.h>
#include <istdplug.h>
#include <meshadj.h>
#include <modstack.h>
#include <imtl.h>
#include <texutil.h>
#include <stdmat.h>
#include <macrorec.h>
#include <vector>

#include "buildver.h"

#define IDS_LIBDESCRIPTION              1
#define ID_CANCEL                       2
#define IDS_CLASS_NAME                  3
#define IDS_GLOBAL_PARAMS               4
#define IDS_CHANNEL_PARAMS              5
#define IDS_CHANNEL_LIST                6
#define IDS_ADVANCED                    7
#define IDS_EMPTY_CHANNEL               8
#define IDS_MXCATEGORY                  9
#define IDS_SUBANIMPARAM                10
#define IDS_UPD_ON                      11
#define IDS_UPD_OFF                     12
#define IDS_DEL_ON                      13
#define IDS_DEL_OFF                     14
#define IDS_PC_MORPH                    15
#define IDS_NOCON                       16
#define IDS_ONCON                       17
#define IDS_MENUITEM1_PS                18
#define IDS_MENUITEM2_DC                19
#define IDS_MENUITEM3_RT                20
#define IDS_CAPTURED                    21
#define IDS_ACTIVE                      22
#define IDS_MENUNAME                    23
#define IDS_FREE                        24
#define IDS_CHANSEL                     25
#define IDS_OPMOVE                      26
#define IDS_OPSWAP                      28
#define IDS_OPOKAY                      29
#define IDS_OPWARN                      30
#define IDS_OPCANT                      31
#define IDS_LOADMULTI                   32
#define IDS_MULTIBTN                    33
#define IDS_NOFREE                      34
#define IDS_CANNOT                      35
#define IDS_LEGEND_DLG                  36
#define IDS_EXTRACT_GRAB_ERROR          37
#define IDS_EXTRACT_MOD_ERROR           38
#define IDS_MORPHMTL                    39
#define IDS_MTL_PARAM                   41
#define IDS_DS_NONE                     42
#define IDS_MTL_CNAME                   43
#define IDS_MTL_BASE                    44
#define IDS_MTL_MAPNAME                 45
#define IDS_MTL_BASENAME                46
#define IDS_MTL_MAPNAMEDOTS             47
#define IDS_MTL_NOTARG                  48
#define IDS_MTL_NOOBJ                   49
#define IDS_UNDOMCDEL                   50
#define IDS_PBN_USELIMITS               51
#define IDS_PBN_SPINMIN                 52
#define IDS_PBN_SPINMAX                 53
#define IDS_PBN_USESEL                  54
#define IDS_PBN_VALUEINC                55
#define IDS_PBN_AUTOLOAD                56
#define IDS_DEL_MARKER                  57
#define IDS_CYCLIC                      58
#define IDS_CHANNELS_MOVED              59
#define IDS_CYCLIC_MATERIAL             60
#define IDS_UNDO_COMPACT                61
#define IDS_UNDO_RELOAD                 62
#define IDS_MORPHMTL_OBJECT             63
#define IDS_MOVE_TARGETS                64
#define IDS_DELETE_TARGET               65
#define IDD_GLOBAL_PARAMS               101
#define IDD_GLOBAL_PARAMS_GMAX          102
#define IDD_CHANNEL_PARAMS              103
#define IDD_GLOBAL_PARAMS_PLASMA        104
#define IDD_CHANNEL_LIST                108
#define IDD_LEGEND                      109
#define IDR_CHANSUB                     112
#define IDD_MC_EXPORT                   113
#define IDD_MC_IMPORT                   114
#define IDD_ADVANCED                    115
#define IDD_CHANOP                      116
#define IDD_BINDMORPH                   117
#define IDD_NAMECAP                     119
#define IDB_MORPHPARAMS_MASK            120
#define IDB_MORPHPARAMS_ICON            121
#define IDB_SV_MORPH32_MASK             159
#define IDB_SV_MORPH16                  167
#define IDB_SV_MORPH16_MASK             168
#define IDB_SV_MORPH32                  169
#define IDC_UPDATETARGS                 1003
#define IDC_LOADMULT                    1005
#define IDC_P1                          1012
#define IDC_P2                          1013
#define IDC_P3                          1014
#define IDC_P4                          1015
#define IDC_P5                          1016
#define IDC_P6                          1017
#define IDC_P7                          1018
#define IDC_P8                          1019
#define IDC_P9                          1020
#define IDC_COMPACT                     1020
#define IDC_P10                         1021
#define IDC_MEMSIZE                     1021
#define IDC_COMPACTSTAT                 1022
#define IDC_LISTSCROLL                  1027
#define IDC_LISTFRAME                   1028
#define IDC_I1                          1031
#define IDC_I2                          1032
#define IDC_I3                          1033
#define IDC_LIMIT                       1034
#define IDC_I4                          1034
#define IDC_I5                          1035
#define IDC_I6                          1036
#define IDC_I7                          1037
#define IDC_I8                          1038
#define IDC_I9                          1039
#define IDC_I10                         1040
#define IDC_IC1                         1045
#define IDC_IC2                         1046
#define IDC_IC3                         1047
#define IDC_IC4                         1048
#define IDC_IC5                         1049
#define IDC_CHANNUM                     1051
#define IDC_LOAD                        1053
#define IDC_SAVE                        1054
#define IDC_CANCEL                      1055
#define IDD_MORPHMTL                    1055
#define IDC_DOSAVE                      1056
#define IDC_CMAP                        1058
#define IDC_ADD                         1059
#define IDC_ADD2                        1060
#define IDC_MARKERLIST                  1060
#define IDC_SAVEMARKER                  1061
#define IDC_DELMARKER                   1062
#define IDC_CHANNAME                    1063
#define IDC_PERFORMOP                   1064
#define IDC_ALL_DEACTIVATE              1064
#define IDC_PICK                        1066
#define IDC_CLIST                       1066
#define IDC_MAKE                        1067
#define IDC_CIMP                        1067
#define IDC_USESEL                      1068
#define IDC_SEL                         1068
#define IDC_UPDATETARGET                1069
#define IDC_SELNONE                     1069
#define IDC_DELETE                      1070
#define IDC_SELALL                      1070
#define IDC_STAT                        1070
#define IDC_EXTRACT                     1071
#define IDC_SELUSED                     1071
#define IDC_ZEROOTHERS                  1072
#define IDC_SELINVERT                   1072
#define IDC_DELETE2                     1072
#define IDC_AUTOLOAD                    1074
#define IDC_BIND                        1075
#define IDC_REFRESH                     1076
#define IDC_MORPHNAME                   1078
#define IDC_MODLIST                     1079
#define IDC_UPDATE1                     1082
#define IDC_UPDATE2                     1083
#define IDC_MTLASSIGN                   1083
#define IDC_UPDATE3                     1084
#define IDC_NEWNAME                     1085
#define IDC_USECHAN                     1086
#define IDC_ALL_ACTIVATE                1087
#define IDC_LISTFRAME2                  1088
#define IDC_TARGETLIST                  1089
#define IDC_BASE                        1096
#define IDC_MAX_EDIT                    1135
#define IDC_MAX_SPIN                    1136
#define IDC_MIN_EDIT                    1137
#define IDC_MIN_SPIN                    1138
#define IDC_CURVE_EDIT                  1139
#define IDC_CURVE_EDIT_SPIN		1140
#define IDC_TARGET_PERCENT_EDIT		1141
#define IDC_TARGET_PERCENT_SPIN		1142
#define IDC_PAGE                        1168
#define IDC_UP_DOWN                     1253
#define IDC_FE1                         1401
#define IDC_FE2                         1402
#define IDC_FE3                         1403
#define IDC_FE4                         1404
#define IDC_FE5                         1405
#define IDC_FE6                         1406
#define IDC_FE7                         1407
#define IDC_FE8                         1408
#define IDC_FE9                         1409
#define IDC_FE10                        1410
#define IDC_FS1                         1421
#define IDC_FS2                         1422
#define IDC_FS3                         1423
#define IDC_FS4                         1424
#define IDC_FS5                         1425
#define IDC_FS6                         1426
#define IDC_FS7                         1427
#define IDC_FS8                         1428
#define IDC_FS9                         1429
#define IDC_FS10                        1430
#define IDC_MORPH_MAT1                  1521
#define IDC_MORPH_MAT2                  1522
#define IDC_MORPH_MAT3                  1523
#define IDC_MORPH_MAT4                  1524
#define IDC_MORPH_MAT5                  1525
#define IDC_MORPH_MAT6                  1526
#define IDC_MORPH_MAT7                  1527
#define IDC_MORPH_MAT8                  1528
#define IDC_MORPH_MAT9                  1529
#define IDC_MORPH_MAT10                 1530
#define IDC_MORPH_BASE                  1531
#define IDC_V1                          1601
#define IDC_V2                          1602
#define IDC_V3                          1603
#define IDC_TEXON1                      9001
#define IDC_TEXON2                      9002
#define IDC_TEXON3                      9003
#define IDC_TEXON4                      9004
#define IDC_TEXON5                      9005
#define IDC_TEXON6                      9006
#define IDC_TEXON7                      9007
#define IDC_TEXON8                      9008
#define IDC_TEXON9                      9009
#define IDC_TEXON10                     9010
#define IDC_MNAME1                      9101
#define IDC_MNAME2                      9102
#define IDC_MNAME3                      9103
#define IDC_MNAME4                      9104
#define IDC_MNAME5                      9105
#define IDC_MNAME6                      9106
#define IDC_MNAME7                      9107
#define IDC_MNAME8                      9108
#define IDC_MNAME9                      9109
#define IDC_MNAME10                     9110
#define ID_CSM_MOVE                     40003
#define ID_CSM_SWAP                     40004
#define ID_MENU_TARGET_MOVEUP           40008
#define ID_MENU_TARGET_MOVEDOWN         40009
#define ID_MENU_TARGET_DELETED          40010
#define ID_MENU_PICK_TARGET             40011

static Class_ID M3MatClassID(0x4b9937e0, 0x3a1c3da4);
#define MR3_CLASS_ID		Class_ID(0x17bb6854, 0xa5cba2a3)
#define MR3_NUM_CHANNELS	100
#define MAX_TARGS			25

#define MR3_MORPHERVERSION	010


// Save codes for the morphChannel class
#define MR3_POINTCOUNT					0x0100
#define MR3_POINTCOUNT2					0x0101
#define MR3_SELARRAY					0x0110
#define MR3_NAME						0x0120
#define MR3_PARAMS						0x0130
#define MR3_POINTDATA_MP				0x0140
#define MR3_POINTDATA_MW				0x0150
#define MR3_POINTDATA_MD				0x0160
#define MR3_POINTDATA_MO				0x0170 
#define MR3_PROGRESSIVE_PARAMS			0x0180 
#define MR3_PROGRESSIVE_TARGET_PERCENT		0x0181
#define MR3_PROGRESSIVE_CHANNEL_PERCENT		0x0182
#define MR3_PROGRESSIVE_CHANNEL_CURVATURE	0x0183

#define MR3_TARGETCACHE					0x0190
#define MR3_TARGETCACHE_POINTS			0x0192

#define MR3_FILE_VERSION				0x0195

#define MR3_TARGET_CACHE_SUBCHUNK		0x0300

// Save codes for the MorphR3 class
#define MR3_MARKERNAME		0x0180
#define MR3_MARKERINDEX		0x0185
#define MR3_MC_CHUNK		0x0190
#define MR3_MC_SUBCHUNK		0x0200
#define MR3_FLAGS			0x0210



// paramblock index table
#define PB_OV_USELIMITS		0
#define PB_OV_SPINMIN		1
#define PB_OV_SPINMAX		2
#define PB_OV_USESEL		3
#define PB_AD_VALUEINC		4
#define PB_CL_AUTOLOAD		5


// Channel operation flags
#define OP_MOVE				0
#define OP_SWAP				1


// two handy macros to set cursors for busy or normal operation
#define UI_MAKEBUSY			SetCursor(LoadCursor(NULL, IDC_WAIT));
#define UI_MAKEFREE			SetCursor(LoadCursor(NULL, IDC_ARROW));
			

// Morph Material ui defines
#define NSUBMTL 10

// Updater flags
#define UD_NORM				0
#define UD_LINK				1

#define IDC_TARGET_UP		0x4000
#define IDC_TARGET_DOWN		0x4010

extern ClassDesc* GetMorphR3Desc();

#ifndef NO_MTL_MORPHER
extern ClassDesc* GetM3MatDesc();
#endif // NO_MTL_MORPHER

extern HINSTANCE hInstance;

TCHAR *GetString(int id);

//   SV Integration
// *----------------*
extern HIMAGELIST hIcons32, hIcons16;
extern COLORREF bkColor;
enum IconIndex {II_MORPHER};
void LoadIcons(COLORREF bkColor);
// *----------------*

class MorphR3;
class M3Mat;
class M3MatDlg;
class Restore_FullChannel;

// Dialog handlers
INT_PTR CALLBACK Legend_DlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Globals_DlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Advanced_DlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChannelParams_DlgProc	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChannelList_DlgProc	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK IMPORT_DlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EXPORT_DlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ChannelOpDlgProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK BindProc				(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK NameDlgProc			(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static Point3 junkpoint(0,0,0);
class morphChannel;

class TargetCache
{
public:
	long mNumPoints;
	INode *mTargetINode;
	std::vector<Point3> mTargetPoints;
	float mTargetPercent;

	TargetCache(){
		mNumPoints=0;
		mTargetINode=NULL;
		mTargetPercent = 0.0f;
	}

	TargetCache(const TargetCache &tcache);

	~TargetCache(){
		mTargetPoints.erase(mTargetPoints.begin(), mTargetPoints.end());
		mTargetPoints.resize(0);
	}

	void Clear(void){
		mTargetPoints.erase(mTargetPoints.begin(), mTargetPoints.end());
		mTargetPoints.resize(0);
		mNumPoints=0;
		mTargetINode=NULL;
	}

	operator=(const TargetCache &tcache); 

	operator=(const morphChannel &mchan);
	void Init( INode *nd);
	void Reset(void);
	const Point3 &GetPoint(const long &index) { 
		if(index>=0 && index<mNumPoints) return mTargetPoints[index]; 
		return junkpoint;
	}
	
	IOResult Save(ISave* isave);
	IOResult Load(ILoad* iload);
	INode *RefNode(void) { return mTargetINode; }
	
};


/*===========================================================================*\
 | Morph channel manager class
 | Storage for all the morph data needed
\*===========================================================================*/

class morphChannel
{
public:

	// Construct/Destruct
	~morphChannel();
	morphChannel();
	morphChannel(const morphChannel & from);

	MorphR3		*mp;

	float mCurvature;

	// Number of points
	int			mNumPoints;

	int iTargetListSelection;

	// Actual morphable points
	std::vector<Point3>		mPoints;
	std::vector<Point3>		mDeltas;
	std::vector<double>		mWeights;
	
	std::vector<TargetCache>	mTargetCache;

        // BitArray to check against for point selection
	BitArray	mSel;
	// INode that we use to update and reload from, if possible
	INode*		mConnection;
	
	// Name for the morph channel
	TSTR		mName;
	//
	int mNumProgressiveTargs;
	float mTargetPercent;

	// Various, non-animatable stuff
	// mActive is TRUE if the channel has data in it in some form
	// mModded is TRUE if the channel has been changed in SOME form,
	//		ie, had its name changed or similar
	BOOL		mActive, mModded, mUseLimit, mUseSel;
	float		mSpinmin,mSpinmax;

	// TRUE if the channel has been marked as bad. It will not be 
	// considered when building the morph results.
	BOOL		mInvalid;

	// Channel enabled/disabled
	BOOL		mActiveOverride;

	void InitTargetCache(const int &targnum, INode *nd){ mTargetCache[targnum].Init( nd); }

	// paramblock for the morph channels values
	IParamBlock* cblock;
	
	// Delete and reset channel
	MorphExport void ResetMe();


	MorphExport void AllocBuffers( int sizeA, int sizeB );

	// Do some rough calculations about how much space this channel
	// takes up
	// This isn't meant to be fast or terribly accurate!
	MorphExport float getMemSize();


	// The rebuildChannel call will recalculate the optimization data
	// and refill the mSel selection array. This will be called each time a
	// targeted node is changed, or any of the 'Update Target' buttons is
	// pressed on the UI
	MorphExport void rebuildChannel();

	// Initialize a channel using a scene node
	MorphExport void buildFromNode( INode *node , BOOL resetTime=TRUE , TimeValue t=0, BOOL picked = FALSE );

	// Transfer data to another channel
	MorphExport void operator=(const morphChannel& from);
	void operator=(const TargetCache& tcache);

	// Load/Save channel to stream
	MorphExport IOResult Save(ISave* isave);
	MorphExport IOResult Load(ILoad* iload);
	void SetUpNewController();
	int  NumProgressiveTargets(void) { return mNumProgressiveTargs; }
	void ResetRefs(MorphR3 *, const int&);
	float GetTargetPercent(const int &which);
	void ReNormalize();
	void CopyTargetPercents(const morphChannel &chan);
};




// Internally-used local object morph cache
class morphCache
{

public:

	BOOL CacheValid;

	Point3*		oPoints;
	double*		oWeights;
	BitArray	sel;

	int		Count;

	morphCache ();
	~morphCache () { NukeCache(); }

	MorphExport void MakeCache(Object *obj);
	MorphExport void NukeCache();

	MorphExport BOOL AreWeCached();
};



/*===========================================================================*\
 | Morph Channel restore object
\*===========================================================================*/

class Restore_FullChannel : public RestoreObj {

public:
	MorphR3 *mp;
	morphChannel undoMC;
	morphChannel redoMC;
	int mcIndex;
	BOOL update;
	float *targpercents_undo, *targpercents_redo;
	int ntargs_undo, ntargs_redo;
	
	
	// Constructor
	Restore_FullChannel(MorphR3 *mpi, const int idx, const BOOL upd = TRUE);
	~Restore_FullChannel();

	// Called when Undo is selected
	void Restore(int isUndo);

	// Called when Redo is selected
	void Redo();

	// Called to return the size in bytes of this RestoreObj
	int Size();
};

/*===========================================================================*\
 | Morph Channel restore object
\*===========================================================================*/

class Restore_Marker : public RestoreObj {

public:
	MorphR3 *mp;

	Tab<int>			mIndex;
	NameTab				mName;
	int					markerSel;

	// Constructor
	Restore_Marker(MorphR3 *mpi);

	
	// Called when Undo is selected
	void Restore(int isUndo);

	// Called when Redo is selected
	void Redo();

	// Called to return the size in bytes of this RestoreObj
	int Size();
};

class Restore_Display : public RestoreObj {

public:
	MorphR3 *mp;

	// Constructor
	Restore_Display(MorphR3 *mpi);

	
	// Called when Undo is selected
	void Restore(int isUndo);

	// Called when Redo is selected
	void Redo();

	// Called to return the size in bytes of this RestoreObj
	int Size();
};


class Restore_CompactChannel : public RestoreObj {

public:
	MorphR3 *mp;

	Tab<int>			mtarg;
	Tab<int>			msrc;

	// Constructor
	Restore_CompactChannel(MorphR3 *mpi, Tab<int> &targ, Tab<int> &src);

	
	// Called when Undo is selected
	void Restore(int isUndo);

	// Called when Redo is selected
	void Redo();

	// Called to return the size in bytes of this RestoreObj
	int Size(){ return 0; }
};

class Restore_TargetMove : public RestoreObj {

public:
	MorphR3 *mp;

	int		from, to;

	// Constructor
	Restore_TargetMove(MorphR3 *mpi, const int &fr, const int &t)
	{
		mp = mpi; from =fr; to = t;
	}

	void Restore(int isUndo);
	void Redo();

	int Size(){ return 0; }
};


/*===========================================================================*\
 | Modifer class definition
\*===========================================================================*/
class MorphR3 : public Modifier, TimeChangeCallback {
	public:

		float mFileVersion;

		// Access to the interface
		static IObjParam *ip;
		
		// Pointer to the morph channels
		std::vector<morphChannel>	chanBank;

		// Pointer to the morph material bound to this morpher
		M3Mat *morphmaterial;
		
		// Currently selected channel (0-9)
		int					chanSel;
		
		// Currently viewable channel banks (0-99)
		int					chanNum;

		// Spinners from main page
		ISpinnerControl		*chanSpins[10];

		// Spinners from global settings page
		ISpinnerControl		*glSpinmin,*glSpinmax;

		// Spinners from the channel params dlg
		ISpinnerControl		*cSpinmin,*cSpinmax,*cCurvature,*cTargetPercent;

		// Global parameter block
		IParamBlock			*pblock;

		// The window handles for the 4 rollout pages
		HWND hwGlobalParams, hwChannelList,	hwChannelParams, hwAdvanced, hwLegend;
		static HWND hMaxWnd;

		// For the namer dialog
		ICustEdit			*newname;

		// Morph Cache
		morphCache MC_Local;

		BOOL tccI;
		char trimD[50];

		// 'Save as Current' support
		BOOL recordModifications;
		int recordTarget;


		// Marker support
		Tab<int>			markerIndex;
		NameTab				markerName;
		int					markerSel;


		// Channel operation flag for dialog use
		int					cOp;
		int					srcIdx;

		// storage variable for states between dialog procs
		bool hackUI;


		//Constructor/Destructor
		MorphR3();
		~MorphR3();


		// TimeChangeCallback
		void TimeChanged(TimeValue t) {
			if(hwChannelList) Update_channelValues();

			Interval valid=FOREVER;	int itmp; 
			Interface *Cip = GetCOREInterface();

			if(pblock&&Cip)
			{
				pblock->GetValue(PB_CL_AUTOLOAD, 0, itmp, valid);
				if(itmp==1) NotifyDependents(FOREVER, PART_OBJ, REFMSG_CHANGE);
			}

		}


		// From Animatable
		void DeleteThis() { delete this; }
		void GetClassName(TSTR& s) { s= TSTR(GetString(IDS_CLASS_NAME)); }  
		virtual Class_ID ClassID() { return MR3_CLASS_ID;}		
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
		TCHAR *GetObjectName() { return GetString(IDS_CLASS_NAME); }

		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);

		//From Modifier
		ChannelMask ChannelsUsed()  { return PART_GEOM|PART_TOPO|SELECT_CHANNEL; }
		ChannelMask ChannelsChanged() { return PART_GEOM; }

		void Bez3D(Point3 &b, const Point3 *p, const float &u);
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
		Class_ID InputType() {
			// removed the following
			// it was screwing with modstack evaluation severely!
			//Interface *ip = GetCOREInterface();
			//if(ip&&ip->GetSelNodeCount()>1) return Class_ID(0,0);
			return defObjectClassID;
		}
	
		Interval LocalValidity(TimeValue t);
		void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc);

		void DeleteChannel(const int &);
		// From BaseObject
		BOOL ChangeTopology() {return FALSE;}
		int GetParamBlockIndex(int id) {return id;}

		//From ReferenceMaker
		int NumRefs();
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);
		
		int NumSubs();
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);
		bool CheckMaterialDependency( void );
		bool CheckSubMaterialDependency( void );
		RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message);
		void TestMorphReferenceDependencies( const RefTargetHandle hTarget);
		
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);

		Interval GetValidity(TimeValue t);
		ParamDimension *GetParameterDim(int pbIndex);
		TSTR GetParameterName(int pbIndex);

		// Handles the scroll bar on the channel list UI
		MorphExport void VScroll(int code, short int cpos );
		// Clamps channel number to valid range
		MorphExport void Clamp_chanNum();

		
		MorphExport void ChannelOp(int src, int targ, int flags);


		MorphExport void Update_globalParams();
		MorphExport void Update_advancedParams();	
		MorphExport void Update_channelParams();


		// evaluate the value increments setting
		MorphExport float GetIncrements();
		// SetScale on the channel list spinners
		MorphExport void Update_SpinnerIncrements();

		// Functions to update the channel list dialog box:
		MorphExport void Update_colorIndicators();
		MorphExport void Update_channelNames();
		MorphExport void Update_channelValues();
		MorphExport void Update_channelLimits();
		MorphExport void Update_channelInfo();
		MorphExport void Update_channelMarkers();
		// Seperated cause this function is pretty expensive
		// Lots done, complete update - calls all functions above
		MorphExport void Update_channelFULL();
		
		// Used to trim fp values to a number of decimal points
		MorphExport float TrimDown(float value, int decimalpts);

		BOOL inRender;

		int RenderBegin(TimeValue t, ULONG flags) {	
			inRender = TRUE;
			return 1; 	
			}
		int RenderEnd(TimeValue t) { 	
			inRender = FALSE;	
			return 1; 	
			}

		int CurrentChannelIndex(void) {return chanNum + chanSel; }
		morphChannel &CurrentChannel(void) { return chanBank[chanNum + chanSel]; }

		float GetCurrentTargetPercent(void); 
		void SetCurrentTargetPercent(const float &fval);
	
		void DeleteTarget(void);
		void Update_TargetListBoxNames(void);
		void SwapTargets(const int way);
		void SwapTargets(const int from, const int to, const bool isundo);

		int GetRefNumber(int chanNum, int targNum) { return (200 + targNum + (chanNum * MAX_TARGS)); }
		void DisplayMemoryUsage(void );
};



/*===========================================================================*\
 |
 | Morph Material definitions
 |
\*===========================================================================*/

class M3MatDlg : public ParamDlg {
	public:		
		HWND hwmedit;

		IMtlParams *ip;

		M3Mat *theMtl;

		HWND hPanel; 

		ICustButton *iBut[NSUBMTL];
		ICustButton *bBut;
		ICustButton *pickBut;

		MtlDADMgr dadMgr;

		BOOL valid;

		M3MatDlg(HWND hwMtlEdit, IMtlParams *imp, M3Mat *m); 
		~M3MatDlg();
		
		BOOL WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);				
		void Invalidate();		
		void DragAndDrop(int ifrom, int ito);
		void UpdateSubMtlNames();
		void ActivateDlg(BOOL onOff) {}
		
		// methods inherited from ParamDlg:		
		void ReloadDialog();
		Class_ID ClassID() {return M3MatClassID;}
		void SetThing(ReferenceTarget *m);
		ReferenceTarget* GetThing() { 
			return (ReferenceTarget *)theMtl; 
		}
		void DeleteThis() { delete this;  }	
		void SetTime(TimeValue t) {Invalidate();}

		int FindSubMtlFromHWND(HWND hw);

		MorphExport void VScroll(int code, short int cpos );
		MorphExport void Clamp_listSel();

		MorphExport void UpdateMorphInfo(int upFlag);
};

class M3Mat : public Mtl, public IReshading  {	
	public:
		M3MatDlg *matDlg;

		// 100 materials for 100 morph channels, plus 1 base material
		Mtl *mTex[101];
		BOOL mapOn[100];

		BOOL inRender;

		// Morph mod pointer
		MorphR3	*morphp;
		TSTR obName;

		// Temp node pointer used in the mtl pickmode
		INode *Wnode;

		IParamBlock *pblockMat;

		Interval ivalid;
		int listSel;

		//- ke - 5.30.02 - merge reshading code
		ReshadeRequirements mReshadeRQ;


		M3Mat(BOOL loading);
		~M3Mat();

		void* GetInterface(ULONG id);

		void NotifyChanged() {
			NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		}
		

		// From MtlBase and Mtl
		void SetAmbient(Color c, TimeValue t) {}		
		void SetDiffuse(Color c, TimeValue t) {}		
		void SetSpecular(Color c, TimeValue t) {}
		void SetShininess(float v, TimeValue t) {}				
		
		Color GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
	    Color GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
		Color GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
		float GetXParency(int mtlNum=0, BOOL backFace=FALSE);
		float GetShininess(int mtlNum=0, BOOL backFace=FALSE);		
		float GetShinStr(int mtlNum=0, BOOL backFace=FALSE);
		float WireSize(int mtlNum=0, BOOL backFace=FALSE);
				
		ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		
		void Shade(ShadeContext& sc);
		float EvalDisplacement(ShadeContext& sc); 
		Interval DisplacementValidity(TimeValue t); 
		void Update(TimeValue t, Interval& valid);
		void Reset(  );
		Interval Validity(TimeValue t);
		
		Class_ID ClassID() {return M3MatClassID; }
		SClass_ID SuperClassID() {return MATERIAL_CLASS_ID;}
		void GetClassName(TSTR& s) {s=GetString(IDS_MORPHMTL);}  

		// begin - ke - 5.30.02 - merge reshading code
		BOOL SupportsRenderElements(){ return FALSE; }
		ReshadeRequirements GetReshadeRequirements() { return mReshadeRQ; } // mjm - 06.02.00
		void PreShade(ShadeContext& sc, IReshadeFragment* pFrag);
		void PostShade(ShadeContext& sc, IReshadeFragment* pFrag, int& nextTexIndex, IllumParams* ip);
		// end - ke - merge reshading code



		void DeleteThis() {
			delete this;
		}	

		// Methods to access sub-materials of meta-materials
	   	int NumSubMtls() {return 101;}
		Mtl* GetSubMtl(int i) {
			return mTex[i];
		}
		void SetSubMtl(int i, Mtl *m);
		TSTR GetSubMtlSlotName(int i) {
			if(i==100) return GetString(IDS_MTL_BASENAME);

			char s[25];
			if(morphp) sprintf(s,"Mtl %i (%s)",i+1,morphp->chanBank[i].mName);
			else sprintf(s,GetString(IDS_MTL_MAPNAME),i+1);
			if(i<101) return s;

			return _T("x");
		}


		int NumSubs() {return 101;} 
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);
		int SubNumToRefNum(int subNum) {return subNum;}

		// From ref
 		int NumRefs() {return 103;}
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);

		RefTargetHandle Clone(RemapDir &remap = NoRemap());
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message);

		
		int RenderBegin(TimeValue t, ULONG flags) {	
			if(flags!=RENDERBEGIN_IN_MEDIT) inRender = TRUE;
			return 1; 	
			}
		int RenderEnd(TimeValue t) { 	
			inRender = FALSE;	
			return 1; 	
			}


		// IO
		IOResult Save(ISave *isave); 
		IOResult Load(ILoad *iload); 

		// From Mtl
		bool IsOutputConst( ShadeContext& sc, int stdID );
		bool EvalColorStdChannel( ShadeContext& sc, int stdID, Color& outClr );
		bool EvalMonoStdChannel( ShadeContext& sc, int stdID, float& outVal );
	};


class MorphR3PostLoadCallback : public PostLoadCallback {
	public:
		MorphR3 *mp;
		MorphR3PostLoadCallback(ParamBlockPLCB *c) {mp=NULL;}
		MorphR3PostLoadCallback(MorphR3 *m) {mp = m;}
		void proc(ILoad *iload) ;
};

#endif
