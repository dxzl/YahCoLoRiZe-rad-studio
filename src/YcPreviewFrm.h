// I believe we can safely construe that this software may be released under
// the Free Software Foundation's GPL - I adopted YcEdit for my project
// more than 15 years ago and have made significant changes in various
// places. Below is the original license. (Scott Swift 2015 dxzl@live.com)
//===========================================================================
// Copyright © 1999 Thin Air Enterprises and Robert Dunn.  All rights reserved.
// Free for non-commercial use.  Commercial use requires license agreement.
// See http://home.att.net/~robertdunn/Yacs.html for the most current version.
//===========================================================================
//---------------------------------------------------------------------------
// TaePreviewFrm.h - header file for TYcEdit print preview.
//---------------------------------------------------------------------------
#ifndef YcPreviewFrmH
#define YcPreviewFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>

#include "..\YcEdit\YcEdit.h"
//---------------------------------------------------------------------------
enum TPreviewMode { OneUp, TwoUp, Zoomed };

//---------------------------------------------------------------------------
class TYcPreviewWindow : public TPanel
//class PACKAGE TYcPreviewWindow : public TPanel
{
protected:
  TYcEdit* FYcEdit;
  int FPage;

  void __fastcall SetPage(int page)
  {
    FPage = page;
    Invalidate();
  };

  void __fastcall SetRichEdit(TYcEdit* edit)
  {
    FYcEdit = edit;
    Invalidate();
  };

public:
  int FHdc;

  __fastcall virtual TYcPreviewWindow(TComponent* Owner) : TPanel(Owner), FPage(0), FHdc(0)
  {
    Color = clWhite;
  };

  __fastcall virtual void Paint(void)
  {
    TPanel::Paint();
    if (!FYcEdit) return;
    // set canvas brush (otherwise, DrawFocusRect() draws white on white)
    Canvas->Brush->Color = clWhite;
    // copy background into rendering rect
    FYcEdit->YcPrint->RenderPage(Canvas->Handle, ClientWidth, ClientHeight, FPage + 1);
  };

  __property TYcEdit* YcEdit = { read = FYcEdit, write = SetRichEdit, nodefault };
  __property int Page = { read = FPage, write = SetPage, nodefault };
};
//---------------------------------------------------------------------------

class PACKAGE TYcPreviewForm : public TForm
{
__published:  // IDE-managed Components;
  TSpeedButton *NextPageBtn;
  TSpeedButton *LastPageBtn;
  TSpeedButton *FirstPageBtn;
  TSpeedButton *PrevPageBtn;
  TStatusBar *StatusBar;
  TComboBox *ScaleCB;
  TPanel *ToolBar;
  TPanel *ClientPanel;
  TSpeedButton *OneUpBtn;
  TSpeedButton *TwoUpBtn;
  TSpeedButton *ZoomBtn;
  TSpeedButton *MarginsBtn;
  TSpeedButton *ZoomInBtn;
  TSpeedButton *ZoomOutBtn;
  TSpeedButton *PrintBtn;
  TSpeedButton *PrintSetupBtn;
  TPanel *ScrollBoxContainerPanel;
  TScrollBox *ScrollBox;
  TPanel *ScrollPanel;
  TUpDown *UpDownScaleY;
  TLabel *LabelScaleY;
  TLabel *LabelScaleX;
  TUpDown *UpDownScaleX;
  void __fastcall FormResize(TObject *Sender);

  void __fastcall FormShow(TObject *Sender);

  void __fastcall OneUpBtnClick(TObject *Sender);
  void __fastcall TwoUpBtnClick(TObject *Sender);
  void __fastcall ZoomBtnClick(TObject *Sender);

  void __fastcall FirstPageBtnClick(TObject *Sender);
  void __fastcall PrevPageBtnClick(TObject *Sender);
  void __fastcall NextPageBtnClick(TObject *Sender);
  void __fastcall LastPageBtnClick(TObject *Sender);
  void __fastcall MarginsBtnClick(TObject *Sender);
  void __fastcall ZoomInBtnClick(TObject *Sender);
  void __fastcall ZoomOutBtnClick(TObject *Sender);
  void __fastcall ScaleCBChange(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall PrintBtnClick(TObject *Sender);
  void __fastcall PrintSetupBtnClick(TObject *Sender);
  void __fastcall UpDownScaleYClick(TObject *Sender, TUDBtnType Button);
  void __fastcall UpDownScaleXClick(TObject *Sender, TUDBtnType Button);

private:  // User declarations
  int xPerInch, yPerInch;
  int cXsize, cYsize;
  
protected:
  TYcEdit* FYcEdit;
  int PageCount;
  int FirstPage;
  TPreviewMode Mode;
  int Scale, FCorrectionX, FCorrectionY;
  HDC Hdc;
  TYcPreviewWindow* Page1;
  TYcPreviewWindow* Page2;
  TYcPreviewWindow* Page3;

  void __fastcall SetCorrectionY(int Value);
  void __fastcall SetCorrectionX(int Value);

public:    // User declarations
  __fastcall TYcPreviewForm(TComponent* Owner);
  __fastcall ~TYcPreviewForm();
  int __fastcall Execute(TYcEdit* taeRichEdit); // use this instead of Show() or ShowModal()
  void __fastcall GotoPage(int page);

  __property int CorrectionX = { read = FCorrectionX, write = SetCorrectionX };
  __property int CorrectionY = { read = FCorrectionY, write = SetCorrectionY };
  __property HDC RendDC = { read = Hdc, write = Hdc, nodefault };
  __property TYcEdit* YcEdit = { read = FYcEdit,
                        write = FYcEdit, default = 0, stored = false };
};
//---------------------------------------------------------------------------
//extern PACKAGE TYcPreviewForm *YcPreviewForm;
extern TYcPreviewForm *YcPreviewForm;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
