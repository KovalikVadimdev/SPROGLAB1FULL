#include <windows.h> // підключення бібліотеки з функціями API
#include "resource.h"
// Глобальні змінні:

HINSTANCE hInst; 	//Дескриптор програми	
LPCTSTR szWindowClass = "Kovalik";
LPCTSTR szTitle = "Kovalik";

// Попередній опис функцій

ATOM MyRegisterClass(HINSTANCE hInstance); //Реєстрація класу вікна
BOOL InitInstance(HINSTANCE, int); // Створення вікна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //Віконна процедура
// Основна програма 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	// Реєстрація класу вікна 
	MyRegisterClass(hInstance);

	// Створення вікна програми
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обробки повідомлень
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; 		//стиль вікна
	wcex.lpfnWndProc = (WNDPROC)WndProc; 		//віконна процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance; 			//дескриптор програми
	wcex.hIcon = LoadIcon(NULL, IDI_SHIELD); 		//визначення іконки
	wcex.hCursor = LoadCursor(NULL, IDC_UPARROW); 	//визначення курсору
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //установка фону
	wcex.lpszMenuName = NULL; 				//визначення меню
	wcex.lpszClassName = szWindowClass; 		//ім’я класу
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex); 			//реєстрація класу вікна
}

// FUNCTION: InitInstance (HANDLE, int)
// Створює вікно програми і зберігає дескриптор програми в змінній hInst

int getTaskBarHeight() {
	RECT rect;
	HWND taskBar = FindWindow("Shell_traywnd", NULL);
	GetWindowRect(taskBar, &rect);
	return rect.bottom - rect.top;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; //зберігає дескриптор додатка в змінній hInst
	int PositionY = GetSystemMetrics(SM_CYSCREEN) - 300 - getTaskBarHeight();
	hWnd = CreateWindow(szWindowClass, 	// ім’я класу вікна
		szTitle, 				// назва програми
		WS_VSCROLL | WS_MINIMIZE | WS_SYSMENU,			// стиль вікна
		0, 			// положення по Х	
		PositionY,			// положення по Y	
		500, 			// розмір по Х
		300, 			// розмір по Y
		NULL, 					// дескриптор батьківського вікна	
		NULL, 					// дескриптор меню вікна
		hInstance, 				// дескриптор програми
		NULL); 				// параметри створення.

	if (!hWnd) 	//Якщо вікно не творилось, функція повертає FALSE
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_MINIMIZE); 		//Показати вікно
	UpdateWindow(hWnd); 				//Оновити вікно
	return TRUE;
}

// FUNCTION: WndProc (HWND, unsigned, WORD, LONG)
// Віконна процедура. Приймає і обробляє всі повідомлення, що приходять
//  в додаток

INT_PTR CALLBACK AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam,
	LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return (INT_PTR)TRUE;
		case IDCANCEL2:
			EndDialog(hDlg, IDCANCEL);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CustomMessageBoxProc(HWND hDlg, UINT message, WPARAM wParam,
	LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			EndDialog(hDlg, IDC_BUTTON1);
			PostQuitMessage(0);
			return (INT_PTR)TRUE;
		case IDC_BUTTON2:
			EndDialog(hDlg, IDC_BUTTON2);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Close(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		case WM_INITDIALOG:{
			SetTimer(hDlg, 1, 2000, NULL);
			return (INT_PTR)TRUE;
		}
		case WM_TIMER:{
			EndDialog(hDlg, IDC_BUTTON2);
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SecondDialogProc(HWND hDlg, UINT message, WPARAM wParam,
	LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			CHAR buffer[256];
			GetDlgItemText(hDlg, IDC_EDIT1, buffer, 256);
			MessageBox(hDlg, buffer, "Read Text", MB_OK);
			return (INT_PTR)TRUE;
		case IDCANCEL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MESSAGE), hDlg, Close);
			EndDialog(hDlg, IDC_BUTTON2);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void CreateOldMenu(HWND hWnd) {

	HMENU hMenu = CreateMenu();
	HMENU hAboutMenu = CreatePopupMenu();
	AppendMenu(hAboutMenu, MF_STRING, ID_MENU_ABOUTTHEPROGRAM,
		"About the program");
	AppendMenu(hAboutMenu, MF_STRING, ID_MENU_SECONDDIALOGBOX,
		"Second dialog box");
	AppendMenu(hAboutMenu, MF_STRING, ID_MENU_CHANGEMENU, "Change menu");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hAboutMenu, "Menu");
	HMENU hPopupMenu = CreatePopupMenu();
	AppendMenu(hPopupMenu, MF_STRING, IDS_STRING101, "First headline");
	AppendMenu(hPopupMenu, MF_STRING, IDS_STRING102, "Second headline");
	AppendMenu(hPopupMenu, MF_STRING, IDS_STRING103, "Third headline");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopupMenu, "Change");
	SetMenu(hWnd, hMenu);

}

void CreateNewMenu(HWND hWnd)
{
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreatePopupMenu();
	HMENU NewMenuItem1 = CreatePopupMenu();
	HMENU NewMenuItem2 = CreatePopupMenu();
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_CREATE, "Create");
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_EDIT, "Edit");
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_RETURNOLDMENU, "Return old menu");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(NewMenuItem1, MF_STRING, ID_NEWMENUITEM1_GIT, "Git");
	AppendMenu(NewMenuItem1, MF_STRING, ID_NEWMENUITEM1_YOUTUBE, "YouTube");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)NewMenuItem1, "NewMenuItem1");
	AppendMenu(NewMenuItem2, MF_STRING, ID_NEWMENUITEM2_VADIM, "Vadim");
	AppendMenu(NewMenuItem2, MF_STRING, ID_NEWMENUITEM2_DIMA, "Dima");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)NewMenuItem2, "NewMenuItem2");

	SetMenu(hWnd, hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE: { 				//Повідомлення приходить при створенні вікна
		CreateOldMenu(hWnd);
	}
		break;
		case WM_COMMAND: 				//Повідомлення від меню
			switch (LOWORD(wParam))
			{
			case ID_MENU_ABOUTTHEPROGRAM:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDialogProc);
				break;
			case ID_MENU_SECONDDIALOGBOX:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_READ), hWnd, SecondDialogProc);
				break;
			case IDS_STRING101:
				SetWindowText(hWnd, "First headline");
				break;
			case IDS_STRING102:
				SetWindowText(hWnd, "Second headline");
				break;
			case IDS_STRING103:
				SetWindowText(hWnd, "Third headline");
				break;
			case ID_MENU_CHANGEMENU:
				CreateNewMenu(hWnd);
				break;
			case ID_FILE_RETURNOLDMENU:
				CreateOldMenu(hWnd);
				break;
			}
			break;

	case WM_LBUTTONDBLCLK: 				//Подвійний щелчок лівою кнопкою миші
		MessageBox(hWnd, "Double-click the left mouse button", "Double-click",
			MB_OK);
		break;

	case WM_PAINT: 				//Перемалювати вікно
		hdc = BeginPaint(hWnd, &ps); 	//Почати графічний вивід	
		GetClientRect(hWnd, &rt); 		//Область вікна для малювання
		DrawText(hdc, "Hello, world!", -1, &rt, DT_SINGLELINE | DT_CENTER 
			| DT_VCENTER);
		EndPaint(hWnd, &ps); 		//Закінчити графічний вивід	
		break;

	case WM_CLOSE:{ 				//Закрити вікно
		HWND hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_QUESTIONS), hWnd,
			CustomMessageBoxProc);
		ShowWindow(hDlg, SW_SHOWNORMAL);
	}
		break;

	case WM_DESTROY:				//Завершення роботи
		PostQuitMessage(0);
		break;
	default:
		//Обробка повідомлень, які не оброблені користувачем
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}