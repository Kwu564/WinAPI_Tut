#include <windows.h>
#include <string>
#include <resource.h>

const char g_szClassName[] = "myWindowClass";
HWND g_hToolbar = NULL;
HWND controls = NULL;

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
   switch (Message) {
   case WM_INITDIALOG:
      return TRUE;
   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         EndDialog(hwnd, IDOK);
         break;
      case IDCANCEL:
         EndDialog(hwnd, IDCANCEL);
         break;
      }
      break;
   default:
      return FALSE;
   }
   return TRUE;
}

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
   switch (Message) {
      case WM_COMMAND:
         switch (LOWORD(wParam)) {
            case ID_PRESS:
               MessageBox(hwnd, "Hi!", "This is a message", MB_OK | MB_ICONEXCLAMATION);
               break;
            case ID_OTHER:
               MessageBox(hwnd, "Bye!", "This is also a message", MB_OK | MB_ICONEXCLAMATION);
               break;
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}

BOOL CALLBACK ControlDlgPRoc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
   switch (Message) {
      case WM_INITDIALOG:
         SetDlgItemText(hwnd, IDC_TEXT, "This is a string");
         SetDlgItemInt(hwnd, IDC_NUMBER, 5, FALSE);
         break;
      case WM_COMMAND:
         switch (LOWORD(wParam)) {
            case IDC_ADD:
            {
               BOOL bSuccess;
               int nTimes = GetDlgItemInt(hwnd, IDC_NUMBER, &bSuccess, FALSE);
               if (bSuccess) {
                  int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT));
                  if (len > 0) {
                     int i;
                     char* buf;
                     buf = (char*)GlobalAlloc(GPTR, len + 1);
                     GetDlgItemText(hwnd, IDC_TEXT, buf, len + 1);

                     for (i = 0; i < nTimes; i++) {
                        int index = SendDlgItemMessage(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM)buf);
                        SendDlgItemMessage(hwnd, IDC_LIST, LB_SETITEMDATA, (WPARAM)index, (LPARAM)nTimes);
                     }
                     GlobalFree((HANDLE)buf);
                  }
               }
            }
               break;
            case IDC_REMOVE:
            {
               HWND hList = GetDlgItem(hwnd, IDC_LIST);
               int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
               if (count != LB_ERR) {
                  if (count != 0) {
                     int i;
                     int* buf = (int*)GlobalAlloc(GPTR, sizeof(int) * count);
                     SendMessage(hList, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);

                     for (i = count - 1; i >= 0; i--) {
                        SendMessage(hList, LB_DELETESTRING, (WPARAM)buf[i], 0);
                     }

                     GlobalFree(buf);
                  }
                  else {
                     MessageBox(hwnd, "No items selected.", "Warning,", MB_OK);
                  }
               }
               else {
                  MessageBox(hwnd, "No items selected.", "Warning", MB_OK);
               }
               break;
            }
            case IDC_CLEAR:
               SendDlgItemMessage(hwnd, IDC_LIST, LB_RESETCONTENT, 0, 0);
               break;
            case IDC_LIST:
               switch (HIWORD(wParam)) {
                  case LBN_SELCHANGE:
                  {
                     HWND hList = GetDlgItem(hwnd, IDC_LIST);
                     int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
                     if (count != LB_ERR) {
                        if (count == 1) {
                           int index;
                           int err = SendMessage(hList, LB_GETSELITEMS, (WPARAM)1, (LPARAM)& index);
                           if (err != LB_ERR) {
                              int data = SendMessage(hList, LB_GETITEMDATA, (WPARAM)index, 0);

                              SetDlgItemInt(hwnd, IDC_SHOWCOUNT, data, FALSE);
                           }
                           else {
                              MessageBox(hwnd, "Error getting selected item", "Warning", MB_OK);
                           } 
                        }
                        else {
                           SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
                        }
                     }
                     else {
                        MessageBox(hwnd, "Error counting items", "Warning", MB_OK);
                     }
                  }
                     break;
               }
               break;
         }
         break;
      case WM_CLOSE:
         EndDialog(hwnd, 0);
         break;
      default:
         return FALSE;
   }
   return TRUE;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
      case WM_CREATE:
         g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hwnd, ToolDlgProc);
         if (g_hToolbar != NULL) {
            ShowWindow(g_hToolbar, SW_SHOW);
         }
         else {
            MessageBox(hwnd, "CreateDialog returned NULL", "Warning!", MB_OK | MB_ICONINFORMATION);
         }
         controls = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAIN), hwnd, ControlDlgPRoc);
         if (g_hToolbar != NULL) {
            ShowWindow(controls, SW_SHOW);
         }
         else {
            MessageBox(hwnd, "CreateDialog returned NULL", "Warning!", MB_OK | MB_ICONINFORMATION);
         }
         break;
      case WM_LBUTTONDOWN:
      {
         //char szFileName[MAX_PATH];
         //HINSTANCE hInstance = GetModuleHandle(NULL);

         //GetModuleFileName(hInstance, szFileName, MAX_PATH);
         //MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
      }
         break;
      case WM_COMMAND:
         switch (LOWORD(wParam))
         {
            case ID_STUFF_SHOW:
               ShowWindow(g_hToolbar, SW_SHOW);
               break;
            case ID_STUFF_HIDE:
               ShowWindow(g_hToolbar, SW_HIDE);
               break;
            case ID_FILE_EXIT:
               PostMessage(hwnd, WM_CLOSE, 0, 0);
               break;
            case ID_STUFF_ABOUT:
               int ret = DialogBox(GetModuleHandle(NULL),
                  MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
               if (ret == IDOK) {
                  MessageBox(hwnd, "Dialogue exited with ID_OK.", "Notice", MB_OK | MB_ICONINFORMATION);
               }
               else if (ret == IDCANCEL) {
                  MessageBox(hwnd, "Dialog exited with ID_CANCEL.", "Notice", MB_OK | MB_ICONINFORMATION);
               }
               else if (ret == -1) {
                  MessageBox(hwnd, "Dialog failed!", "Error", MB_OK | MB_ICONINFORMATION);
               }
               break;
         }
         break;
      case WM_CLOSE:
         DestroyWindow(hwnd);
         break;
      case WM_DESTROY:
         DestroyWindow(g_hToolbar);
         PostQuitMessage(0);
         break;
      default:
         return DefWindowProc(hwnd, msg, wParam, lParam);
   }
   return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   WNDCLASSEX wc;
   HWND hwnd;
   MSG Msg;

   //Step 1: Registering the Window Class
   wc.cbSize = sizeof(WNDCLASSEX);
   wc.style = 0;
   wc.lpfnWndProc = WndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = hInstance;
   wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
   wc.lpszClassName = g_szClassName;
   wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));

   if (!RegisterClassEx(&wc))
   {
      MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
      return 0;
   }

   // Step 2: Creating the Window
   hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      g_szClassName,
      "The title of my window",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
      NULL, NULL, hInstance, NULL);

   if (hwnd == NULL)
   {
      MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
      return 0;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   // Step 3: The Message Loop
   while (GetMessage(&Msg, NULL, 0, 0) > 0)
   {
      if (!IsDialogMessage(g_hToolbar, &Msg)) {
         TranslateMessage(&Msg);
         DispatchMessage(&Msg);
      }
   }

   return Msg.wParam;
}