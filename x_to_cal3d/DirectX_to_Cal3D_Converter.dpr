//****************************************************************************//
// DirectX_to_Cal3D_Converter.dpr                                             //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
program DirectX_to_Cal3D_Converter;

uses
  Forms,
  xFileReaderUnit in 'xFileReaderUnit.pas' {ConverterGUI},
  cmfData in 'cmfData.pas',
  XFiles in 'XFiles.pas',
  utility in 'utility.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TConverterGUI, ConverterGUI);
  Application.Run;
end.
