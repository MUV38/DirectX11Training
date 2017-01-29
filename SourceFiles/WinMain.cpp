#include <windows.h>
#include "../MDX/System/MDX_System.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // ウィンドウ クラスを登録する
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // ウィンドウを作成する

    HWND hwnd = CreateWindowEx(
        0,                              // オプションのウィンドウ スタイル
        CLASS_NAME,                     // ウィンドウ クラス
        L"DX11 Demo",					// ウィンドウ テキスト
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,     // ウィンドウ スタイル

        // サイズと位置
        0, 0, 1280, 720,

        NULL,       // 親ウィンドウ    
        NULL,       // メニュー
        hInstance,  // インスタンス ハンドル
        NULL        // 追加のアプリケーション データ
        );

    if (hwnd == NULL)
    {
        return 0;
    }

	if( !MDX_GET_SYSTEM.Init(hwnd) ){
		MDX_GET_SYSTEM.Release();
		return 0;
	}

    ShowWindow(hwnd, nCmdShow);

    // メッセージ ループを実行する
    MSG msg = { };
	do{
		if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			// ここに主処理を
			MDX_GET_DX_SYSTEM->Update();
			MDX_GET_SYSTEM.Update();
			MDX_GET_SYSTEM.Draw();
		}
	}while( msg.message != WM_QUIT );

	MDX_GET_SYSTEM.Release();

    return 0;
}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// imguiイベント
	if (ImGui_ImplDX11_WndProcHandler(hWnd, uMsg, wParam, lParam)){
		return true;
	}

    switch (uMsg)
    {
	case WM_KEYUP:
		if( wParam == VK_ESCAPE ){
			DestroyWindow(hWnd);
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		MDX_GET_DX_SYSTEM->ResizeWindow();
		break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}