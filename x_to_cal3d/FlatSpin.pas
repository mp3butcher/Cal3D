//****************************************************************************//
// FlatSpin.pas                                                               //
// Copyright (C) 2006 Ken Schafer                                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//
unit FlatSpin;
                             
interface

uses Windows, Classes, StdCtrls, ExtCtrls, Controls, Messages, SysUtils,
  Forms, Graphics, Menus, Buttons, EnterTab;

const
  InitRepeatPause = 400;  { pause before repeat timer (ms) }
  RepeatPause     = 100;  { pause before hint window displays (ms)}

type

  TNumGlyphs = Buttons.TNumGlyphs;

  TTimerSpeedButton = class;

{ This is a simple up/down speedbutton with arrows }
  TFlatSpinButtonState = (spNormal, spUp, spDown);

  TFlatSpinButton = class(TCustomControl)
  private
    FOnUpClick: TNotifyEvent;
    FOnDownClick: TNotifyEvent;
    FState: TFlatSpinButtonState;
    Timer: TTimer;
    FStartTime: DWORD;
    MouseOverButton:integer;
    procedure SetState(x: TFlatSpinButtonState);
    function CheckMouseOverNewButton:Boolean;
  protected
    procedure OnTimer(Sender: TObject);
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
    procedure CMMouseEnter(var Message: TMessage); message CM_MOUSEENTER;
    procedure CMMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
    procedure CMMouseMove(var Message: TMessage); message WM_MOUSEMOVE;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Paint; override;
    property State: TFlatSpinButtonState read FState write SetState;
  published
    property OnDownClick: TNotifyEvent read FOnDownClick write FOnDownClick;
    property OnUpClick: TNotifyEvent read FOnUpClick write FOnUpClick;
    property Align;
    property DragCursor;
    property DragMode;
    property Enabled;
    property ParentCtl3D;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnEnter;
    property OnExit;
  end;


{ TFlatSpinEdit }

  TFlatSpinEdit = class(TCustomPanel)
  private
    FMinValue: LongInt;
    FMaxValue: LongInt;
    FIncrement: LongInt;
    FButton: TFlatSpinButton;
    FEditor: TEnterEdit;
    fAllowNegative: Boolean;
    FEditorEnabled: Boolean;
    fShowSpinButtons  : Boolean;
    fAutoHideSpinButtons  : Boolean;
    funits:String;
    procedure SetUnits(New:String);
    procedure SetAllowNegative(New:Boolean);
    function GetValue: LongInt;
    function CheckValue (NewValue: LongInt): LongInt;
    procedure SetValue (NewValue: LongInt);
    procedure SetEditRect;
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure CMEnter(var Message: TCMGotFocus); message CM_ENTER;
    procedure CMExit(var Message: TCMExit);   message CM_EXIT;
    procedure WMPaste(var Message: TWMPaste);   message WM_PASTE;
    procedure WMCut(var Message: TWMCut);   message WM_CUT;
    function GetText:String;
    procedure SetText(New:String);
    procedure InternalOnChange(Sender:TObject);

    function GetAutoSelect:boolean;
    procedure SetAutoSelect(New:boolean);
    function GetMaxLength:Integer;
    procedure SetMaxLength(New:Integer);
    function GetReadOnly:boolean;
    procedure SetReadOnly(New:boolean);
    function GetOnChange:TNotifyEvent;
    procedure SetOnChange(New:TNotifyEvent);
    procedure SetColor(New:TColor);
    function GetColor:TColor;
    function GetAlignment:TAlignment;
    procedure SetAlignment(New:TAlignment);
    procedure SetShowSpinButtons(SetVisible: Boolean);
    procedure SetAutoHideSpinButtons(SetVisible: Boolean);

  protected
    externalOnChange:TNotifyEvent;
    procedure SetEnabled(New:Boolean);
    function GetEnabled:Boolean;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function IsValidChar(Key: Char): Boolean; virtual;
    procedure UpClick (Sender: TObject); virtual;
    procedure DownClick (Sender: TObject); virtual;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure KeyPress(var Key: Char); override;
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWnd; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Loaded; override;
    property Button: TFlatSpinButton read FButton;
  published
    property Anchors;
    property Align;
    property AllowNegative:Boolean read fAllowNegative write SetAllowNegative;
    property Alignment:TAlignment read GetAlignment write SetAlignment;
    property AutoSelect:boolean read GetAutoSelect write SetAutoSelect;
    property AutoSize;
    property Color:TColor read GetColor write SetColor;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragMode;
    property EditorEnabled: Boolean read FEditorEnabled write FEditorEnabled default True;
    property Enabled:Boolean read GetEnabled write SetEnabled;
    property Font;
    property Increment: LongInt read FIncrement write FIncrement default 1;
    property MaxLength:integer read GetMaxLength write SetMaxLength;
    property MaxValue: LongInt read FMaxValue write FMaxValue;
    property MinValue: LongInt read FMinValue write FMinValue;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ReadOnly:boolean read GetReadOnly write SetReadOnly;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Text:String read GetText write SetText;
    property Value: LongInt read GetValue write SetValue;
    property Visible;
    property OnChange:TNotifyEvent read GetOnChange write SetOnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDrag;
    property units:String read fUnits write SetUnits;
    property ShowSpinButtons:Boolean read fShowSpinButtons write SetShowSpinButtons;
    property AutoHideSpinButtons:Boolean read fAutoHideSpinButtons write SetAutoHideSpinButtons;

  end;

  TCustomFloatEdit = class(TCustomEdit)
  private
    { Private-Deklarationen }
    FDigits      : Byte;
    FMin,FMax    : Extended;
    FDec         : Char;
    FErrorText     : string;
    FOldVal      : Extended;
    FAlignment   : TAlignment;
    FOnEnterPress: TKeyPressEvent;
    FTempValue   : Extended;
    function GetAbout: string; virtual;
    procedure SetAbout(const Value: string);
    function GetText: string;
    procedure SetText(New: string);
    procedure ShowError;
  protected
    { Protected-Deklarationen }
    procedure CreateParams(var Params:TCreateParams); override;
    procedure SetValue(AValue : Extended);
    procedure SetMin(AValue : Extended);
    procedure SetMax(AValue : Extended);
    procedure SetDigits(AValue : Byte);
    function GetValue : Extended; virtual;
    procedure KeyPress(var Key: Char); override;
    procedure DoExit; override;
    procedure DoEnter; override;
    procedure SetAlignment(AValue:TAlignment);
    property OnEnterPress : TKeyPressEvent read FOnEnterPress write FOnEnterPress;
    property Digits   : Byte read FDigits write SetDigits;
    property Min : Extended read FMin write SetMin;
    property Max : Extended read FMax write SetMax;
    property Value    : Extended read GetValue write SetValue;
    property ErrorMessage :string read FErrorText write FErrorText;
    property Alignment: TAlignment read FAlignment write SetAlignment default taLeftJustify;
    property About: string read GetAbout write SetAbout stored false;
  public
    { Public-Deklarationen }
    property Text: string read GetText write SetText;
    procedure Loaded; override;
  published
    { Published-Deklarationen }
    constructor Create (AOwner : TComponent);override;
  end;

  TFloatEdit = class (TCustomFloatEdit)
  published
    property About;
    property Digits;
    property Value;
    property Min;
    property Max;
    property ErrorMessage;
    property Alignment;
    property OnEnterPress;
    property Anchors;
    property AutoSelect;
    property AutoSize;
    property Color;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragMode;
    property Enabled;
    property Font;
    property MaxLength;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ReadOnly;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDrag;
  end;

  TUnitFloatEdit = class (TFloatEdit)
  private
    fUnits:String;
  protected
    procedure DoExit; override;
    procedure DoEnter; override;
    function GetValue : Extended; override;
  published
    property Units:String read fUnits write fUnits;
  end;


  // for computed increment
  TComputedIncrementEvent = procedure (Sender: TObject; const bIncrement:Boolean) of object;

  TSpinFloatEdit = class (TCustomFloatEdit)
  private
    FIncrement: Extended;
    FButton: TFlatSpinButton;
    FEditorEnabled : Boolean;
    FComputedIncrement: TComputedIncrementEvent;
    function GetMinHeight: Integer;
    procedure SetEditRect;
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure WMPaste(var Message: TWMPaste);   message WM_PASTE;
    procedure WMCut(var Message: TWMCut);   message WM_CUT;
  protected
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    procedure UpClick (Sender: TObject); virtual;
    procedure DownClick (Sender: TObject); virtual;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    procedure KeyPress(var Key: Char); override;
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CheckIncDec(const bIncrement:Boolean);
    procedure CreateWnd; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property Button: TFlatSpinButton read FButton;
  published
    property About;
    property anchors;
    property Align;
    property EditorEnabled: Boolean read FEditorEnabled write FEditorEnabled default True;
    property Increment: Extended read FIncrement write FIncrement;
    property OnComputedIncrement: TComputedIncrementEvent read FComputedIncrement write FComputedIncrement;
    property Digits;
    property Value;
    property Min;
    property Max;
    property ErrorMessage;
    property Alignment;
    property OnEnterPress;
    property AutoSelect;
    property AutoSize;
    property Color;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragMode;
    property Enabled;
    property Font;
    property MaxLength;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ReadOnly;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDrag;
  end;


  TFlatFloatSpinEdit = class(TCustomPanel)
  private
    FMinValue: Extended;
    FMaxValue: Extended;
    FIncrement: Extended;
    FButton: TFlatSpinButton;
    FEditor: TUnitFloatEdit;
    fModified:Boolean;

    FEditorEnabled: Boolean;
    fAllowNegative:Boolean;
    funits:String;
    FDigits:Byte;
    ExternalOnChange:TNotifyEvent;
    procedure InternalOnChange(Sender:TObject);
    procedure SetUnits(New:String);
    procedure SetDigits(new:Byte);
    procedure SetMaxValue(new:Extended);
    procedure SetMinValue(new:Extended);

    procedure SetAllowNegative(New:Boolean);

    function GetValue: Extended;
    function CheckValue (NewValue: Extended): Extended;
    procedure SetValue (NewValue: Extended);
    procedure SetEditRect;
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure CMEnter(var Message: TCMGotFocus); message CM_ENTER;
    procedure CMExit(var Message: TCMExit);   message CM_EXIT;
    procedure WMPaste(var Message: TWMPaste);   message WM_PASTE;
    procedure WMCut(var Message: TWMCut);   message WM_CUT;
    function GetText:String;
    procedure SetText(New:String);

    function GetAutoSelect:boolean;
    procedure SetAutoSelect(New:boolean);
    function GetMaxLength:Integer;
    procedure SetMaxLength(New:Integer);
    function GetReadOnly:boolean;
    procedure SetReadOnly(New:boolean);
    function GetOnChange:TNotifyEvent;
    procedure SetOnChange(New:TNotifyEvent);
    procedure SetColor(New:TColor);
    function GetColor:TColor;
    function GetAlignment:TAlignment;
    procedure SetAlignment(New:TAlignment);

  protected
    procedure EditorKeyDown(Sender:TObject; var Key: Word; Shift: TShiftState);
    procedure EditorKeyPress(Sender:TObject; var Key: Char);

    procedure SetEnabled(New:Boolean);
    function GetEnabled:Boolean;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function IsValidChar(Key: Char): Boolean; virtual;
    procedure UpClick (Sender: TObject); virtual;
    procedure DownClick (Sender: TObject); virtual;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    procedure KeyPress(var Key: Char); override;
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWnd; override;
  public
    function Focused:Boolean; override;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Loaded; override;
    property Button: TFlatSpinButton read FButton;
  published
    property Align;
    property Anchors;
    property Alignment:TAlignment read GetAlignment write SetAlignment;
    property AutoSelect:boolean read GetAutoSelect write SetAutoSelect;
    property AllowNegative:Boolean read fAllowNegative write SetAllowNegative;
    property AutoSize;
    property Color:TColor read GetColor write SetColor;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragMode;
    property EditorEnabled: Boolean read FEditorEnabled write FEditorEnabled default True;
    property Enabled:Boolean read GetEnabled write SetEnabled;
    property Font;
    property Increment: Extended read FIncrement write FIncrement;
    property MaxValue: Extended read FMaxValue write SetMaxValue;
    property MinValue: Extended read FMinValue write SetMinValue;
    property Modified:Boolean read fModified write fModified;
    property MaxLength:Integer read GetMaxLength write SetMaxLength;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ReadOnly: boolean read GetReadOnly write SetReadOnly;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Text:String read GetText write SetText;
    property Value: Extended read GetValue write SetValue;
    property Visible;
    property OnChange:TNotifyEvent read GetOnChange write SetOnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDrag;
    property units  : String read fUnits write SetUnits;
    property Digits : Byte read FDigits write SetDigits;
  end;


{ TTimerSpeedButton }

  TTimeBtnState = set of (tbFocusRect, tbAllowTimer);

  TTimerSpeedButton = class(TSpeedButton)
  private
    FRepeatTimer: TTimer;
    FTimeBtnState: TTimeBtnState;
    procedure TimerExpired(Sender: TObject);
  protected
    procedure Paint; override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
  public
    destructor Destroy; override;
    property TimeBtnState: TTimeBtnState read FTimeBtnState write FTimeBtnState;
  end;

procedure Register;

implementation

uses LMDUtils, xpDisplay, lmdXPStyles, dialogs, KensRoutines;

{ TFlatSpinButton	}

constructor TFlatSpinButton.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  if AOwner is TWinControl then
     Parent:=TWinControl(AOwner);
  Timer := TTimer.Create(Self);
  Timer.OnTimer := OnTimer;
  Timer.Enabled := FALSE;
  ControlStyle := ControlStyle - [csAcceptsControls, csSetCaption] + [csOpaque];
  if not (XPThemesAvailable and IsThemeActive) then
     ControlStyle := ControlStyle + [csFramed];
  Width := 20;
  Height := 25;
  Ctl3d := FALSE;
end;

destructor TFlatSpinButton.Destroy;
begin
  Timer.Free;
  inherited Destroy;
end;

procedure TFlatSpinButton.OnTimer(Sender: TObject);
var
  l: DWORD;
begin
  l := GetTickCount - FStartTime;
  case State of
    spUp:
      if assigned(FOnUpClick) then FOnUpClick(Self);
    spDown:
      if assigned(FOnDownClick) then FOnDownClick(Self);
  else
    Timer.Enabled := FALSE;
  end;

  if l > 4000 then
  begin
    Timer.Enabled := FALSE;
    while FState <> spNormal do
    begin
      l := GetTickCount + 50;
      while GetTickCount < l do
        Application.ProcessMessages;
      case State of
        spUp:
          if assigned(FOnUpClick) then FOnUpClick(Self);
        spDown:
          if assigned(FOnDownClick) then FOnDownClick(Self);
      end;
    end;
  end
  else if l > 3000 then
    Timer.Interval := 66
  else if l > 2000 then
    Timer.Interval := 100
  else if l > 1000 then
    Timer.Interval := 200;

end;

procedure TFlatSpinButton.MouseDown(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  if y > Height div 2 then
    State := spDown
  else
    State := spUp;
  MouseCapture := TRUE;
  Timer.Interval := 300;
  FStartTime := GetTickCount;
  Timer.Enabled := TRUE;
  case FState of
    spUp:
      if assigned(FOnUpClick) then FOnUpClick(Self);
    spDown:
      if assigned(FOnDownClick) then FOnDownClick(Self);
  end;
end;

procedure TFlatSpinButton.MouseUp(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  MouseCapture := FALSE;
  State := spNormal;
  Timer.Enabled := FALSE;
end;

procedure TFlatSpinButton.CMMouseMove(var Message: TMessage);
begin
  inherited;
  if XPThemesAvailable and IsThemeActive and CheckMouseOverNewButton then
     Paint;
end;


procedure TFlatSpinButton.CMMouseEnter(var Message: TMessage);
begin
     inherited;
     if XPThemesAvailable and IsThemeActive then
        Paint;
end;

procedure TFlatSpinButton.CMMouseLeave(var Message: TMessage);
begin
     inherited;
     if XPThemesAvailable and IsThemeActive then
        Paint;
end;

function TFlatSpinButton.CheckMouseOverNewButton:Boolean;
var
   CurPos:TPoint;
   NewMouseOverButton:Integer;
begin
     GetCursorPos(CurPos);
     CurPos:=ScreenToClient(CurPos);
     NewMouseOverButton:=0;
     if (CurPos.X >= 0) and (CurPos.X <= ClientWidth) then
      if (CurPos.Y > 0) and (CurPos.Y <= Pred(ClientHeight DIV 2)) then
          NewMouseOverButton:=1
      else if (CurPos.Y > Pred(ClientHeight DIV 2)) and
           (CurPos.Y < ClientHeight) then
          NewMouseOverButton:=2;
     Result:=NewMouseOverButton <> MouseOverButton;
     MouseOverButton:=NewMouseOverButton;
end;

procedure TFlatSpinButton.SetState(x: TFlatSpinButtonState);
begin
  if FState <> x then
  begin
    FState := x;
    invalidate;
  end;
end;

procedure TFlatSpinButton.Paint;
var
  h, d: Integer;
  rec: TRect;
  XPState:TLMD_ButtonState;

  procedure Frame(r: TRect; down: Boolean);
  begin
    if down then
      with Canvas do
      begin
        Pen.Color := clBtnShadow;
        MoveTo(r.Left, r.Bottom);
        LineTo(r.Left, r.Top);
        LineTo(r.Right, r.Top);
        Pen.Color := clWhite;
        LineTo(r.Right, r.Bottom);
        LineTo(r.Left, r.Bottom);
      end
    else
      with Canvas do
      begin
        Pen.Color := clWhite;
        MoveTo(r.Left, Pred(r.Bottom));
        LineTo(r.Left, r.Top);
        LineTo(r.Right - 1, r.Top);
        Pen.Color := clBtnShadow;
        LineTo(r.Right - 1, r.Bottom);
        LineTo(r.Left, r.Bottom);
        Pen.Color := clBlack;
        MoveTo(r.Right, r.Top + 1);
        LineTo(r.Right, r.Bottom);
      end;
  end;

  procedure DrawArrow(x, y, w: Integer; up: Boolean);
  var
    i, j: integer;
  begin
    i := w;
    j := 0;
    x := x - w;
    while i >= 0 do
    begin
      Canvas.MoveTo(x + j, y);
      Canvas.LineTo(x + w + i, y);
      dec(i);
      inc(j);
      if up then
        dec(y)
      else
        inc(y);
    end;
  end;

var
   SaveRec:TRect;
begin
  h := Height div 2;
  rec := ClientRect;
{ if Parent is TFlatSpinEdit then
     InflateRect(Rec,-0,-1);}
  if not (XPThemesAvailable and IsThemeActive) then
   begin
    Canvas.Brush.Color := clBtnFace;
    Canvas.Brush.Style := bsSolid;
    Canvas.FillRect(rec);
    Canvas.Pen.Color := clBtnFace;
    Canvas.Rectangle(rec.Left, Rec.Top, Rec.Right, Rec.Bottom);
   end;

  //inc(rec.Left, 1);
  rec.Bottom := h - 1;
  if not (XPThemesAvailable and IsThemeActive) then
     Frame(rec, FState = spUp)
  else
   begin
     if fState = spUp then
        XPState := lmdbsDown
     else
        XPState := lmdbsUp;
     SaveRec:=Rec;
     CheckMouseOverNewButton;
     XPDrawBtnFaceExt(Self.Parent, Canvas, MouseOverButton = 1,
       Rec, xpSpinTop,XPState, BDF_ROUNDED, Color);
     Rec:=SaveRec;
   end;
  rec.Bottom := Height - 1;
  rec.Top := h;
  if not (XPThemesAvailable and IsThemeActive) then
     Frame(rec, FState = spDown)
  else
   begin
     if fState = spDown then
        XPState := lmdbsDown
     else
        XPState := lmdbsUp;
     OffsetRect(Rec,0,1);
     XPDrawBtnFaceExt(Self.Parent, Canvas, MouseOverButton = 2,
         Rec, xpSpinBottom, XPState, BDF_ROUNDED, Color);
     exit;
   end;

  if Enabled then
    Canvas.Pen.Color := clBlack
  else
    Canvas.Pen.Color := clBtnShadow;
  if FState = spUp then
    d := 1
  else
    d := 0;
  DrawArrow(Width div 2 + d{ + 1}, (h DIV 2) + Succ(d), 3, true);
  if FState = spDown then
    d := 1
  else
    d := 0;
  DrawArrow(Width div 2 + d{ + 1}, ((h - 3) DIV 2) + h + d, 3, FALSE);
end;

{ TFlatSpinEdit }

constructor TFlatSpinEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  FButton := TFlatSpinButton.Create(Self);
  FButton.Parent:=Self;
  FButton.Width := 12;
  FButton.Align:=alright;

  FEditor:=TEnterEdit.Create(Self);
  FEditor.Parent := Self;
  FEditor.BorderStyle:=bsNone;
  FEditor.Ctl3D :=False;
  FEditor.Text := '0';
  FEditor.OnChange:=InternalOnChange;
  Color:=clWindow;

  ControlStyle := ControlStyle - [csSetCaption];
  funits:='';
  FIncrement := 1;
  FEditorEnabled := True;
  FShowSpinButtons:=True;
  BevelOuter:=bvLowered;
  BevelInner:=bvNone;
end;

procedure TFlatSpinEdit.Loaded;
begin
  inherited;
  FButton.Visible := FShowSpinButtons and not FAutoHideSpinButtons;
  FButton.OnUpClick := UpClick;
  FButton.OnDownClick := DownClick;

  FEditor.Visible := True;
  FEditor.SetBounds(3,3,ClientWidth-22,ClientHeight-4);
  FEditor.ParentShowHint:=True;
  FEditor.OnClick:=OnClick;
  FEditor.OnDblClick:=OnDblClick;
  FEditor.OnDragDrop:=OnDragDrop;
  FEditor.OnDragOver:=OnDragOver;
  FEditor.OnEndDrag:=OnEndDrag;
  FEditor.OnEnter:=OnEnter;
  FEditor.OnExit:=OnExit;
  FEditor.OnKeyDown:=OnKeyDown;
  FEditor.OnKeyPress:=OnKeyPress;
  FEditor.OnKeyUp:=OnKeyUp;
  FEditor.OnMouseDown:=OnMouseDown;
  FEditor.OnMouseMove:=OnMouseMove;
  FEditor.OnMouseUp:=OnMouseUp;
  FEditor.OnStartDrag:=OnStartDrag;
end;

procedure TFlatSpinEdit.SetAutoSelect(New:boolean);
begin
     Feditor.AutoSelect:=New;
end;

procedure TFlatSpinEdit.SetMaxLength(New:Integer);
begin
     Feditor.MaxLength:=New;
end;

procedure TFlatSpinEdit.SetReadOnly(New:boolean);
begin
     Feditor.ReadOnly:=New;
end;

procedure TFlatSpinEdit.SetOnChange(New:TNotifyEvent);
begin
     ExternalOnChange:=New;
end;

procedure TFlatSpinEdit.SetEnabled(New:Boolean);
begin
   fEditor.Enabled:=New;
   FButton.Enabled:=New;
end;

function TFlatSpinEdit.GetEnabled:Boolean;
begin
   result:=fEditor.Enabled;
end;

procedure TFlatSpinEdit.SetColor(New:TColor);
begin
   fEditor.Color:=New;
   inherited Color:=New;
end;

function TFlatSpinEdit.GetColor:TColor;
begin
   result:=fEditor.Color;
end;


function TFlatSpinEdit.GetAutoSelect:boolean;
begin
     Result:=FEditor.AutoSelect;
end;

function TFlatSpinEdit.GetMaxLength:integer;
begin
     Result:=FEditor.MaxLength;
end;

function TFlatSpinEdit.GetReadOnly:boolean;
begin
     Result:=FEditor.ReadOnly;
end;

procedure TFlatSpinEdit.InternalOnChange(Sender:TObject);
begin
     if assigned(ExternalOnChange) then
        ExternalOnChange(Self);
end;


function TFlatSpinEdit.GetOnChange:TNotifyEvent;
begin
     Result:=ExternalOnChange;
end;

function TFlatSpinEdit.GetText:String;
begin
     result:=FEditor.Text;
     if not (csLoading in componentstate) and (funits <> '') and (Pos(funits,Result) > 0) then
       Delete(Result,Pos(funits,Result),255);
     result:=Trim(Result);
end;

procedure TFlatSpinEdit.SetText(New:String);
var
   SStart:Integer;
begin
    SStart:=FEditor.SelStart;
     if not (csLoading in componentstate) and (funits <> '') and not fEditor.Focused then
        FEditor.Text:=New+' '+fUnits
     else
        FEditor.Text:=New;
    FEditor.SelStart:=SStart;
end;


destructor TFlatSpinEdit.Destroy;
begin
  FButton.Free;
  FButton := nil;
  FEditor.Free;
  FEditor:=Nil;
  inherited Destroy;
end;

procedure TFlatSpinEdit.GetChildren(Proc: TGetChildProc; Root: TComponent);
begin
end;

procedure TFlatSpinEdit.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if Key = VK_UP then UpClick (Self)
  else if Key = VK_DOWN then DownClick (Self);
  inherited KeyDown(Key, Shift);
end;

procedure TFlatSpinEdit.KeyPress(var Key: Char);
begin
  if not IsValidChar(Key) then
  begin
    Key := #0;
    MessageBeep(0)
  end;
  if Key <> #0 then inherited KeyPress(Key);
end;

function TFlatSpinEdit.IsValidChar(Key: Char): Boolean;
begin
  Result := (Key in [DecimalSeparator, '+', '-', '0'..'9']) or
    ((Key < #32) and (Key <> Chr(VK_RETURN)));
  if not FEditorEnabled and Result and ((Key >= #32) or
      (Key = Char(VK_BACK)) or (Key = Char(VK_DELETE))) then
    Result := False;
end;

procedure TFlatSpinEdit.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
{  Params.Style := Params.Style and not WS_BORDER;  }
  Params.Style := Params.Style or ES_MULTILINE or WS_CLIPCHILDREN;
end;

procedure TFlatSpinEdit.CreateWnd;
begin
  inherited CreateWnd;
  SetEditRect;
end;

procedure TFlatSpinEdit.SetEditRect;
var
  Loc: TRect;
begin
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));
  Loc.Bottom := ClientHeight + 1;  {+1 is workaround for windows paint bug}
  Loc.Right := ClientWidth - FButton.Width - 2;
  Loc.Top := 0;  
  Loc.Left := 0;  
  SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));  {debug}
end;

procedure TFlatSpinEdit.WMSize(var Message: TWMSize);
begin
  inherited;
  FEditor.SetBounds(3,3,Message.Width-20,Message.Height-4);
end;

procedure TFlatSpinEdit.UpClick (Sender: TObject);
begin
  if ReadOnly then
     MessageBeep(0)
  else
     Value := Value + FIncrement;
end;

procedure TFlatSpinEdit.DownClick (Sender: TObject);
begin
  if ReadOnly then
     MessageBeep(0)
  else
     Value := Value - FIncrement;
end;

procedure TFlatSpinEdit.WMPaste(var Message: TWMPaste);   
begin
  if not FEditorEnabled or ReadOnly then
  Exit;
  inherited;
end;

procedure TFlatSpinEdit.WMCut(var Message: TWMPaste);
begin
  if not FEditorEnabled or ReadOnly then Exit;
  inherited;
end;

procedure TFlatSpinEdit.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
     inherited;
     if FEditorEnabled and not ReadOnly then
        FEditor.SetFocus;
end;


procedure TFlatSpinEdit.CMExit(var Message: TCMExit);
begin
  inherited;
  if CheckValue (Value) <> Value then
    SetValue (Value);
  if FAutoHideSpinButtons then
     FButton.Visible:=False;
end;

function TFlatSpinEdit.GetValue: LongInt;
begin
  try
{KJS ADDED}
    if Trim(Text)='' then
       result:=0
    else {KJS END ADD}
       Result := StrToInt (Text);
  except
    Result := FMinValue;
  end;
end;

function TFlatSpinEdit.GetAlignment:TAlignment;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        result:=fEditor.Alignment
     else
        result:=taLeftJustify;
end;

procedure TFlatSpinEdit.SetAlignment(New:TAlignment);
begin
     If not (csloading in componentstate) and assigned(FEditor) and (fEditor.Alignment <> New) then
        fEditor.Alignment:=New;
end;

procedure TFlatSpinEdit.SetUnits(New:String);
begin
     if funits <> New then
      begin
        funits:=New;
        Text:=Text;
      end;
end;

procedure TFlatSpinEdit.SetAllowNegative(New:Boolean);
begin
     if fAllowNegative <> New then
      begin
        fAllowNegative:=New;
        if not fAllowNegative and (Value < 0) then
           Value:=0;
      end;
end;


procedure TFlatSpinEdit.SetShowSpinButtons(SetVisible: Boolean);
begin
     if (fShowSpinButtons <> SetVisible) or (FButton.Visible <> SetVisible) then
      begin
        fShowSpinButtons:=SetVisible;
        FButton.Visible:=SetVisible;
      end;
end;

procedure TFlatSpinEdit.SetAutoHideSpinButtons(SetVisible: Boolean);
begin
     if (fAutoHideSpinButtons <> SetVisible) then
      begin
        fAutoHideSpinButtons:=SetVisible;
        if not fAutoHideSpinButtons and fShowSpinButtons then
           FButton.Visible:=True
        else if FAutoHideSpinButtons then
          if Focused then
             SetShowSpinButtons(True)
          else
             FButton.Visible:=False;
      end;
end;


procedure TFlatSpinEdit.SetValue (NewValue: LongInt);
begin
  Text := IntToStr (CheckValue (NewValue));
end;

function TFlatSpinEdit.CheckValue (NewValue: LongInt): LongInt;
begin
  Result := NewValue;
  if (FMaxValue <> FMinValue) then
  begin
    if NewValue < FMinValue then
      Result := FMinValue
    else if (NewValue > FMaxValue) and (FMaxValue <> 0) then
      Result := FMaxValue;
  end;
  if not fALlowNegative and (result < 0) then
     result:=0;
end;

procedure TFlatSpinEdit.CMEnter(var Message: TCMGotFocus);
begin
  if AutoSelect and not (csLButtonDown in ControlState) then
   begin
     FEditor.SetFocus;
     FEditor.SelectAll;
   end;
   if FAutoHideSpinButtons then
      SetShowSpinButtons(True);
  inherited;
end;

{TFlatFloatSpinEdit}

constructor TFlatFloatSpinEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  FButton := TFlatSpinButton.Create(Self);
  FButton.Parent:=Self;
  FButton.Width := 12;
  FButton.Align:=alright;

  FEditor:=TUnitFloatEdit.Create(Self);
  FEditor.Parent := Self;
  FEditor.BorderStyle:=bsNone;
  FEditor.Ctl3D :=False;
  FEditor.Text := '0';
  FEditor.Digits:=1;
  FEditor.Onchange:=InternalOnChange;

  Color:=clWindow;

  ControlStyle := ControlStyle - [csSetCaption];

  FIncrement := 1;
  funits:='';
  FEditorEnabled := True;
  BevelOuter:=bvLowered;
  BevelInner:=bvNone;
end;

procedure TFlatFloatSpinEdit.InternalOnChange(Sender:TObject);
begin
     if assigned(ExternalOnChange) then
        ExternalOnChange(Self);
end;


procedure TFlatFloatSpinEdit.Loaded;
begin
  inherited;
  FButton.Visible := True;
  FButton.OnUpClick := UpClick;
  FButton.OnDownClick := DownClick;

  FEditor.Visible := True;
  FEditor.Max:=fMaxValue;
  fEditor.Min:=fMinValue;
  FEditor.Digits:=FDigits;

  FEditor.SetBounds(3,3,ClientWidth-22,ClientHeight-4);
  FEditor.ParentShowHint:=True;
  FEditor.OnClick:=OnClick;
  FEditor.OnDblClick:=OnDblClick;
  FEditor.OnDragDrop:=OnDragDrop;
  FEditor.OnDragOver:=OnDragOver;
  FEditor.OnEndDrag:=OnEndDrag;
  FEditor.OnEnter:=OnEnter;
  FEditor.OnExit:=OnExit;
  FEditor.OnKeyDown:=EditorKeyDown;
  FEditor.OnKeyPress:=EditorKeyPress;
  FEditor.OnKeyUp:=OnKeyUp;
  FEditor.OnMouseDown:=OnMouseDown;
  FEditor.OnMouseMove:=OnMouseMove;
  FEditor.OnMouseUp:=OnMouseUp;
  FEditor.OnStartDrag:=OnStartDrag;
  FEditor.Units:=fUnits;
  ExternalOnChange:=OnChange;
end;

procedure TFlatFloatSpinEdit.SetAllowNegative(New:Boolean);
begin
     if fAllowNegative <> New then
      begin
        fAllowNegative:=New;
        if not fAllowNegative and (Value < 0) then
           Value:=0;
      end;
end;

procedure TFlatFloatSpinEdit.SetAutoSelect(New:boolean);
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Feditor.AutoSelect:=New;
end;

procedure TFlatFloatSpinEdit.SetMaxLength(New:Integer);
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Feditor.MaxLength:=New;
end;

procedure TFlatFloatSpinEdit.SetReadOnly(New:boolean);
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Feditor.ReadOnly:=New;
end;

procedure TFlatFloatSpinEdit.SetOnChange(New:TNotifyEvent);
begin
     ExternalOnChange:=New;
//     If {not (csloading in componentstate) and }assigned(FEditor) then
//        Feditor.OnChange:=New;
end;

procedure TFlatFloatSpinEdit.SetEnabled(New:Boolean);
begin
   If not (csloading in componentstate) and assigned(FEditor) then
      fEditor.Enabled:=New;
   if Assigned(FButton) then
      FButton.Enabled:=New;
end;

function TFlatFloatSpinEdit.GetEnabled:Boolean;
begin
   If not (csloading in componentstate) and assigned(FEditor) then
      result:=fEditor.Enabled
   else
      Result:=True;
end;

procedure TFlatFloatSpinEdit.SetColor(New:TColor);
begin
   If not (csloading in componentstate) and assigned(FEditor) then
      fEditor.Color:=New;
   inherited Color:=New;
end;

function TFlatFloatSpinEdit.GetColor:TColor;
begin
   If not (csloading in componentstate) and assigned(FEditor) then
      result:=fEditor.Color
   else
      Result:=clNone;
end;


function TFlatFloatSpinEdit.GetAutoSelect:boolean;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Result:=FEditor.AutoSelect
     else
        Result:=True;
end;

function TFlatFloatSpinEdit.GetMaxLength:integer;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Result:=FEditor.MaxLength
     else
        Result:=0;
end;

function TFlatFloatSpinEdit.GetReadOnly:boolean;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Result:=FEditor.ReadOnly
     else
        Result:=False;
end;

function TFlatFloatSpinEdit.GetOnChange:TNotifyEvent;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        Result:=ExternalOnChange
     else
        Result:=Nil;
end;

function TFlatFloatSpinEdit.Focused:Boolean;
begin
     result:=Assigned(FEditor) and FEditor.Focused;
end;

function TFlatFloatSpinEdit.GetText:String;
begin
     if not Assigned(Feditor) then
      begin
        Result:='0';
        Exit;
      end;

     result:=FEditor.Text;
     if not (csLoading in componentstate) and (funits <> '') and
      (Pos(' '+funits,Result) > 0) then
         Delete(Result,Pos(' '+funits,Result),255);
end;

procedure TFlatFloatSpinEdit.SetText(New:String);
var
   SSTart:Integer;
begin
     if not Assigned(Feditor) then
        Exit;
     SStart:=FEditor.SelStart;
     if not (csLoading in componentstate) and (funits <> '') and not fEditor.Focused then
        FEditor.Text:=New+' '+fUnits
     else
        FEditor.Text:=New;
     FEditor.SelStart:=SStart;
end;


destructor TFlatFloatSpinEdit.Destroy;
begin
  FButton.Free;
  FButton := nil;
  FEditor.Free;
  FEditor:=Nil;
  inherited Destroy;
end;

procedure TFlatFloatSpinEdit.GetChildren(Proc: TGetChildProc; Root: TComponent);
begin
end;

procedure TFlatFloatSpinEdit.EditorKeyDown(Sender:TObject; var Key: Word; Shift: TShiftState);
begin
   KeyDown(Key,Shift);
   if not (Key in [VK_UP,VK_DOWN]) then
      Inherited;
end;


procedure TFlatFloatSpinEdit.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if Key = VK_UP then
     UpClick (Self)
  else if Key = VK_DOWN then
     DownClick (Self);
  inherited KeyDown(Key, Shift);
end;

procedure TFlatFloatSpinEdit.EditorKeyPress(Sender:TObject;var Key: Char);
begin
   KeyPress(Key);
   if Key <> #0 then
      Inherited;
end;

procedure TFlatFloatSpinEdit.KeyPress(var Key: Char);
begin
  if not IsValidChar(Key) then
  begin
    Key := #0;
    MessageBeep(0)
  end
  else if Key <> #0 then
   begin
     inherited KeyPress(Key);
     FModified:=True;
   end;
end;

function TFlatFloatSpinEdit.IsValidChar(Key: Char): Boolean;
begin
  Result := (Key in [DecimalSeparator, '+', '-', '0'..'9']) or
    ((Key < #32) and (Key <> Chr(VK_RETURN)));
  if not FEditorEnabled and Result and ((Key >= #32) or
      (Key = Char(VK_BACK)) or (Key = Char(VK_DELETE))) then
    Result := False;
end;

procedure TFlatFloatSpinEdit.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
{  Params.Style := Params.Style and not WS_BORDER;  }
  Params.Style := Params.Style or ES_MULTILINE or WS_CLIPCHILDREN;
end;

procedure TFlatFloatSpinEdit.CreateWnd;
begin
  inherited CreateWnd;
  SetEditRect;
end;

procedure TFlatFloatSpinEdit.SetEditRect;
var
  Loc: TRect;
begin
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));
  Loc.Bottom := ClientHeight + 1;  {+1 is workaround for windows paint bug}
  Loc.Right := ClientWidth - FButton.Width - 2;
  Loc.Top := 0;
  Loc.Left := 0;
  SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));  {debug}
end;

procedure TFlatFloatSpinEdit.WMSize(var Message: TWMSize);
begin
  inherited;
  If not (csloading in componentstate) and assigned(FEditor) then
     FEditor.SetBounds(3,3,Message.Width-20,Message.Height-4);
end;

procedure TFlatFloatSpinEdit.UpClick (Sender: TObject);
begin
  if ReadOnly then
     MessageBeep(0)
  else
   begin
     Value := Value + FIncrement;
     FModified:=True;
   end;
end;

procedure TFlatFloatSpinEdit.DownClick (Sender: TObject);
begin
  if ReadOnly then
     MessageBeep(0)
  else
   begin
     Value := Value - FIncrement;
     FModified:=True;
   end;
end;

procedure TFlatFloatSpinEdit.WMPaste(var Message: TWMPaste);
begin
  if not Assigned(Feditor) or not FEditorEnabled or ReadOnly then
     Exit;
  FModified:=True;
  inherited;
end;

procedure TFlatFloatSpinEdit.WMCut(var Message: TWMPaste);
begin
  if not Assigned(Feditor) or not FEditorEnabled or ReadOnly then Exit;
  inherited;
end;

procedure TFlatFloatSpinEdit.CMExit(var Message: TCMExit);
begin
  inherited;
  if CheckValue (Value) <> Value then
    SetValue (Value);
end;

function TFlatFloatSpinEdit.GetValue: Extended;
begin
  if (csLoading in ComponentState) then
     Result:=0
  else
    try
{KJS ADDED}
    if Trim(Text)='' then
       result:=0
    else {KJS END ADD}    
      Result := StringToFloat(Text);
    except
      Result := FMinValue;
    end;
end;

function TFlatFloatSpinEdit.GetAlignment:TAlignment;
begin
     If not (csloading in componentstate) and assigned(FEditor) then
        result:=fEditor.Alignment
     else
        result:=taLeftJustify;
end;

procedure TFlatFloatSpinEdit.SetAlignment(New:TAlignment);
begin
     If not (csloading in componentstate) and assigned(FEditor) and (fEditor.Alignment <> New) then
        fEditor.Alignment:=New;
end;

procedure TFlatFloatSpinEdit.SetUnits(New:String);
begin
     if funits <> New then
      begin
        funits:=New;
        Feditor.Units:=fUnits;
        Text:=Text;
      end;
end;

{function TFlatFloatSpinEdit.GetDigits:Byte;
begin
     Result:=FEditor.Digits;
end;}

procedure TFlatFloatSpinEdit.SetDigits(new:Byte);
begin
     FDigits:=new;
     if not (csloading in componentstate) and Assigned(FEditor) then
        FEditor.Digits:=FDigits;
end;

procedure TFlatFloatSpinEdit.SetMaxValue(new:Extended);
begin
    if FMaxValue <> new then
       fMaxvalue:=New;
    If not (csloading in componentstate) and assigned(FEditor) then
       feditor.Max:=fMaxvalue
end;

procedure TFlatFloatSpinEdit.SetMinValue(new:Extended);
begin
    if FMinValue <> new then
       fMinvalue:=New;
    If not (csloading in componentstate) and assigned(FEditor) then
       feditor.Min:=fMinvalue
end;

procedure TFlatFloatSpinEdit.SetValue (NewValue: Extended);
begin
  Text := FloatToString (CheckValue (NewValue), fDigits);
end;

function TFlatFloatSpinEdit.CheckValue (NewValue: Extended): Extended;
begin
  Result := NewValue;
  if (FMaxValue <> FMinValue) then
  begin
    if NewValue < FMinValue then
      Result := FMinValue
    else if (NewValue > FMaxValue) and (fMaxValue <> 0) then
      Result := FMaxValue;
  end;
  if (result < 0) and not fAllowNegative then
     result:=0;
end;

procedure TFlatFloatSpinEdit.CMEnter(var Message: TCMGotFocus);
begin
  If not (csloading in componentstate) and assigned(FEditor) and AutoSelect and not (csLButtonDown in ControlState) then
   begin
     FEditor.SetFocus;
     FEditor.SelectAll;
   end;
  inherited;
end;


{TTimerSpeedButton}

destructor TTimerSpeedButton.Destroy;
begin
  if FRepeatTimer <> nil then
    FRepeatTimer.Free;
  inherited Destroy;
end;

procedure TTimerSpeedButton.MouseDown(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  inherited MouseDown (Button, Shift, X, Y);
  if tbAllowTimer in FTimeBtnState then
  begin
    if FRepeatTimer = nil then
      FRepeatTimer := TTimer.Create(Self);

    FRepeatTimer.OnTimer := TimerExpired;
    FRepeatTimer.Interval := InitRepeatPause;
    FRepeatTimer.Enabled  := True;
  end;
end;

procedure TTimerSpeedButton.MouseUp(Button: TMouseButton; Shift: TShiftState;
                                  X, Y: Integer);
begin
  inherited MouseUp (Button, Shift, X, Y);
  if FRepeatTimer <> nil then
    FRepeatTimer.Enabled  := False;
end;

procedure TTimerSpeedButton.TimerExpired(Sender: TObject);
begin
  FRepeatTimer.Interval := RepeatPause;
  if (FState = Buttons.bsDown) and MouseCapture then
  begin
    try
      Click;
    except
      FRepeatTimer.Enabled := False;
      raise;
    end;
  end;
end;

procedure TTimerSpeedButton.Paint;
var
  R: TRect;
begin
  inherited Paint;
  if tbFocusRect in FTimeBtnState then
  begin
    R := Bounds(0, 0, Width, Height);
    InflateRect(R, -3, -3);
    if FState = buttons.bsDown then
      OffsetRect(R, 1, 1);
    DrawFocusRect(Canvas.Handle, R);
  end;
end;

const
  NO_TEXT = '';
  SR_VER  = '1.3';
  SR_COPY = '© 1997-2001 markus stephany, merkes@mirkes.de, http://www.mirkes.de';
  
constructor TCustomFloatEdit.Create (AOwner : TComponent);
begin
  inherited Create(AOwner);
  FAlignment := taRightJustify;
  FDec := DecimalSeparator;
  FDigits := 1;
  FMin := 0;
  FMax := 100;//1E110;
  FErrorText := NO_TEXT;
  inherited Text := FloatToStrF(FMin,ffFixed,18,fDigits);
end;

function TCustomFloatEdit.GetAbout: string;
begin
  Result := self.ClassName+' '+SR_VER+','+SR_COPY
end;

procedure TCustomFloatEdit.SetAbout(const Value: string);
begin
  // no change
end;

function TCustomFloatEdit.GetText: string;
begin
  Result := inherited Text;
end;

procedure TCustomFloatEdit.SetText(New: string);
begin
  if inherited Text <> New then
     inherited Text:=New;
end;


procedure TCustomFloatEdit.Loaded;
begin
  inherited Loaded;
  Value := FTempValue;
end;

procedure TCustomFloatEdit.SetAlignment(AValue:TAlignment);
begin
  if FAlignment<>AValue then
  begin
    FAlignment:=AValue;
    RecreateWnd;
  end;
end;

procedure TCustomFloatEdit.CreateParams(var Params:TCreateParams);
const
  Alignments: array[Talignment] of Word= (ES_LEFT, ES_RIGHT, ES_CENTER);
begin
  inherited CreateParams(Params);
  Params.Style:=Params.Style or ES_MULTILINE or Alignments[FAlignment];
end;

procedure TCustomFloatEdit.DoEnter;
begin
  if AutoSelect and not (csLButtonDown in ControlState) then
    SelectAll;
  FOldVal:=GetValue;
  inherited;
end;

procedure TCustomFloatEdit.DoExit;
var
  ts : string;
  Res: Extended;
begin
  ts := Text;
  if ts = '' then
    ts := FloatToStrF(FMin,ffFixed,18,fDigits);
  inherited;
  try
    Res := StringToFloat(ts);
    //last check to is ensure that floating point errors don't cause a problem
    if ((Res < FMin) and (FMin-Res >= 0.01)) or
       ((Res > FMax) and (FMax <> 0) and (Res - FMax >= 0.01)) then
          Raise Exception.Create(FErrorText);
  except
    ShowError;
    Value := FOldVal;
    SelectAll;
    SetFocus;
    Exit;
  end;
  // Ausgabe formatieren
  inherited Text := FloatToStrF(Res,ffFixed,18,FDigits);
  Value:= StringToFloat(Text);
  inherited;
end;

procedure TCustomFloatEdit.SetValue(AValue : Extended);
var
  tmp : string;
begin
  if csLoading in ComponentState then
    FTempValue := AValue
  else
  begin
    if (AValue > FMax) and (FMax <> 0) then
    begin
      ShowError;
      AValue := FMax;
    end;
    if (AValue < FMin) then
    begin
      ShowError;
      AValue := FMin;
    end;
    tmp := FloatToStrF(AValue,ffFixed,18,FDigits);
    inherited Text:=tmp;
  end;
end;

function TCustomFloatEdit.GetValue : Extended;
var
  ts : string;
begin
  if csLoading in ComponentState
  then
    Result := FTempValue
  else
  begin
    ts := text;
    if (ts = '-') or (ts = FDec) or (ts = '') or (ts = 'E') then
       ts := '0';
    try
       Result := StringToFloat(ts);
    except
      Result := FMin;
    end;
    if Result < FMin
    then
       Result := FMin
    else
    if (Result > FMax) and (FMax <> 0) then
       Result := FMax
  end;
end;

procedure TCustomFloatEdit.SetDigits(AValue : Byte);
begin
  if FDigits <> AValue then
  begin
    if AValue > 18 then
      AValue := 18;
    FDigits := AValue;
    Value := Value;
  end;
end;

procedure TCustomFloatEdit.SetMin(AValue : Extended);
begin                             
  if FMin <> AValue then
  begin
    if (AValue > FMax) and (FMax <> 0) then
      raise Exception.Create('Minimum value must not be greater than maximum value !');
    FMin := AValue;
    Value := Value;
  end;
end;

procedure TCustomFloatEdit.SetMax(AValue : Extended);
begin
  if FMax <> AValue then
  begin
    if AValue < FMin then
      raise Exception.Create('Maximum value must not be less than minimum value !');
    FMax := AValue;
    Value := Value;
  end;
end;

procedure TCustomFloatEdit.KeyPress(var Key: Char);
var
  ts : string;
begin
  if Key in [#10,#13] then
  begin
    if Assigned(FOnEnterPress) then FOnEnterPress(self,Key)
    else
      Key := #0;
    Exit;
  end;

  if Key = #27 then
  begin
    Value := fOldVal;
    SelectAll;
    inherited;
    Exit;
  end;

  if Key < #32
  then
    inherited
  else
  begin
    ts := Copy(Text,1,SelStart)+Copy(Text,SelStart+SelLength+1,MaxInt);

    if not (Key in ['0'..'9',FDec,'-','e']) then
    begin
      inherited;
      Key := #0;
      Exit;
   end;

     if Key in [FDec,'-','e']
     then
       if Pos(Key,ts) <> 0 then
       begin
         inherited;
         Key := #0;
         Exit;
       end;

     if Key = '-'
     then
       if FMin >= 0 then
       begin
         inherited;
         Key := #0;
         Exit;
       end;

     if Key = FDec
     then
       if FDigits = 0 then
       begin
         inherited;
         Key := #0;
         Exit;
       end;

     // seltext durch key ersetzen
     ts := Copy(Text,1,SelStart)+Key+Copy(Text,SelStart+SelLength+1,MaxInt);

     // Überprüfen, ob gültiger wert;
     if Pos(FDec,ts) <> 0 then
     begin
       if Length(ts)-Pos(FDec,ts) > FDigits then
       begin
         inherited;
         Key := #0;
         Exit;
       end;
     end;

     if Key = '-'
     then
       if Pos('-',ts) <> 1 then
       begin
         inherited;
         Key := #0;
         Exit;
       end;

     if ts = '' then
     begin
       inherited;
       Key := #0;
       inherited Text := FloatToStrF(FMin,ffFixed,18,FDigits);
       SelectAll;
       Exit;
     end;

     if ts = '-' then
     begin
       inherited;
       Key := #0;
       inherited Text := '-0';
       SelStart := 1;
       SelLength:=1;
       Exit;
     end;

     if ts = FDec then
     begin
       inherited;
       Key := #0;
       inherited Text := '0'+FDec+'0';
       SelStart :=2;
       SelLength:=1;
       Exit;
     end;
     inherited;
   end;
end;

procedure TCustomFloatEdit.ShowError;
begin
  if FErrorText <> NO_TEXT
  then
    ShowMessage(FErrorText);
end;

procedure TUnitFloatEdit.DoEnter;
begin
  if (FUnits <> '') and (Pos(fUnits,Text) > 0) then
     Text:=Trim(Copy(Text,1,Pred(Pos(fUnits,Text))));
  if AutoSelect and not (csLButtonDown in ControlState) then
    SelectAll;
  FOldVal:=GetValue;
  inherited;
end;

procedure TUnitFloatEdit.DoExit;
var
  ts : string;
  Res: Extended;
begin
  ts := Text;
  if ts = '' then
    ts := FloatToStrF(FMin,ffFixed,18,fDigits)
  else if (FUnits <> '') and (Pos(fUnits,ts) > 0) then
    ts:=Trim(Copy(ts,1,Pred(Pos(fUnits,ts))));
  try
    Res := StringToFloat(ts);
    //last check to is ensure that floating point errors don't cause a problem
    if ((Res < FMin) and (FMin-Res >= 0.01)) or
       ((Res > FMax) and (FMax <> 0) and (Res - FMax >= 0.01)) then
          Raise Exception.Create(FErrorText);
  except
    ShowError;
    Value := FOldVal;
    SelectAll;
    SetFocus;
    Exit;
  end;
  // Ausgabe formatieren
  inherited Text := FloatToStrF(Res,ffFixed,18,FDigits);
  Value:= StringToFloat(Text);
  if fUnits <> '' then
     inherited Text := FloatToStrF(Res,ffFixed,18,FDigits)+' '+fUnits;
end;

function TUnitFloatEdit.GetValue : Extended;
var
  ts : string;
begin
  if csLoading in ComponentState
  then
    Result := FTempValue
  else
  begin
    ts := text;
    if (fUnits <> '') and (Pos(fUnits,ts) > 0) then
     begin
       Delete(Ts,Pos(fUnits,ts),255);
       ts:=trim(ts);
     end;
    if (ts = '-') or (ts = FDec) or (ts = '') or (ts = 'E') then
       ts := '0';
    try
       Result := StringToFloat(ts);
    except
      Result := FMin;
    end;
    if Result < FMin
    then
       Result := FMin
    else
    if (Result > FMax) and (FMax <> 0) then
       Result := FMax
  end;
end;



{ TSpinFloatEdit }


constructor TSpinFloatEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FButton := TFlatSpinButton.Create (Self);
  FButton.Width := 12;
  FButton.Align:=alRight;//Height := 17;
  FButton.Visible := True;
  FButton.Parent := Self;
//  FButton.FocusControl := Self;
  FButton.OnUpClick := UpClick;
  FButton.OnDownClick := DownClick;
  ControlStyle := ControlStyle - [csSetCaption];
  FIncrement := 1;
  FEditorEnabled := True;
end;

procedure TSpinFloatEdit.CheckIncDec(const bIncrement: Boolean);
begin
  if not Assigned(FComputedIncrement)
  then
    case bIncrement of
      False: Value := Value - FIncrement;
      True:  Value := Value + FIncrement;
    end
  else
    FComputedIncrement(Self, bIncrement);
end;

procedure TSpinFloatEdit.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  Params.Style := Params.Style or ES_MULTILINE or WS_CLIPCHILDREN;
end;

procedure TSpinFloatEdit.CreateWnd;
begin
  inherited CreateWnd;
  SetEditRect;
end;

destructor TSpinFloatEdit.Destroy;
begin
  FButton := nil;
  inherited;
end;

procedure TSpinFloatEdit.DownClick(Sender: TObject);
begin
  if ReadOnly then MessageBeep(0)
  else
    CheckIncDec(False);
end;

procedure TSpinFloatEdit.GetChildren(Proc: TGetChildProc;
  Root: TComponent);
begin
end;

function TSpinFloatEdit.GetMinHeight: Integer;
var
  DC: HDC;
  SaveFont: HFont;
  I: Integer;
  SysMetrics, Metrics: TTextMetric;
begin
  DC := GetDC(0);
  GetTextMetrics(DC, SysMetrics);
  SaveFont := SelectObject(DC, Font.Handle);
  GetTextMetrics(DC, Metrics);
  SelectObject(DC, SaveFont);
  ReleaseDC(0, DC);
  I := SysMetrics.tmHeight;
  if I > Metrics.tmHeight then I := Metrics.tmHeight;
  Result := Metrics.tmHeight + I div 4 + GetSystemMetrics(SM_CYBORDER) * 4 + 2;
end;

procedure TSpinFloatEdit.KeyDown(var Key: Word; Shift: TShiftState);
begin
  if not FEditorEnabled
  then
    Key := 0;
  if Key = VK_UP then UpClick (Self)
  else if Key = VK_DOWN then DownClick (Self);
  inherited KeyDown(Key, Shift);
end;

procedure TSpinFloatEdit.KeyPress(var Key: Char);
begin
  inherited;
  if not FEditorEnabled
  then
    Key := #0;
end;

procedure TSpinFloatEdit.SetEditRect;
var
  Loc: TRect;
begin
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));
  Loc.Bottom := ClientHeight + 1;  {+1 is workaround for windows paint bug}
  Loc.Right := ClientWidth - FButton.Width - 2;
  Loc.Top := 0;
  Loc.Left := 0;
  SendMessage(Handle, EM_SETRECTNP, 0, LongInt(@Loc));
  SendMessage(Handle, EM_GETRECT, 0, LongInt(@Loc));  {debug}
end;

procedure TSpinFloatEdit.UpClick(Sender: TObject);
begin
  if ReadOnly then MessageBeep(0)
  else
    CheckIncDec(True);
end;

procedure TSpinFloatEdit.WMCut(var Message: TWMCut);
begin
  if not FEditorEnabled or ReadOnly then Exit;
  inherited;
end;

procedure TSpinFloatEdit.WMPaste(var Message: TWMPaste);
begin
  if not FEditorEnabled or ReadOnly then Exit;
  inherited;
end;

procedure TSpinFloatEdit.WMSize(var Message: TWMSize);
var
  MinHeight: Integer;
begin
  inherited;
  MinHeight := GetMinHeight;
    { text edit bug: if size to less than minheight, then edit ctrl does
      not display the text }
  if Height < MinHeight then
    Height := MinHeight
  else if FButton <> nil then
  begin
    if NewStyleControls and Ctl3D then
      FButton.SetBounds(Width - FButton.Width - 5, 0, FButton.Width, Height - 5)
    else FButton.SetBounds (Width - FButton.Width, 1, FButton.Width, Height - 3);
    SetEditRect;
  end;
end;

procedure Register;
begin
  RegisterComponents('mirkes.de', [TFloatEdit,TSpinFloatEdit]);
  registerComponents('Samples',[TFlatSpinButton,TFlatSpinEdit,TFlatFloatSpinEdit]);
end;


end.
