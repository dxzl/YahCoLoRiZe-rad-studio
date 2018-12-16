//---------------------------------------------------------------------------
#ifndef PlayH
#define PlayH

#include <Classes.hpp>

#define NUMBER_OF_PLUGINS_IN_INI_FILE 5

typedef long (*tColorStart)(LPTSTR Service, LPTSTR Channel, LPTSTR Filename, int PlayTime, bool bUseFile);
typedef long (*tColor)(void);

class TPlay
{
  private:
    bool __fastcall SendYahELite(AnsiString S);
    bool __fastcall SendColorizeNet(AnsiString sPlayChan, AnsiString S, UINT winMsg);
    void __fastcall SendPaltalk(AnsiString sPlayChan, AnsiString S); // UTF-8 strings

    String Class, Name;
    int lineWidthInChars, lineWidthInBytes;

    TDTSColor* dts; // easy pointer to main form...

  public:
    __fastcall TPlay(TComponent* Owner); // constructor

    bool __fastcall SendToClient(bool bFindLineLength);
    void __fastcall SendStringToClient(AnsiString sPlayChan, AnsiString S);
    void __fastcall SetClassAndWindName(void);
    void __fastcall SendDDE(AnsiString client, AnsiString command, AnsiString data);
    tColor __fastcall LoadDLL(HINSTANCE &handle, AnsiString entry);
    tColorStart __fastcall LoadDll(HINSTANCE &handle);
    HWND __fastcall FindClient(HWND h);
    void __fastcall DoStopPlay(void);
    void __fastcall DoPausePlay(void);
    void __fastcall DoResumePlay(void);

  __property int LineWidthInChars = {read = lineWidthInChars};
  __property int LineWidthInBytes = {read = lineWidthInBytes};
};
//---------------------------------------------------------------------------
extern TPlay* Play;
#endif
