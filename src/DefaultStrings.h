//---------------------------------------------------------------------------
#ifndef DefaultStringsH
#define DefaultStringsH
//---------------------------------------------------------------------------
// NOTE: don't use TCHAR because we want these arrays to be UTF-8 and
// C++ in this version soes NOT have a char8_t type! HTMLCODES and HTMLCHARS
// are wchar_t. ALSO: we want to keep the definition of TCHAR in Options as
// wchar_t, not char - if we change it to char, all of the calls wo microsoft
// Win32 functions stop using the "W" appended letter which messes things up...
// In summary, what a pain in the pizzerinctum!

#define TOTALEDITPOPUPMENU1 12 //EDITPOPUPMENU1
extern const char* EDITPOPUPMENU1[];
#define TOTALEDITMSG 35 //EDITMSG
extern const char* EDITMSG[];
#define TOTALPLAYMSG 44 //PLAYMSG
extern const char* PLAYMSG[];
#define TOTALSTRINGS 266 //DS
extern const char* DS[];
#define TOTAL_KEY_STRINGS 20 //Key-Processing messages
extern const char* KEYSTRINGS[];
#define FONTITEMS 99 //DlgFont
extern const char* FONTS[];

extern const char* FN[]; // file-names, etc.
extern const char* MAINMENU[]; // Can add what we need...
extern const char* MAINPOPUPMENU[];
extern const char* WINGEDITMAINMENU[];
extern const char* EDITPOPUPMENU2[];
extern const char* REPLACEDLGPOPUPMENU[];
extern const char* REPLACEDLGMAINMENU[];
extern const char* REPLACEMSG[];
extern const char* SPELLINGMSG[];
extern const char* WEBEXPORTMSG[];
extern const char* STATUS[];
extern const char* COLORCOPYPASTEMENU[];
extern const char* CLIENTS[];
extern const char* VIEWS[];
extern const char* TABITEMS[];
extern const char* BLENDDIRECTIONITEMS[];
extern const char* BLENDSCOPEITEMS[];
extern const char* FONTS[];
extern const char* COLORDIALOGMSG[];
extern const char* FAVCOLORSMAINMENU[];
extern const char* FAVCOLORSDLG[];
extern const char* STRIPDLG[];

extern const wchar_t* HTMLCODES[];

// This one does not have char * (!!!)
extern const wchar_t* HTMLCHARS;
#endif
