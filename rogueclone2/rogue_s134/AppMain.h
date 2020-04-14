#ifndef APPMAIN_H
#define APPMAIN_H

/**************************************************************************
	ä÷êîêÈåæ
**************************************************************************/

short getcharWIN();
void  charreset ();
int read_mesg(char *strmesg);

int loadINI();
int saveINI();

LRESULT	CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
BOOL	CALLBACK DlgProc_Version(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp);
BOOL	CALLBACK DlgProc_Config (HWND hDlg,UINT msg,WPARAM wp,LPARAM lp);

/*************************************************************************/

void SetClientWindow(HWND hw,DWORD s,int w,int h);
void SetCenterWindow(HWND hw);
int OpenFileDialog(HWND hWnd,char *szFile,char *szDir,char *szTitle,char *Filter);
int SaveFileDialog(HWND hWnd,char *strFile,char *strDir,char *strTitle,char *Filter,char *strExt);

/*************************************************************************/

#endif

