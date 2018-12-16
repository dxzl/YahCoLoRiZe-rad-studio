//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Main.cpp", DTSColor);
USEFORM("FormSFDlg.cpp", SFDlgForm);
USEFORM("FormOFSSDlg.cpp", OFSSDlgForm);
USEFORM("DlgUnderline.cpp", UnderlineForm);
USEFORM("DlgTaePrintCancel.cpp", TaePrintCancelDialog);
USEFORM("DlgStrip.cpp", StripModeForm);
USEFORM("DlgWingEdit.cpp", WingEditForm);
USEFORM("DlgWebExport.cpp", WebExportForm);
USEFORM("YcPreviewFrm.cpp", YcPreviewForm);
USEFORM("DlgSpellCheck.cpp", SpellCheckForm);
USEFORM("DlgAlternate.cpp", AlternateForm);
USEFORM("DlgAbout.cpp", AboutForm);
USEFORM("DlgColor.cpp", ColorForm);
USEFORM("DlgChooseDict.cpp", ChooseDictForm);
USEFORM("DlgBlend.cpp", BlendChoiceForm);
USEFORM("DlgFavColors.cpp", FavColorsForm);
USEFORM("DlgReplaceText.cpp", ReplaceTextForm);
USEFORM("DlgPlay.cpp", PlayForm);
USEFORM("DlgPadSpaces.cpp", PadSpacesForm);
USEFORM("DlgSmiley.cpp", SmileyForm);
USEFORM("DlgShowDict.cpp", ShowDictForm);
USEFORM("DlgSetColors.cpp", SetColorsForm);
USEFORM("DlgMorph.cpp", MorphForm);
USEFORM("DlgFontType.cpp", FontTypeForm);
USEFORM("DlgFontSize.cpp", FontSizeForm);
USEFORM("DlgFgBgColors.cpp", FgBgColorsForm);
USEFORM("DlgMonkey.cpp", MonkeyForm);
USEFORM("DlgLicense.cpp", LicenseForm);
USEFORM("DlgIncDec.cpp", IncDecForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Name = "DTSColor"; // Name used to reference the main class in our code
     Application->Title = "YahCoLoRiZe"; // Product name
     Application->MainFormOnTaskBar = true;
     Application->CreateForm(__classid(TDTSColor), &DTSColor);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
