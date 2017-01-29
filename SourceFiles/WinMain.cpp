#include <windows.h>
#include "../MDX/System/MDX_System.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // �E�B���h�E �N���X��o�^����
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // �E�B���h�E���쐬����

    HWND hwnd = CreateWindowEx(
        0,                              // �I�v�V�����̃E�B���h�E �X�^�C��
        CLASS_NAME,                     // �E�B���h�E �N���X
        L"DX11 Demo",					// �E�B���h�E �e�L�X�g
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,     // �E�B���h�E �X�^�C��

        // �T�C�Y�ƈʒu
        0, 0, 1280, 720,

        NULL,       // �e�E�B���h�E    
        NULL,       // ���j���[
        hInstance,  // �C���X�^���X �n���h��
        NULL        // �ǉ��̃A�v���P�[�V���� �f�[�^
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

    // ���b�Z�[�W ���[�v�����s����
    MSG msg = { };
	do{
		if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			// �����Ɏ又����
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
	// imgui�C�x���g
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