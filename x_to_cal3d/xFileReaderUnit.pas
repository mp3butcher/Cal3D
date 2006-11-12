//****************************************************************************//
// xFileReaderUnit.pas                                                        //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
unit xFileReaderUnit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, CMFData, Math, contnrs, ComCtrls, Spin, ExtCtrls,
  FlatSpin, Registry, Xfiles, Utility

  {$IFDEF FF3D},

  {If building for conversion to FrameForge 3D Studio Cal3D "Flavor"}

//############################################
//  Native VCL Zip/Unzip Component -- http://www.vclzip.com
//############################################
   VCLUnZip, VCLZip,

//############################################
//  ImageEn Image Processing -- http://www.hicomponents.com
//############################################
   imageenio
{$ENDIF FF3D};
type

  TConverterGUI = class(TForm)
    OpenFileDialog: TOpenDialog;
    LoadMesh: TBitBtn;
    Label1: TLabel;
    SkippedMembers: TListBox;
    SkipLabel: TLabel;
    TreeView1: TTreeView;
    DiscardUnderXFaces: TCheckBox;
    FaceCount: TFlatSpinEdit;
    WriteCFG: TCheckBox;
    CreateFF3DCZP: TCheckBox;
    ScaleFactor: TFlatFloatSpinEdit;
    ScaleLabel: TLabel;
    LoadTimer: TTimer;
    procedure LoadClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure CreateFF3DCZPClick(Sender: TObject);
    procedure DiscardUnderXFacesClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
{$IFDEF FF3D}
    Zipper:TVCLZip;
    ImageConverter: TImageEnIO;
    function SetupFF3DCal3DHeader(ObjectID:String):String;
{$ENDIF FF3D}
  public
    { Public declarations }
  end;

var
  ConverterGUI: TConverterGUI;

implementation

{$R *.DFM}

procedure TConverterGUI.LoadClick(Sender: TObject);
var
   xFile:tDirectXFile;
   ConvertedFile:TCal3DMesh;
   CV,I,v,f,InfluenceIndex:Integer;
   currentxMesh:d3d_Mesh;
   CurrentSubmesh:TSubmeshData;
   CurrentVertex:TVertexData;
   CheckVertex:TVertexData;
   CurrentFace:TFaceData;
   CurrentInfluence:TinfluenceData;
   CheckInfluence:TinfluenceData;
   TargetNormal:d3d_Vector;
   TargetUVS:d3d_Coords2d;
   isTheSameVertex:Boolean;
   DupVert,TargetThreadID:Integer;
   HasInfluenceData:Boolean;
   RealNumInfluences:integer;
   MeshList,MaterialList:TStringList;
   TargetTexture:String;
   TargetMeshIndex:Integer;
begin
{$IFDEF FF3D}
     Zipper.FilesList.Clear;
{$ENDIF FF3D}
     MeshList:=TStringList.Create;
     MaterialList:=TStringList.Create;
     LoadTimer.Enabled:=False;
     if ((Sender=LoadTimer) and FIleExists(OpenFIleDialog.FileName)) or OpenFileDialog.Execute then
      try
        ChDir(ExtractFilePath(OpenFileDialog.FileName));
        LoadMesh.Enabled:=False;
        Label1.Caption:='Reading '+ExtractFileName(OpenFileDialog.FileName);
        Label1.Refresh;
        xFile:=tDirectXFile.Create(OpenFileDialog.FileName);
        Label1.Caption:='Writing Skeleton';
        Label1.Refresh;
        if not xFile.WriteSkeleton then
         begin
           MessageBox(GetActiveWindow,'No Defined Skeleton','Conversion Failed',MB_ICONERROR);
           close;
           exit;
         end;
        with TRegistry.Create do
         begin
           RootKey:=HKEY_LOCAL_MACHINE;
           if OpenKey('X File to CMF Converter',true) then
            begin
              WriteString('Scale Factor Value',ScaleFactor.Text);
              WriteBool('DiscardUnderXFaces',DiscardUnderXFaces.Checked);
              WriteBool('Write CFG',WriteCFG.Checked);
{$IFDEF FF3D}
              WriteBool('Check State','CreateFF3DCZP',CreateFF3DCZP.Checked);
{$ENDIF FF3D}
            end;
          Free;
        end;
        xfile.LowerMeshTransforms;
        CurrentSubmesh:=Nil;
        TargetThreadID:=0;
        for i:=1 to xFile.MeshCount do
         begin
           currentxMesh:=xFile.GetMesh(Pred(i));
           if ConverterGUI.DiscardUnderXFaces.Checked and (CurrentXMesh.nFaces <= ConverterGUI.FaceCount.Value) then
              continue;
           ConvertedFile:=TCal3DMesh.Create;
           ConvertedFile.NumberOfSubmeshes:=1;
           if not assigned(CurrentSubmesh) or not assigned(ConvertedFile.SubMeshes) then
            begin
              ConvertedFile.SubMeshes:=TSubmeshData.Create(Nil);
              CurrentSubmesh:=ConvertedFile.SubMeshes;
            end
           else
            begin
              CurrentSubmesh.NextSubmesh:=TSubmeshData.Create(CurrentSubmesh);
              CurrentSubmesh:=CurrentSubmesh.NextSubmesh;
            end;
           CurrentSubMesh.NumberOfvertices:=CurrentxMesh.nVertices;
           CurrentSubMesh.NumberOfFaces:=CurrentxMesh.nFaces;
           CurrentSubmesh.MaterialThreadId:=TargetThreadID;
           MeshList.AddObject(ChangeFileExt(CurrentXMesh.MeshName,'.cmf'),TObject(TargetThreadID));
           if not assigned(CurrentXMesh.MeshMaterialList) or
              (High(CurrentXMesh.MeshMaterialList.MaterialNames)=-1) then
              MaterialList.AddObject('_'+ChangeFileExt(CurrentXMesh.MeshName,'.xrf'),TObject(TargetThreadID))
           else
              MaterialList.AddObject(ChangeFileExt(CurrentXMesh.MeshMaterialList.MaterialNames[0],'.xrf'),TObject(TargetThreadID));
           xFile.WriteMaterial(MaterialList[Pred(MaterialList.Count)]);

           Inc(TargetThreadID);
           CurrentVertex:=Nil;
           HasInfluenceData:=High(currentxMesh.SkinWeights) > -1;
           for V:=0 to Pred(CurrentSubMesh.NumberOfvertices) do
            begin
               if V MOD 100 = 0 then
                begin
                  ConverterGUI.Label1.Caption:='Converting Vertex #: '+IntToStr(Succ(v))+' of '+IntToStr(CurrentSubMesh.NumberOfvertices);
                  Application.ProcessMessages;
                end;
               if not assigned(CurrentVertex) then
                begin
                  CurrentSubMesh.VertexData:=TVertexData.Create(Nil);
                  CurrentVertex:=CurrentSubmesh.VertexData;
                end
               else
                begin
                  CurrentVertex.NextVertex:=TVertexData.Create(CurrentVertex);
                  CurrentVertex:=CurrentVertex.NextVertex;
                end;
               CurrentVertex.Position[x]:=CurrentxMesh.vertices[v].x;
               CurrentVertex.Position[y]:=CurrentxMesh.vertices[v].y;
               CurrentVertex.Position[z]:=CurrentxMesh.vertices[v].z;
               TargetNormal:=currentxMesh.GetNormals(v);
               if assigned(TargetNormal) then
                begin
                     CurrentVertex.Normal[x]:=TargetNormal.x;
                     CurrentVertex.Normal[y]:=TargetNormal.y;
                     CurrentVertex.Normal[z]:=TargetNormal.z;
                end
               else
                raise Exception.Create('Missing Vertex NORMALS!');
               TargetUVs:=currentxMesh.GetUVs(v);
               if assigned(TargetUVs) then
                begin
                  CurrentSubmesh.NumberOfmaps:=1;
                  CurrentVertex.CoordinateMaps:=TMapData.Create(Nil);
                  CurrentVertex.CoordinateMaps.Coordinates[3]:=TargetUVs.u;
                  CurrentVertex.CoordinateMaps.Coordinates[4]:=TargetUVs.v;
                end;
                CurrentInfluence:=Nil;
                if not HasInfluenceData then
                   CurrentVertex.NumberOfInfluences:=0
                else
                 begin
                   CurrentVertex.NumberOfInfluences:=currentxMesh.GetNumInfluences(v);
                   RealNumInfluences:=0;
                   if CurrentVertex.NumberofInfluences > 0 then
                    with currentxMesh.GetInfluences(xFile.TheBoneList,v) do
                      for InfluenceIndex:=0 to Pred(CurrentVertex.NumberofInfluences) do
                       begin
                         if InfluenceAmt[InfluenceIndex] = 0 then
                            continue
                         else
                           Inc(RealNumInfluences);
                         if not Assigned(CurrentInfluence) then
                          begin
                             CurrentVertex.Influences:=TinfluenceData.Create(Nil);
                             CurrentInfluence:=CurrentVertex.Influences;
                          end
                         else
                          begin
                             CurrentInfluence.NextInfluence:=TinfluenceData.Create(CurrentInfluence);
                             CurrentInfluence:=CurrentInfluence.NextInfluence;
                          end;
                          CurrentInfluence.BoneID:=BoneID[InfluenceIndex];
                          CurrentInfluence.weight:=InfluenceAmt[InfluenceIndex];
                          if CurrentInfluence.weight > 1 then
                             CurrentInfluence.weight:=1;
                       end;
                   CurrentVertex.NumberOfInfluences:=RealNumInfluences;
                 end;
            end;
           CurrentFace:=Nil;
           for f:=0 to Pred(CurrentSubMesh.NumberOfFaces) do
            begin
               if F MOD 100 = 0 then
                begin
                  ConverterGUI.Label1.Caption:='Converting Face #: '+IntToStr(Succ(F))+' of '+IntToStr(CurrentSubMesh.NumberOfFaces);
                  Application.ProcessMessages;
                end;
               if not assigned(CurrentFace) then
                begin
                  CurrentSubMesh.Faces:=TFaceData.Create(Nil);
                  CurrentFace:=CurrentSubmesh.Faces;
                end
               else
                begin
                  CurrentFace.NextFace:=TFaceData.Create(CurrentFace);
                  CurrentFace:=CurrentFace.NextFace;
                end;
               if CurrentxMesh.faces[f].nFaceVertexIndices <> 3 then
                  raise Exception.Create('Must have TRIANGULATED FACES!');

               CurrentFace.VertexID[1]:=CurrentxMesh.faces[f].faceVertexIndices[0];
               CurrentFace.VertexID[2]:=CurrentxMesh.faces[f].faceVertexIndices[1];
               CurrentFace.VertexID[3]:=CurrentxMesh.faces[f].faceVertexIndices[2];
            end;

           CurrentVertex:=CurrentSubmesh.VertexData;
           v:=0;
           DupVert:=0;
           repeat
             CheckVertex:=CurrentVertex.NextVertex;
             CV:=Succ(V);
             while assigned(CheckVertex) do
              begin
                isTheSameVertex :=
                   FloatEqual(CheckVertex.Position[x],CurrentVertex.Position[x]) and
                   FloatEqual(CheckVertex.Position[y],CurrentVertex.Position[y]) and
                   FloatEqual(CheckVertex.Position[z],CurrentVertex.Position[z]) and
                   FloatEqual(CheckVertex.Normal[x],CurrentVertex.Normal[x]) and
                   FloatEqual(CheckVertex.Normal[y],CurrentVertex.Normal[y]) and
                   FloatEqual(CheckVertex.Normal[z],CurrentVertex.Normal[z]) and
                   (assigned(CheckVertex.CoordinateMaps) = assigned(CurrentVertex.CoordinateMaps)) and
                   (
                     (  Assigned(CheckVertex.CoordinateMaps) and
                       FloatEqual(CheckVertex.CoordinateMaps.Coordinates[3],CurrentVertex.CoordinateMaps.Coordinates[3]) and
                       FloatEqual(CheckVertex.CoordinateMaps.Coordinates[4],CurrentVertex.CoordinateMaps.Coordinates[4])
                     ) or not Assigned(CheckVertex.CoordinateMaps)
                   ) and
                    (CheckVertex.NumberOfInfluences = CurrentVertex.NumberOfInfluences);
                if IsTheSameVertex then
                  begin
                        CurrentInfluence:=CurrentVertex.Influences;
                        CheckInfluence:=CheckVertex.Influences;
                        if CheckInfluence <> nil then
                        repeat
                          IsTheSameVertex:=(CurrentInfluence.BoneID = CheckInfluence.BoneID) and
                                           FloatEqual(CurrentInfluence.Weight,CheckInfluence.Weight);
                          if Not IsTheSameVertex then
                             Checkinfluence:=Nil
                          else
                           begin
                             CurrentInfluence:=CurrentInfluence.NextInfluence;
                             CheckInfluence:=CheckInfluence.NextInfluence;
                           end;
                        until not IsTheSameVertex or (CheckInfluence=nil)
                  end;
                if isTheSameVertex then
                 begin
                      Inc(DupVert);
                      ConverterGUI.Label1.Caption:='Removing Duplicate Vertices #: '+IntToStr(Succ(DupVert));
                      Application.ProcessMessages;
                      if CurrentSubmesh.VertexData = CheckVertex then
                         CurrentSubmesh.VertexData := CheckVertex.NextVertex;
                      if assigned(CheckVertex.PrevVertex) then
                         CheckVertex.PrevVertex.NextVertex:=CheckVertex.NextVertex;
                      if assigned(CheckVertex.NextVertex) then
                         CheckVertex.NextVertex.PrevVertex:=CheckVertex.PrevVertex;
                      CheckVertex.PrevVertex:=nil;
                      CheckVertex.NextVertex:=Nil;
                      CheckVertex.Free;
                      Dec(CurrentSubmesh.NumberOfvertices);
                      CurrentFace:=CurrentSubmesh.Faces;
                      while assigned(CurrentFace) do
                       begin
                         for f:=1 to 3 do
                           if CurrentFace.VertexID[f] > CV then
                              Dec(CurrentFace.VertexID[f])
                           else if CurrentFace.VertexID[f] = CV then
                              CurrentFace.VertexID[f]:=V;
                         CurrentFace:=CurrentFace.NextFace;
                       end;
                 end;
                CheckVertex:=CheckVertex.NextVertex;
                Inc(CV);
              end;
             CurrentVertex:=CurrentVertex.NextVertex;
             Inc(V);
            until not assigned(CurrentVertex);
            ConvertedFile.WriteCMF(ExtractFilePath(OpenFileDialog.FileName)+CurrentxMesh.MeshName+'.cmf');
            ConvertedFile.Free;
            CurrentSubmesh:=Nil;
            CurrentFace:=Nil;
         end;
        ConverterGUI.Label1.Caption:='Writing CMF File';
        Application.ProcessMessages;
        if WriteCFG.Checked then
           with TStringList.Create do
            begin
               Add('################################################');
               Add('#');
               Add('# Cal3d cfg File');
               Add('#');
               Add('################################################');
               Add('');
               Add('scale=0.5');
               Add('');
               Add('################# Skeleton #####################');
               Add('skeleton=Skeleton.CSF');
               Add('');
               Add('################# Meshes #######################');
               for i:=0 to Pred(MeshList.Count) do
                 Add('mesh='+ExtractFileName(MeshList[i]));
               Add('############### Materials ######################');
               for i:=0 to Pred(MaterialList.Count) do
                 Add('material='+ExtractFileName(MaterialList[i]));
               SaveToFile(ChangeFileExt(OpenFileDialog.FileName,'.cfg'));
               Free;
            end;
{$IFDEF FF3D}
        if CreateFF3DCZP.Checked then
         begin
           Zipper.FilesList.Clear;
           Zipper.FilesList.Add('skeleton.csf');
           for i:=0 to Pred(MeshList.Count) do
               Zipper.FilesList.Add(MeshList[i]);
           for i:=0 to Pred(MaterialList.Count) do
            begin
               Zipper.FilesList.Add(ChangeFileExt(MaterialList[i],'_ff3d.xrf'));
               TargetTexture:=xFile.GetTextureForMaterial(MaterialList[i]);
               if (TargetTexture <> '') then
                  Zipper.FilesList.Add(TargetTexture);
            end;
           With TSTringList.Create do
           begin
             Text:=SetupFF3DCal3DHeader(ChangeFileExt(ExtractFileName(OpenFileDialog.FileName),''));
             Add('<meshes>');
             for i:=0 to Pred(MeshList.Count) do
                 Add(#9+'<mesh name="'+ChangeFileExt(MeshList[i],'')+'" file="'+MeshList[i]+'"/>');
             Add('</meshes>');
             Add('');
             Add('<materials>');
             for i:=0 to Pred(MaterialList.Count) do
              begin
                 TargetMeshIndex:=MeshList.IndexOfObject(MaterialList.Objects[i]);
                 if TargetMeshIndex > -1 then
                    Add(#9+'<material_file name="'+ChangeFileExt(MeshList[TargetMeshIndex],'')+'" threadID="'+
                        IntToStr(Integer(MaterialList.Objects[i]))+'">'+
                          ChangeFIleExt(MaterialList[i],'_ff3d.xrf')+'</material_file>')
                 else
                    Add(#9+'<material_file name="'+ChangeFileExt(MaterialList[i],'')+'" threadID="'+
                        IntToStr(Integer(MaterialList.Objects[i]))+'">'+
                          ChangeFIleExt(MaterialList[i],'_ff3d.xrf')+'</material_file>');
              end;
             Add('</materials>');
             Add('</cal_config>');
             saveToFile(ChangeFileExt(OpenFileDialog.FileName,'.xfg'));
             Free;
             Zipper.FilesList.Add(ChangeFileExt(OpenFileDialog.FileName,'.xfg'));
           end;
           Zipper.ZipName:=ChangeFileExt(OpenFileDialog.FileName,'.czp');
           Zipper.Zip;
           for i:=0 to Pred(MaterialList.Count) do
             DeleteFile(ChangeFileExt(MaterialList[i],'_ff3d.xrf'));
           DeleteFile(ChangeFileExt(OpenFileDialog.FileName,'.xfg'));
         end;
{$ENDIF FF3D}
      finally
        LoadMesh.Enabled:=True;
        MeshList.Free;
        MaterialList.Free;
        xFile.Free;
      end;
      Close;
end;

procedure TConverterGUI.FormShow(Sender: TObject);
begin
        with TRegistry.Create do
         begin
           RootKey:=HKEY_LOCAL_MACHINE;
           if OpenKey('X File to CMF Converter',false) then
            begin
             try
               ScaleFactor.Text:=ReadString('Scale Factor Value');
             except
               ScaleFactor.Text:='0.005';
             end;

             try
               DiscardUnderXFaces.Checked:=ReadBool('DiscardUnderXFaces');
             except
               DiscardUnderXFaces.Checked:=True;
             end;

             try
               WriteCFG.Checked:=ReadBool('WriteCFG');
             except
               WriteCFG.Checked:=True;
             end;
{$IFDEF FF3D}
             try
               CreateFF3DCZP.Checked:=ReadBool('CreateFF3DCZP');
             except
               CreateFF3DCZP.Checked:=True;
             end;
             CreateFF3DCZPClick(Nil);
{$ENDIF FF3D}
            end;
          Free;
        end;
     DiscardUnderXFacesClick(Nil);
     Refresh;
     Application.ProcessMessages;
     if (ParamCount > 0) and FileExists(ParamStr(1)) then
      begin
        OpenFileDialog.FileName:=ParamStr(1);
        LoadTimer.Enabled:=True;
      end;
end;

procedure TConverterGUI.CreateFF3DCZPClick(Sender: TObject);
begin
    ScaleFactor.Enabled:=CreateFF3DCZP.Checked;
end;

procedure TConverterGUI.DiscardUnderXFacesClick(Sender: TObject);
begin
    FaceCount.Enabled:=DiscardUnderXFaces.Checked;
end;

procedure TConverterGUI.FormCreate(Sender: TObject);
begin
{$IFDEF FF3D}
   Zipper:=TVCLZip.Create(Self);
   ImageConverter:=TImageEnIO.Creaet(Self);
   CreateFF3DCZP.Top:=138;
   ScaleLabel.Top:=162;
   ScaleFactor.Top:=158;
   SkipLabel.Top:=184;
   SkippedMembers.SetBounds(SkippedMembers.Left,204,SkippedMembersWidth,98);
{$ENDIF FF3D}
end;

{$IFDEF FF3D}
function TConverterGUI.SetupFF3DCal3DHeader(ObjectID:String):String;
begin
     with TSTringList.Create do
     begin
       Add('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>');
       add('<!DOCTYPE cal_config [');
       add('	<!ENTITY lt     "&#38;#60;">');
       add('	<!ENTITY gt     "&#62;">');
       add('	<!ENTITY amp    "&#38;#38;">');
       add('	<!ENTITY apos   "&#39;">');
       add('	<!ENTITY quot   "&#34;">');
       add('');
       add('	<!ELEMENT cal_config (scale?, flags?, skeleton_file, bone_renames?, control_bones?, bone_limits?, animation_file?, meshes, materials, switches?, morphs?, glance_adjust?)>');
       add('	<!ELEMENT scale (#PCDATA)>	<!--a number such as 1.0-->');
       add('	<!ELEMENT flags (invert_v?, rotate_x?)>');
       add('	<!ELEMENT invert_v EMPTY>');
       add('	<!ELEMENT rotate_x EMPTY>');
       add('	<!ELEMENT skeleton_file (#PCDATA)>	<!--a file name-->');
       add('	<!ELEMENT bone_renames (bone_rename)+>');
       add('	<!ELEMENT bone_rename EMPTY>');
       add('	<!ATTLIST bone_rename');
       add('		old CDATA #REQUIRED');
       add('		new CDATA #REQUIRED>');
       add('	<!ELEMENT control_bones (control_bone)+>');
       add('	<!ELEMENT control_bone (twist_bone)+>');
       add('	<!ATTLIST control_bone');
       add('		name CDATA #REQUIRED');
       add('		base CDATA #REQUIRED>');
       add('			<!-- name is name of the control bone, base is the normal bone that bends when the user manipulates the control bone -->');
       add('	<!ELEMENT twist_bone EMPTY>');
       add('	<!ATTLIST twist_bone');
       add('		name CDATA #REQUIRED>');
       add('	<!ELEMENT bone_limits (bone_limit)+>');
       add('	<!ELEMENT bone_limit (bone_limit_plane)+>');
       add('	<!ATTLIST bone_limit');
       add('		name CDATA #REQUIRED');
       add('		twist_min CDATA #REQUIRED');
       add('		twist_max CDATA #REQUIRED>');
       add('	<!ELEMENT bone_limit_plane EMPTY>');
       add('	<!ATTLIST bone_limit_plane');
       add('		x CDATA #REQUIRED');
       add('		y CDATA #REQUIRED');
       add('		z CDATA #REQUIRED>');
       add('	<!ELEMENT animation_file (#PCDATA)>	<!--a file name-->');
       add('	<!ELEMENT meshes (mesh)+>');
       add('	<!ELEMENT mesh (submesh_names?)>');
       add('	<!ATTLIST mesh');
       add('		name CDATA #REQUIRED');
       add('		file CDATA #REQUIRED');
       add('		backfacing (both | remove) "remove"');
       add('		split (yes|no) "no">	<!-- name is a mesh name, file is a file name,');
       add('								split means put each submesh in a');
       add('								separate mesh -->');
       add('	<!ELEMENT submesh_names (submesh_name)*>');
       add('	<!ELEMENT submesh_name EMPTY>');
       add('	<!ATTLIST submesh_name name CDATA #REQUIRED>');
       add('	<!ELEMENT materials (material_file)+>');
       add('	<!ELEMENT material_file (#PCDATA)>		<!--a file name-->');
       add('	<!ATTLIST material_file');
       add('		name CDATA #REQUIRED');
       add('		threadID CDATA #IMPLIED>');
       add('	<!ELEMENT switches (switch)+>');
       add('	<!ELEMENT switch (case)+>');
       add('	<!ATTLIST switch name CDATA #REQUIRED>');
       add('	<!ELEMENT case (mesh_active)*>');
       add('	<!ATTLIST case name CDATA #REQUIRED>');
       add('	<!ELEMENT mesh_active EMPTY>');
       add('	<!ATTLIST mesh_active');
       add('		name CDATA #REQUIRED>');
       add('	<!ELEMENT morphs (morphable)+>');
       add('	<!ELEMENT morphable (target)+>');
       add('	<!ATTLIST morphable');
       add('		base CDATA #REQUIRED>	<!--a mesh name-->');
       add('	<!ELEMENT target EMPTY>');
       add('	<!ATTLIST target');
       add('		name CDATA #REQUIRED');
       add('		file CDATA #REQUIRED>');
       add('	<!ELEMENT glance_adjust (#PCDATA)>	<!-- an angle in degrees, + or - -->');
       add('	<!--');
       add('	In each switch, exactly one case is active.  The first case in the');
       add('	switch is the one that will be active by default.  A mesh will be');
       add('	attached to the model if and only if every switch case that contains');
       add('	the mesh is active.');
       add('');
       add('	No mesh should belong to more than one case of the same switch.');
       add('');
       add('	No mesh should belong to more than one morph.  No morph target should be');
       add('	listed as a switch case, however a morph base could be a switch case.');
       add('	-->');
       add(']>');
       add(' <cal_config>');
       add('	<object_id>'+ObjectID+'</object_id>');
       add('	<object_version major="1" minor="0" bugfix="0"/>');
       add('	<scale>'+ScaleFactor.Text+'</scale>');
       add('	<flags>');
       add('	     <invert_v/>');
       add('	     <rotate_x/>');
       add('	</flags>');
       add('');
       add('	<skeleton_file>Skeleton.CSF</skeleton_file>');
       Result:=Text;
       Free;
     end;
end;
{$ENDIF FF3D}



end.
