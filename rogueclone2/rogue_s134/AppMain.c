#include "rogue.h"
#include "resource.h"

/**************************************************************************
	変数定義
**************************************************************************/

extern short party_room;
#ifndef ORIGINAL
extern char *nick_name;
char mesg[507][80];	/* for separation */
#endif

extern short buffer[DROWS][DCOLS];
extern boolean score_only;

extern char *rest_file;
extern boolean do_restore;

extern char login_name[30];

/* windows */
HWND hWnd;
HINSTANCE hInst;
const char strClassName[]="ROGUE_CLONE_II_WIN"; // ウィンドウクラス名
const char strAppName[]="Rogue Clone II ver 1.3.4"; // アプリケーション名

char strini[256];
char strmesg[256];
char strdefname[30];

char strfile[256];
short ch = 0;
int reset = 0;

LOGFONT lf;
int iPoint;

/**************************************************************************
	WinMain
**************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wcx;
/*	HDC hDC;
	HFONT hFont,hFontOld;
*/

#ifndef ORIGINAL
	int first = 1;
	char buf[80];
#endif

	int ret = 0;
	char str[256];

	hInst=hInstance;

	/* Font 作成 */
/*	hDC=GetDC(hWnd);
	hFont=CreateFont(16,8,0,0,0,0,0,0,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,"ＭＳ明朝");
	hFontOld=(HFONT)SelectObject(hDC,hFont);
*/
	/* 二重起動防止 */
	if(FindWindow(strClassName,0)){ MessageBox(0,"もうすでに起動しています！",strAppName,0); return 0;}

	/* ウィンドウクラスの登録 */
	wcx.cbSize			= sizeof(WNDCLASSEX);
	wcx.style			= 0;/*CS_DBLCLKS;*/
	wcx.lpfnWndProc		= WinProc;
	wcx.cbClsExtra		= 0;
	wcx.cbWndExtra		= 0;
	wcx.hInstance		= hInst;
	wcx.hIcon			= (HICON)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	wcx.hIconSm			= (HICON)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	wcx.hCursor			= (HCURSOR)LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH); // 太字対応
	wcx.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU);
	wcx.lpszClassName	= strClassName;
	if(!RegisterClassEx(&wcx)){ MessageBox(NULL,"ウィンドウクラスの登録に失敗しました！",0,0); return 0;} // クラス登録

	/* ウィンドウ作成 */
	hWnd=CreateWindowEx(WS_EX_ACCEPTFILES,strClassName,strAppName,WS_POPUP|WS_SYSMENU|WS_MINIMIZEBOX|WS_CAPTION,0,0,0,0,HWND_DESKTOP,NULL,hInst,0);
	if(!hWnd){ MessageBox(NULL,"ウィンドウの作成に失敗しました！",0,0); return 0;}

	/* INI読み込み */
	GetModuleFileName(NULL,strini,MAX_PATH);
	strini[strlen(strini)-3]=0; strcat(strini,"INI");
	loadINI();
	ShowWindow(hWnd,nCmdShow);

	/* Rogue 初期化 */
	memset(buffer,0x20,DROWS*DCOLS*sizeof(short));

	/* mesg file */
/*	strcpy(strmesg,"mesg");*/
	if((ret = read_mesg(strmesg)) != 0)
	{
		if(ret < 0)
		{
			sprintf(str,"%s が開けません！",strmesg);
			MessageBox(hWnd,str,0,0);
		}
		else if(ret > 0)
		{
			sprintf(str,"%s の %d 番目にエラーがあります！",strmesg,ret);
			MessageBox(hWnd,str,0,0);
		}

		MessageBox(hWnd,"代わりに mesg.msg を開きます",strAppName,0);
		if((ret = read_mesg("mesg.msg")) != 0)
		{
			MessageBox(hWnd,"またエラーです。終了します。",0,0);
			return 1;
		}
	}

	/* s: score r: restore */
/*	if (init(argc - 1, argv + 1))*/
	if(init(0,""))
	{	/* restored game */
		goto PL;
	}

LOAD:

	if(reset) /* load file */
	{
		restore(rest_file);
		reset = 0;
		goto PL;
	}

	for (;;)
	{
		clear_level();
		make_level();
		put_objects();
		put_stairs();
		add_traps();
		put_mons();
		put_player(party_room);
		print_stats(STAT_ALL);
#ifndef ORIGINAL
		if (first)
		{
			sprintf(buf,mesg[10],nick_name);
			message(buf, 0);
		}
	PL:
		first = 0;
#else
	PL:
#endif
		play_level(); /* 無限ループ */
		free_stuff(&level_objects);
		free_stuff(&level_monsters);

		if(reset) goto LOAD; /* windows */
	}

	return 0; /* msg.wParam; */
}

WPARAM MessageLoop()
{
	MSG msg;
	HACCEL hAccel=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));

	/* メッセージを処理 */
	if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		TranslateAccelerator(hWnd,hAccel,&msg);

		if(!GetMessage(&msg,NULL,0,0)) return -1; // WM_QUITなら終了
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else WaitMessage();

	DestroyAcceleratorTable(hAccel);
	return msg.wParam;
}

short getcharWIN(){	return ch;}
void  charreset (){	ch = 0;}

/*************************************************************************/

/* メッセージファイルを読み込む
-：ファイルがないとかで開けない。
0：正常
+：その番号にエラーが。
 */
int read_mesg(char *strmesg)
{
	FILE *mesg_file;
	char buf[256];
	int i, n, s, e;

	if ((mesg_file = fopen(strmesg, "r")) == NULL) return -1;

	while (fgets(buf, 256, mesg_file) != NULL) /* 一行の最大文字数は 256byte まで */
	{
		if ((n = atoi(buf)) > 0 && n < 500)
		{
			/* " を探す */
			for (i = 0; i < 256 && buf[i] && buf[i] != '\"'; ++i);

			if(i< 256 && buf[i]) s = i + 1;
			else return n;

			/* " を探す */
			for (i = s; i < 256 && buf[i] && buf[i] != '\"'; ++i); 

			if(i< 256 && buf[i]) e = i - 1;
			else return n;

			/* メッセージ読み出し */
			for (i = 0; i < e-s+1 && i < 79; ++i) mesg[n][i] = buf[s + i];
			mesg[n][i] = '\0';
		}
	}
	return 0;
}

int loadINI()
{
	HDC hDC;
	TEXTMETRIC tm;
	HFONT hFont,hFontOld;
	int x,y;

	RECT rcWin;
	rcWin.left	= CW_USEDEFAULT;
	rcWin.top	= CW_USEDEFAULT;
	rcWin.right	= 640;
	rcWin.bottom= 480;

	iPoint = 12;
	hDC = GetDC(hWnd);
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(iPoint, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = SHIFTJIS_CHARSET;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(lf.lfFaceName,"ＭＳ ゴシック");
	ReleaseDC(hWnd,hDC);

	// Get Ini Data
	GetPrivateProfileStruct("CONFIG","RECT_WIN",&rcWin,sizeof(RECT),strini);
	GetPrivateProfileStruct("CONFIG","FONT",&lf,sizeof(LOGFONT),strini);

	GetPrivateProfileString("CONFIG","MESG","mesg.msg",strmesg,256,strini);
	GetPrivateProfileString("CONFIG","NAME","戦士",strdefname,30,strini);

	// Get Font Size
	hDC = GetDC(hWnd);
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hDC,hFont);

	GetTextMetrics(hDC,&tm);
	x = tm.tmAveCharWidth;
	y = tm.tmHeight;

	SelectObject(hDC,hFontOld);
	DeleteObject(hFont);
	DeleteObject(hFontOld);
	ReleaseDC(hWnd,hDC);

	// Window Size
	if(rcWin.left == CW_USEDEFAULT) /* 初回のみ中央 */
	{
		SetClientWindow(hWnd,WS_POPUP|WS_SYSMENU|WS_MINIMIZEBOX|WS_CAPTION,x*DCOLS,y*DROWS);
		SetCenterWindow(hWnd,0);
	}
	else
	{
		SetClientWindow(hWnd,WS_POPUP|WS_SYSMENU|WS_MINIMIZEBOX|WS_CAPTION,x*DCOLS,y*DROWS);
		SetWindowPos(hWnd,HWND_TOP,rcWin.left,rcWin.top,0,0,SWP_NOSIZE);
	}

	return 0;
}

int saveINI()
{
	if(!IsZoomed(hWnd)&&!IsIconic(hWnd))
	{
		RECT rcWin;
		GetWindowRect(hWnd,&rcWin);
		WritePrivateProfileStruct("CONFIG","RECT_WIN",&rcWin,sizeof(RECT),strini);
	}

	WritePrivateProfileStruct("CONFIG","FONT",&lf,sizeof(LOGFONT),strini);

	WritePrivateProfileString("CONFIG","MESG",strmesg,strini);
	WritePrivateProfileString("CONFIG","NAME",strdefname,strini);

	return 0;
}

/**************************************************************************
	Procedure
**************************************************************************/

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	short bufferb[DROWS][DCOLS];

	HDC hDC;
	PAINTSTRUCT ps;

	switch(msg)
	{
/**** コマンド系 ****/
	case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
			case IDM_OPEN:
				if(MessageBox(hWnd,"今のゲームはセーブされませんがよろしいですか？","確認",MB_YESNO) == IDNO) break;
				if(OpenFileDialog(hWnd,strfile,"","セーブデータを開く","Save Data(*.sav)\0*.sav\0All Files(*.*)\0*.*\0")) return 1;

				do_restore = 1;
				rest_file = strfile;
//				restore(rest_file);
				reset = 1;
				break;
			case IDM_SAVE:
				save_game();
				break;
			case IDM_SCORE:
				memcpy(bufferb,buffer,DROWS*DCOLS*sizeof(short));

				score_only = 1;
				message("", 0);
				put_scores(0,0);
				score_only = 0;

				memcpy(buffer,bufferb,DROWS*DCOLS*sizeof(short));
				InvalidateRect(hWnd,0,1);
				break;

			case IDM_FONTDIALOG:
				{
					CHOOSEFONT cf;
					ZeroMemory(&cf, sizeof(CHOOSEFONT));
					cf.lStructSize = sizeof(CHOOSEFONT);
					cf.hwndOwner = hWnd;
					cf.lpLogFont = &lf;
					cf.iPointSize = iPoint;
					cf.hInstance = hInst;
					cf.Flags = CF_SCREENFONTS | CF_FIXEDPITCHONLY | CF_INITTOLOGFONTSTRUCT | CF_SELECTSCRIPT;

					if(ChooseFont(&cf))
					{
						TEXTMETRIC tm;
						HFONT hFont,hFontOld;
						int x,y;

						iPoint = cf.iPointSize / 10;

						// Get Font Size
						hDC = GetDC(hWnd);
						hFont = CreateFontIndirect(&lf);
						hFontOld = (HFONT)SelectObject(hDC,hFont);

						GetTextMetrics(hDC,&tm);
						x = tm.tmAveCharWidth;
						y = tm.tmHeight;

						SelectObject(hDC,hFontOld);
						DeleteObject(hFont);
						DeleteObject(hFontOld);
						ReleaseDC(hWnd,hDC);

						// Window Size
						SetClientWindow(hWnd,WS_POPUP|WS_SYSMENU|WS_MINIMIZEBOX|WS_CAPTION,x*DCOLS,y*DROWS);
//						SetWindowPos(hWnd,HWND_TOP,rcWin.left,rcWin.top,0,0,SWP_NOSIZE);

						InvalidateRect(hWnd,0,1);
					}
				}break;

			case IDM_CONFIG:	DialogBox(hInst,MAKEINTRESOURCE(IDD_CONFIG ),hWnd,(DLGPROC)DlgProc_Config ); break;
			case IDM_VERSION:	DialogBox(hInst,MAKEINTRESOURCE(IDD_VERSION),hWnd,(DLGPROC)DlgProc_Version); break;
			case IDM_QUIT:		PostMessage(hWnd,WM_CLOSE,0,0); break;
			default: break;
			}
		}break;

/**** 入力系 ****/
	case WM_KEYDOWN:
		switch(wp)
		{
		case VK_LEFT:	ch='h'; break;
		case VK_RIGHT:	ch='l'; break;
		case VK_UP:		ch='k'; break;
		case VK_DOWN:	ch='j'; break;

		case VK_NUMPAD1: ch='b'; break;
		case VK_NUMPAD2: ch='j'; break;
		case VK_NUMPAD3: ch='n'; break;
		case VK_NUMPAD4: ch='h'; break;
		case VK_NUMPAD6: ch='k'; break;
		case VK_NUMPAD7: ch='y'; break;
		case VK_NUMPAD8: ch='k'; break;
		case VK_NUMPAD9: ch='u'; break;
		}
		break;
	case WM_CHAR:
		switch(wp)
		{
/*		case '1': ch='b'; break;
		case '2': ch='j'; break;
		case '3': ch='n'; break;
		case '4': ch='h'; break;
		case '6': ch='l'; break;
		case '7': ch='y'; break;
		case '8': ch='k'; break;
		case '9': ch='u'; break;
*/		default:  ch=wp; break;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			ch=' ';
		}break;

/**** 描画系 ****/
	case WM_PAINT:
		{
			hDC=BeginPaint(hWnd,&ps);

			refreshw();

			EndPaint(hWnd,&ps);
		}break;

/**** 作成・破棄系 ****/
	case WM_CLOSE:
		{
			if(MessageBox(hWnd,"セーブせずに終了しますか？","Rogue",MB_YESNO) == IDNO) return 1;
			killed_by((object*)0,QUIT); /* 逃亡 */
		}break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			exit(0);
		}break;

	default: return DefWindowProc(hWnd,msg,wp,lp);
	}
	return 0;
}

BOOL CALLBACK DlgProc_Version(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp)
{
	static HFONT hFontLink=0;
	static HCURSOR hCurLink=0;
	static unsigned char bFloat=0;

	LOGFONT lf;
	char str[256];

	switch(msg)
	{
	case WM_INITDIALOG:{
		SetCenterWindow(hDlg,hWnd);
		SetFocus(GetDlgItem(hDlg,IDOK));
		SetWindowText(GetDlgItem(hDlg,IDC_VERTITLE),strAppName);

		/* Create Link Resource */
		memset(&lf,0,sizeof(LOGFONT));
		lf.lfHeight			= 12;
		lf.lfWeight			= 600;
		lf.lfItalic			= TRUE;
		lf.lfUnderline		= TRUE;
		lf.lfCharSet		= DEFAULT_CHARSET;
		strcpy(lf.lfFaceName,"Verdana");
		hFontLink=CreateFontIndirect(&lf);
		if(hFontLink) SendMessage(GetDlgItem(hDlg,IDCANCEL),WM_SETFONT,(WPARAM)hFontLink,MAKELPARAM(TRUE,0));

		hCurLink=LoadCursor(NULL,MAKEINTRESOURCE(32649)); // IDC_HAND or IDC_HELP
		if(!hCurLink) hCurLink=LoadCursor(NULL,IDC_HELP);

		}break;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDC_HOMEPAGE:
		case IDC_EMAIL:
			GetDlgItemText(hDlg,LOWORD(wp),str,256);
			ShellExecute(hDlg,NULL,str,NULL,NULL,SW_SHOWNORMAL);
			break;

		case IDCANCEL:
			DeleteObject(hFontLink); hFontLink=0;
			DestroyCursor(hCurLink); hCurLink=0;
			bFloat=0;
			EndDialog(hDlg,FALSE);
			break;
		default: return FALSE;
		}
		break;
	case WM_SETCURSOR:
		{
			if( (HWND)wp == GetDlgItem(hDlg,IDC_HOMEPAGE)
			  ||(HWND)wp == GetDlgItem(hDlg,IDC_EMAIL)
			  ||(HWND)wp == GetDlgItem(hDlg,IDCANCEL ) )
			{
				/* カーソル設定 */
				SetCursor(hCurLink);
				SetWindowLong(hDlg,DWL_MSGRESULT,MAKELONG(TRUE,0));

				/* フローティング開始 */
				if(!bFloat)
				{
					bFloat=1;
					InvalidateRect((HWND)wp,NULL,FALSE);
				}
				break;
			}

			/* フローティング解除 */
			if(bFloat)
			{
				bFloat=0;
				InvalidateRect(GetDlgItem(hDlg,IDC_HOMEPAGE	),NULL,FALSE);
				InvalidateRect(GetDlgItem(hDlg,IDC_EMAIL	),NULL,FALSE);
				InvalidateRect(GetDlgItem(hDlg,IDCANCEL		),NULL,FALSE);
				PostMessage(hDlg,WM_MENUSELECT,(WPARAM)MAKELONG(0,0xFFFF),0);
			}

			return FALSE;
		}break;
	case WM_CTLCOLORSTATIC:
		{
			HDC hDC = (HDC)wp;
			if( (HWND)lp == GetDlgItem(hDlg,IDC_HOMEPAGE)
			  ||(HWND)lp == GetDlgItem(hDlg,IDC_EMAIL) )
			{
				if(hFontLink) SelectObject(hDC,hFontLink);
				SetTextColor(hDC,(bFloat)?0x000000FF:0x00FF8800);
				SetBkMode(hDC,TRANSPARENT);
				return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
			}
			if( (HWND)lp == GetDlgItem(hDlg,IDC_USERDATA) ) return (BOOL)(HBRUSH)GetStockObject(WHITE_BRUSH);
			return (BOOL)(HBRUSH)(COLOR_BTNFACE + 1);
		}break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK DlgProc_Name(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp) 
{
	switch(msg)
	{
	case WM_INITDIALOG:
		SetCenterWindow(hDlg,hWnd);
		SetDlgItemText(hDlg,IDC_NAMEMSG,mesg[13]);
		SetDlgItemText(hDlg,IDC_NAME,login_name);
		SendMessage(GetDlgItem(hDlg,IDC_NAME),EM_LIMITTEXT,30,0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDCANCEL:
			GetDlgItemText(hDlg,IDC_NAME,login_name,30);
			EndDialog(hDlg,LOWORD(wp));
			break;
		default: return FALSE;
		}
		break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK DlgProc_Config(HWND hDlg,UINT msg,WPARAM wp,LPARAM lp) 
{
	switch(msg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hDlg,IDOK));
		SetDlgItemText(hDlg,IDC_MESG,strmesg);
		SetDlgItemText(hDlg,IDC_NAME,strdefname);
		break;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDOK:
			GetDlgItemText(hDlg,IDC_MESG,strmesg,256);
			GetDlgItemText(hDlg,IDC_NAME,strdefname,30);
			EndDialog(hDlg,LOWORD(wp));
			break;
		case IDCANCEL:	EndDialog(hDlg,LOWORD(wp)); break;
		default:		return FALSE;
		}
		break;
	default: return FALSE;
	}
	return TRUE;
}

/*************************************************************************/

/* ウィンドウサイズを設定 */
void SetClientWindow(HWND hw,DWORD s,int w,int h)
{
	RECT rc;
	int cx,cy;

	rc.left = rc.top = 0;
	rc.right = w; rc.bottom = h;
	AdjustWindowRectEx(&rc,s,1,0); /* メニュー有りで */

	cx=rc.right  - rc.left;
	cy=rc.bottom - rc.top ;
	SetWindowPos(hw,HWND_TOP,0,0,cx,cy,SWP_NOMOVE);
}

/* ウィンドウを画面中央に表示 */
void SetCenterWindow(HWND hw,HWND hwD)
{
	HWND hwDesk;
	RECT rcDesk,rcWnd;
	int x,y;
	
	hwDesk=hwD;/*GetDesktopWindow();*/

	GetWindowRect(hwDesk,&rcDesk);
	GetWindowRect(hw    ,&rcWnd );

	x=(rcDesk.right-rcDesk.left) /2 + rcDesk.left - (rcWnd.right-rcWnd.left) /2;
	y=(rcDesk.bottom-rcDesk.top) /2 + rcDesk.top  - (rcWnd.bottom-rcWnd.top) /2;
	SetWindowPos(hw,HWND_TOP,x,y,0,0,SWP_NOSIZE);
}

int OpenFileDialog(HWND hWnd,char *szFile,char *szDir,char *szTitle,char *Filter)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize		=sizeof(OPENFILENAME);
	ofn.hwndOwner		=hWnd;
	ofn.lpstrFilter		=Filter;
	ofn.lpstrFile		=szFile;
	ofn.nMaxFile		=MAX_PATH;
	ofn.lpstrInitialDir	=(szFile[0])?"":szDir;
	ofn.lpstrTitle		=szTitle;
	ofn.Flags			=OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER;

	if(!GetOpenFileName(&ofn)) return 1;
	return 0;
}

int SaveFileDialog(HWND hWnd,char *strFile,char *strDir,char *strTitle,char *Filter,char *strExt)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize		=sizeof(OPENFILENAME);
	ofn.hwndOwner		=hWnd;
	ofn.lpstrFilter		=Filter;
	ofn.lpstrFile		=strFile;
	ofn.nMaxFile		=MAX_PATH;
	ofn.lpstrInitialDir	=(strFile[0])?"":strDir;
	ofn.lpstrTitle		=strTitle;
	ofn.lpstrDefExt		=strExt;
	ofn.Flags			=OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_EXTENSIONDIFFERENT;

	if(!GetSaveFileName(&ofn)) return 1;
	return 0;
}

