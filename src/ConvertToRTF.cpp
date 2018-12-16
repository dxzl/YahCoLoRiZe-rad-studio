//---------------------------------------------------------------------------
// YahCoLoRiZe - Edit, process and send colorized text into IRC chat-rooms
// via various chat-clients such as mIRC, IceCHat and LeafCHat
//
// Author: Scott Swift
//
// Released to GitHub under GPL v3 October, 2016
//
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Controls.hpp>
#include "Main.h"
// Include header files common to all files before this directive!
#pragma hdrstop

#include <stdio.h>

#include "ConvertToRTF.h"
#include "Utils.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TConvertToRTF::TConvertToRTF(TComponent* Owner, TYcEdit* re,
                                                             bool bShowStatus)
// We always show progress and allow the ESC key to abort, set
// bShowStatus just determins if the status-message panel pops up.
{
  this->dts = static_cast<TDTSColor*>(Owner);
  this->re = re;

  // set property to use for deftabXXX twips
  // default is 720 - 20 twips per point
  if (this->re)
    this->tabTwips = re->TabWidthTwips; // use passed-in control
  else
    this->tabTwips = tae->TabWidthTwips; // use main edit-control

  this->bShowStatus = bShowStatus;

  // These can be changed prior to execute via their properties!
  // (Set to NO_COLOR and it won't be written to the color table)
  defaultFgColor = dts->Afg;
  defaultBgColor = dts->Abg;

  bStripFontType = false;
  bStripFontSize = false;

  maxColors = -1; // no limit on # colors

  linesInHeader = 0; // LinesInHeader property (read only)
}
//---------------------------------------------------------------------------
__fastcall TConvertToRTF::~TConvertToRTF(void)
{
}
//---------------------------------------------------------------------------
TMemoryStream* __fastcall TConvertToRTF::Execute(TMemoryStream* msOrig, int &retVal)
// In this case, ms would have UTF-16 (wchar_t) in it!
{
  int msOrigSize = msOrig->Size;

  if (!msOrig || !msOrigSize)
  {
    retVal = 2; // Bad input parms
    return NULL;
  }

  wchar_t* buf = NULL;
  TMemoryStream* msNew = NULL;

  try
  {
    buf = utils->StreamToBufferW(msOrig);

    if (buf)
      msNew = Execute(buf, msOrigSize, retVal);
  }
  __finally
  {
    if (buf)
      delete [] buf;
  }

  return msNew;
}
//---------------------------------------------------------------------------
TMemoryStream* __fastcall TConvertToRTF::Execute(TStringsW* sl, int &retVal)
{
  if (sl == NULL || sl->Count == 0)
  {
    retVal = 2; // Bad input parms
    return NULL;
  }

  wchar_t* buf = NULL;
  TMemoryStream* msNew = NULL;

  try
  {
    if ((buf = sl->GetTextBuf()) != NULL)
      msNew = Execute(buf, sl->TotalLength, retVal);
  }
  __finally
  {
    if (buf)
      delete [] buf;
  }

  return msNew;
}
//---------------------------------------------------------------------------
TMemoryStream* __fastcall TConvertToRTF::Execute(WideString S, int &retVal)
{
  return Execute(S.c_bstr(), S.Length(), retVal);
}
//---------------------------------------------------------------------------
TMemoryStream* __fastcall TConvertToRTF::Execute(wchar_t* buf,
                                                int size, int &retVal)
// retVal returns 1 if user abort or 0 if no errors. Returns a memory-stream
// of RTF converted text. buf has the IRC-coded text we need to render.
{
  if (buf == NULL || size == 0)
  {
    retVal = 30;
    return NULL;
  }

  TMemoryStream* msNew = NULL;
  TStringsW* slRtfFonts = NULL;

  try
  {
    utils->PageBreaksToFormFeed(buf, size); // convert <<page>> to C_FF

    slRtfFonts = CreateFontList(buf, size);
    msNew = new TMemoryStream();

    try
    {
      // Stream to write the output RTF text
      if (msNew && slRtfFonts)
        retVal = this->Convert(buf, size, msNew, slRtfFonts);
      else
        retVal = 31;
    }
    catch(...)
    {
#if DEBUG_ON
      dts->CWrite("\r\nException in TConvertToRTF::Execute()\r\n");
#endif

      retVal = 32; // Error
    }
  }
  __finally
  {
    if (slRtfFonts)
      delete slRtfFonts;
  }

  // no errors, return the stream's pointer
  if (retVal == 0)
    return msNew;

  // Error
  if (msNew)
    try { delete msNew; } catch(...) {}

  return NULL;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::Convert(wchar_t* buf, int size,
                TMemoryStream* msOut, TStringsW* slRtfFonts)
// returns an error-code or 1 if user abort... zero if no problems
// (uses TMemoryStream msOut for output...)
{
  if (!buf || !slRtfFonts || size == 0)
    return 20;

  // Change processing status panel... Processing to RTF format
  // (set CpMaxValue 1 until we know iSize and set it below)
  if (this->bShowStatus)
    dts->CpShow(STATUS[2], DS[70]);

  int retVal;

  try
  {
    try
    {
      AnsiString sLine, sOut;
      PUSHSTRUCT State;

      // NOTE: PosRgbList has ONLY positive rrggbb binary numbers in it
      // Our own code keeps out dublicates...
      TList* PosRgbList = new TList();

      // We won't alloc new memory unless we go over 256 colors
      PosRgbList->Capacity = 256;

      // Add a default fg color of black if no colors before first
      // non-space char
      PUSHSTRUCT ps;
      if (utils->SetStateFlags(buf, size, 0, ps, true) >= 0)
        if (ps.fg == NO_COLOR)
          AddOrResolveColor(IRCBLACK, PosRgbList);

      wchar_t c;

      while (GetAsyncKeyState(VK_ESCAPE) < 0); // Dummy read to clear...

      dts->CpSetMaxIterations(size); // set progress bar max iterator

      // Convert
      for (int ii = 0; ii < size; ii++)
      {
        // User abort???
        Application->ProcessMessages();

        if ((int)GetAsyncKeyState(VK_ESCAPE) < 0)
          break; // Cancel

        // Advance progress-bar
        dts->CpUpdate(ii);

        if ((c = buf[ii]) == C_NULL)
          break;

        // To insert special chars use "\\'xx" where xx is two hex digits

        // Three weird escapes...
        if (c == '\\')
          sLine += "\\\\";
        else if (c == '{')
          sLine += "\\{";
        else if (c == '}')
          sLine += "\\}";
        else if (c == C_TAB) // tab
          sLine += "\\tab ";
        else if (c == CTRL_O)
        {
          State.bBold = false;
          State.bUnderline = false;
          State.bItalics = false;

          sLine += "\\plain ";
        }
        else if (c == CTRL_B)
        {
          State.bBold = !State.bBold;

          if (State.bBold)
            sLine += "\\b ";
          else
            sLine += "\\b0 ";
        }
        else if (c == CTRL_U)
        {
          State.bUnderline = !State.bUnderline;

          if (State.bUnderline)
            sLine += "\\ul ";
          else
            sLine += "\\ulnone ";
        }
        else if (c == CTRL_R)
        {
          State.bItalics = !State.bItalics;

          if (State.bItalics)
            sLine += "\\i ";
          else
            sLine += "\\i0 ";
        }
        else if (c == CTRL_PUSH)
        {
          State.pushCounter++;
          sLine += "{\\strike "; // Start group and strikethrough
        }
        else if (c == CTRL_POP)
        {
          State.pushCounter--;
          sLine += "}"; // End group and strikethrough
        }
        else if (c == CTRL_F)
        {
          int ft = utils->CountFontSequence(buf, ii, size);

          if (ft >= 0)
          {
            ii += 2; // skip over 2-digit decimal font-size code

            if (ft == 0)
              State.fontType = dts->cType;
            else
              State.fontType = ft;

            WideString sFont = utils->GetLocalFontString(State.fontType);

            int idx;

            if (!sFont.IsEmpty() && slRtfFonts->Count > 0)
              idx = slRtfFonts->IndexOf(sFont);
            else
              idx = 0;

            if (idx < 0)
              idx = 0; // use default font if lookup failed

            if (!bStripFontType)
            {
              sLine += "\\f" + AnsiString(idx);

              // Write space if next char is not a color or font-size or newline
              if (ii+1 < size && !utils->AnySpecialChar(buf[ii+1]))
                sLine += " ";
            }
          }
        }
        else if (c == CTRL_S)
        {
          int fs = utils->CountFontSequence(buf, ii, size);

          if (fs >= 0)
          {
            ii += 2; // skip over 2-digit decimal font-size code

            if (fs == 0)
              State.fontSize = dts->cSize;
            else
              State.fontSize = fs;

            if (!bStripFontSize)
            {
              sLine += "\\fs" + AnsiString(State.fontSize*2);

              // Write space if next char is not a color or font-size or newline
              if (ii+1 < size && !utils->AnySpecialChar(buf[ii+1]))
                sLine += " ";
            }
          }
        }
        else if (c == CTRL_K)
        {
          int fg = NO_COLOR;
          int bg = NO_COLOR;
          int idx;

          ii += utils->CountColorSequence(buf, ii, size, fg, bg);

          bool bFgNoColor = (fg == NO_COLOR || fg == IRCNOCOLOR);
          bool bBgNoColor = (bg == NO_COLOR || bg == IRCNOCOLOR);

          if (bFgNoColor && bBgNoColor)
          {
            // Turn off colors or go to previous colors
          }
          else
          {
            if (!bFgNoColor)
            {
              idx = this->AddOrResolveColor(fg, PosRgbList);
              sLine += "\\cf" + AnsiString(idx+1);

              // Write space if next char is not a color or font-size or newline
              // and we aren't going to write a bg color...
              if (bBgNoColor && ii+1 < size &&
                                       !utils->AnySpecialChar(buf[ii+1]))
                sLine += " ";
            }

            if (!bBgNoColor)
            {
              idx = this->AddOrResolveColor(bg, PosRgbList);
              sLine += "\\highlight" + AnsiString(idx+1);

              // Write space if next char is not a color or font-size or newline
              if (ii+1 < size && !utils->AnySpecialChar(buf[ii+1]))
                sLine += " ";
            }
          }
        }
        else if (c == C_FF || utils->FoundCRLF(buf, size, ii)) // may increment ii
        {
          sLine = TerminateLine(sLine, State);

          if (c == C_FF) // form-feed (page-break)
          {
            // skip first cr/lf immediately following a page-break
            int idx = ii+1;
            if (utils->FoundCRLF(buf, size, idx))
              ii = idx;

            // Problem I am having: If the user wants to delete a page-break
            // while in the RTF view - there literally is "nothing to delete"
            // because the RTF DLL simpley ignores the \page...
            //
            // We don't want to insert a "real-character" into the document
            //sLine += "\\page\\u9810P";
            //sLine += "\\pard\\page\\par";

            // PAGE_BREAK will need encoding as "\\u65510?", Etc if it has any
            // unicode chars in-future!!!!!!!!

            // have to add extra leading escape to avoid taking it as RTF!
  //          sLine += "\\" + AnsiString(PAGE_BREAK) + "\\par";
  //          sLine += "{\\cf0\\highlight0" + AnsiString(dts->cSize) +
  //                                  " \\\\pagebreak\\par}";

  // one problem now is that TUtils::PageBreaksToRtf() will replace
  // \pagebreak with \page - the \page has a "built-in" \par
  // so we end up printing one line down from the top of the second page!
            sLine += "\\cf0\\highlight0 \\\\pagebreak\\par";

          }
          else
            sLine += "\\par"; // (note: inserting a \line puts in a C_VT char)

          sLine += CRLF;
          sOut += sLine;
          sLine = "";

          State.Clear();
        }
        else if (!utils->AnySpecialChar(c))
        {
          //  if (c == '@')
          //    sLine += "\\u65510?"; // test!!!!!!!!! "wan sign"
          if (c < 256)
            sLine += (char)c;
  //        if (c < 128)
  //          sLine += (char)c;
  //        else if (c < 256)
  //          sLine += "\\'" + IntToHex((int)c, 2); // hex format for 128-255
          else // NOTE: for unicode to work you need the \uc1 keyword (\ud possibly...)!
            sLine += "\\u" + AnsiString((unsigned int)c) + "?"; // ? is used if the char can't be decoded
        }
      }

      // if we exit without a cr/lf, write the last string to sOut
      if (!sLine.IsEmpty())
        sOut += TerminateLine(sLine, State) + "\\par" + CRLF;

      // Convert Colors to an RTF table
      AnsiString ColorTable = this->RgbToRtfColorTable(PosRgbList);
      delete PosRgbList;

      AnsiString FontTable = this->GetFontTable(slRtfFonts);

      // Write to gMs Memory-Stream
      retVal = this->WriteRtfToStream(sOut, ColorTable, FontTable, msOut);
    }
    catch(...)
    {
  #if DEBUG_ON
      dts->CWrite("\r\nException in TConvertToRTF::Convert()\r\n");
  #endif
      retVal = 21;
    }
  }
  __finally
  {
    dts->CpHide();
  }

  return retVal;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TConvertToRTF::TerminateLine(AnsiString sLine, PUSHSTRUCT State)
{
  // Terminate effect at end of line
  if (State.bBold)
    sLine += "\\b0 ";

  if (State.bUnderline)
    sLine += "\\ulnone ";

  if (State.bItalics)
            sLine += "\\i0 ";

  // Unwind any rogue pushes
  if (State.pushCounter > 0)
    while(State.pushCounter--)
      sLine += "}"; // End group and strikethrough

  // We have to reset highlight at the end of a line or we
  // get a phantom colored space at the end! (Don't use \plain
  // because it also resets the font-size!)
  sLine += RESET_HIGHLIGHT_STR;
  
  return sLine;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::AddOrResolveColor(int C, TList* cList)
// C is a YahCoLoRiZe +/- color. cList is a list of
// POSITIVE RGB colors with size limit this->maxColors (a property)
//
// We return a 0-based index into cList.
{
  C = -utils->YcToRgb(C); // Convert YahCoLoRiZe +/- color to pos RGB

  int idx = this->FindColorIndex(C, cList);

  if (idx < 0) // Not already in the table?
  {
    if (this->maxColors < 0)
      idx = this->AddColor(C, cList); // no limit...
    else
    {
      if (cList->Count >= this->maxColors)
        idx = this->ResolveColor(C, cList);
      else
        idx = this->AddColor(C, cList);
    }
  }

  return idx;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::AddColor(int C, TList* cList)
// Add C to table, returns index of new color. If the max colors
// have been written we return the index of the first color in the table
//
// C must be a positive RGB binary number
{
  if (this->maxColors == -1 || cList->Count < this->maxColors)
  {
    cList->Add((void*)C);
    return cList->Count-1;
  }

  return 0;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::FindColorIndex(int C, TList* cList)
// Returns -1 if color is not in the table, -2 if error.
// Otherwise the 0-based index is returned...
{
  try
  {
    for (int ii = 0; ii < cList->Count; ii++)
      if (C == (int)cList->Items[ii])
        return ii;
  }
  catch(...)
  {
#if DEBUG_ON
    dts->CWrite("\r\nException in TConvertToRTF::FindColorIndex()\r\n");
#endif
    return -2; // Error
  }

  return -1;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::ResolveColor(int C, TList* cList)
// Returns an index into our TList of existing colors that most closely matches
// C. C is a positive RGB value.
{
  int smallestdistance = 500000; // big miscelaneous number
  int idx = 0;

  BlendColor BC = utils->RgbToBlendColor(C);

  BlendColor P;

  int rDiff, gDiff, bDiff, distance;

  for (int ii = 0; ii < cList->Count; ii++)
  {
    P = utils->RgbToBlendColor((int)cList->Items[ii]);

    // Geometric distance
    // largest val (255*255)+(255*255)+(255*255) = 195075
    rDiff = BC.red-P.red;
    gDiff = BC.red-P.red;
    bDiff = BC.red-P.red;

    distance = (rDiff*rDiff) + (gDiff*gDiff) + (bDiff*bDiff);

    if (distance < smallestdistance)
    {
      smallestdistance = distance;
      idx = ii;
    }
  }

  return idx;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TConvertToRTF::RgbToRtfColorTable(TList* cList)
// Returns empty string if error or a new RTF color-table
{
  if (!cList)
    return ""; // Error

  AnsiString sOut = "{\\colortbl;"; // Color 0 is "auto" color!

  // Initialize first two colors to our default colors
  if (cList->Count == 0)
  {
    if (defaultFgColor == NO_COLOR)
      AddOrResolveColor(IRCBLACK, cList);
    else
      AddOrResolveColor(defaultFgColor, cList);
  }

// Causing trouble with paltalk to have a color the same as the bg color?
// let's test and see... the idea here was to have at least one color for the
// FG and one for the BG but Paltalk has no BG colors... or does it?
//
// DO WE NEED THIS FOR ANYTHING?????
//
//  if (Colors->Count == 1)
//  {
//    if (defaultBgColor == NO_COLOR)
//      AddOrResolveColor(IRCWHITE, cList);
//    else
//      AddOrResolveColor(defaultBgColor, cList);
//  }

  try
  {
    int len = cList->Count;
    for (int ii = 0; ii < len; ii++)
    {
      sOut += this->YcToRtfColorString((int)cList->Items[ii]);

      // 4 colors per text-line...
      if ((ii % 4)==0 && ii < len-1)
      {
        sOut += "\r\n";
        linesInHeader++;
      }
    }

    sOut += "}\r\n"; // Terminate
    linesInHeader++;
  }
  catch(...)
  {
#if DEBUG_ON
    dts->CWrite("\r\nException in TConvertToRTF::RgbToRtfColorTable()\r\n");
#endif
    return ""; // Error
  }

  return sOut;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TConvertToRTF::YcToRtfColorString(int C)
// Color is a positive RGB color
{
  BlendColor bc = utils->RgbToBlendColor(C);

  return "\\red" + AnsiString(bc.red) + "\\green" +
                  AnsiString(bc.green) + "\\blue" + AnsiString(bc.blue) + ";";
}
//---------------------------------------------------------------------------
TStringsW* __fastcall TConvertToRTF::CreateFontList(wchar_t* buf, int size)
// returns an error-code or 1 if user abort... zero if no problems
// (uses TStringsW slRtfFonts for output...)
//
// fonttbl{\f0\froman Tms Rmn;}{\f1\fdecor Symbol;}{\f2\fswiss Helv;}}
{
  if (!buf || size == 0)
    return NULL;

  // Show status "Creating font-list..."
  if (this->bShowStatus)
    dts->CpShow(STATUS[12], DS[70]);

  TStringsW* slRtfFonts = NULL;

  try
  {
    try
    {
      slRtfFonts = new TStringsW();

      if (!slRtfFonts)
        return NULL;

      wchar_t c;

      // add the default font
      int idx;

      if (dts->IsCli(C_PALTALK))
        idx = PALTALK_DEF_TYPE;
      else
        idx = USER_DEF_TYPE;

      slRtfFonts->Add(utils->GetLocalFontString(idx));

      // don't need more than the default font for Paltalk...
      if (this->bStripFontType)
        return slRtfFonts;

      AnsiString sFont;

      while (GetAsyncKeyState(VK_ESCAPE) < 0); // Dummy read to clear...

      dts->CpSetMaxIterations(size);

      // Convert
      for (int ii = 0; ii < size; ii++)
      {
        // User abort???
        Application->ProcessMessages();
        if ((int)GetAsyncKeyState(VK_ESCAPE) < 0)
          break; // Cancel

        // Advance progress-bar
        dts->CpUpdate(ii);

        if ((c = buf[ii]) == C_NULL)
          break;

        if (c == CTRL_F)
        {
          int ft = utils->CountFontSequence(buf, ii, size);

          if (ft >= 0)
          {
            ii += 2; // skip over 2-digit decimal font-size code

            if (ft == 0)
            {
              // look up first or second item in FONTS[] (default)
              if (dts->IsCli(C_PALTALK))
                ft = PALTALK_DEF_TYPE;
              else
                ft = USER_DEF_TYPE;
            }

            // Unfortunately, can't do a case-independant search?
            sFont = utils->GetLocalFontString(ft);

            // Add just the font name (from FONTS[]) if not already in the table
            if (!sFont.IsEmpty() && slRtfFonts->IndexOf(sFont) < 0)
              slRtfFonts->Add(sFont);
          }
        }
      }
    }
    catch(...)
    {
      if (slRtfFonts)
      {
        delete slRtfFonts;
        slRtfFonts = NULL;
      }

  #if DEBUG_ON
      dts->CWrite("\r\nException in TConvertToRTF::CreateFontList()\r\n");
  #endif
    }
  }
  __finally
  {
    dts->CpHide();
  }

  return slRtfFonts;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TConvertToRTF::GetFontTable(TStringsW* slRtfFonts)
// Creates an RTF font-table string from slRtfFonts
{
  if (!slRtfFonts)
    return "";

  AnsiString sFont;

  try
  {
    AnsiString sTable = "{\\falt\\fnil Fixedsys;}";

    // Overweite the font stringlist with the table info added...
    // fonttbl{\f0\froman Tms Rmn;}{\f1\fdecor Symbol;}{\f2\fswiss Helv;}}
    int count = slRtfFonts->Count;
    for (int ii = 0; ii < count; ii++)
      // (\fcharset0 is the ANSI charset, 1 is default)
      // (\fprq1 is the fixed-pitch, 2 is variable and 0 is default)
      sTable += "{\\f" + AnsiString(ii) + "\\fnil " +
                  utils->WideToUtf8(slRtfFonts->GetString(ii)) + ";}";

    // NOTES 5/19/2015: in place of \rtf1 use \urtf8 if the documant is in UTF-8.
    // (is \urtf1 UTF-16?)
    // (Rich-Edit 3.0 and up). Prefix an RTF plain-text file with Byte-Order-Mark
    // 0xfeff.

    //ShowMessage(String((int)tae->Font->Charset));
    //ShowMessage(tae->Font->Name);

    // TODO: We should let user choose the default Charset and codepage (name)???????
    sFont = AnsiString("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033\\deftab") +
             AnsiString(tabTwips) + AnsiString("{\\fonttbl") + sTable + AnsiString("}\r\n");
  }
  catch(...)
  {
#if DEBUG_ON
    dts->CWrite("\r\nException in TConvertToRTF::GetFontTable()\r\n");
#endif
  }

  return sFont;
}
//---------------------------------------------------------------------------
int __fastcall TConvertToRTF::WriteRtfToStream(AnsiString sBody, AnsiString ColorTable,
                                    AnsiString FontTable, TMemoryStream* msOut)
// Returns 0 if success
{
  if (!msOut)
  {
#if DEBUG_ON
    dts->CWrite("\r\nms NULL in TConvertToRTF::WriteRtfToStream()\r\n");
#endif
    return 10;
  }

  try
  {
    msOut->Clear();

    msOut->WriteBuffer(FontTable.c_str(), FontTable.Length()); // Preamble

    linesInHeader++;

    msOut->WriteBuffer(ColorTable.c_str(), ColorTable.Length()); // Color table

    //  Temp = "{\\*\\generator YahCoLoRiZe " + AnsiString(REVISION) + ";}";
    //  msOut->WriteBuffer(Temp.c_str(), Temp.Length()); // Generator

    // \ud is "unicode destination", \uc1 is "1-byte" substitute
    // char-code (the '?' in \uCCCCC?)
//    AnsiString sFont = "\\ud\\uc1\\pard\\cf1\\f" + AnsiString(DEF_FONT_NUM) +
//                       "\\fs" + AnsiString(dts->cSize*2) + CRLF;
    AnsiString sFont = "\\uc1\\pard\\cf1\\f" + AnsiString(DEF_FONT_NUM) +
                       "\\fs" + AnsiString(dts->cSize*2) + CRLF;

    msOut->WriteBuffer(sFont.c_str(), sFont.Length()); // Remaining preamble

    linesInHeader++;

    msOut->WriteBuffer(sBody.c_str(), sBody.Length()); // Main body

    // document begins with "{\rtf1..." - terminate it with "}"
    sFont = RESET_HIGHLIGHT_STR + AnsiString("}");
    msOut->WriteBuffer(sFont.c_str(), sFont.Length()); // Terminating }

    msOut->Position = 0; // Reset

    // Diagnostic code!!!!!!!!!!!!!!
    //ShowMessage((String)utils->StreamToStringA(msOut));

    return 0;
  }
  catch(...)
  {
#if DEBUG_ON
    dts->CWrite("\r\nException in TConvertToRTF::WriteRtfToStream()\r\n");
#endif
    return 11; // Must be > 1 (1 is a user-abort)
  }
}
//---------------------------------------------------------------------------
// TODO: use Win32 calls to enumerate fonts and build a better font-table
/*
UINT uAlignPrev;
    int aFontCount[] = { 0, 0, 0 };
    char szCount[8];
        HRESULT hr;
        size_t * pcch;

    EnumFontFamilies(hdc, (LPCTSTR) NULL,
        (FONTENUMPROC) EnumFamCallBack, (LPARAM) aFontCount);

    uAlignPrev = SetTextAlign(hdc, TA_UPDATECP);

    MoveToEx(hdc, 10, 50, (LPPOINT)NULL);
    TextOut(hdc, 0, 0, "Number of raster fonts: ", 24);
    itoa(aFontCount[0], szCount, 10);

        hr = AnsiStringCchLength(szCount, 9, pcch);
        if (FAILED(hr))
        {
        // TODO: write error handler
        }
    TextOut(hdc, 0, 0, szCount, *pcch);

    MoveToEx(hdc, 10, 75, (LPPOINT)NULL);
    TextOut(hdc, 0, 0, "Number of vector fonts: ", 24);
    itoa(aFontCount[1], szCount, 10);
        hr = AnsiStringCchLength(szCount, 9, pcch);
        if (FAILED(hr))
        {
        // TODO: write error handler 
        }
    TextOut(hdc, 0, 0, szCount, *pcch); 

    MoveToEx(hdc, 10, 100, (LPPOINT)NULL); 
    TextOut(hdc, 0, 0, "Number of TrueType fonts: ", 26); 
    itoa(aFontCount[2], szCount, 10);
        hr = AnsiStringCchLength(szCount, 9, pcch);
        if (FAILED(hr))
        {
        // TODO: write error handler 
        }
    TextOut(hdc, 0, 0, szCount, *pcch); 
 
    SetTextAlign(hdc, uAlignPrev); 
 
BOOL CALLBACK EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID aFontCount) 
{ 
    int far * aiFontCount = (int far *) aFontCount; 
 
    // Record the number of raster, TrueType, and vector  
    // fonts in the font-count array.  
 
    if (FontType & RASTER_FONTTYPE) 
        aiFontCount[0]++; 
    else if (FontType & TRUETYPE_FONTTYPE) 
        aiFontCount[2]++; 
    else 
        aiFontCount[1]++; 
 
    if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2]) 
        return TRUE; 
    else 
        return FALSE; 
 
    UNREFERENCED_PARAMETER( lplf ); 
    UNREFERENCED_PARAMETER( lpntm ); 
} 
*/

