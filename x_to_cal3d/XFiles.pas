//****************************************************************************//
// XFiles.pas                                                                 //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
unit XFiles;

interface

uses
  Windows, Messages, Forms, SysUtils, Classes, Graphics, CMFData, Math,
  contnrs, comctrls, Utility,

//############################################
//  Open Source Geometry function-- http://www.lischke-online.de/Geometry.php
//############################################
  geometry
  ;

type
    tPropTypes = (propWORD, propDWORD, propFLOAT, PropDOUBLE, propCHAR, propUCHAR,
                  propBYTE, propSTRING, propCSTRING, propUNICODE, propMatrix4x4);

    tDirectXFile = class;

    t4x4Matrix = array[1..4,1..4] of Single;
    
    TBoneRecordArray = record
      BoneID         : array of Integer;
      InfluenceAmt   : array of Single;
    end;

    TQuat = packed record
       w        :Single;
       x        :Single;
       y        :Single;
       z        :Single;
    end;

    TTrans = packed record
       x        :Single;
       y        :Single;
       z        :Single;
    end;


    d3d_frame = class;
    d3d_SkinWeights = class;
    d3d_Mesh        = class;

    TBone = class
       Child_Bones          :Array of Integer;

       bone_name            :string;
       parent_bone_name     :string;
       bone_id              :integer;
       rotation             :TQuat;
       local_rotation       :TQuat;
       translation          :TTrans;
       Local_translation    :TTrans;

       AdjustedMatrix       :t4x4Matrix;

       parent_bone_id       :integer;
       SourceMesh           :d3d_Mesh;

       function Length_of_Bone_Name:LongInt;
       function number_of_children:LongInt;
    end;


    d3d_ancestor = class
     private
       DataString:PChar;
       DataStringEnd:PChar;
       procedure IncrementMemoryStream(TheStream:TMemoryStream;HowManySemiColons:integer);
       procedure FindEndMarker(TheStream:TMemoryStream);
       procedure IncPastTrailingSemi(TheStream:TMemoryStream);
       procedure IncPastTrailingComma(TheStream:TMemoryStream);
       function GetFirstValue(Separator:Char):ShortString;
     end;

    d3d_Vector = class(d3d_ancestor)
       x : Single; //
       y : Single; //
       z : Single; //
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Coords2d  = class(d3d_ancestor)
       u : Single; //
       v : Single; //
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_XSkinMeshHeader  = class(d3d_ancestor)
     nMaxSkinWeightsPerVertex:Word;
     nMaxSkinWeightsPerFace:WORD;
     nBones:WORD;
     TrueOwningFile:tDirectXFile;
     OwningMesh:d3d_Mesh;
     function Read(FromStream:TMemoryStream):Boolean;
   end;

    d3d_ColorRGBA = class(d3d_ancestor)
      red: Single;
      green: Single;
      blue: Single;
      alpha: Single;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_ColorRGB = class(d3d_ancestor)
      red: Single;
      green: Single;
      blue: Single;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Boolean = class(d3d_ancestor)
      truefalse: LongBool;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_TextureFilename = class(d3d_ancestor)
      filename:String;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshFace = class(d3d_ancestor)
      nFaceVertexIndices:DWORD;
      faceVertexIndices:array of DWORD;
    public
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Matrix4x4 = class(d3d_ancestor)
       matrix:t4x4Matrix;
    public
      function Read(FromStream:TMemoryStream):Boolean;
    end;


    d3d_SkinWeights = class(d3d_ancestor)
       Bone_Name : STRING;
       nWeights : DWORD;
       vertexIndices: array of DWORD;// [nWeights];
       weights:array of Single;// [nWeights];
       matrixOffset:d3d_Matrix4x4;
       OwningMesh:d3d_Mesh;
    public
       constructor Create;
       destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
   end;

    d3d_Quaternion = class(d3d_ancestor)
      s : Single;
      v : d3d_Vector;
    public
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_IndexedColor = class(d3d_ancestor)
      index:DWORD;
      indexColor:d3d_ColorRGBA;
    public
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Boolean2d = class(d3d_ancestor)
      u:d3d_Boolean;
      v:d3d_Boolean;
    public
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshFaceWraps  = class(d3d_ancestor)
       nFaceWrapValues:DWORD;
       faceWrapValues:d3d_Boolean2d;
    public
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshTextureCoords = class(d3d_ancestor)
      nTextureCoords:DWORD;
      textureCoords:array of d3d_Coords2d;
    public
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshNormals = class(d3d_ancestor)
       nNormals:DWORD;
       normals:array of d3d_Vector;
       nFaceNormals:DWORD;
       faceNormals: array of d3d_MeshFace;
    public
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Material = class(d3d_ancestor)
      faceColor:d3d_ColorRGBA;
      power:Single;
      specularColor:d3d_ColorRGB;
      emissiveColor:d3d_ColorRGB;
      TextureFilename:d3d_TextureFilename;
    public
      MaterialName:String;
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_FrameTransformMatrix = class(d3d_ancestor)
      frameMatrix:d3d_Matrix4x4;
    public
      constructor Create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshVertexColors = class(d3d_ancestor)
       nVertexColors:DWORD;
       vertexColors:array of d3d_IndexedColor;
    public
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_MeshMaterialList = class(d3d_ancestor)
      nMaterials:DWORD;
      nFaceIndexes:DWORD;
      FaceIndexes:array of DWORD;
      //optional
      MaterialNames:array of string;
      OwningFile:tDirectXFile;
    public
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Mesh = class(d3d_ancestor)
      nVertices:DWORD;
      vertices:array of d3d_Vector;
      nFaces:DWORD;
      faces:array of d3d_MeshFace;
      //Optional Data Elements

      MeshFaceWraps:d3d_MeshFaceWraps;
      MeshTextureCoords:d3d_MeshTextureCoords;
      MeshNormals:d3d_MeshNormals;
      MeshVertexColors:d3d_MeshVertexColors;
      MeshMaterialList:d3d_MeshMaterialList;
      SkinWeights:array of d3d_SkinWeights;
      XSkinMeshHeader:d3d_XSkinMeshHeader;

      owningFrame:d3d_Frame;
      OwningFile:tDirectXFile;

      function GetNormals(ForVertexID:Integer):d3d_Vector;
      function GetUVs(ForVertexID:Integer):d3d_Coords2d;
      function GetNumInfluences(ForVertexID:integer):Integer;
      function GetInfluences(OrderList:TStringList;ForVertexID:Integer):TBoneRecordArray;
      function MeshName:String;
    public
      constructor create;
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;

    d3d_Frame = class(d3d_ancestor)
      //optional
      FrameName:String;
      local_Transform:d3d_FrameTransformMatrix;
      childFrames:array of d3d_Frame;
      childMeshes:array of d3d_mesh;
      OwningFrame:d3d_frame;
      OwningFile:tDirectXFile;
      function NamedFrameCount:Integer;
      function MeshCount:Integer;
      function GetXSkinMeshHeader:d3d_XSkinMeshHeader;
    public
      destructor Destroy; override;
      function Read(FromStream:TMemoryStream):Boolean;
    end;


    xTemplate = class
      TemplateName:String;
      PropNames:Array of string;
      PropTypes:Array of tPropTypes;
      ArrayMembers:Array of String;
      IsOpen:Boolean;
      NextTemplate:xTemplate;
//      Members : xTemplateMembers;
    public
      destructor Destroy; override;
      constructor create;
    end;

    tDirectXFile = class
      MagicNumber:string[4];
      MajorVerNumber:string[2];
      MinorVerNumber:string[2];
      FormatType:String[4];
      Filesize:Single;
      SourceStream:TMemoryStream;
      Templates:xTemplate;
      TheBoneList:TstringList;
    public
      materials:array of d3d_Material;
      Frames:array of d3d_Frame;

      function MeshCount:Integer;
      function NamedFrameCount:integer;
      function GetMesh(Index:Integer):d3d_Mesh;

      function SkinWeightCount:Integer;
      function GetSkinWeight(Index:Integer):d3d_SkinWeights;

      function CreateNewMaterials(MaterialName:string):d3d_Material;
      function CreateNewFrames(FrameName:string):d3d_Frame;
      procedure InitializeBoneList;
      procedure LowerMeshTransforms;
    private
      TemplateEnd:PChar;

      function FindSkinWeights(BoneName:String):d3d_SkinWeights;
      function FindFrame(TargetBoneName:String;var ParentFrameName:String):d3d_Frame;
      procedure ConvertxBone(TargetBone:TBone;SourceFrame:d3d_frame;SourceSkinWeights:d3d_SkinWeights);

      function ReadHeader:Boolean;
      function ReadTemplates:Boolean;
      function ReadMembers:Boolean;
      function GetTextureForMaterial(TargetMaterialName:String):String;
    public
      function WriteMaterial(TargetMaterialName:String):Boolean;
      function WriteSkeleton:Boolean;
      constructor Create(SourceFile:TFileName);
      destructor Destroy; override;
    end;

implementation

uses XFileReaderUnit;

function TBone.Length_of_Bone_Name:LongInt;
begin
     result:=Length(bone_name);
end;

function TBone.number_of_children:LongInt;
begin
     result:=succ(high(child_bones));
end;

function d3d_ancestor.GetFirstValue(Separator:Char):ShortString;
var
   EndPosChar:Char;
begin
     EndPosChar:=DataStringEnd[0];
     DataStringEnd[0]:=#0;
     result:=DataString;
     DataStringEnd[0]:=EndPosChar;
     if Pos(Separator,Result) > 0 then
        delete(result,Pos(Separator,result),Length(result));
     Inc(DataString,Succ(Length(Result)));
     result:=SmartTrim(result);
     if result = '' then
         raise Exception.Create('Unable to find Expected Value!');
     if result[Length(Result)] in [',',';'] then
        Delete(Result,Length(Result),1);
end;


constructor d3d_IndexedColor.Create;
begin
  inherited create;
  indexColor:=d3d_ColorRGBA.Create;
end;

destructor d3d_IndexedColor.Destroy;
begin
  indexColor.Free; //:d3d_ColorRGBA;
  inherited Destroy;
end;

constructor d3d_Boolean2d.Create;
begin
  inherited create;
  u := d3d_Boolean.Create;
  v := d3d_Boolean.Create;
end;

destructor d3d_Boolean2d.Destroy;
begin
  u.Free;  //d3d_Boolean;
  v.Free;  //d3d_Boolean;
  inherited Destroy;
end;

constructor d3d_Quaternion.Create;
begin
  inherited Create;
  v:=d3d_Vector.Create;
end;

destructor d3d_Quaternion.Destroy;
begin
  v.Free;
  inherited Destroy;
end;

constructor d3d_SkinWeights.Create;
begin
  inherited Create;
  matrixOffset:=d3d_Matrix4x4.Create;
end;

destructor d3d_SkinWeights.Destroy;
begin
  matrixOffset.Free;
  inherited Destroy;
end;


constructor d3d_Material.Create;
begin
  inherited create;
  faceColor := d3d_ColorRGBA.Create;
  specularColor := d3d_ColorRGB.Create;
  emissiveColor := d3d_ColorRGB.Create;
end;

destructor d3d_Material.Destroy;
begin
  faceColor.Free;  //d3d_ColorRGBA;
  specularColor.Free;  //d3d_ColorRGB;
  emissiveColor.Free;  //d3d_ColorRGB;
  if assigned(TextureFilename) then
     TextureFilename.Free;  //d3d_TextureFilename;
  inherited Destroy;
end;


constructor d3d_MeshFaceWraps.Create;
begin
     inherited create;
     faceWrapValues:=d3d_Boolean2d.Create;
end;

destructor d3d_MeshFaceWraps.Destroy;
begin
     faceWrapValues.Free;
     inherited Destroy;
end;

destructor d3d_MeshTextureCoords.Destroy;
var
   i:integer;
begin
  for i:=0 to High(textureCoords) do
    textureCoords[i].Free;
  inherited destroy;
end;


destructor d3d_MeshNormals.Destroy;
var
   i:integer;
begin
  for i:=0 to High(normals) do
    normals[i].Free;
  for i:=0 to High(faceNormals) do
    faceNormals[i].Free;
  inherited destroy;
end;

procedure d3d_ancestor.IncPastTrailingComma(TheStream:TMemoryStream);
begin
   DataSTring:=TheStream.Memory;
   Inc(DataSTring,VirtualStreamPos);
   while DataSTring[0] in [#1..#32] do
    begin
      Inc(VirtualStreamPos);
      Inc(DataSTring);
    end;
   if DataSTring[0]=',' then
      Inc(VirtualStreamPos);
end;


procedure d3d_ancestor.IncPastTrailingSemi(TheStream:TMemoryStream);
begin
   DataSTring:=TheStream.Memory;
   Inc(DataSTring,VirtualStreamPos);
   while DataSTring[0] in [#1..#32] do
    begin
      Inc(VirtualStreamPos);
      Inc(DataSTring);
    end;
   if DataString[0]=';' then
      Inc(VirtualStreamPos);
end;

procedure d3d_ancestor.FindEndMarker(TheStream:TMemoryStream);
var
   EndPos:PChar;
begin
   EndPos:=TheStream.Memory;
   Inc(EndPos,VirtualStreamPos);
   EndPos:=StrFScan(EndPos,'}');
   if assigned(EndPos) then
      VirtualStreamPos:=Succ(EndPos-TheStream.Memory);
end;


procedure d3d_ancestor.IncrementMemoryStream(TheStream:TMemoryStream;HowManySemiColons:integer);
begin
     DataString:=TheStream.Memory;
     Inc(DataString,VirtualStreamPos);
     DataStringEnd:=DataString;
     repeat
       DataStringEnd:=StrFScan(DataStringEnd,';');
       if not assigned(DataStringEnd) then
          Raise Exception.Create('Unable to Find Target Semicolon!');
       Inc(DataStringEnd);
       Dec(HowManySemiColons);
     until HowManySemiColons =0;
     VirtualStreamPos:=DataStringEnd-TheStream.Memory;
end;


function d3d_MeshFaceWraps.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,1);
      nFaceWrapValues:=StrToInt(GetFirstValue(';'));
      faceWrapValues.Read(FromStream);
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_ColorRGBA.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,4);
      red:=StrToFloat(GetFirstValue(';'));
      green:=StrToFloat(GetFirstValue(';'));
      blue:=StrToFloat(GetFirstValue(';'));
      alpha:=StrToFloat(GetFirstValue(';'));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_ColorRGB.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,3);
      red:=StrToFloat(GetFirstValue(';'));
      green:=StrToFloat(GetFirstValue(';'));
      blue:=StrToFloat(GetFirstValue(';'));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_IndexedColor.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,1);
      index:=StrToInt(GetFirstValue(';'));
      indexColor.Read(FromStream);
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Boolean.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,1);
      truefalse:=LongBool(StrToInt(GetFirstValue(';')));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Quaternion.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,1);
      s:=StrToFloat(GetFirstValue(';'));
      v.Read(FromStream);
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Vector.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,3);
      x:=StrToFloat(GetFirstValue(';'));
      y:=StrToFloat(GetFirstValue(';'));
      z:=StrToFloat(GetFirstValue(';'));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Coords2d.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      IncrementMemoryStream(FromStream,2);
      u:=StrToFloat(GetFirstValue(';'));
      v:=StrToFloat(GetFirstValue(';'));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Boolean2d.Read(FromStream:TMemoryStream):Boolean;
begin
   try
      u.Read(FromStream);
      v.Read(FromStream);
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_MeshFace.Read(FromStream:TMemoryStream):Boolean;
var
   i:integer;
begin
   try
      IncrementMemoryStream(FromStream,1);
      nFaceVertexIndices:=StrToInt(GetFirstValue(';'));
      SetLength(faceVertexIndices,nFaceVertexIndices);
      //only do one because it's an array!
      IncrementMemoryStream(FromStream,1);
      for i:=0 to Pred(nFaceVertexIndices) do
         faceVertexIndices[i]:=StrToInt(GetFirstValue(','));
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;

function d3d_Matrix4x4.Read(FromStream:TMemoryStream):Boolean;
var
   x,y:integer;
begin
   try
      IncrementMemoryStream(FromStream,1);
      for x:=1 to 4 do
       for y:=1 to 4 do
         matrix[x,y]:=StrToFloat(GetFirstValue(','));
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
end;


function d3d_MeshTextureCoords.Read(FromStream:TMemoryStream):Boolean;
var
   i:integer;
begin
 try
   try
      IncrementMemoryStream(FromStream,1);
      nTextureCoords:=StrToInt(GetFirstValue(';'));
      SetLength(TextureCoords,nTextureCoords);
      for i:=0 to Pred(nTextureCoords) do
       begin
         TextureCoords[i]:=d3d_Coords2d.Create;
         TextureCoords[i].Read(FromStream);
         IncPastTrailingComma(FromStream);
       end;
      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
 finally
   FindEndMarker(FromStream);
 end;
end;

function d3d_MeshNormals.Read(FromStream:TMemoryStream):Boolean;
var
   i:integer;
begin
 try
   try
      IncrementMemoryStream(FromStream,1);
      nNormals:=StrToInt(GetFirstValue(';'));
      SetLength(Normals,nNormals);
      for i:=0 to Pred(nNormals) do
       begin
         if i MOD 10 = 0 then
          begin
            ConverterGUI.Label1.Caption:='Reading Mesh Normals #: '+IntToStr(i)+' of '+IntToStr(nNormals);
            Application.ProcessMessages;
          end;

         Normals[i]:=d3d_Vector.Create;
         Normals[i].Read(FromStream);
         IncPastTrailingComma(FromStream);
       end;

      IncrementMemoryStream(FromStream,1);
      nFaceNormals:=StrToInt(GetFirstValue(';'));
      SetLength(FaceNormals,nFaceNormals);
      for i:=0 to Pred(nFaceNormals) do
       begin
         if i MOD 10 = 0 then
          begin
            ConverterGUI.Label1.Caption:='Reading Face Normals #: '+IntToStr(i)+' of '+IntToStr(nFaceNormals);
            Application.ProcessMessages;
          end;
         FaceNormals[i]:=d3d_MeshFace.Create;
         FaceNormals[i].Read(FromStream);
         IncPastTrailingComma(FromStream);
       end;

      IncPastTrailingSemi(FromStream);
      Result:=True;
   except
      Result:=False;
      raise;
   end;
 finally
   FindEndMarker(FromStream);
 end;
end;


function d3d_TextureFilename.Read(FromStream:TMemoryStream):Boolean;
begin
 try
   try
      IncrementMemoryStream(FromStream,1);
      filename:=GetFirstValue(';');
      result:=True;
   except
      result:=False;
      raise;
   end;
 finally
   FindEndMarker(FromStream);
 end;
end;

function d3d_XSkinMeshHeader.Read(FromStream:TMemoryStream):Boolean;
var
   TemplateType,TemplateName:String;
begin
  try
    try
       IncrementMemoryStream(FromStream,3);
       nMaxSkinWeightsPerVertex:=StrToInt(GetFirstValue(';'));
       nMaxSkinWeightsPerFace:=StrToInt(GetFirstValue(';'));
       nBones:=StrToInt(GetFirstValue(';'));
       while GetNextTemplate(TemplateType,TemplateName,FromStream) do
         if CompareText(templateType,'skinWeights')=0 then
          with OwningMesh do
          begin
               SetLength(SkinWeights,High(SkinWeights)+2);
               SkinWeights[High(SkinWeights)]:=d3d_SkinWeights.Create;
               SkinWeights[High(SkinWeights)].Read(FromStream);
          end
         else
           SkipTemplate(ConverterGUI.SkippedMembers.Items,FromStream,'d3d_XSkinMeshHeader SubMember: '+TemplateType);
       result:=True;
    except
       result:=false;
       raise;
    end;
 finally
   FindEndMarker(FromStream);
 end;
end;


function d3d_SkinWeights.Read(FromStream:TMemoryStream):Boolean;
var
   i:Integer;
begin
  try
   try
       IncrementMemoryStream(FromStream,2);
       Bone_Name:=SmartTrim(GetFirstValue(';'));

       nWeights :=StrToInt(GetFirstValue(';'));
       SetLength(vertexIndices,nWeights);

       //For Array
       IncrementMemoryStream(FromStream,1);
       for i:=0 to Pred(nWeights) do
         VertexIndices[i]:=StrToInt(GetFirstValue(','));
       IncPastTrailingSemi(FromStream);

       SetLength(weights,nWeights);
       //For Array
       IncrementMemoryStream(FromStream,1);
       for i:=0 to Pred(nWeights) do
         weights[i]:=StrToFloat(GetFirstValue(','));
       IncPastTrailingSemi(FromStream);

       matrixOffset.Read(FromStream);
       result:=True;
   except
       result:=False;
       raise;
   end;
 finally
   FindEndMarker(FromStream);
 end;
end;


function d3d_Material.Read(FromStream:TMemoryStream):Boolean;
var
   TemplateType,TemplateName:String;
begin
  try
   try
      faceColor.Read(FromStream);
      IncrementMemoryStream(FromStream,1);
      power:=StrToFloat(GetFirstValue(';'));
      specularColor.Read(FromStream);
      emissiveColor.Read(FromStream);
      if GetNextTemplate(TemplateType,TemplateName,FromStream) then
        if CompareText(templateType,'TextureFilename')=0 then
         begin
           TextureFilename:=d3d_TextureFileName.Create;
           TextureFilename.Read(FromStream);
           if not FileExists(TextureFileName.filename) then
             begin
               MessageBox(GetActiveWIndow(),PChar('Missing Texture: '+TextureFileName.FileName),'Attention',MB_ICONERROR);
               FreeAndNil(TextureFilename);
             end
{$IFDEF FF3D}
           else if ConverterGUI.CreateFF3DCZP.Checked and (CompareText(ExtractFileExt(TextureFileName.filename),'.tga') <> 0) then
            try
              ConverterGUI.ImageConverter.LoadFromFile(TextureFileName.filename);
              TextureFileName.FileName:=ChangeFileExt(TextureFileName.filename,'.tga');
              ConverterGUI.ImageConverter.SaveToFileTGA(TextureFileName.filename);
            except
              MessageBox(GetActiveWindow,PChar('Failed Converting Texture '+TextureFileName.FileName+'to TGA format'),'Attention',MB_ICONERROR);
            end
{$ENDIF FF3D};
         end
        else
          SkipTemplate(ConverterGUI.SkippedMembers.Items,FromStream,'d3d_Material SubMember: '+TemplateType);
      result:=True;
    except
      result:=false;
      raise;
   end;
  finally
   FindEndMarker(FromStream);
 end;
end;

function d3d_Frame.Read(FromStream:TMemoryStream):Boolean;
var
   TemplateType,TemplateName:String;
begin
  try
   try
      ConverterGUI.Label1.Caption:='Reading Frame: '+FrameName;
      Application.ProcessMessages;

      while GetNextTemplate(TemplateType,TemplateName,FromStream) do
        if CompareText(templateType,'FrameTransformMatrix')=0 then
         begin
            local_Transform:=d3d_FrameTransformMatrix.Create;
            local_Transform.Read(FromStream);
         end
        else if CompareText(templateType,'mesh')=0 then
         begin
           SetLength(ChildMeshes,High(ChildMeshes)+2);
           ChildMeshes[High(ChildMeshes)]:=d3d_mesh.Create;
           ChildMeshes[High(ChildMeshes)].OwningFrame:=self;
           ChildMeshes[High(ChildMeshes)].OwningFile:=OwningFile;
           ChildMeshes[High(ChildMeshes)].Read(FromStream);
         end
        else if CompareText(templateType,'frame')=0 then
          begin
            SetLength(ChildFrames,High(ChildFrames)+2);
            childFrames[High(ChildFrames)]:=d3d_Frame.Create;
            childFrames[High(ChildFrames)].FrameName:=TemplateName;
            childFrames[High(ChildFrames)].OwningFrame:=self;
            childFrames[High(ChildFrames)].OwningFile:=OwningFile;
            childFrames[High(ChildFrames)].Read(FromStream);
          end
        else if CompareText(templateType,'skinweights')=0 then
         begin
            if High(ChildMeshes) > -1 then
            with ChildMeshes[HIgh(ChildMeshes)] do
            begin
                 SetLength(SkinWeights,High(SkinWeights)+2);
                 SkinWeights[High(SkinWeights)]:=d3d_SkinWeights.Create;
                 SkinWeights[High(SkinWeights)].OwningMesh:=ChildMeshes[High(ChildMeshes)];
                 SkinWeights[High(SkinWeights)].Read(FromStream);
            end
           else
             raise Exception.Create('No Mesh for SkinWeights!');
         end
        else if CompareText(templateType,'XskinMeshHeader')=0 then
         begin
            if High(ChildMeshes) > -1 then
            with ChildMeshes[High(ChildMeshes)] do
             begin
              XSkinMeshHeader:=d3d_XSkinMeshHeader.Create;
              XSkinMeshHeader.Read(FromStream);
             end
            else
             raise Exception.Create('No Mesh for XSkinMeshHeader!');
         end
         else
          SkipTemplate(ConverterGUI.SkippedMembers.Items,FromStream,'d3d_Frame SubMember: '+TemplateType);
       result:=True;
    except
      result:=false;
      raise;
   end;
  finally
   Application.ProcessMessages;
   FindEndMarker(FromStream);
 end;
end;

function d3d_MeshMaterialList.Read(FromStream:TMemoryStream):Boolean;
var
   I:Integer;
   TemplateType,TemplateName:String;
   LiveMaterial:d3d_material;
begin
  try
   try
      IncrementMemoryStream(FromStream,2);
      nMaterials:=StrToInt(GetFirstValue(';'));
      nFaceIndexes:=StrToInt(GetFirstValue(';'));
      SetLength(FaceIndexes,nFaceIndexes);
      if nFaceIndexes > 0 then
       begin
         //only do one because it's an array!
         IncrementMemoryStream(FromStream,1);
         for i:=0 to Pred(nFaceIndexes) do
            FaceIndexes[i]:=StrToInt(GetFirstValue(','));
         IncPastTrailingSemi(FromStream);
       end;
      while GetNextTemplate(TemplateType,TemplateName,FromStream) do
       if CompareText(TemplateType,'Material')=0 then
        with OwningFile do
        begin
              LiveMaterial:=CreateNewMaterials(TemplateName);
              LiveMaterial.Read(SourceStream);
              SetLength(MaterialNames,High(MaterialNames)+2);
              MaterialNames[High(MaterialNames)]:=LiveMaterial.MaterialName;
        end
       else
        SkipTemplate(ConverterGUI.SkippedMembers.Items,FromStream,'d3d_MeshMaterialList SubMember: '+TemplateType);

      while GetNextBlankTemplate(TemplateName,FromStream) do
        begin
          SetLength(MaterialNames,High(MaterialNames)+2);
          MaterialNames[High(MaterialNames)]:=TemplateName;
        end;

      result:=True;
    except
      result:=false;
      raise;
   end;
  finally
   FindEndMarker(FromStream);
 end;
end;

function d3d_Mesh.GetNumInfluences(ForVertexID:integer):Integer;
var
   SkinID,WeightID:Integer;
begin
     result:=0;
     if High(SkinWeights)=-1 then
        exit;
     for SkinID:=0 to High(SkinWeights) do
      with SkinWeights[SkinID] do
        if vertexIndices[Pred(nWeights)] < ForVertexID then
           continue
        else for WeightID:=0 to pred(nWeights) do
         if vertexIndices[WeightID]=ForVertexID then
           begin
             Inc(Result);
             break;
           end
         else if vertexIndices[WeightID] > ForVertexID then
           break;
end;

function d3d_Mesh.GetInfluences(OrderList:TStringList;ForVertexID:Integer):TBoneRecordArray;
var
   TargetBoneID,SkinID,WeightID:Integer;
begin
    SetLength(result.BoneID,0);
    SetLength(Result.InfluenceAmt,0);

     if High(SkinWeights)=-1 then
        exit;
     for SkinID:=0 to High(SkinWeights) do
      with SkinWeights[SkinID] do
        if vertexIndices[Pred(nWeights)] < ForVertexID then
           continue
        else for WeightID:=0 to pred(nWeights) do
          if vertexIndices[WeightID]=ForVertexID then
           with result do
            begin
               SetLength(BoneID, High(BoneID)+2);
               SetLength(InfluenceAmt,High(InfluenceAmt)+2);
               TargetBoneID:=OrderList.IndexOf(Bone_Name);
               if TargetBoneID = -1 then
                  raise Exception.Create('Cannot find Corresponding Bone: '+Bone_Name+'!');
               BoneID[High(BoneID)]:=TargetBoneID;
               InfluenceAmt[High(InfluenceAmt)]:=Weights[WeightID];
               break;
            end
          else if vertexIndices[WeightID] > ForVertexID then
            break;
end;


function d3d_Mesh.GetUVs(ForVertexID:Integer):d3d_Coords2d;
begin
  result:=Nil;

  if assigned(MeshTextureCoords) and (ForVertexID < MeshTextureCoords.nTextureCoords) then
     result:=MeshTextureCoords.textureCoords[ForVertexID];
end;

function d3d_Mesh.MeshName:String;
var
   CheckFrame:d3d_frame;
begin
   CheckFrame:=owningFrame;
   while assigned(CheckFrame) and (CheckFrame.FrameName = '') do
     CheckFrame:=CheckFrame.owningframe;
   if assigned(CheckFrame) then
    begin
      result:=CheckFrame.frameName;
      if Copy(Result,1,1)='!' then
         delete(result,1,1);
      end
   else
     result:='';
end;

function d3d_Mesh.GetNormals(ForVertexID:Integer):d3d_Vector;
var
   FaceID:Integer;
   VertexIndex:Integer;
   TargetNormalID:Integer;
begin
     result:=nil;
     if not assigned(MeshNormals) then
        exit;
     for FaceID:=0 to Pred(nFaces) do
      for VertexIndex:=0 to Pred(faces[faceID].nFaceVertexIndices) do
       if Faces[FaceID].faceVertexIndices[VertexIndex]=ForVertexID then
       begin
          if (FaceID < MeshNormals.nFaceNormals) and
             (VertexIndex < MeshNormals.faceNormals[FaceID].nFaceVertexIndices) then
               TargetNormalID:=MeshNormals.faceNormals[FaceID].faceVertexIndices[VertexIndex]
          else
            raise Exception.Create('Unable to Find Normal!');
          if TargetNormalID < MeshNormals.nNormals then
             result:=MeshNormals.Normals[TargetNormalID]
          else
            raise Exception.Create('Unable to Find Normal!');
          exit;
       end;
end;


function d3d_Mesh.Read(FromStream:TMemoryStream):Boolean;
var
   i:Integer;
   TemplateType,TemplateName:String;
begin
  ConverterGUI.BringToFront;
  ConverterGUI.REfresh;
  Application.ProcessMessages;
  try
   try
      IncrementMemoryStream(FromStream,1);
      nVertices:=StrToInt(GetFirstValue(';'));
      SetLength(Vertices,nVertices);
      if nVertices > 0 then
       for i:=0 to pred(nVertices) do
         begin
           if i MOD 10 = 0 then
            begin
              ConverterGUI.Label1.Caption:='Reading Vertices #: '+IntToStr(i)+' of '+IntToStr(nVertices);
              Application.ProcessMessages;
            end;
           Vertices[i]:=d3d_Vector.Create;
           Vertices[i].Read(FromStream);
           IncPastTrailingComma(FromStream);
         end;

      IncrementMemoryStream(FromStream,1);
      nFaces:=StrToInt(GetFirstValue(';'));
      SetLength(Faces,nFaces);
      if nFaces > 0 then
       for i:=0 to pred(nFaces) do
         begin
           if i MOD 10 = 0 then
            begin
              ConverterGUI.Label1.Caption:='Reading face #: '+IntToStr(i)+' of '+IntToStr(nFaces);
              Application.ProcessMessages;
            end;
           Faces[i]:=d3d_MeshFace.Create;
           Faces[i].Read(FromStream);
           IncPastTrailingComma(FromStream);
         end;
      IncPastTrailingSemi(FromStream);

      while GetNextTemplate(TemplateType,TemplateName,FromStream) do
         if CompareText(templateType,'MeshFaceWraps')=0 then
          begin
            MeshFaceWraps:=d3d_MeshFaceWraps.Create;
            MeshFaceWraps.Read(FromStream);
          end
         else if CompareText(templateType,'MeshTextureCoords')=0 then
          begin
            MeshTextureCoords:=d3d_MeshTextureCoords.Create;
            MeshTextureCoords.Read(FromStream);
          end
         else if CompareText(templateType,'MeshNormals')=0 then
          begin
            MeshNormals:=d3d_MeshNormals.Create;
            MeshNormals.Read(FromStream);
          end
         else if CompareText(templateType,'MeshVertexColors')=0 then
          begin
            MeshVertexColors:=d3d_MeshVertexColors.Create;
            MeshVertexColors.Read(FromStream);
          end
         else if CompareText(templateType,'MeshMaterialList')=0 then
          begin
            MeshMaterialList:=d3d_MeshMaterialList.Create;
            MeshMaterialList.OwningFile:=OwningFile;
            MeshMaterialList.Read(FromStream);
          end
         else if CompareText(templateType,'XSkinMeshHeader')=0 then
         begin
            XSkinMeshHeader:=d3d_XSkinMeshHeader.Create;
            XSkinMeshHeader.Read(FromStream);
          end
         else if CompareText(templateType,'SkinWeights')=0 then
          begin
            SetLength(SkinWeights,High(SkinWeights)+2);
            SkinWeights[High(SkinWeights)]:=d3d_SkinWeights.Create;
            SkinWeights[High(SkinWeights)].OwningMesh:=Self;
            SkinWeights[High(SkinWeights)].Read(FromStream);
          end
         else
           SkipTemplate(ConverterGUI.SkippedMembers.Items,FromStream,'d3d_Mesh SubMember: '+TemplateType);
      result:=True;
    except
      result:=false;
      raise;
   end;
  finally
   FindEndMarker(FromStream);
 end;
end;


function d3d_FrameTransformMatrix.Read(FromStream:TMemoryStream):Boolean;
begin
  try
   try
      frameMatrix.Read(FromStream);
      result:=True;
    except
      result:=False;
      raise;
    end;
  finally
   FindEndMarker(FromStream);
 end;
end;

function d3d_MeshVertexColors.Read(FromStream:TMemoryStream):Boolean;
var
   i:Integer;
begin
  try
   try
      IncrementMemoryStream(FromStream,1);
      nVertexColors:=StrToInt(GetFirstValue(';'));
      SetLength(vertexColors,nVertexColors);
      if nVertexColors > 0 then
       for i:=0 to pred(nVertexColors) do
         begin
           vertexColors[i]:=d3d_IndexedColor.Create;
           vertexColors[i].Read(FromStream);
           IncPastTrailingComma(FromStream);
         end;
      result:=true;
    except
      result:=False;
      raise;
    end;
  finally
   FindEndMarker(FromStream);
 end;
end;


constructor d3d_FrameTransformMatrix.Create;
begin
   inherited create;
   frameMatrix:=d3d_Matrix4x4.Create;
end;

destructor d3d_FrameTransformMatrix.Destroy;
begin
   frameMatrix.free;
   inherited Destroy;
end;

destructor d3d_MeshVertexColors.Destroy;
var
   i:integer;
begin
   for i:=0 to High(vertexColors) do
     vertexColors[i].Free;
   inherited Destroy;
end;

destructor d3d_MeshMaterialList.Destroy;
var
   i:integer;
begin
   for i:=0 to High(MaterialNames) do
     MaterialNames[i]:='';
   SetLength(MaterialNames,0);
   inherited Destroy;
end;

constructor d3d_Mesh.create;
begin
   inherited create;
   MeshFaceWraps:=Nil;
   MeshTextureCoords:=Nil;
   MeshNormals:=Nil;
   MeshVertexColors:=Nil;
   MeshMaterialList:=Nil;
   XSKinMeshHeader:=nil;
end;

destructor d3d_Mesh.Destroy;
var
   i:integer;
begin
   for i:=0 to High(vertices) do
     vertices[i].Free;
   for i:=0 to High(faces) do
     faces[i].Free;
   for i:=0 to High(SkinWeights) do
      SkinWeights[i].Free;

   if assigned(MeshFaceWraps) then
      MeshFaceWraps.Free;
   if assigned(MeshTextureCoords) then
      MeshTextureCoords.Free;
   if assigned(MeshNormals) then
      MeshNormals.Free;
   if assigned(MeshVertexColors) then
      MeshVertexColors.Free;
   if assigned(MeshMaterialList) then
      MeshMaterialList.Free;
   if assigned(XSkinMeshHeader) then
      XSkinMeshHeader.Free;
   inherited Destroy;
end;

destructor d3d_Frame.Destroy;
var
   i:Integer;
begin
   if assigned(local_Transform) then
      local_Transform.Free;
   if High(childFrames) > -1 then
     for i:=0 to High(ChildFrames) do
       childFrames[i].Free;
   if High(childMeshes) > -1 then
      for i:=0 to High(childMeshes) do
        ChildMeshes[i].Free;
   inherited Destroy;
end;

destructor xTemplate.Destroy;
begin
     if NextTemplate <> nil then
        NextTemplate.Free;
     inherited Destroy;
end;


constructor xTemplate.create;
begin
     inherited create;
     NextTemplate:=Nil;
     IsOpen:=False;
end;


function d3d_frame.NamedFrameCount:Integer;
var
   i:Integer;
begin
     if FrameName <> '' then
        result:=1
     else
        result:=0;
     if high(ChildFrames) > -1 then
      for i:=0 to High(ChildFrames) do
        Inc(Result,ChildFrames[i].NamedFrameCount);
end;

function d3d_frame.MeshCount:Integer;
var
   i:Integer;
begin
     result:=Succ(High(ChildMeshes));
     if high(ChildFrames) > -1 then
      for i:=0 to High(ChildFrames) do
        Inc(Result,ChildFrames[i].MeshCount);
end;

function d3d_frame.GetXSkinMeshHeader:d3d_XSkinMeshHeader;
var
   i:Integer;
begin
     result:=Nil;
     if high(ChildMeshes) > -1 then
      for i:=0 to High(ChildMeshes) do
       begin
        result:=ChildMeshes[i].XSkinMeshHeader;
        if result <> nil then
           exit;
       end;
      if High(ChildFrames) > -1 then
       for i:=0 to High(ChildFrames) do
        begin
          Result:=ChildFrames[i].GetXSkinMeshHeader;
          if result <> nil then
             exit;
        end;
end;

function tDirectXFile.NamedFrameCount:integer;
var
   I:Integer;
begin
     result:=0;
     for I:=0 to high(frames) do
        Inc(result,frames[i].NamedFrameCount);
end;


function tDirectXFile.MeshCount:Integer;
var
   I:Integer;
begin
     result:=0;
     for I:=0 to high(frames) do
       Inc(Result,frames[i].MeshCount);
end;

function tDirectXFile.SkinWeightCount:Integer;

procedure AddChildWeightCount(Frame:d3d_Frame);
var
   i:Integer;
   MeshID:integer;
begin
     if High(frame.childMeshes) > -1 then
        for MeshID:=0 to High(frame.ChildMeshes) do
          Inc(result,Succ(High(frame.ChildMeshes[MeshID].SkinWeights)));

     if High(Frame.ChildFrames) > -1 then
      for i:=0 to High(Frame.ChildFrames) do
         AddChildWeightCount(Frame.ChildFrames[i]);
end;

var
   i:Integer;
begin
     result:=0;
     for I:=0 to high(frames) do
        AddChildWeightCount(frames[i]);
end;

function tDirectXFile.GetSkinWeight(Index:Integer):d3d_SkinWeights;

function GetChildWeight(Frame:d3d_Frame):d3d_SkinWeights;
var
   i:Integer;
   MeshID:integer;
begin
    result:=Nil;
     if High(frame.childMeshes) > -1 then
      begin
        for MeshID:=0 to High(frame.ChildMeshes) do
         begin
          if Index <= High(frame.ChildMeshes[MeshID].SkinWeights) then
            begin
              result:=frame.ChildMeshes[MeshID].SkinWeights[Index];
              exit;
            end;
          Dec(Index,Succ(High(frame.ChildMeshes[MeshID].SkinWeights)));
         end;
      end;

     if High(Frame.ChildFrames) > -1 then
      for i:=0 to High(Frame.ChildFrames) do
       begin
         result:=GetChildWeight(Frame.ChildFrames[i]);
         if assigned(Result) then
            exit;
       end;
end;

var
   i:Integer;
begin
     result:=Nil;
     for I:=0 to high(frames) do
      begin
        result:=GetChildWeight(frames[i]);
        if assigned(Result) then
           exit;
      end;
end;

function tDirectXFile.GetMesh(Index:Integer):d3d_Mesh;

function GetChildMesh(Frame:d3d_Frame):d3d_mesh;
var
   i:Integer;
begin
    result:=Nil;
     if High(frame.childMeshes) > -1 then
      begin
        if Index <= High(frame.childMeshes) then
         begin
          result:=frame.ChildMeshes[Index];
          exit;
         end;

        Dec(Index,Succ(High(frame.childMeshes)));
      end;
     if High(Frame.ChildFrames) > -1 then
      for i:=0 to High(Frame.ChildFrames) do
       begin
         result:=GetChildMesh(Frame.ChildFrames[i]);
         if assigned(Result) then
            exit;
       end;
end;

var
   i:Integer;
begin
     result:=Nil;
     for I:=0 to high(frames) do
      begin
        result:=GetChildMesh(frames[i]);
        if assigned(Result) then
           exit;
      end;
end;

function tDirectXFile.FindSkinWeights(BoneName:String):d3d_SkinWeights;
var
   MeshID,SkinID:Integer;
begin
     result:=nil;
     for MeshID:=0 to Pred(MeshCount) do
      with GetMesh(MeshID) do
       if high(SkinWeights) > -1 then
         for SkinID:=0 to High(SkinWeights) do
          if CompareText(BoneName,SkinWeights[SkinID].Bone_Name)=0 then
            begin
              result:=SkinWeights[SkinID];
              exit;
            end;
end;

procedure tDirectXFile.LowerMeshTransforms;

procedure ApplyTransformation(Thevertices:d3d_vector;TheTransform:d3d_Matrix4x4);
var
   NewX,NewY,NewZ:Single;
   m:t4x4matrix;
begin
  M:=TheTransform.matrix;

  Newx:=TheVertices.x*m[1,1]+TheVertices.y*m[2,1]+TheVertices.z*m[3,1]+m[4,1];
  Newy:=TheVertices.x*m[1,2]+TheVertices.y*m[2,2]+TheVertices.z*m[3,2]+m[4,2];
  Newz:=TheVertices.x*m[1,3]+TheVertices.y*m[2,3]+TheVertices.z*m[3,3]+m[4,3];
  TheVertices.x:=NewX;
  TheVertices.Y:=NewY;
  TheVertices.Z:=NewZ;

end;

function t4x4MatrixInvert(PassedBoneID:Integer):T4x4Matrix;
var
   m2:TMatrix;
   r,c:Integer;
begin
   try
     for r:=1 to 4 do
      for c:=1 to 4 do
        m2[Pred(r),Pred(c)]:=TBone(TheBoneList.Objects[PassedBoneID]).AdjustedMatrix[r,c];

     MatrixInvert(m2);
   except
     MessageBox(GetActiveWindow(),Pchar(FloatToStr(M2[0,0])),'Attention',MB_ICONERROR);
   end;

   for r:=1 to 4 do
    for c:=1 to 4 do
      result[r,c]:=m2[Pred(r),Pred(c)];
end;

function t4x4MatrixMultiply(const M1, M2: T4x4Matrix): T4x4Matrix;
var I, J: Integer;

begin
  for I := 1 to 4 do
    for J := 1 to 4 do
      result[I,J] := M1[I, 1] * M2[1, J] +
                     M1[I, 2] * M2[2, J] +
                     M1[I, 3] * M2[3, J] +
                     M1[I, 4] * M2[4, J];
end;

var
   Transforms:Array of d3d_FrameTransformMatrix;
   FrameID,MeshID:Integer;
   TheMesh:d3d_mesh;
   Theowningframe:d3d_frame;
   v,TransformID,MeshIndex:Integer;
   TransformNames:array of String;
   LocalAdjustor:Single;

procedure SetupTransformMatrix(TheWeight:d3d_SkinWeights;var TargetMatrix:t4x4Matrix);
var
   BoneIndex:Integer;
begin
     BoneIndex:=TheBoneList.IndexOf(TheWeight.Bone_Name);
     if BoneIndex = -1 then
        raise Exception.Create('Unable to find Bone in Skeleton: '+TheWeight.Bone_Name);
     TargetMatrix:=t4x4MatrixMultiply(TheWeight.matrixOffset.matrix,t4x4MatrixInvert(BoneIndex));
end;

function NamedOwningFrame(FromMesh:d3d_mesh):String;
var
   WorkFrame:d3d_frame;
begin
   WorkFrame:=FromMesh.owningframe;
   while assigned(WorkFrame) and not ((WorkFrame.FrameName <> '') and (TheBoneList.IndexOf(WorkFrame.FrameName) > -1)) do
      WorkFrame:=WorkFrame.OwningFrame;
   if assigned(WorkFrame) and (WorkFrame.FrameName <> '') and (TheBoneList.IndexOf(WorkFrame.FrameName) > -1) then
     result:=WOrkFrame.FrameName
   else
     result:='';
end;

procedure ProcessMeshAdjustment;
var
   v:integer;
   LocalAdjustor:Single;
begin
   with TheMesh.skinWeights[0].matrixOffset do
     LocalAdjustor:=Sqrt(Sqr(Matrix[1,1])+Sqr(Matrix[1,2])+Sqr(Matrix[1,3]));
   with TheMesh do
    begin
     for V:=0 to Pred(nVertices) do
      with TheMesh.vertices[V] do
       begin
        x:=x*LocalAdjustor;
        y:=y*LocalAdjustor;
        z:=z*LocalAdjustor;
       end;
   end
end;

procedure ProcessFrameTransforms(OwningBone:String = '');
begin
      Theowningframe:=TheMesh.owningFrame;
      setLength(Transforms,0);
      setLength(TransformNames,0);
      while assigned(TheOwningFrame) do
       begin
         if (OwningBone <> '') and (TheOwningframe.FrameName = OwningBone) then
            exit;
         if assigned(TheOwningFrame) and assigned(TheOwningFrame.local_Transform) then
          begin
            setLength(Transforms,High(Transforms)+2);
            Transforms[High(Transforms)]:=TheOwningFrame.local_Transform;
            SetLength(TransformNames,High(TransformNames)+2);
            TransformNames[High(TransformNames)]:=TheOwningFrame.FrameName;
          end;
         TheOwningFrame:=TheOwningFrame.OwningFrame;
      end;
end;

procedure ProcessBufferedTransforms(freeTransform:Boolean = false);
var
   v:Integer;
   TransformID:Integer;
begin
  if High(Transforms) > -1 then
   for TransformID:= 0 to High(Transforms) do
    begin
      with TheMesh do
        for V:=0 to Pred(nVertices) do
            ApplyTransformation(TheMesh.vertices[V],Transforms[TransformID].frameMatrix);
      if FreeTransform then 
         Transforms[TransformID].Free;
    end;
end;

var
   WeightID:Integer;
   BoneIndex:Integer;
   TargetSkinWeights:d3d_SkinWeights;
   OwningBone:String;
   r,c:integer;

   lvertexIndices: array of DWORD;// [nWeights];
   lweights:array of Single;// [nWeights];

begin
   MeshIndex:=0;
   for FrameID:=0 to High(Frames) do
    begin
      if Frames[FrameID].MeshCount > 0 then
       for MeshID:=0 to Pred(Frames[FrameID].MeshCount) do
        begin
          TheMesh:=GetMesh(MeshIndex);
          Inc(MeshIndex);
          if ConverterGUI.DiscardUnderXFaces.Checked and (TheMesh.nFaces <= ConverterGUI.FaceCount.Value) then
             continue;
          if not assigned(TheMesh) then
             raise Exception.Create('Huh -- Where''s the Mesh?');

          SetLength(Transforms,0);
          if (High(TheMesh.SkinWeights) = -1) then
           begin
             OwningBone:=NamedOwningFrame(TheMesh);
             if OwningBone <> '' then
              begin
                 ProcessFrameTransforms(OwningBone);
                 if High(Transforms) > -1 then
                    ProcessBufferedTransforms;
                 SetLength(Transforms,0);


                SetLength(TheMesh.SkinWeights,1);
                TheMesh.SkinWeights[0]:=d3d_SkinWeights.Create;
                TheMesh.SkinWeights[0].Bone_Name:=OwningBone;
                for r:=1 to 4 do
                 for c:=1 to 4 do
                    TheMesh.SkinWeights[0].MatrixOffset.Matrix[r,c]:=Ord(r=c);

                TheMesh.SkinWeights[0].nWeights:=TheMesh.nVertices;
                SetLength(TheMesh.SkinWeights[0].weights, TheMesh.nVertices);
                SetLength(TheMesh.SkinWeights[0].vertexIndices, TheMesh.nVertices);
                for V:=0 to Pred(TheMesh.nVertices) do
                  begin
                     TheMesh.SkinWeights[0].vertexIndices[v]:=v;
                     TheMesh.SkinWeights[0].weights[v]:=1.0;
                  end;

                 SetLength(Transforms,1);
                 transforms[0]:=d3d_FrameTransformMatrix.Create;
                 BoneIndex:=TheBoneList.IndexOf(OwningBone);
                 if BoneIndex = -1 then
                     raise Exception.Create('Unable to find Bone in Skeleton: '+OwningBone);
                 transforms[0].FrameMatrix.Matrix:=t4x4MatrixInvert(BoneIndex);
                 ProcessBufferedTransforms(True);
                 SetLength(Transforms,0);
                 continue;
              end;
           end;
          if (High(TheMesh.SkinWeights) > -1) then //is Skinned
           begin
             BoneIndex:=TheBoneList.IndexOf(TheMesh.SkinWeights[0].Bone_name);
             if TBone(TheBoneList.Objects[BoneIndex]).SourceMesh = TheMesh then
              begin
                ProcessMeshAdjustment;
                continue;
              end
             else
               begin
                   SetLength(Transforms,0);
                   SetLength(Transforms,1);
                   transforms[0]:=d3d_FrameTransformMatrix.Create;
                   SetupTransformMatrix(TheMesh.SkinWeights[0],transforms[0].frameMatrix.Matrix);
               end;
           end
          else
             processFrameTransforms;

          ProcessBufferedTransforms(High(TheMesh.SkinWeights) > -1);
        end;
    end;
end;


function tDirectXFile.FindFrame(TargetBoneName:String;var ParentFrameName:String):d3d_Frame;

function LocalFindFrame(SearchFrames:Array of d3d_Frame):d3d_frame;
var
   FrameID:Integer;
begin
     result:=Nil;

     For FrameID:=0 to High(SearchFrames) do
      if CompareText(SearchFrames[FrameID].FrameName,TargetBoneName)=0 then
       begin
         result:=SearchFrames[FrameID];
         exit;
       end;

     For FrameID:=0 to High(SearchFrames) do
      if High(SearchFrames[FrameID].ChildFrames) > -1 then
       begin
        if SearchFrames[FrameID].FrameName <> '' then
           ParentFrameName:=SearchFrames[FrameID].FrameName;
        result:=LocalFindFrame(SearchFrames[FrameID].ChildFrames);
        if result <> Nil then
           exit;
       end;
end;

begin
     ParentFrameName:='';
     result:=LocalFindFrame(Frames);
end;

function _copySign(v1,v2:Single):Single;
begin
     if v2 < 0 then
        result:=-abs(v1)
     else
        result:=abs(v1);
end;

function MatrixToQuaternion(M:t4x4Matrix):TQuat;
begin
   result.w := sqrt( max( 0, 1 + M[1][1] + m[2][2] + m[3][3] ) ) / 2;
   result.x := sqrt( max( 0, 1 + M[1][1] - m[2][2] - m[3][3] ) ) / 2;
   result.y := sqrt( max( 0, 1 - M[1][1] + m[2][2] - m[3][3] ) ) / 2;
   result.z := sqrt( max( 0, 1 - M[1][1] - m[2][2] + m[3][3] ) ) / 2;

   result.x := _copysign( result.x, m[3][2] - m[2][3] );
   result.y := _copysign( result.y, m[1][3] - m[3][1] );
   result.z := _copysign( result.z, m[2][1] - m[1][2] );
end;

procedure tDirectXFile.ConvertxBone(TargetBone:TBone;SourceFrame:d3d_frame;SourceSkinWeights:d3d_SkinWeights);

var
   FrameMatrix:t4x4Matrix;
   r,c:integer;
   LocalAdjustor:Single;
begin
     for r:=1 to 4 do
      for c:=1 to 4 do
       begin
        FrameMatrix[r,c]:=SourceFrame.local_Transform.frameMatrix.Matrix[r,c];
        TargetBone.AdjustedMatrix[r,c]:=SourceSkinWeights.matrixOffset.Matrix[r,c];
       end;

     LocalAdjustor:=sqrt(sqr(TargetBone.AdjustedMatrix[1,1])+sqr(TargetBone.AdjustedMatrix[1,2])+sqr(TargetBone.AdjustedMatrix[1,3]));
     for r:=1 to 3 do
      for c:=1 to 4 do
        TargetBone.AdjustedMatrix[r,c]:=TargetBone.AdjustedMatrix[r,c]/LocalAdjustor;

     TargetBone.rotation:=MatrixToQuaternion(FrameMatrix);
     TargetBone.local_rotation:=MatrixToQuaternion(TargetBone.AdjustedMatrix);

     TargetBone.translation.x:=FrameMatrix[4][1];
     TargetBone.translation.y:=FrameMatrix[4][2];
     TargetBone.translation.z:=FrameMatrix[4][3];

     TargetBone.local_translation.x:=TargetBone.AdjustedMatrix[4][1];
     TargetBone.local_translation.y:=TargetBone.AdjustedMatrix[4][2];
     TargetBone.local_translation.z:=TargetBone.AdjustedMatrix[4][3];

     TargetBone.Bone_Name:=SourceFrame.FrameName;
     TargetBone.SourceMesh:=SourceSkinWeights.OwningMesh;
end;

procedure tDirectXFile.InitializeBoneList;

procedure AddBonesToTree(SearchFrames:Array of d3d_Frame;ParentNode:ttreeNode = nil);
var
   FrameID:Integer;
begin
     For FrameID:=0 to High(SearchFrames) do
      begin
         if SearchFrames[FrameID].FrameName <> '' then
            ParentNode:=ConverterGUI.TreeView1.Items.AddChild(ParentNode,SearchFrames[FrameID].FrameName);
         if High(SearchFrames[FrameID].ChildFrames) > -1 then
            AddBonesToTree(SearchFrames[FrameID].ChildFrames,ParentNode);
      end;
end;

procedure BuildBoneList(SearchFrames:Array of d3d_Frame);
var
   FrameID:Integer;
begin
     For FrameID:=0 to High(SearchFrames) do
      begin
         if SearchFrames[FrameID].FrameName <> '' then
          begin
            //otherwise it's really a name for the mesh
            if not ((High(SearchFrames[FrameID].childFrames) = 0) and
               (High(SearchFrames[FrameID].ChildFrames[0].childMeshes) = 0)) then
                  TheBoneList.Add(SearchFrames[FrameID].FrameName);
          end;
         if High(SearchFrames[FrameID].ChildFrames) > -1 then
            BuildBoneList(SearchFrames[FrameID].ChildFrames);
      end;
end;

var
   FrameID:integer;
   SkinWeights:d3d_skinWeights;
begin
  TheBoneList.Clear;
  for FrameID:=0 to High(Frames) do
    if (Frames[FrameID].FrameName <> '') and (FindSkinWeights(Frames[FrameID].FrameName) <> nil) then
     begin
       AddBonesToTree([Frames[FrameID]]);
       BuildBoneList([Frames[FrameID]]);
       ConverterGUI.TreeView1.Items[0].Expand(True);
       break;
     end;
end;

function tDirectXFile.WriteSkeleton:Boolean;

function CreateMoboBone:TBone;
var
  RootBone              :TBone;
  RootFrame             :d3d_frame;
  RootSkinWeight        :d3d_skinWeights;
  r,c                   :integer;
begin
  RootFrame:=d3d_frame.Create;
  RootFrame.local_Transform:=d3d_FrameTransformMatrix.Create;
  with RootFrame.Local_Transform.frameMatrix do
   for r:=1 to 4 do
    for c:=1 to 4 do
      Matrix[r,c]:=Ord(r=c);

  RootSkinWeight:=d3d_skinWeights.Create;
  with RootSkinWeight.matrixOffset do
    for r:=1 to 4 do
     for c:=1 to 4 do
       Matrix[r,c]:=Ord(r=c);

  Result:=TBone.Create;
  ConvertxBone(Result,RootFrame,RootSkinWeight);

  RootFrame.Free;
  RootSkinWeight.Free;

  Result.bone_name:='Mobo';
  Result.parent_bone_name:='';
  Result.parent_bone_id:=-1;
end;

function FindValidParent(FrameName:String):String;
var
   TargetFrame:d3d_Frame;
   DummyParent:String;
begin
   TargetFrame:=FindFrame(FrameName,DummyParent);
   if assigned(TargetFrame) then
      TargetFrame:=TargetFrame.OwningFrame;
   while assigned(targetFrame) and ((TargetFrame.FrameName = '') or (TheBoneList.IndexOf(TargetFrame.FrameName)=-1)) do
   begin
     repeat
       TargetFrame:=TargetFrame.OwningFrame;
     until not assigned(TargetFrame) or (TargetFrame.FrameName <> '');
   end;
   if assigned(TargetFrame) then
      result:=TargetFrame.FrameName
   else
      result:='';
end;

function IsParent(Index:Integer):Boolean;
var
   CheckIndex:INteger;
begin
   result:=False;
   for CheckIndex:=0 to Pred(TheBoneList.Count) do
    if Index <> CheckIndex then
      with TBone( TheBoneList.Objects[Checkindex]) do
       begin
         result:=Parent_Bone_Name=TheBoneList[Index];
         if result then
            exit;
       end;
end;

var
  CSFStreamOut          :TFileStream;
  number_of_bones       :integer;
  BoneID                :Integer;
  FrameID               :Integer;
  CorrespondingWeight   :d3d_skinWeights;
  MeshID,SkinID         :Integer;
  CorrespondingFrame    :d3d_frame;
  NewBone               :TBone;
  TargetBone            :integer;
  LocalLength           :Integer;
  ParentFrameName       :String;
  ChildID               :Integer;
  ParentlessBones       :Integer;
  TargetID              :Integer;
  IdentityWeight        :d3d_skinWeights;
  r,c                   :integer;
begin
 try
  InitializeBoneList;

  IdentityWeight:=d3d_skinWeights.Create;
  with IdentityWeight.matrixOffset do
   for r:=1 to 4 do
     for c:=1 to 4 do
     matrix[r,c]:=Ord(r=c);

  For BoneID:=0 to Pred(TheBoneList.Count) do
   begin
      CorrespondingWeight:=FindSkinWeights(TheBoneList[BoneID]);
      if not assigned(CorrespondingWeight) then
       begin
         CorrespondingFrame:=FindFrame(TheBoneList[BoneID],ParentFrameName);
         if not assigned(CorrespondingFrame) then
            raise Exception.Create(PChar('Cannot Find Bone: '+TheBoneList[BoneID]));
         NewBone:=TBone.Create;
         CorrespondingFrame.FrameName:='!'+CorrespondingFrame.FrameName;
         ConvertxBone(NewBone,CorrespondingFrame,IdentityWeight);
         NewBone.parent_bone_name:=ParentFrameName;
         TheBoneList.Objects[BoneID]:=NewBone;
         TheBoneList[BoneID]:='!'+TheBoneList[BoneID];
        end
      else
       begin
         CorrespondingFrame:=FindFrame(TheBoneList[BoneID],ParentFrameName);
         if not assigned(CorrespondingFrame) then
            raise Exception.Create(PChar('Cannot Find Bone: '+TheBoneList[BoneID]));
         NewBone:=TBone.Create;
         ConvertxBone(NewBone,CorrespondingFrame,CorrespondingWeight);
         NewBone.parent_bone_name:=ParentFrameName;
         TheBoneList.Objects[BoneID]:=NewBone;
         CorrespondingWeight.Bone_Name:='_'+CorrespondingWeight.Bone_Name;
       end;
   end;
  IdentityWeight.Free;
  For BoneID:=Pred(TheBoneList.Count) downto 0 do
   if (Copy(TheBoneList[BoneID],1,1)='!') and not IsParent(BoneID) then
    begin
      TheBoneList.Objects[BoneID].Free;
      TheBoneList.Delete(BoneID);
    end;

  For BoneID:=Pred(SkinWeightCount) downto 0 do
   with GetSkinWeight(BoneID) do
   if Copy(Bone_Name,1,1)='_' then
      Delete(Bone_Name,1,1);

  if TheBoneList.Count = 0 then
   begin
     if assigned(CSFStreamOut) then
        FreeAndNil(CSFStreamOut);
     DeleteFile(ExtractFilePath(ConverterGUI.OpenFileDialog.FileName)+'skeleton.csf');
     MessageBox(GetActiveWindow,'There is no defined skeleton for this file!','Unable to Convert!',MB_ICONERROR);
     exit;
   end;

  ParentlessBones:=0;
  for BoneID:=0 to Pred(TheBoneList.Count) do
   begin
      TBone(TheBoneList.Objects[BoneID]).parent_bone_name:=FindValidParent(TBone(TheBoneList.Objects[BoneID]).bone_name);
      if TBone(TheBoneList.Objects[BoneID]).parent_bone_name = '' then
       begin
         TargetBone:=-1;
         Inc(ParentlessBones);
       end
      else
       begin
         TargetBone:=TheBoneList.IndexOf(TBone(TheBoneList.Objects[BoneID]).parent_bone_name);
         if TargetBone = -1 then
            raise Exception.Create(PChar('Cannot Find Parent Bone: '+TBone(TheBoneList.Objects[BoneID]).parent_bone_name));
       end;
     TBone(TheBoneList.Objects[BoneID]).parent_bone_id:=TargetBone;
     if TargetBone > -1 then
      if TheBoneList.Objects[TargetBone] = nil then
         raise Exception.Create('Parent Bone has no Object!  ParentBone ID: '+INtToSTr(TargetBone))
      else with TBone(TheBoneList.Objects[TargetBone]) do
        begin
          SetLength(Child_Bones,High(Child_Bones)+2);
          Child_Bones[High(Child_Bones)]:=BoneID;
        end;
   end;

  if (ParentlessBones <>  1) then
   begin
     TheBoneList.InsertObject(0,'Mobo',CreateMoboBone);
     for BoneID:=1 to Pred(TheBoneList.Count) do
       with TBone(TheBoneList.Objects[BoneID]) do
        begin
          Inc(Parent_Bone_ID);
          Inc(bone_id);
          if High(Child_Bones) > -1 then
           for TargetBone:=0 to High(Child_Bones) do
             Inc(Child_Bones[TargetBone]);
        end;
   end;

  CSFStreamOut:=TFileStream.Create(ExtractFilePath(ConverterGUI.OpenFileDialog.FileName)+'skeleton.csf',fmCreate);
  CSFStreamOut.Write('CSF'#0,4);
  LocalLength:=1000;
  CSFStreamOut.Write(LocalLength,4);
  LocalLength:=TheBoneList.Count;
  CSFStreamOut.Write(LocalLength,4);

  For BoneID:=0 to Pred(TheBoneList.Count) do
   with TBone(TheBoneList.Objects[BoneID]) do
   begin
      Bone_Name:=Bone_Name+#0;
      LocalLength:=Length(Bone_Name);
      CSFStreamOut.Write(LocalLength,4);
      CSFStreamOut.Write(bone_name[1],LocalLength);

      CSFStreamOut.Write(translation.x,4);
      CSFStreamOut.Write(translation.y,4);
      CSFStreamOut.Write(translation.z,4);

      CSFStreamOut.Write(rotation.x,4);
      CSFStreamOut.Write(rotation.y,4);
      CSFStreamOut.Write(rotation.z,4);
      CSFStreamOut.Write(rotation.w,4);

      CSFStreamOut.Write(local_translation.x,4);
      CSFStreamOut.Write(local_translation.y,4);
      CSFStreamOut.Write(local_translation.z,4);

      CSFStreamOut.Write(local_rotation.x,4);
      CSFStreamOut.Write(local_rotation.y,4);
      CSFStreamOut.Write(local_rotation.z,4);
      CSFStreamOut.Write(local_rotation.w,4);

      CSFStreamOut.Write(parent_bone_id,4);
      LocalLength:=number_of_children;
      CSFStreamOut.Write(LocalLength,4);
      if number_of_Children > 0 then
       for ChildID:=0 to Pred(Number_of_Children) do
         CSFStreamOut.Write(child_bones[ChildID],4);
   end;
  CSFStreamOut.Free;
  result:=True;
 except
  result:=False;
  raise;
 end;
end;

function tDirectXFile.GetTextureForMaterial(TargetMaterialName:String):String;
var
   MatID:Integer;
begin
  result:='';
  if High(Materials) > -1 then
   for MatID:=0 to High(Materials) do
    with Materials[MatID] do
    begin
      if TargetMaterialName <> MaterialName+'.xrf' then
         continue;
      if Assigned(TextureFileName) then
         result:=TextureFileName.filename;
      break;
    end;
end;

function tDirectXFile.WriteMaterial(TargetMaterialName:String):Boolean;

procedure WriteXRF(TextureFileName:d3d_TextureFilename;TargetFileName:String;FaceColor:d3d_ColorRGBA;
        EmissiveColor,SpecularColor:d3d_ColorRGB;UseZeroMap:Boolean);

function ConvertTo255Value(inValue:Single):String;
var
   TempResult:Integer;
begin
     TempResult:=255-Round(InValue*255);
     if TempResult > 255 then
        TempResult:=255
     else if TempResult < 0 then
        TempResult:=0;
     result:=IntToStr(TempResult);
end;

var
   MatToWrite:TStringList;
begin
    MatToWrite:=TStringList.Create;
    MatToWrite.Clear;
    MatToWrite.Add('<HEADER MAGIC="XRF" VERSION="1000"/>');

    if (assigned(TextureFileName) and (TextureFilename.filename <> '')) or UseZeroMap then
       MatToWrite.Add('<MATERIAL NUMMAPS="1">')
    else
       MatToWrite.Add('<MATERIAL NUMMAPS="0">');
    MatToWrite.Add('<AMBIENT>'+ ConvertTo255Value(faceColor.red)+
                ' '+ConvertTo255Value(faceColor.green)+
                ' '+ConvertTo255Value(faceColor.blue)+
                ' '+ConvertTo255Value(faceColor.alpha)+
                '</AMBIENT>');
    MatToWrite.Add('<DIFFUSE>'+ ConvertTo255Value(emissiveColor.red)+
                ' '+ConvertTo255Value(emissiveColor.green)+
                ' '+ConvertTo255Value(emissiveColor.blue)+
                ' 0</DIFFUSE>');
    MatToWrite.Add('<SPECULAR>'+ ConvertTo255Value(specularColor.red)+
                ' '+ConvertTo255Value(specularColor.green)+
                ' '+ConvertTo255Value(specularColor.blue)+
                ' 0</SPECULAR>');
    MatToWrite.Add('<SHININESS>0</SHININESS>');
    if not assigned(TextureFileName) or (TextureFilename.filename = '') then
     begin
       if UseZeroMap then
          MatToWrite.Add('<MAP>0</MAP>')
     end
    else
       MatToWrite.Add('<MAP>'+TextureFilename.filename+'</MAP>');
    MatToWrite.Add('</MATERIAL>');
    MatToWrite.savetofile(TargetFileName);
    MatToWRite.Free;
end;

var
   FColor:d3d_ColorRGBA;
   EColor,SColor:d3d_ColorRGB;
   MatID:Integer;
begin
  result:=False;
  try
    if High(Materials) > -1 then
     for MatID:=0 to High(Materials) do
      with Materials[MatID] do
      begin
        if TargetMaterialName <> MaterialName+'.xrf' then
           continue;
        if ConverterGUI.WriteCFG.Checked then
           WriteXRF(TextureFileName,ChangeFileExt(MaterialName,'.xrf'),FaceColor,EmissiveColor,SpecularColor,False);
        if ConverterGUI.CreateFF3DCZP.Checked then
           WriteXRF(TextureFileName,ChangeFileExt(MaterialName,'_ff3d.xrf'),FaceColor,EmissiveColor,SpecularColor,True);
        result:=true;
        break;
      end;
    if not result then // make everything white
     begin
        FColor.red:=0;
        FColor.green:=0;
        FColor.blue:=0;
        FColor.alpha:=0;

        EColor.red:=0;
        EColor.green:=0;
        EColor.blue:=0;

        SColor.red:=0;
        SColor.green:=0;
        SColor.blue:=0;

        if ConverterGUI.WriteCFG.Checked then
           WriteXRF(Nil,ChangeFileExt(TargetMaterialName,'.xrf'),FColor,EColor,SColor,false);
        if ConverterGUI.CreateFF3DCZP.Checked then
           WriteXRF(Nil,ChangeFileExt(TargetMaterialName,'_ff3d.xrf'),FColor,EColor,SColor,true);
     end;
  except
    result:=False;
  end;
end;

function tDirectXFile.ReadHeader:Boolean;
begin
      SourceStream.Position:=0;

      SetLength(MagicNumber,4);
      SetLength(MajorVerNumber,2);
      SetLength(MinorVerNumber,2);
      SetLength(FormatType,4);

      SourceStream.Read(MagicNumber[1],4);
      SourceStream.Read(MajorVerNumber[1],2);
      SourceStream.Read(MinorVerNumber[1],2);
      SourceStream.Read(FormatType[1],4);
      SourceStream.Read(Filesize,4);
      result:= (MagicNumber='xof ') and (FormatType='txt ');
end;

{function tDirectXFile.FindTemplate(TemplateName:String):xTemplate;
begin
     result:=Templates;
     while assigned(Result) and (CompareText(Result.TemplateName,TemplateName) <> 0) do
      result:=result.NextTemplate;
end;}

function tDirectXFile.ReadMembers:Boolean;
var
   LiveMaterial:d3d_material;
   LiveFrame:d3d_Frame;
   TemplateType,TemplateName:String;
begin
     result:=assigned(TemplateEnd);
     if not result then exit;

     VirtualStreamPos:=TemplateEnd-SourceStream.memory;

     while GetNextTemplate(TemplateType,TemplateName,SourceStream) do
      begin
        if CompareText(TemplateType,'Material') = 0 then
         begin
              LiveMaterial:=CreateNewMaterials(TemplateName);
              LiveMaterial.Read(SourceStream);
         end
        else if CompareText(TemplateType,'Frame') = 0 then
         begin
              LiveFrame:=CreateNewFrames(TemplateName);
              LiveFrame.Read(SourceStream);
         end
        else
          SkipTemplate(ConverterGUI.SkippedMembers.Items,SourceStream,'.x File: '+TemplateType);
      end;
end;

function tDirectXFile.CreateNewFrames(FrameName:string):d3d_Frame;
begin
     result:=d3d_Frame.Create;
     result.FrameName:=FrameName;
     result.OwningFile:=Self;
     result.OwningFrame:=Nil;
     SetLength(Frames,High(Frames)+2);
     Frames[High(Frames)]:=result;
end;

function tDirectXFile.CreateNewMaterials(MaterialName:string):d3d_Material;
begin
     SetLength(Materials,High(Materials)+2);

     result:=d3d_Material.Create;
     If Trim(MaterialName)='' then
        MaterialName:='Material_'+IntToStr(Succ(High(Materials)));
     result.MaterialName:=MaterialName;
     Materials[High(Materials)]:=result;
end;

function tDirectXFile.ReadTemplates:Boolean;
var
  TheMembers:TStringList;

function GetFirstWord(i:Integer):String;
begin
     Result:=TheMembers[i];
     //strip out the first word
     if Pos(' ',Result) = 0 then
        TheMembers[i]:=''
     else
      begin
        Delete(Result,Pos(' ',Result),Length(Result));
        //remove it from the rest of the line
        TheMembers[i]:=Trim(Copy(TheMembers[i],Succ(Length(Result)),Length(TheMembers[i])));
      end;
     Result:=SmartTrim(Result);
end;

var
  ActiveTemplate:PChar;
  EndPos:PChar;
  LastLiveTemplate:xTemplate;
  LiveTemplate:xTemplate;
  MemberString:String;
  I:integer;
  ThePropType:String;
  IsArray:Boolean;
begin
     result:=false;
     ActiveTemplate:=SourceStream.Memory;
     ActiveTemplate:=AnsiStrPos(ActiveTemplate,'template ');
     if ActiveTemplate = nil then
        exit;
     LiveTemplate:=Nil;
     TheMembers:=TStringList.Create;
     repeat
       //get past the word ActiveTemplate;
       Inc(ActiveTemplate,9);
       EndPos:=StrPos(ActiveTemplate,' {');
       if not assigned(EndPos) then
          raise exception.create('Invalid ActiveTemplate format!');
       LastLiveTemplate:=LiveTemplate;
       LiveTemplate:=xTemplate.Create;
       if not assigned(LastLiveTemplate) then
          Templates:=LiveTemplate
       else
          LastLiveTemplate.NextTemplate:=LiveTemplate;
       //end the C String
       EndPos[0]:=#0;
       LiveTemplate.TemplateName:=ActiveTemplate;
       //restore the space!
       EndPos[0]:=' ';
       //find end of GUID
       ActiveTemplate:=StrFScan(ActiveTemplate,'>');
       if not assigned(ActiveTemplate) then
          raise exception.create('Invalid ActiveTemplate format -- NO GUID END');
       //get past the '>'
       Inc(ActiveTemplate);
       EndPos:=StrFScan(ActiveTemplate,'}');
       if not assigned(EndPos) then
          raise exception.create('Invalid ActiveTemplate format -- NO MEMBER END');

       EndPos[0]:=#0;
       //set the member string to all the members!
       MemberString:=ActiveTemplate;
       //Stripout all hard returns / line feeds
       MemberString:=StringReplace(MemberString,#13,'',[rfReplaceAll]);
       MemberString:=StringReplace(MemberString,#10,'',[rfReplaceAll]);
       //replace all semi colons with hard returns;
       MemberString:=StringReplace(MemberString,';',#13#10,[rfReplaceAll]);

       TheMembers.Clear;
       TheMembers.Text:=MemberString;
       for i:=Pred(TheMembers.Count) downto 0 do
        begin
          TheMembers[i]:=SmartTrim(TheMembers[i]);
          if Themembers[i] = '' then
             TheMembers.Delete(i);
        end;
       with LiveTemplate do
         for i:=0 to Pred(TheMembers.Count) do
          begin
            MemberString:=TheMembers[i];
            SetLength(PropNames, High(PropNames)+2);
            SetLength(PropTypes, High(PropTypes)+2);
            SetLength(ArrayMembers, High(ArrayMembers)+2);
            ThePropType:=GetFirstWord(i);
            IsArray:=CompareText(ThePropType,'array')=0;
            if IsArray then
               ThePropType:=GetFirstWord(i);
            if CompareText(ThePropType,'WORD')=0 then
               PropTypes[High(PropTypes)]:=propWord
            else if CompareText(ThePropType,'DWORD') = 0 then
               PropTypes[High(PropTypes)]:=propDWORD
            else if CompareText(ThePropType,'FLOAT') = 0 then
              PropTypes[High(PropTypes)]:=propFLOAT
            else if CompareText(ThePropType,'DOUBLE') = 0 then
              PropTypes[High(PropTypes)]:=propDOUBLE
            else if CompareText(ThePropType,'CHAR') = 0 then
              PropTypes[High(PropTypes)]:=propCHAR
            else if CompareText(ThePropType,'UCHAR') = 0 then
              PropTypes[High(PropTypes)]:=propUCHAR
            else if CompareText(ThePropType,'BYTE') = 0 then
              PropTypes[High(PropTypes)]:=propByte
            else if CompareText(ThePropType,'STRING') = 0 then
             PropTypes[High(PropTypes)]:=propSTRING
            else if CompareText(ThePropType,'CSTRING') = 0 then
             PropTypes[High(PropTypes)]:=propCSTRING
            else if CompareText(ThePropType,'UNICODE') = 0 then
             PropTypes[High(PropTypes)]:=propUNICODE
            else if CompareText(ThePropType,'Matrix4x4')= 0 then
             PropTypes[High(PropTypes)]:=propMatrix4x4
            else if CompareText(RemoveSpaces(ThePropType),'[...]')= 0 then
             begin
               IsOpen:=True;
               continue;
             end
            else
             begin
{               MessageBox(GetActiveWindow(),PChar(MemberString+#13#13+'Unable to identify PropertyType: '+ThePropType),
                 'Attention',MB_ICONERROR);}
               continue;
             end;

            PropNames[High(PropNames)]:=GetFirstWord(i);
            if IsArray and (TheMembers[i] <> '') then
               ArrayMembers[High(ArrayMembers)]:=GetFirstWord(i);
          end;
       //restore the space!
       EndPos[0]:='}';
       ActiveTemplate:=AnsiStrPos(EndPos,'template ');
     until not assigned(ActiveTemplate);
     TheMembers.Free;
     result:=Assigned(Templates);
     TemplateEnd:=EndPos;
     Inc(TempLateEnd);
end;

constructor tDirectXFile.Create(SourceFile:TFileName);
var
   FStream:TFileStream;
begin
     inherited create;
     FStream:=TFIleStream.Create(SourceFile,fmOpenRead);
     SourceStream:=TMemoryStream.Create;
     TheBoneList:=TStringList.Create;
     FStream.Position:=0;
     SourceStream.CopyFrom(FStream,FStream.Size);
     SourceStream.Position:=0;
     FStream.Free;
     Templates:=Nil;
     ConverterGUI.Label1.Caption:='Reading Header';
     Application.ProcessMessages;
     if not ReadHeader then
      begin
         if FormatType <> 'txt 'then
            MessageBox(GetActiveWIndow,'Can only read .X files in TEXT format!','Attention',MB_ICONERROR)
         else
            MessageBox(GetActiveWIndow,'Unidentified File Format!','Attention',MB_ICONERROR);
            exit;
      end;
     ConverterGUI.Label1.Caption:='Reading Templates';
     Application.ProcessMessages;
     if not ReadTemplates then
      begin
            MessageBox(GetActiveWIndow,'Error Reading Templates!','Attention',MB_ICONERROR);
            exit;
      end;
     ConverterGUI.Label1.Caption:='Reading Members';
     Application.ProcessMessages;
     if not ReadMembers then
      begin
            MessageBox(GetActiveWIndow,'Error Reading Members!','Attention',MB_ICONERROR);
            exit;
      end;
     ConverterGUI.Label1.Caption:='Read All';
end;

destructor tDirectXFile.Destroy;
var
   i:Integer;
begin
      for i:=0 to high(materials) do
        materials[i].Free;
      for i:=0 to high(Frames) do
        Frames[i].Free;
      if assigned(TheBoneList) then
       begin
        for i:=0 to Pred(TheBoneList.Count) do
          TheBoneList.Objects[i].Free;
        TheBoneList.Free;
       end;
      SourceStream.Free;
      inherited destroy;
end;

end.
