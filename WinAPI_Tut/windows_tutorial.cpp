#include <windows.h>
#include <string>
#include <resource.h>
#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

const char g_szClassName[] = "myWindowClass";

void addMenus(HWND hwnd) {
   HMENU hMenu, hSubMenu;

   hMenu = CreateMenu();

   hSubMenu = CreatePopupMenu();
   AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "&Exit");

   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

   hSubMenu = CreatePopupMenu();
   AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");

   SetMenu(hwnd, hMenu);


}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
      case WM_CREATE:
         addMenus(hwnd);
         break;
      case WM_LBUTTONDOWN:
      {
         char szFileName[MAX_PATH];
         HINSTANCE hInstance = GetModuleHandle(NULL);

         GetModuleFileName(hInstance, szFileName, MAX_PATH);
         MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
      }
         break;
      case WM_COMMAND:
         switch (LOWORD(wParam))
         {
            case ID_FILE_EXIT:
               PostMessage(hwnd, WM_CLOSE, 0, 0);
               break;
            case ID_STUFF_GO:
               break;
         }
      case WM_CLOSE:
         DestroyWindow(hwnd);
         break;
      case WM_DESTROY:
         PostQuitMessage(0);
         break;
      default:
         return DefWindowProc(hwnd, msg, wParam, lParam);
   }
   return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{
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
   wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wc.lpszMenuName = NULL;
   wc.lpszClassName = g_szClassName;
   wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

   if (!RegisterClassEx(&wc))
   {
      MessageBox(NULL, "Window Registration Failed!", "Error!",
         MB_ICONEXCLAMATION | MB_OK);
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
      MessageBox(NULL, "Window Creation Failed!", "Error!",
         MB_ICONEXCLAMATION | MB_OK);
      return 0;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);



   // Step 3: The Message Loop
   while (GetMessage(&Msg, NULL, 0, 0) > 0)
   {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
   }
   return Msg.wParam;
}