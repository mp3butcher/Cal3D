//****************************************************************************//
// Cal3DFormatConv.cpp                                                        //
// Copyright (C) 2002 Laurent Desmecht                                        //
// Copyright (C) 2006 Loic Dachary <loic@gnu.org>                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#include "cal3d/cal3d.h"
#include <cstring>
#define SKELETON 0
#define MESH 1
#define ANIMATION 2
#define MATERIAL 3

using namespace std;

int GetFileType(std::string strFilename)
{
	if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC)==0
		|| strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_FILE_MAGIC)==0)
		return SKELETON;
	if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC)==0
		|| strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_FILE_MAGIC)==0)
		return MESH;
	if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC)==0
		|| strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_FILE_MAGIC)==0)
		return ANIMATION;
	if(strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC)==0
		|| strFilename.size()>= 3 && stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_FILE_MAGIC)==0)
		return MATERIAL;
	return -1;
}


int main(int argc, char* argv[])
{
	std::string strFilename1,strFilename2;
	int Type=-1;
	
	if(argc==1)
	{
		cout << "Enter the name of the source file :";
		cin >> strFilename1;

		Type = GetFileType(strFilename1);
		switch(Type)
		{
		case SKELETON:
			cout << "The file is a skeleton\nEnter the name of the destination file (use .xsf for a XML file) :";
			cin >> strFilename2;
			break;
		case MESH:
			cout << "The file is a mesh\nEnter the name of the destination file (use .xmf for a XML file) :";
			cin >> strFilename2;
			break;
		case ANIMATION:
			cout << "The file is an animation\nEnter the name of the destination file (use .xaf for a XML file) :";
			cin >> strFilename2;
			break;
		case MATERIAL:
			cout << "The file is a material\nEnter the name of the destination file (use .xrf for a XML file) :";
			cin >> strFilename2;
			break;
		case -1:
			cout << "Format of the file unknown (check the extention)\n";
			return 1;
		}
	}
	else if(argc==3)
	{
		strFilename1 = argv[1];
		Type  = GetFileType(strFilename1);
		if(Type==-1)
		{
			cout << "Format of the file unknown (check the extention)\n";
			return 1;
		}

		strFilename2 = argv[2];
	}
	else
	{
		cout << "Usage :\n";
		cout << "Cal3DFormatConv [Source Dest]\n";
	}


	CalSaver Saver;
	CalLoader Loader;

	if(Type==SKELETON)
	{
	    CalCoreSkeletonPtr Ske = Loader.loadCoreSkeleton(strFilename1);
		if(!Ske)
		{
			cout << "Error during loading of "<< strFilename1<< endl;
			cout << "Error was:" << CalError::getLastErrorDescription() << endl;
			return 1;
		}
		Saver.saveCoreSkeleton(strFilename2,Ske.get());
	}
	if(Type==MESH)
	{	
		CalCoreMeshPtr Mesh = Loader.loadCoreMesh(strFilename1);
		if(!Mesh)
		{
			cout << "Error during loading of "<< strFilename1<< endl;
			cout << "Error was:" << CalError::getLastErrorDescription() << endl;
			return 1;
		}
		Saver.saveCoreMesh(strFilename2,Mesh.get());
	}

	if(Type==ANIMATION)
	{
		CalCoreAnimationPtr Ani = Loader.loadCoreAnimation(strFilename1);
		if(!Ani)
		{
			cout << "Error during loading of "<< strFilename1<< endl;
			cout << "Error was:" << CalError::getLastErrorDescription() << endl;
			return 1;
		}
		Saver.saveCoreAnimation(strFilename2,Ani.get());
	}
	if(Type==MATERIAL)
	{
		CalCoreMaterialPtr Mat = Loader.loadCoreMaterial(strFilename1);
		if(!Mat)
		{
			cout << "Error during loading of "<< strFilename1<< endl;
			cout << "Error was:" << CalError::getLastErrorDescription() << endl;
			return 1;
		}
		Saver.saveCoreMaterial(strFilename2,Mat.get());
	}
	return 0;
}
