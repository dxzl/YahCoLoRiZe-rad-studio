//---------------------------------------------------------------------------
#ifndef ConvertToRTFH
#define ConvertToRTFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------

#define DEF_FONT "Arial"
#define DEF_FONT_NUM 0
#define RESET_HIGHLIGHT_STR "\\highlight0"

class TConvertToRTF
{
private:
  AnsiString __fastcall TerminateLine(AnsiString sLine, PUSHSTRUCT State);
  int __fastcall AddColor(int C, TList* cList);
  int __fastcall AddOrResolveColor(int C, TList* cList);
  int __fastcall FindColorIndex(int C, TList* cList);
  int __fastcall ResolveColor(int C, TList* cList);
  AnsiString __fastcall RgbToRtfColorTable(TList* cList);
  AnsiString __fastcall YcToRtfColorString(int C);

  TStringsW* __fastcall CreateFontList(wchar_t* buf, int size);
  AnsiString __fastcall GetFontTable(TStringsW* slRtfFonts);
  int __fastcall Convert(wchar_t* buf, int size,
               TMemoryStream* msOut, TStringsW* slRtfFonts);
  int __fastcall WriteRtfToStream(AnsiString sBody, AnsiString ColorTable,
                                    AnsiString FontTable, TMemoryStream* msOut);

  // properties
  int defaultFgColor, defaultBgColor, linesInHeader, maxColors, tabTwips;
  bool bShowStatus, bStripFontType, bStripFontSize;

  TYcEdit* re;

  TDTSColor* dts; // easy pointer to main form...

public:
  __fastcall TConvertToRTF(TComponent* Owner, TYcEdit* re,
                                                bool bShowStatus);
  __fastcall ~TConvertToRTF(void);

  TMemoryStream* __fastcall Execute(wchar_t* buf, int size, int &retVal);
  TMemoryStream* __fastcall Execute(WideString S, int &retVal);
  TMemoryStream* __fastcall Execute(TMemoryStream* msIn, int &retVal);
  TMemoryStream* __fastcall Execute(TStringsW* sl, int &retVal);

  __property bool StripFontType = {read = bStripFontType, write = bStripFontType};
  __property bool StripFontSize = {read = bStripFontSize, write = bStripFontSize};

  __property int DefaultFgColor = {read = defaultFgColor, write = defaultFgColor};
  __property int DefaultBgColor = {read = defaultBgColor, write = defaultBgColor};

  // Reports back the header's # lines so we can position the cursor on
  // the appropriare line for the previous view
  __property int LinesInHeader = {read = linesInHeader};

  // Ran into a max of 7 colors for PalTalk so set this to MaxColors
  // (-1 is no-limit)
  __property int MaxColors = {read = maxColors, write = maxColors};
  __property int TabTwips = {read = tabTwips, write = tabTwips};
};
//---------------------------------------------------------------------------
#endif
