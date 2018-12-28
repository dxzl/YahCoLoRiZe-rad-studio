//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("Main.cpp", DTSColor);
USEFORM("FormSFDlg.cpp", SFDlgForm);
USEFORM("DlgUnderline.cpp", UnderlineForm);
USEFORM("DlgStrip.cpp", StripModeForm);
USEFORM("DlgSpellCheck.cpp", SpellCheckForm);
USEFORM("DlgWebExport.cpp", WebExportForm);
USEFORM("FormOFSSDlg.cpp", OFSSDlgForm);
USEFORM("DlgWingEdit.cpp", WingEditForm);
USEFORM("YcPrintCancelDlg.cpp", YcPrintCancelDialog);
USEFORM("YcPreviewFrm.cpp", YcPreviewForm);
USEFORM("DlgAbout.cpp", AboutForm);
USEFORM("DlgAlternate.cpp", AlternateForm);
USEFORM("DlgColor.cpp", ColorForm);
USEFORM("DlgChooseDict.cpp", ChooseDictForm);
USEFORM("DlgBlend.cpp", BlendChoiceForm);
USEFORM("DlgPlay.cpp", PlayForm);
USEFORM("DlgPadSpaces.cpp", PadSpacesForm);
USEFORM("DlgMorph.cpp", MorphForm);
USEFORM("DlgReplaceText.cpp", ReplaceTextForm);
USEFORM("DlgSmiley.cpp", SmileyForm);
USEFORM("DlgShowDict.cpp", ShowDictForm);
USEFORM("DlgSetColors.cpp", SetColorsForm);
USEFORM("DlgFontSize.cpp", FontSizeForm);
USEFORM("DlgFgBgColors.cpp", FgBgColorsForm);
USEFORM("DlgFavColors.cpp", FavColorsForm);
USEFORM("DlgFontType.cpp", FontTypeForm);
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
