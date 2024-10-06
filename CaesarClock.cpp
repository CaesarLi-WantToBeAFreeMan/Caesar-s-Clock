#include <Windows.h>
#include <cstdio>
#include "resource.h"
#include "Ctm.h"
#include "Ccount.h"
using namespace std;

enum Languages { ENUS = 0, ZHTW = 1, ZHCN = 2 };
enum Colors { WHITE = 0, GREY = 1, BLACK = 2 , BLUE = 3, CYAN = 4, GREEN = 5, 
			YELLOW = 6, GOLD = 7, BROWN = 8, ORANGE = 9, RED = 10, PINK = 11, 
			PURPLE = 12};
bool alarm = false, countup = false, countdown = false;
int Lang = ENUS, timeForm = 1, dateForm = 1, zone = 0, bRed = 0, 
	bGreen = 0, bBlue = 0, tRed = 255, tGreen = 255, tBlue = 255;
const int STRSIZE = 256;
wchar_t strTitle[STRSIZE], strText[STRSIZE], funTitle[STRSIZE], funText[STRSIZE];
HINSTANCE MWGHI = 0;
HWND MWGH = 0;
Ctm* Cnow = new Ctm();
Ctm* Czone = new Ctm();
Ctm* Calarm = new Ctm(-1, -1, -1, -1, -1, -1);
Ccount* Ccountup = new Ccount();
Ccount* Ccountdown = new Ccount(-1, -1, -1);
void Common_Color_Warning();

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam);
INT CALLBACK DialogProc(HWND hDlg, UINT msgID, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR lpCmdLine, int nCmdShow) {
	MWGHI = hIns;
	
	RECT * rect = new RECT;
	SystemParametersInfo(SPI_GETWORKAREA, 0, rect, 0);
	int x = (rect->left + rect->right) / 2 - 450;
	int y = (rect->top + rect->bottom) / 2 - 250;
	delete rect;

	WNDCLASS* wc = new WNDCLASS;
	wc->cbClsExtra = 0;
	wc->cbWndExtra = 0;
	wc->hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wc->hCursor = NULL;
	wc->hIcon = LoadIcon(hIns, (wchar_t*)IDI_ICON);
	wc->hInstance = hIns;
	wc->lpfnWndProc = MainWindowProc;
	wc->lpszClassName = L"Caesar";
	wc->lpszMenuName = (wchar_t*)IDR_MENU1;
	wc->style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(wc);
	HWND hWnd = CreateWindowEx(0, L"Caesar", L"Caesar Clock", WS_BORDER | WS_CAPTION | WS_SYSMENU, x, y, 900, 500, NULL, NULL, hIns, NULL);
	MWGH = hWnd;
	delete wc;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	Cnow->now();
	Czone->nowgm(zone);
	SetTimer(hWnd, 1, 1000, NULL);

	HACCEL hAccel = LoadAccelerators(hIns, (wchar_t*)IDR_ACCELERATOR1);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID) {
	case WM_CREATE: {
		PostMessage(hWnd, WM_PAINT, 0, 0);
	}

	case WM_TIMER:
		switch (wParam) {
		case 1:
			Cnow->now();
			Czone->nowgm(zone);
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case 2:
			PostMessage(hWnd, WM_PAINT, -2, 0);
			break;
		case 3:
			Ccountup->add_second();
			PostMessage(hWnd, WM_PAINT, -3, 0);
			break;
		case 4:
			Ccountdown->minus_second();
			PostMessage(hWnd, WM_PAINT, -4, 0);
			break;
		}

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) {
			switch (Lang) {
			case ENUS:
				LoadString(MWGHI, STR_Quit_Title_en_us, (wchar_t*)strTitle, STRSIZE);
				LoadString(MWGHI, STR_Quit_Text_en_us, (wchar_t*)strText, STRSIZE);
				break;
			case ZHTW:
				LoadString(MWGHI, STR_Quit_Title_zh_tw, (wchar_t*)strTitle, STRSIZE);
				LoadString(MWGHI, STR_Quit_Text_zh_tw, (wchar_t*)strText, STRSIZE);
				break;
			case ZHCN:
				LoadString(MWGHI, STR_Quit_Title_zh_cn, (wchar_t*)strTitle, STRSIZE);
				LoadString(MWGHI, STR_Quit_Text_zh_cn, (wchar_t*)strText, STRSIZE);
				break;
			}
			if (MessageBox(hWnd, (wchar_t*)strText, (wchar_t*)strTitle, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK) {
				PostQuitMessage(0);
			}
			else {
				return 1;
			}
		}
		break;
	case WM_PAINT: {
		InvalidateRect(hWnd, NULL, true);
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(bRed, bGreen, bBlue));
		HGDIOBJ OldPen = SelectObject(hdc, hPen);
		HBRUSH hBrush = CreateSolidBrush(RGB(bRed, bGreen, bBlue));
		HGDIOBJ OldBrush = SelectObject(hdc, hBrush);
		Rectangle(hdc, 0, 0, 884, 441);
		SelectObject(hdc, OldPen);
		DeleteObject(hPen);
		SelectObject(hdc, OldBrush);
		DeleteObject(hBrush);
		SetTextColor(hdc, RGB(tRed, tGreen, tBlue));
		SetBkColor(hdc, RGB(bRed, bGreen, bBlue));
		switch (Lang) {
		case ENUS:
			LoadString(MWGHI, STR_Now_en_us, strTitle, STRSIZE);
			LoadString(MWGHI, STR_Zone_en_us, strText, STRSIZE);
			break;
		case ZHTW:
			LoadString(MWGHI, STR_Now_zh_tw, strTitle, STRSIZE);
			LoadString(MWGHI, STR_Zone_zh_tw, strText, STRSIZE);
			break;
		case ZHCN:
			LoadString(MWGHI, STR_Now_zh_cn, strTitle, STRSIZE);
			LoadString(MWGHI, STR_Zone_zh_cn, strText, STRSIZE);
			break;
		}

		RECT* Rec = new RECT;
		Rec->left = 0;
		Rec->top = 25;
		Rec->right = 884;
		Rec->bottom = 391;
		DrawText(hdc, strTitle, wcslen(strTitle), Rec, DT_CENTER | DT_TOP);

		wchar_t strTemp[STRSIZE] = L"0";
		Rec->top = 75;
		Rec->right = 400;
		wcscpy_s(strTemp, STRSIZE, Cnow->show_time(timeForm));
		DrawText(hdc, strTemp, wcslen(strTemp), Rec, DT_RIGHT | DT_TOP);

		Rec->left = 500;
		Rec->right = 884;
		wcscpy_s(strTemp, STRSIZE, Cnow->show_date(dateForm));
		DrawText(hdc, strTemp, wcslen(strTemp), Rec, DT_LEFT | DT_TOP);

		Rec->top = 125;
		Rec->left = 0;
		Rec->right = 884;
		wsprintf(strTemp, L"%s %d", strText, zone);
		DrawText(hdc, strTemp, wcslen(strTemp), Rec, DT_CENTER | DT_TOP);

		Rec->top = 175;
		Rec->right = 400;
		wcscpy_s(strTemp, STRSIZE, Czone->show_time(timeForm));
		
		DrawText(hdc, strTemp, wcslen(strTemp), Rec, DT_RIGHT | DT_TOP);

		Rec->left = 500;
		Rec->right = 884;
		wcscpy_s(strTemp, STRSIZE, Czone->show_date(dateForm));
		DrawText(hdc, strTemp, wcslen(strTemp), Rec, DT_LEFT | DT_TOP);

		if (wParam == -1) {
			switch (lParam) {
				HMENU hMenu;
			case ENUS:
				hMenu = LoadMenu(MWGHI, (wchar_t*)IDR_MENU1);
				SetMenu(hWnd, hMenu);
				break;
			case ZHTW:
				hMenu = LoadMenu(MWGHI, (wchar_t*)IDR_MENU2);
				SetMenu(hWnd, hMenu);
				break;
			case ZHCN:
				hMenu = LoadMenu(MWGHI, (wchar_t*)IDR_MENU3);
				SetMenu(hWnd, hMenu);
				break;
			}
			return 1;
		}

		if (alarm || countup || countdown) {
			wchar_t timing[STRSIZE] = L"0";
			hPen = CreatePen(PS_SOLID, 1, RGB(tRed, tGreen, tBlue));
			OldPen = SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(tRed, tGreen, tBlue));
			OldBrush = SelectObject(hdc, hBrush);
			MoveToEx(hdc, 0, 223, NULL);
			LineTo(hdc, 884, 223);

			switch (Lang) {
			case ENUS:
				if (alarm) {
					LoadString(MWGHI, STR_Aalarm_Title_en_us, funTitle, STRSIZE);
				}
				else if (countup) {
					LoadString(MWGHI, STR_Countup_Title_en_us, funTitle, STRSIZE);
				}
				else if (countdown) {
					LoadString(MWGHI, STR_Countdown_Title_en_us, funTitle, STRSIZE);
				}
				LoadString(MWGHI, STR_Function_Text_en_us, funText, STRSIZE);
				break;
			case ZHTW:
				if (alarm) {
					LoadString(MWGHI, STR_Aalarm_Title_zh_tw, funTitle, STRSIZE);
				}
				else if (countup) {
					LoadString(MWGHI, STR_Countup_Title_zh_tw, funTitle, STRSIZE);
				}
				else if (countdown) {
					LoadString(MWGHI, STR_Countdown_Title_zh_tw, funTitle, STRSIZE);
				}
				LoadString(MWGHI, STR_Function_Text_zh_tw, funText, STRSIZE);
				break;
			case ZHCN:
				if (alarm) {
					LoadString(MWGHI, STR_Aalarm_Title_zn_cn, funTitle, STRSIZE);
				}
				else if (countup) {
					LoadString(MWGHI, STR_Countup_Title_zh_cn, funTitle, STRSIZE);
				}
				else if (countdown) {
					LoadString(MWGHI, STR_Countdown_Title_zh_cn, funTitle, STRSIZE);
				}
				LoadString(MWGHI, STR_Function_Text_zh_cn, funText, STRSIZE);
				break;
			}

			Rec->top = 270;
			Rec->left = 0;
			Rec->right = 884;
			DrawText(hdc, funTitle, wcslen(funTitle), Rec, DT_CENTER | DT_TOP);
			Rec->top = 320;
			DrawText(hdc, funText, wcslen(funText), Rec, DT_CENTER | DT_TOP);
			Rec->top = 370;

			switch (wParam) {
			case -2: {
				if (Calarm->is_alarm_end(Cnow)) {
					wchar_t title[STRSIZE] = L"0", text[STRSIZE] = L"0";
					KillTimer(hWnd, 2);
					alarm = false;
					Calarm->clean();
					switch (Lang) {
					case ENUS:
						LoadString(MWGHI, STR_Alarm_End_Title_en_us, title, STRSIZE);
						LoadString(MWGHI, STR_Alarm_End_Text_en_us, text, STRSIZE);
						break;
					case ZHTW:
						LoadString(MWGHI, STR_Alarm_End_Title_zh_tw, title, STRSIZE);
						LoadString(MWGHI, STR_Alarm_End_Text_zh_tw, text, STRSIZE);
						break;
					case ZHCN:
						LoadString(MWGHI, STR_Alarm_End_Title_zh_cn, title, STRSIZE);
						LoadString(MWGHI, STR_Alarm_End_Text_zh_cn, text, STRSIZE);
						break;
					}
					MessageBox(hWnd, text, title, MB_OK | MB_ICONWARNING);
					break;
				}
				wcscpy_s(timing, STRSIZE, Calarm->different(Cnow));
				DrawText(hdc, timing, wcslen(timing), Rec, DT_CENTER | DT_TOP);
				break;
			}
				
			case -3: {
				wcscpy_s(timing, STRSIZE, Ccountup->show());
				DrawText(hdc, timing, wcslen(timing), Rec, DT_CENTER | DT_TOP);
				break;
			}
			case -4: {
				if (Ccountdown->is_end()) {
					wchar_t title[STRSIZE] = L"0", text[STRSIZE] = L"0";
					KillTimer(hWnd, 4);
					countdown = false;
					Ccountdown->clean();
					switch (Lang) {
					case ENUS:
						LoadString(MWGHI, STR_Countdown_End_Title_en_us, title, STRSIZE);
						LoadString(MWGHI, STR_Countdown_End_Text_en_us, text, STRSIZE);
						break;
					case ZHTW:
						LoadString(MWGHI, STR_Countdown_End_Title_zh_tw, title, STRSIZE);
						LoadString(MWGHI, STR_Countdown_End_Text_zh_tw, text, STRSIZE);
						break;
					case ZHCN:
						LoadString(MWGHI, STR_Countdown_End_Title_zh_cn, title, STRSIZE);
						LoadString(MWGHI, STR_Countdown_End_Text_zh_cn, text, STRSIZE);
						break;
					}
					MessageBox(hWnd, text, title, MB_OK | MB_ICONWARNING);
					break;
				}
				wcscpy_s(timing, STRSIZE, Ccountdown->show());
				DrawText(hdc, timing, wcslen(timing), Rec, DT_CENTER | DT_TOP);
				break;
			}
			}

		}
		delete Rec;
		SelectObject(hdc, OldPen);
		DeleteObject(hPen);
		SelectObject(hdc, OldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
	}

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_File_Quit:
			PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case ID_Settings_Time_1:
			timeForm = 1;
			break;
		case ID_Settings_Time_2:
			timeForm = 2;
			break;
		case ID_Settings_Time_3:
			timeForm = 3;
			break;
		case ID_Settings_Date_1:
			dateForm = 1;
			break;
		case ID_Settings_Date_2:
			dateForm = 2;
			break;
		case ID_Settings_Date_3:
			dateForm = 3;
			break;
		case ID_Settings_Background_White:
			bRed = 255;
			bGreen = 255;
			bBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Grey:
			bRed = 190;
			bGreen = 190;
			bBlue = 190;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Black:
			bRed = 0;
			bGreen = 0;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Red:
			bRed = 255;
			bGreen = 0;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Blue:
			bRed = 0;
			bGreen = 0;
			bBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Green:
			bRed = 0;
			bGreen = 255;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Yellow:
			bRed = 255;
			bGreen = 255;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Gold:
			bRed = 255;
			bGreen = 215;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Orange:
			bRed = 255;
			bGreen = 165;
			bBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Pink:
			bRed = 255;
			bGreen = 192;
			bBlue = 203;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Brown:
			bRed = 165;
			bGreen = 42;
			bBlue = 42;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Cyan:
			bRed = 0;
			bGreen = 255;
			bBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Background_Purple:
			bRed = 160;
			bGreen = 32;
			bBlue = 240;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Language_en_us:
			Lang = ENUS;
			PostMessage(hWnd, WM_PAINT, -1, ENUS);
			break;
		case ID_Settings_Language_zh_cn:
			Lang = ZHCN;
			PostMessage(hWnd, WM_PAINT, -1, ZHCN);
			break;
		case ID_Settings_Language_zh_tw:
			Lang = ZHTW;
			PostMessage(hWnd, WM_PAINT, -1, ZHTW);
			break;
		case ID_Settings_Text_White:
			tRed = 255;
			tGreen = 255;
			tBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Grey:
			tRed = 190;
			tGreen = 190;
			tBlue = 190;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Black:
			tRed = 0;
			tGreen = 0;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Red:
			tRed = 255;
			tGreen = 0;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Blue:
			tRed = 0;
			tGreen = 0;
			tBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Green:
			tRed = 0;
			tGreen = 255;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Yellow:
			tRed = 255;
			tGreen = 255;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Gold:
			tRed = 255;
			tGreen = 215;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Orange:
			tRed = 255;
			tGreen = 165;
			tBlue = 0;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Pink:
			tRed = 255;
			tGreen = 192;
			tBlue = 203;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Brown:
			tRed = 165;
			tGreen = 42;
			tBlue = 42;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Cyan:
			tRed = 0;
			tGreen = 255;
			tBlue = 255;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_Text_Purple:
			tRed = 160;
			tGreen = 32;
			tBlue = 240;
			Common_Color_Warning();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case ID_Settings_TimeZone_N1200:
			zone = -1200;
			break;
		case ID_Settings_TimeZone_N1100:
			zone = -1100;
			break;
		case ID_Settings_TimeZone_N1000:
			zone = -1000;
			break;
		case ID_Settings_TimeZone_N0930:
			zone = -930;
			break;
		case ID_Settings_TimeZone_N0900:
			zone = -900;
			break;
		case ID_Settings_TimeZone_N0800:
			zone = -800;
			break;
		case ID_Settings_TimeZone_N0700:
			zone = -700;
			break;
		case ID_Settings_TimeZone_N0600:
			zone = -600;
			break;
		case ID_Settings_TimeZone_N0500:
			zone = -500;
			break;
		case ID_Settings_TimeZone_N0400:
			zone = -400;
			break;
		case ID_Settings_TimeZone_N0300:
			zone = -300;
			break;
		case ID_Settings_TimeZone_N0230:
			zone = -230;
			break;
		case ID_Settings_TimeZone_N0200:
			zone = -200;
			break;
		case ID_Settings_TimeZone_N0100:
			zone = -100;
			break;
		case ID_Settings_TimeZone_Zero:
			zone = 0;
			break;
		case ID_Settings_TimeZone_P0100:
			zone = 100;
			break;
		case ID_Settings_TimeZone_P0200:
			zone = 200;
			break;
		case ID_Settings_TimeZone_P0300:
			zone = 300;
			break;
		case ID_Settings_TimeZone_P0330:
			zone = 330;
			break;
		case ID_Settings_TimeZone_P0400:
			zone = 400;
			break;
		case ID_Settings_TimeZone_P0430:
			zone = 430;
			break;
		case ID_Settings_TimeZone_P0500:
			zone = 500;
			break;
		case ID_Settings_TimeZone_P0530:
			zone = 530;
			break;
		case ID_Settings_TimeZone_P0545:
			zone = 545;
			break;
		case ID_Settings_TimeZone_P0600:
			zone = 600;
			break;
		case ID_Settings_TimeZone_P0630:
			zone = 630;
			break;
		case ID_Settings_TimeZone_P0700:
			zone = 700;
			break;
		case ID_Settings_TimeZone_P0800:
			zone = 800;
			break;
		case ID_Settings_TimeZone_P0845:
			zone = 845;
			break;
		case ID_Settings_TimeZone_P0900:
			zone = 900;
			break;
		case ID_Settings_TimeZone_P0930:
			zone = 930;
			break;
		case ID_Settings_TimeZone_P1000:
			zone = 1000;
			break;
		case ID_Settings_TimeZone_P1030:
			zone = 1030;
			break;
		case ID_Settings_TimeZone_P1100:
			zone = 1100;
			break;
		case ID_Settings_TimeZone_P1200:
			zone = 1200;
			break;
		case ID_Settings_TimeZone_P1245:
			zone = 1245;
			break;
		case ID_Settings_TimeZone_P1300:
			zone = 1300;
			break;
		case ID_Settings_TimeZone_P1400:
			zone = 1400;
			break;
		case ID_Functions_Alarm:{
			if (countup) {
				KillTimer(hWnd, 3);
				countup = false;
				Ccountup->clean();
			}
			else if (countdown) {
				KillTimer(hWnd, 4);
				countdown = false;
				Ccountdown->clean();
			}
			if (!alarm) {
				bool choice;
				switch (Lang) {
				case ENUS:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG4, hWnd, DialogProc);
					break;
				case ZHTW:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG5, hWnd, DialogProc);
					break;
				case ZHCN:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG6, hWnd, DialogProc);
					break;
				}
				if (choice) {
					SetTimer(hWnd, 2, 1000, NULL);
					alarm = true;
				}
			}
			else {
				KillTimer(hWnd, 2);
				alarm = false;
				Calarm->clean();
			}
			break;
		}

		case ID_Functions_Countup: {
			if (alarm) {
				KillTimer(hWnd, 2);
				alarm = false;
				Calarm->clean();
			}
			else if (countdown) {
				KillTimer(hWnd, 4);
				countdown = false;
				Ccountdown->clean();
			}

			if (!countup) {
				SetTimer(hWnd, 3, 1000, NULL);
				countup = true;
			}
			else {
				KillTimer(hWnd, 3);
				countup = false;
				Ccountup->clean();
			}
			break;
		}

		case ID_Functions_Countdown: {
			if (alarm) {
				KillTimer(hWnd, 2);
				alarm = false;
				Calarm->clean();
			}
			else if (countup) {
				KillTimer(hWnd, 3);
				countup = false;
				Ccountup->clean();
			}
			if (!countdown) {
				bool choice;
				switch (Lang) {
				case ENUS:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG1, hWnd, DialogProc);
					break;
				case ZHTW:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG2, hWnd, DialogProc);
					break;
				case ZHCN:
					choice = DialogBox(MWGHI, (wchar_t*)IDD_DIALOG3, hWnd, DialogProc);
					break;
				}
				if (choice) {
					SetTimer(hWnd, 4, 1000, NULL);
					countdown = true;
				}
			}
			else {
				KillTimer(hWnd, 4);
				countdown = false;
				Ccountdown->clean();
			}
			break;
		}
		}
	}
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

void Common_Color_Warning(){
	if (bRed == tRed && bGreen == tGreen && bBlue == tBlue) {
			switch (Lang) {
			case ENUS:
				LoadString(MWGHI, STR_Common_Color_Title_en_us, strTitle, STRSIZE);
				LoadString(MWGHI, STR_Common_Color_Text_en_us, strText, STRSIZE);
				break;
			case ZHTW:
				LoadString(MWGHI, STR_Common_Color_Title_zh_tw, strTitle, STRSIZE);
				LoadString(MWGHI, STR_Common_Color_Text_zh_tw, strText, STRSIZE);
				break;
			case ZHCN:
				LoadString(MWGHI, STR_Common_Color_Title_zh_cn, strTitle, STRSIZE);
				LoadString(MWGHI, STR_Common_Color_Text_zh_cn, strText, STRSIZE);
				break;
			}
			MessageBox(MWGH, strText, strTitle, MB_OK | MB_ICONWARNING);
		}
}

INT CALLBACK DialogProc(HWND hDlg, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID) {
	case WM_CLOSE:
		EndDialog(hDlg, 0);

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDBCD_Ok: {
			wchar_t temp[STRSIZE] = L"0", tempText[STRSIZE] = L"0";
			HWND hHour = GetDlgItem(hDlg, IDECD_Hour);
			GetWindowText(hHour, temp, STRSIZE);
			Ccountdown->SetHour((_wtoi(temp)));

			HWND hMinute = GetDlgItem(hDlg, IDECD_Minute);
			GetWindowText(hMinute, temp, STRSIZE);
			Ccountdown->SetMinute((_wtoi(temp)));

			HWND hSecond = GetDlgItem(hDlg, IDECD_Second);
			GetWindowText(hSecond, temp, STRSIZE);
			Ccountdown->SetSecond((_wtoi(temp)));

			if (Ccountdown->check()) {
				EndDialog(hDlg, 1);
			}
			else {
				switch (Lang) {
				case ENUS:
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Title_en_us, temp, STRSIZE);
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Text_en_us, tempText, STRSIZE);
					break;
				case ZHTW:
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Title_zh_tw, temp, STRSIZE);
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Text_zh_tw, tempText, STRSIZE);
					break;
				case ZHCN:
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Title_zh_cn, temp, STRSIZE);
					LoadString(MWGHI, STR_Countdown_Set_Number_Error_Text_zh_cn, tempText, STRSIZE);
					break;
				}
				MessageBox(hDlg, tempText, temp, MB_OK | MB_ICONERROR);
			}

			break;
		}
		case IDBA_Ok: {
			wchar_t temp[STRSIZE] = L"0", tempText[STRSIZE] = L"0";

			HWND hYear = GetDlgItem(hDlg, IDEA_Year);
			GetWindowText(hYear, temp, STRSIZE);
			Calarm->SetYear((_wtoi(temp)));

			HWND hMonth = GetDlgItem(hDlg, IDEA_Month);
			GetWindowText(hMonth, temp, STRSIZE);
			Calarm->SetMonth((_wtoi(temp)));

			HWND hDay = GetDlgItem(hDlg, IDEA_Day);
			GetWindowText(hDay, temp, STRSIZE);
			Calarm->SetDay((_wtoi(temp)));

			HWND hHour = GetDlgItem(hDlg, IDEA_Hour);
			GetWindowText(hHour, temp, STRSIZE);
			Calarm ->SetHour((_wtoi(temp)));

			HWND hMinute = GetDlgItem(hDlg, IDEA_Minute);
			GetWindowText(hMinute, temp, STRSIZE);
			Calarm->SetMinute((_wtoi(temp)));

			HWND hSecond = GetDlgItem(hDlg, IDEA_Second);
			GetWindowText(hSecond, temp, STRSIZE);
			Calarm->SetSecond((_wtoi(temp)));

			if (Calarm->check() && Calarm->is_set_right(Cnow)) {
				EndDialog(hDlg, 1);
			}
			else if(Calarm->is_set_right(Cnow)) {
				switch (Lang) {
				case ENUS:
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Title_en_us, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Text_en_us, tempText, STRSIZE);
					break;
				case ZHTW:
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Title_zh_tw, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Text_zh_tw, tempText, STRSIZE);
					break;
				case ZHCN:
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Title_zh_cn, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Set_Number_Error_Text_zh_cn, tempText, STRSIZE);
					break;
				}
				MessageBox(hDlg, tempText, temp, MB_OK | MB_ICONERROR);
			}
			else {
				switch (Lang) {
				case ENUS:
					LoadString(MWGHI, STR_Alarm_Past_Time_Title_en_us, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Past_Time_Text_en_us, tempText, STRSIZE);
					break;
				case ZHTW:
					LoadString(MWGHI, STR_Alarm_Past_Time_Title_zh_tw, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Past_Time_Text_zh_tw, tempText, STRSIZE);
					break;
				case ZHCN:
					LoadString(MWGHI, STR_Alarm_Past_Time_Title_zh_cn, temp, STRSIZE);
					LoadString(MWGHI, STR_Alarm_Past_Time_Text_zh_cn, tempText, STRSIZE);
					break;
				}
				MessageBox(hDlg, tempText, temp, MB_OK | MB_ICONERROR);
			}

			break;
		}
			
		case IDBCD_Cancel:
		case IDBA_Cancel:
			EndDialog(hDlg, 0);
			break;
		}
	}
	return false;
}