//****************************************************************************//
// cmfData.pas                                                                //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
unit cmfData;

interface

const
  x = 0; 
  y = 1;
  z = 2;
  u = 3;
  v = 4;

type

   TVertexPos = Array[x..z] of single;

   tVertexData = class;

   TDifferenceData = packed record
     VertexID      : Integer;
     VertexOffsets : TVertexPos;
     NormalOffsets : TVertexPos;
   end;


  TinfluenceData = class
    BoneID          : Longint;
    weight          : Single;
    NextInfluence   : TInfluenceData;
    PrevInfluence   : TInfluenceData;
    Constructor Create(PriorInfluence:TInfluenceData);
    Destructor Destroy; override;
  end;

  TSpringData = class
    VertexID         : Array[1..2] of Longint;
    SpringCoefficient: Single;
    IdleLength       : Single;
    NextSpring   : TSpringData;
    PrevSpring   : TSpringData;

    Constructor Create(PriorSpring:TSpringData);
    Destructor Destroy; override;
  end;

  TFaceData = class
    VertexID   : Array [1..3] of Longint;
    NextFace   : TFaceData;
    PrevFace   : TFaceData;

    BufferedVData:Array [1..3] of TVertexPos;
    function GetVertexPos(FromVertices:TVertexData;Index:Integer):TVertexPos;
    function GetVertex(FromVertices:TVertexData;Index:Integer):TVertexData;

    Constructor Create(PriorFace:TFaceData);
    Destructor Destroy; override;
  end;

   TMapData = class
    Coordinates   : array[u..v] of Single;
    NexTMap       : TMapData;
    PrevMap       : TMapData;

    Constructor Create(PriorMap:TMapData);
    Destructor Destroy; override;
  end;

   tVertexData = class
    Position          : TVertexPos;
    Normal            : TVertexPos;
    CollapseID        : Longint;
    FaceCollapseCount : Longint;

    NumberOfInfluences: Integer;
    Weight            : Single;

    NextVertex        : TVertexData;
    PrevVertex        : TVertexData;

    CoordinateMaps    : TMapData;
    Influences        : TInfluenceData;

    Constructor Create(PriorVertex:TVertexData);
    Destructor Destroy; override;
   end;

  TSubMeshData = class
    MaterialThreadId  : longInt;
    NumberOfvertices  : longInt;
    NumberOffaces     : longInt;
    NumberOflodSteps  : longInt;
    NumberOfsprings   : longInt;
    NumberOfmaps      : longInt;
    NextSubmesh       : TSubMeshData;
    PrevSubmesh       : TSubMeshData;

    VertexData        : tVertexData;
    Springs           : TSpringData;
    Faces             : TFaceData;

    Constructor Create(PriorSubmesh:TSubMeshData);
    Destructor Destroy; override;
  end;

   TCal3DMesh = class
    MagicToken  : Array[0..3] of Char;
    FileVersion : LongInt;
    NumberOfSubmeshes : Longint;
    SubMeshes   : TSubMeshData;
    function NumOfVertices:Integer;
    procedure WriteCMF(FileName:String);
    procedure ReadCMF(FileName:String);
   public
    constructor Create;
    Destructor Destroy; override;
  end;

   tVertexDifference = class
      VertexID       : Integer;
      Differences    : array[x..z] of Single;
      NextDifferentVertex : tVertexDifference;
      PrevDifferentVertex : tVertexDifference;
      Constructor Create(PriorVertex:tVertexDifference);
      Destructor Destroy; override;
    end;

   TSubMeshDifferenceData = class
      SubmeshID   : Integer;
      VertexData  : tVertexDifference;
      NormalData  : tVertexDifference;
      NextSubmesh : TSubMeshDifferenceData;
      PrevSubmesh : TSubMeshDifferenceData;
      Constructor Create(PriorSubmesh:TSubMeshDifferenceData);
      Destructor Destroy; override;
    end;

   TDifferenceMap = class
      MapName : Array[0..59] of Char;
      SubMeshes   : TSubMeshDifferenceData;
      constructor Create(TargetMapName:String;CompareMesh,BaseMesh:TCal3DMesh);
      procedure WriteDifferenceMap(FileName:String);
      Destructor Destroy; override;
  end;


implementation

uses forms, Windows, Sysutils, classes;

Constructor tVertexDifference.Create(PriorVertex:tVertexDifference);
begin
   inherited Create;
   FillChar(Differences,sizeof(Differences),0);
   PrevDifferentVertex:=PriorVertex;
end;

Destructor tVertexDifference.Destroy;
begin
     if Assigned(NextDifferentVertex) then
        NextDifferentVertex.Free;
     Inherited destroy;
end;


Constructor TSubMeshDifferenceData.Create(PriorSubmesh:TSubMeshDifferenceData);
begin
      inherited create;
      PrevSubmesh := PriorSubmesh;
end;

Destructor TSubMeshDifferenceData.Destroy;
begin
     if Assigned(NextSubMesh) then
        NextSubMesh.Free;
     if Assigned(VertexData) then
        VertexData.Free;
     if Assigned(NormalData) then
        NormalData.Free;
     Inherited destroy;
end;

constructor TDifferenceMap.Create(TargetMapName:String;CompareMesh,BaseMesh:TCal3DMesh);
var
   BaseSubmesh:TSubmeshData;
   BaseVertex:TVertexData;
   CompareSubmesh:TSubmeshData;
   CompareVertex:TVertexData;

   MapSubmesh:TSubMeshDifferenceData;
   MapVertex:tVertexDifference;
   MapNormal:tVertexDifference;
   SubMeshID:Integer;
   VertexID:Integer;
   MapCount:Integer;
   NonMapped:Integer;
   PositionID:Integer;
{   DuplicateCount:Integer;}
begin
     inherited create;
     FillChar(MapName,Sizeof(MapName),0);
     if TargetMapName <> '' then
        Move(TargetMapName[1],MapName[0],Length(TargetMapName));

     BaseSubmesh:=BaseMesh.SubMeshes;
     CompareSubmesh:=CompareMesh.SubMeshes;
     SubMeshID:=1;
     MapSubmesh:=Nil;
     MapVertex:=Nil;
     MapNormal:=Nil;
     MapCount:=0;
     NonMapped:=0;

     while Assigned(BaseSubmesh) and Assigned(CompareSubmesh) do
      begin
        VertexID:=1;
        BaseVertex:=BaseSubmesh.VertexData;
        CompareVertex:=CompareSubmesh.VertexData;
        while Assigned(BaseVertex) and Assigned(CompareVertex) do
         begin
            Inc(VertexID);
            for PositionID:=x to z do
             if (Abs(CompareVertex.Position[PositionID] - BaseVertex.Position[PositionID]) > 0.004) or
                (Abs(CompareVertex.Normal[PositionID] - BaseVertex.Normal[PositionID]) > 0.004) then
                begin
                  Inc(MapCount);
                  if not Assigned(SubMeshes) then
                   begin
                     SubMeshes:=TSubMeshDifferenceData.Create(Nil);
                     MapSubmesh:=SubMeshes;
                     MapSubmesh.SubMeshID:=SubMeshID;
                   end
                  else if MapSubmesh.SubmeshID <> SubMeshID then
                   begin
                     MapSubmesh.NextSubmesh:=TSubMeshDifferenceData.Create(MapSubmesh);
                     MapSubmesh:=MapSubmesh.NextSubmesh;
                     MapSubmesh.SubMeshID:=SubMeshID;
                   end;
                  if (CompareVertex.Position[PositionID] <> BaseVertex.Position[PositionID]) then
                   begin
                      if not Assigned(MapSubmesh.VertexData) then
                       begin
                        MapSubmesh.VertexData:=tVertexDifference.Create(nil);
                        MapVertex:=MapSubmesh.VertexData;
                       end
                      else
                       begin
                        MapVertex.NextDifferentVertex:=tVertexDifference.Create(MapVertex);
                        MapVertex:=MapVertex.NextDifferentVertex;
                       end;
                      MapVertex.VertexID:=VertexID;
                      MapVertex.Differences[x]:=CompareVertex.Position[x]-BaseVertex.Position[x];
                      MapVertex.Differences[y]:=CompareVertex.Position[y]-BaseVertex.Position[y];
                      MapVertex.Differences[z]:=CompareVertex.Position[z]-BaseVertex.Position[z];
                   end;
                  if (CompareVertex.Normal[PositionID] <> BaseVertex.Normal[PositionID]) then
                   begin
                      if not Assigned(MapSubmesh.NormalData) then
                       begin
                        MapSubmesh.NormalData:=tVertexDifference.Create(nil);
                        MapNormal:=MapSubmesh.VertexData;
                       end
                      else
                       begin
                        MapNormal.NextDifferentVertex:=tVertexDifference.Create(MapNormal);
                        MapNormal:=MapNormal.NextDifferentVertex;
                       end;
                      MapNormal.VertexID:=VertexID;
                      MapNormal.Differences[x]:=CompareVertex.Normal[x]-BaseVertex.Normal[x];
                      MapNormal.Differences[y]:=CompareVertex.Normal[y]-BaseVertex.Normal[y];
                      MapNormal.Differences[z]:=CompareVertex.Normal[z]-BaseVertex.Normal[z];
                   end;
                  Break;
                end
              else
                Inc(NonMapped);
            baseVertex:=BaseVertex.NextVertex;
            CompareVertex:=CompareVertex.NextVertex;
         end;
      if assigned(BaseVertex) <> Assigned(CompareVertex) then
         Application.MessageBox('Houston we have a problem with mismatched vetices!','Attention',MB_ICONINFORMATION);
      Inc(SubMeshID);
      baseSubmesh:=BaseSubmesh.NextSubmesh;
      CompareSubmesh:=CompareSubmesh.NextSubmesh;
   end;
   if assigned(BaseSubmesh) <> Assigned(CompareSubmesh) then
      Application.MessageBox('Houston we have a problem with mismatched submeshes!','Attention',MB_ICONINFORMATION);
   Application.MessageBox(PChar('Mapped: '+IntToStr(MapCount)+'  Skipped: '+IntToStr(NonMapped)),'Attention',0);
end;

destructor TDifferenceMap.Destroy;
begin
     if Assigned(SubMeshes) then
        SubMeshes.Free;
     inherited Destroy;
end;


procedure TDifferenceMap.WriteDifferenceMap(FileName:String);
begin
//     TO DO!
end;

Constructor TinfluenceData.Create(PriorInfluence:TInfluenceData);
begin
     inherited create;
     PrevInfluence:=PriorInfluence;
     NextInfluence:=Nil;
end;

Destructor TinfluenceData.Destroy;
begin
     if Assigned(NextInfluence) then
        NextInfluence.Free;
     inherited Destroy;
end;

Constructor TSpringData.Create(PriorSpring:TSpringData);
begin
     inherited create;
     PrevSpring:=PriorSpring;
     NextSpring:=Nil;
end;

Destructor TSpringData.Destroy;
begin
     if Assigned(NextSpring) then
        NextSpring.Free;
     inherited Destroy;
end;

function TFaceData.GetVertex(FromVertices:TVertexData;Index:Integer):TVertexData;
var
   FindIndex:Integer;
begin
    FindIndex:=VertexID[Index];
    While (FindIndex > 0) and Assigned(FromVertices) do
     begin
        FromVertices:=FromVertices.NextVertex;
        Dec(FindIndex);
     end;
    if assigned(FromVertices) then
       result:=FromVertices
    else
      result:=Nil;
end;


function TFaceData.GetVertexPos(FromVertices:TVertexData;Index:Integer):TVertexPos;
var
   FindIndex:Integer;
begin
    if not ((BufferedVData[Index][x] = 0) and (BufferedVData[Index][y] = 0) and
            (BufferedVData[Index][y] = 0)) then
         result:=BufferedVData[Index]
    else
     begin
       FindIndex:=VertexID[Index];
       While (FindIndex > 0) and Assigned(FromVertices) do
        begin
           FromVertices:=FromVertices.NextVertex;
           Dec(FindIndex);
        end;
       if assigned(FromVertices) then
        begin
          result:=FromVertices.Position;
          BufferedVData[index]:=Result;
        end;
     end;
end;


Constructor TFaceData.Create(PriorFace:TFaceData);
begin
     inherited create;
     PrevFace:=PriorFace;
     NextFace:=Nil;
end;

Destructor TFaceData.Destroy;
begin
     if Assigned(NextFace) then
        NextFace.Free;
     inherited Destroy;
end;

Constructor TMapData.Create(PriorMap:TMapData);
begin
     inherited create;
     PrevMap:=PriorMap;
     NextMap:=Nil;
end;

Destructor TMapData.Destroy;
begin
     if Assigned(NextMap) then
        NextMap.Free;
     inherited Destroy;
end;

constructor tVertexData.Create(PriorVertex:TVertexData);
begin
     inherited create;
     PrevVertex:=PriorVertex;
     NextVertex:=Nil;
end;

Destructor TVertexData.Destroy;
begin
     if Assigned(NextVertex) then
        NextVertex.Free;
     if Assigned(CoordinateMaps) then
        CoordinateMaps.Free;
     if Assigned(Influences) then
        Influences.Free;
     inherited Destroy;
end;

Constructor TSubMeshData.Create(PriorSubmesh:TSubMeshData);
begin
     inherited create;
     PrevSubmesh:=PriorSubmesh;
     NextSubmesh:=Nil;
end;

Destructor TSubMeshData.Destroy;
begin
     if Assigned(NextSubmesh) then
        NextSubmesh.Free;

     if Assigned(VertexData) then
        VertexData.Free;
     if Assigned(Springs) then
        Springs.Free;
     if Assigned(Faces) then
        Faces.Free;
     inherited Destroy;
end;

constructor TCal3DMesh.Create;
begin
     inherited create;
     StrMove(@MagicToken[0],'CMF'#0,4);
     self.FileVersion:=1000;
end;


destructor TCal3DMesh.Destroy;
begin
     if Assigned(SubMeshes) then
        SubMeshes.Free;
     inherited destroy;
end;

function TCal3DMesh.NumOfVertices:Integer;
var
   CurrentSubmesh:TSubMeshData;
   CurrentVertex:tVertexData;
begin
   Result:=0;
   CurrentSubmesh:=SubMeshes;
   while Assigned(CurrentSubmesh) do
    begin
      CurrentVertex:=CurrentSubmesh.VertexData;
      while Assigned(CurrentVertex) do
       begin
         Inc(Result);
         CurrentVertex:=CurrentVertex.NextVertex;
       end;
      CurrentSubmesh:=CurrentSubmesh.NextSubmesh;
    end;
end;
function FloatSort(List: TStringList; Index1, Index2: Integer): Integer;
begin
  if (Index1=Index2) or (Index1=-1) or (Index2=-1) then
     result:=0
  else if TInfluenceData(List.Objects[Index1]).Weight > TInfluenceData(List.Objects[Index2]).Weight then
     result:=-1
  else
     result:=1;
end;

procedure TCal3DMesh.WriteCMF(FileName:String);

procedure SortInfluences(CurrentVertex:TVertexData);
var
   s:TStringList;
   CurrentInfluence:TinfluenceData;
   i:Integer;
begin
    CurrentInfluence:=CurrentVertex.Influences;
    s:=TStringList.Create;
    while assigned(CurrentInfluence) do
     begin
       s.addObject(FloatToStr(CurrentINfluence.weight),CurrentInfluence);
       CurrentInfluence:=CurrentInfluence.nextInfluence;
     end;
    s.CustomSort(FloatSort);
    for i:=0 to Pred(S.Count) do
     begin
     if i = 0 then
        TInfluenceData(s.Objects[i]).PrevInfluence:=Nil
     else
        TInfluenceData(s.Objects[i]).PrevInfluence:=TInfluenceData(s.Objects[Pred(i)]);
     if i = Pred(S.Count) then
        TInfluenceData(s.Objects[i]).NextInfluence:=Nil
     else
        TInfluenceData(s.Objects[i]).NextInfluence:=TInfluenceData(s.Objects[succ(i)]);
     end;
    CurrentVertex.Influences:=TInfluenceData(s.Objects[0]);
    s.Free;
end;

var
   cmfFile:TFileStream;
   CurrentSubmesh:TSubMeshData;
   CurrentVertex:tVertexData;
   CurrentMap:tMapData;
   CurrentInfluence:TinfluenceData;
   CurrentSpring:TSpringData;
   CurrentFace:TFaceData;
   CVertexID:Integer;
begin
   CurrentSubmesh:=SubMeshes;
   while assigned(CurrentSubmesh) do
    begin
      if not assigned(CurrentSubmesh.VertexData) then
         Dec(NumberOfSubMeshes);
      CurrentSubmesh:=CurrentSubmesh.NextSubmesh;
    end;
   CurrentSubmesh:=SubMeshes;
   cmfFile:=TFileStream.Create(FileName,fmCreate);
   cmfFile.Write('CMF'+#0,4);
   cmfFile.Write(FileVersion,4);
   cmfFile.Write(NumberOfSubmeshes,4);
   while Assigned(CurrentSubmesh) do
    begin
      if assigned(CurrentSubmesh.VertexData) then
       begin
         cmfFile.Write(CurrentSubmesh.MaterialThreadId,4);
         cmfFile.Write(CurrentSubmesh.NumberOfvertices,4);
         cmfFile.Write(CurrentSubmesh.NumberOffaces,4);
         cmfFile.Write(CurrentSubmesh.NumberOflodSteps,4);
         cmfFile.Write(CurrentSubmesh.NumberOfsprings,4);
         cmfFile.Write(CurrentSubmesh.NumberOfmaps,4);
         CurrentVertex:=CurrentSubmesh.VertexData;
         CVertexID:=0;
         while Assigned(CurrentVertex) do
          begin
            Inc(CVertexID);

            cmfFile.Write(CurrentVertex.Position[x],12);
            cmfFile.Write(CurrentVertex.Normal[x],12);
            cmfFile.Write(CurrentVertex.CollapseID,4);
            cmfFile.Write(CurrentVertex.FaceCollapseCount,4);
            CurrentMap:=CurrentVertex.CoordinateMaps;
            while Assigned(CurrentMap) do
             begin
               cmfFile.Write(CurrentMap.coordinates[u],8);
               CurrentMap:=CurrentMap.NextMap;
             end;
            cmfFile.Write(CurrentVertex.NumberOfInfluences,4);
            if CurrentVertex.NumberofInfluences > 1 then
               SortInfluences(CurrentVertex);
{            else if (CurrentVertex.Influences.weight < 0.01) or
               (CurrentVertex.Influences.BoneID = 0) then
               MessageBox(GetActiveWindow(),'Has NO Substantive influences!','Attention',0);}

            CurrentInfluence:=CurrentVertex.Influences;
            while assigned(CurrentInfluence) do
             begin
               cmfFile.Write(CurrentInfluence.BoneID,4);
               cmfFile.Write(CurrentInfluence.weight,4);
               CurrentInfluence:=CurrentInfluence.NextInfluence;
             end;
             if CurrentSubmesh.NumberOfsprings > 0 then
                cmfFile.Write(CurrentVertex.weight,4);
             CurrentVertex:=CurrentVertex.NextVertex;
          end;
         if CVertexID <> CurrentSubmesh.NumberofVertices then
            MEssageBox(GetActiveWindow,PChar('It is set to '+IntToStr(CurrentSubmesh.NumberofVertices)+#13+
                                             'It SHOULD BE '+IntToStr(CVertexID)),
                                             'Invalid value for NumberOfVertices!',MB_ICONERROR);
         CurrentSpring:=CurrentSubmesh.Springs;
         while Assigned(CurrentSpring) do
          begin
            cmfFile.Write(CurrentSpring.VertexID[1],8);
            cmfFile.Write(CurrentSpring.springcoefficient,4);
            cmfFile.Write(CurrentSpring.idleLength,4);
            CurrentSpring:=CurrentSpring.NextSpring;
          end;
          CurrentFace:=CurrentSubmesh.Faces;
          CVertexID:=0;
          while Assigned(CurrentFace) do
           begin
             Inc(CVertexID);
             cmfFile.Write(CurrentFace.VertexID[1],12);
             CurrentFace:=CurrentFace.NextFace;
          end;
         if CVertexID <> CurrentSubmesh.NumberofFaces then
            MEssageBox(GetActiveWindow,PChar('It is set to '+IntToStr(CurrentSubmesh.NumberofFaces)+#13+
                                             'It SHOULD BE '+IntToStr(CVertexID)),
                                             'Invalid value for NumberOfFaces!',MB_ICONERROR);
       end;
      CurrentSubmesh:=CurrentSubmesh.NextSubmesh;
    end;
    cmfFile.Free;
end;

procedure TCal3DMesh.ReadCMF(FileName:String);
var
   CurrentSubmesh:TSubMeshData;
   CurrentVertex:TVertexData;
   CurrentFace:TFaceData;
   CurrentMap:TMapData;
   CurrentSpring:TSpringData;
   CurrentInfluence:TInfluenceData;
   SubmeshCount,MaterialCount:Integer;
   VertexCount,MapCount,InfluenceCount:integer;
   SpringCount,FaceCount:Integer;
   AddedObjectID:Integer;
   cmfFile:TFileSTream;
begin
     cmfFile:=TFileStream.Create(FileName,fmOpenRead);
     cmfFile.Read(MagicToken[0],4);
     cmfFile.Read(FileVersion,4);
     cmfFile.Read(NumberOfSubmeshes,4);
     CurrentSubmesh:=nil;
     if NumberOfSubmeshes >= 1 then
      for SubmeshCount:=1 to NumberOfSubmeshes do
       begin
         if not Assigned(CurrentSubmesh) then
          begin
            SubMeshes:=TSubMeshData.Create(nil);
            CurrentSubmesh:=SubMeshes;
          end
         else
          begin
            CurrentSubmesh.NextSubmesh:=TSubMeshData.Create(CurrentSubmesh);
            CurrentSubmesh:=CurrentSubmesh.NextSubmesh;
          end;
         cmfFile.Read(CurrentSubmesh.MaterialThreadId,4);
         cmfFile.Read(CurrentSubmesh.NumberOfvertices,4);
         cmfFile.Read(CurrentSubmesh.NumberOffaces,4);
         cmfFile.Read(CurrentSubmesh.NumberOflodSteps,4);
         cmfFile.Read(CurrentSubmesh.NumberOfsprings,4);
         cmfFile.Read(CurrentSubmesh.NumberOfmaps,4);
         CurrentVertex:=Nil;

         for VertexCount:=1 to CurrentSubmesh.NumberOfvertices do
           begin
              if not Assigned(CurrentVertex) then
               begin
                 CurrentSubmesh.VertexData:=TVertexData.Create(nil);
                 CurrentVertex:=CurrentSubmesh.VertexData;
               end
              else
               begin
                 CurrentVertex.NextVertex:=TVertexData.Create(CurrentVertex);
                 CurrentVertex:=CurrentVertex.NextVertex;
               end;
               cmfFile.Read(CurrentVertex.Position[x],12);
               cmfFile.Read(CurrentVertex.Normal[x],12);
               cmfFile.Read(CurrentVertex.CollapseID,4);
               cmfFile.Read(CurrentVertex.FaceCollapseCount,4);
               CurrentMap:=nil;

               if CurrentSubmesh.NumberOfmaps > 0 then
                for MapCount:=1 to CurrentSubmesh.NumberOfmaps do
                 begin
                    if not Assigned(CurrentMap) then
                     begin
                       CurrentVertex.CoordinateMaps:=TMapData.Create(nil);
                       CurrentMap:=CurrentVertex.CoordinateMaps;
                     end
                    else
                     begin
                       CurrentMap.NextMap:=TMapData.Create(CurrentMap);
                       CurrentMap:=CurrentMap.NextMap;
                     end;
                    cmfFile.Read(CurrentMap.coordinates[u],8);
                 end;
               cmfFile.Read(CurrentVertex.NumberOfInfluences,4);
               CurrentInfluence:=nil;
               if CurrentVertex.NumberOfInfluences > 0 then
                for InfluenceCount:=1 to CurrentVertex.NumberOfInfluences do
                 begin
                    if not Assigned(CurrentInfluence) then
                     begin
                       CurrentVertex.Influences:=TInfluenceData.Create(nil);
                       CurrentInfluence:=CurrentVertex.Influences;
                     end
                    else
                     begin
                       CurrentInfluence.NextInfluence:=TInfluenceData.Create(CurrentInfluence);
                       CurrentInfluence:=CurrentInfluence.NextInfluence;
                     end;
                     cmfFile.Read(CurrentInfluence.BoneID,4);
                     cmfFile.Read(CurrentInfluence.weight,4);
                 end;
               if CurrentSubmesh.NumberOfsprings > 0 then
                  cmfFile.Read(CurrentVertex.weight,4);
             end;
           CurrentSpring:=Nil;
           if CurrentSubmesh.NumberOfsprings > 0 then
            for SpringCount:=1 to CurrentSubmesh.NumberOfsprings do
              begin
                if not Assigned(CurrentSpring) then
                 begin
                   CurrentSubmesh.Springs:=TSpringData.Create(nil);
                   CurrentSpring:=CurrentSubmesh.Springs;
                 end
                else
                 begin
                   CurrentSpring.NextSpring:=TSpringData.Create(CurrentSpring);
                   CurrentSpring:=CurrentSpring.NextSpring;
                 end;
                 cmfFile.Read(CurrentSpring.VertexID[1],8);
                 cmfFile.Read(CurrentSpring.springcoefficient,4);
                 cmfFile.Read(CurrentSpring.idleLength,4);
              end;
           CurrentFace:=Nil;
           if CurrentSubmesh.NumberOfFaces > 0 then
            for FaceCount:=1 to CurrentSubmesh.NumberOfFaces do
             begin
               if not Assigned(CurrentFace) then
                begin
                  CurrentSubmesh.Faces:=TFaceData.Create(nil);
                  CurrentFace:=CurrentSubmesh.Faces;
                end
               else
                begin
                  CurrentFace.NextFace:=TFaceData.Create(CurrentFace);
                  CurrentFace:=CurrentFace.NextFace;
                end;
                cmfFile.Read(CurrentFace.VertexID[1],12);
             end;
         end;
         cmfFile.Free;
end;

end.
