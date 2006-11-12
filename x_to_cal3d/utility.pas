//****************************************************************************//
// utility.pas                                                               //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
unit utility;

interface

uses Classes, SysUtils;

function SmartTrim(s:String):String;
function StrFScan(InPChar:PChar;SearchChar:Char):PChar;
procedure SkipTemplate(SkipList:TStrings;TheStream:TMemoryStream;TemplateType:String);
function GetNextTemplate(var TemplateType:String;var TemplateName:String;TheStream:TMemoryStream):Boolean;
function GetNextBlankTemplate(var TemplateName:String;TheStream:TMemoryStream):Boolean;
function PosC(SearchChar:Char;InString:String;Count:Integer):Integer;
function RemoveSpaces(inString:String):String;
function FloatEqual(F1,F2:extended):Boolean;

var
   VirtualStreamPos:Integer;

implementation

function FloatEqual(F1,F2:extended):Boolean;
begin
     result:=(F1=F2) or (abs(F1-F2) < 0.000001);
end;


function SmartTrim(s:String):String;
begin
     result:=Trim(StringReplace(S,'"','',[rfReplaceAll]));
end;

//for some reason this is WILDLY, HUGELY more efficient than the StrScanFunction
function StrFScan(InPChar:PChar;SearchChar:Char):PChar;
begin
     result:=InPChar;
     while not (Result[0] in [#0,SearchChar]) do
       Inc(Result);
     if Result[0] = #0 then
        result:=Nil;
end;

procedure SkipTemplate(SkipList:TStrings;TheStream:TMemoryStream;TemplateType:String);
var
   StartCount:Integer;
   StartPos:PChar;
   EndPos:PChar;
begin
   SkipList.Add(TemplateType);
   StartCount:=1;
   StartPos:=TheStream.Memory;
   Inc(StartPos,VirtualStreamPos);
   repeat
     EndPos:=StrFScan(StartPos,'}');
     StartPos:=StrFScan(StartPos,'{');
     if not assigned(EndPos) then
        raise Exception.Create('Missing Ending Bracket!');
     if not assigned(StartPos) or (EndPos < StartPos) then
       begin
         dec(StartCount);
         if StartCount = 0 then
            break;
         Inc(EndPos);
         StartPos:=EndPos;
       end
     else if (StartPos < EndPos) then
      begin
         Inc(StartCount);
         Inc(StartPos);
      end;
   until StartCount = 0;
   if not assigned(EndPos) then
      raise Exception.Create('Missing Ending Bracket!');
   VirtualStreamPos:=EndPos-TheStream.Memory;
end;

function GetNextTemplate(var TemplateType:String;var TemplateName:String;TheStream:TMemoryStream):Boolean;
var
   TemplateStart,TemplateStop:PChar;
   SaveChar:Char;
begin
   TemplateStart:=TheStream.Memory;
   Inc(TemplateStart,VirtualStreamPos);

   //find first real letter
   while not (TemplateStart[0] in [#0,'a'..'z','A'..'Z','_','}','{']) do
       Inc(TemplateStart);

   result:=not (TemplateStart[0] in [#0,'}','{']);
   if not result then
      exit;
   TemplateStop:=TemplateStart;
   while not (TemplateStop[0] in ['{',' ',#0]) do
     Inc(TemplateStop);

   if not assigned(TemplateStop) then
      raise exception.Create('Cannot Find Member Name!');
   SaveChar:=Templatestop[0];
   Templatestop[0]:=#0;
   TemplateType:=SmartTrim(TemplateStart);

   TemplateStart:=TemplateStop;
   //restore it
   TemplateStop[0]:=SaveChar;
   
   //get past it
   if SaveChar = ' ' then
      Inc(TemplateStart);
   //find template true begining
   TemplateStop:=StrFScan(TemplateStart,'{');
   if not assigned(TemplateStop) then
      raise exception.Create('Cannot Starting { !');
   TemplateStop[0]:=#0;
   TemplateName:=SmartTrim(TemplateStart);
   TemplateStop[0]:='{';
   Inc(TemplateStop);
   VirtualStreamPos:=TemplateStop-TheStream.Memory;
end;

function GetNextBlankTemplate(var TemplateName:String;TheStream:TMemoryStream):Boolean;
var
   TemplateStart,TemplateCheck,TemplateStop:PChar;
begin

   TemplateStart:=TheStream.Memory;
   Inc(TemplateStart,VirtualStreamPos);

   TemplateCheck:=TemplateStart;
   //find first real letter
   while not (TemplateCheck[0] in [#0,'a'..'z','A'..'Z','_','}','{']) do
       Inc(TemplateCheck);

   if TemplateCheck[0]='{' then
    begin
      TemplateCheck[0]:=#0;
      result:=SmartTrim(TemplateStart)='';
      TemplateCheck[0]:='{';
    end
   else
     result:=False;

   if not result then
      exit;

   TemplateStart:=TemplateCheck;
   Inc(TemplateStart);
   while not (TemplateStart[0] in [#0,'a'..'z','A'..'Z','_','}','{']) do
       Inc(TemplateStart);

   TemplateStop:=TemplateStart;
   while not (TemplateStop[0] in [#0,'}']) do
     Inc(TemplateStop);
   if TemplateStop[0] <> '}' then
      raise Exception.Create('Unable to Find End Squiggley Bracket!');

   Templatestop[0]:=#0;
   TemplateName:=SmartTrim(TemplateStart);
   //restore it
   TemplateStop[0]:='}';
   //get past it
   Inc(TemplateStop);
   VirtualStreamPos:=TemplateStop-TheStream.Memory;
end;


function PosC(SearchChar:Char;InString:String;Count:Integer):Integer;
var
   TempResult:Integer;
begin
   result:=0;
   repeat
      TempResult:=Pos(SearchChar,Instring);
      if TempResult = 0 then
         raise Exception.Create(PChar('Unable to Find correct # of '+SearchChar+'s!'));
      result:=result+TempResult;
      Dec(Count);
      if Count > 0 then
         Delete(InString,1,TempResult);
   until Count = 0;
end;

function RemoveSpaces(inString:String):String;
begin
     result:=InString;
     while Pos(' ',Result) > 0 do
       Delete(result,Pos(' ',Result),1);
end;


end.
