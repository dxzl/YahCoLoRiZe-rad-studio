// CodeGear C++Builder
// Copyright (c) 1995, 2016 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'MyCheckLst.pas' rev: 32.00 (Windows)

#ifndef MychecklstHPP
#define MychecklstHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <Winapi.Messages.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <System.UITypes.hpp>
#include <System.Types.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>

//-- user supplied -----------------------------------------------------------

namespace Mychecklst
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TMyCheckListBox;
//-- type declarations -------------------------------------------------------
class PASCALIMPLEMENTATION TMyCheckListBox : public Vcl::Stdctrls::TCustomListBox
{
	typedef Vcl::Stdctrls::TCustomListBox inherited;
	
private:
	bool FAllowGrayed;
	bool FFlat;
	int FStandardItemHeight;
	System::Classes::TNotifyEvent FOnClickCheck;
	System::Classes::TList* FSaveStates;
	void __fastcall ResetItemHeight(void);
	void __fastcall DrawCheck(const System::Types::TRect &R, Vcl::Stdctrls::TCheckBoxState AState);
	void __fastcall SetChecked(int Index, bool Checked);
	bool __fastcall GetChecked(int Index);
	void __fastcall SetState(int Index, Vcl::Stdctrls::TCheckBoxState AState);
	Vcl::Stdctrls::TCheckBoxState __fastcall GetState(int Index);
	void __fastcall SetCheckRect(int Index, const System::Types::TRect &ACheckRect);
	System::Types::TRect __fastcall GetCheckRect(int Index);
	void __fastcall ToggleClickCheck(int Index);
	void __fastcall InvalidateCheck(int Index);
	System::TObject* __fastcall CreateWrapper(int Index);
	System::TObject* __fastcall ExtractWrapper(int Index);
	System::TObject* __fastcall GetWrapper(int Index);
	bool __fastcall HaveWrapper(int Index);
	void __fastcall SetFlat(bool Value);
	HIDESBASE MESSAGE void __fastcall CNDrawItem(Winapi::Messages::TWMDrawItem &Message);
	HIDESBASE MESSAGE void __fastcall CMFontChanged(Winapi::Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMDestroy(Winapi::Messages::TWMNoParams &Msg);
	
protected:
	virtual void __fastcall DrawItem(int Index, const System::Types::TRect &Rect, Winapi::Windows::TOwnerDrawState State);
	DYNAMIC void __fastcall SetItemData(int Index, NativeInt AData);
	DYNAMIC NativeInt __fastcall GetItemData(int Index);
	DYNAMIC void __fastcall KeyPress(System::WideChar &Key);
	DYNAMIC void __fastcall MouseDown(System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall ResetContent(void);
	DYNAMIC void __fastcall DeleteString(int Index);
	DYNAMIC void __fastcall ClickCheck(void);
	virtual void __fastcall CreateParams(Vcl::Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DestroyWnd(void);
	int __fastcall GetCheckWidth(void);
	
public:
	__fastcall virtual TMyCheckListBox(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TMyCheckListBox(void);
	__property bool Checked[int Index] = {read=GetChecked, write=SetChecked};
	__property Vcl::Stdctrls::TCheckBoxState State[int Index] = {read=GetState, write=SetState};
	__property System::Types::TRect CheckRect[int Index] = {read=GetCheckRect, write=SetCheckRect};
	
__published:
	__property bool AllowGrayed = {read=FAllowGrayed, write=FAllowGrayed, default=0};
	__property Align = {default=0};
	__property Anchors = {default=3};
	__property BiDiMode;
	__property BorderStyle = {default=1};
	__property Color = {default=-16777211};
	__property Columns = {default=0};
	__property Constraints;
	__property Ctl3D;
	__property DragCursor = {default=-12};
	__property DragKind = {default=0};
	__property DragMode = {default=0};
	__property Enabled = {default=1};
	__property ExtendedSelect = {default=1};
	__property bool Flat = {read=FFlat, write=SetFlat, default=1};
	__property Font;
	__property ImeMode = {default=3};
	__property ImeName = {default=0};
	__property IntegralHeight = {default=0};
	__property ItemHeight = {default=16};
	__property Items;
	__property MultiSelect = {default=0};
	__property ParentBiDiMode = {default=1};
	__property ParentColor = {default=0};
	__property ParentCtl3D = {default=1};
	__property ParentFont = {default=1};
	__property ParentShowHint = {default=1};
	__property PopupMenu;
	__property ShowHint;
	__property Sorted = {default=0};
	__property Style = {default=0};
	__property TabOrder = {default=-1};
	__property TabStop = {default=1};
	__property TabWidth = {default=0};
	__property Visible = {default=1};
	__property OnClick;
	__property System::Classes::TNotifyEvent OnClickCheck = {read=FOnClickCheck, write=FOnClickCheck};
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnDrawItem;
	__property OnEndDock;
	__property OnEndDrag;
	__property OnEnter;
	__property OnExit;
	__property OnKeyDown;
	__property OnKeyPress;
	__property OnKeyUp;
	__property OnMeasureItem;
	__property OnMouseDown;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnStartDock;
	__property OnStartDrag;
public:
	/* TWinControl.CreateParented */ inline __fastcall TMyCheckListBox(HWND ParentWindow) : Vcl::Stdctrls::TCustomListBox(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------
}	/* namespace Mychecklst */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_MYCHECKLST)
using namespace Mychecklst;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// MychecklstHPP
