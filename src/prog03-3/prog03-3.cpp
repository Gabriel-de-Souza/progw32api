//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog03-3.cpp - Bitmaps e sons
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h> // Para fun��o PlaySound()

// Inclui biblioteca winmm.lib, para tocar som (winmm = Windows MultiMedia)
#pragma comment(lib, "winmm.lib")

// Cabe�alho dos recursos
#include "prog03-3-res.h"

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog03-3"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 03-3"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Vari�veis globais
//--------------------------------------------------------------------------
HINSTANCE hInstance = NULL; // Inst�ncia do programa
HDC hMemDC = NULL; // DC de mem�ria
HBITMAP hBmp = NULL; // Imagem do recurso
HBITMAP hBmpOld = NULL; // Imagem antiga do DC

//--------------------------------------------------------------------------
// Prot�tipo das fun��es
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------
// WinMain() -> Fun��o principal
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  // Cria a classe da janela e especifica seus atributos
  WNDCLASSEX wcl;
  wcl.cbSize = sizeof(WNDCLASSEX);
  wcl.style = CS_HREDRAW | CS_VREDRAW;
  wcl.lpfnWndProc = (WNDPROC)WindowProc;
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  wcl.hInstance = hInstance;
  wcl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE)); // Carrega �cone do recurso
  wcl.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MEUCURSOR)); // Carrega cursor do recurso
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcl.lpszMenuName = NULL;
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE)); // Carrega �cone do recurso

  // Registra a classe da janela
  if(RegisterClassEx(&wcl))
  {
    // Cria a janela principal do programa
    HWND hWnd = NULL;
    hWnd = CreateWindowEx(
           NULL,
		   WINDOW_CLASS,
		   WINDOW_TITLE,
		   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		   (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
		   (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		   WINDOW_WIDTH,
		   WINDOW_HEIGHT,
		   HWND_DESKTOP,
		   NULL,
		   hInstance,
		   NULL);

    // Verifica se a janela foi criada
    if(hWnd)
    {
	  // Salva inst�ncia do programa na vari�vel global
	  ::hInstance = hInstance;

      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

	  // Armazena dados da mensagem que ser� obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem n�o for WM_QUIT,
      // obt�m mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
        // Traduz teclas virtuais ou aceleradoras (de atalho)
        TranslateMessage(&msg);

        // Envia mensagem para a fun��o que processa mensagens (WindowProc)
        DispatchMessage(&msg);
      }

      // Retorna ao Windows com valor de msg.wParam
      return((int)msg.wParam);
     }
     // Se a janela n�o foi criada
     else
     {
       // Exibe mensagem de erro e sai do programa
       MessageBox(NULL, "N�o foi poss�vel criar janela.", "Erro!", MB_OK | MB_ICONERROR);

       return(0);
     }
  }
  // Se a classe da janela n�o foi registrada
  else
  {
    // Exibe mensagem de erro e sai do programa
    MessageBox(NULL, "N�o foi poss�vel registrar a classe da janela.", "Erro!", MB_OK | MB_ICONERROR);

    return(0);
  }
	
  // Retorna ao Windows sem passar pelo loop de mensagens
  return(0);
}

//--------------------------------------------------------------------------
// WindowProc() -> Processa as mensagens enviadas para o programa
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // Vari�veis para manipula��o da parte gr�fica do programa
  HDC hDC = NULL;
  PAINTSTRUCT psPaint;

  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {

    case WM_CREATE: // Janela foi criada
    {
      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;
	
    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
	  // Obt�m identificador do DC e preenche PAINTSTRUCT
      hDC = BeginPaint(hWnd, &psPaint);

	  // Cria DC de mem�ria
	  hMemDC = CreateCompatibleDC(hDC);

	  // Carrega imagem do recurso (ID: IDB_MEUBITMAP)
	  hBmp = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_MEUBITMAP), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
  	  // Seleciona nova imagem no DC de mem�ria e salva imagem antiga
	  hBmpOld = (HBITMAP)SelectObject(hMemDC, hBmp);

	  // Transfere conte�do do DC de mem�ria para DC de v�deo
	  BitBlt(hDC, 0, 0, 206, 100, hMemDC, 0, 0, SRCCOPY);

	  // Restaura imagem antiga e deleta nova
	  SelectObject(hMemDC, hBmpOld);
	  DeleteObject(hBmp);

	  // Libera DC de mem�ria
	  DeleteDC(hMemDC);

	  // Libera DC
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

    case WM_CLOSE: // Janela foi fechada
    {
      // Destr�i a janela
      DestroyWindow(hWnd);

      return(0);
    } break;
	
    case WM_DESTROY: // Janela foi destru�da
    {
      // Envia mensagem WM_QUIT para o loop de mensagens
      PostQuitMessage(0);

      return(0);
    } break;

	case WM_LBUTTONDOWN: // Bot�o esquerdo do mouse pressionado
	{
	  // Toca som do recurso (ID: IDW_MEUSOM)
	  PlaySound(MAKEINTRESOURCE(IDW_MEUSOM), hInstance, SND_RESOURCE);

	  return(0);
	} break;

	default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que n�o foram verificadas na fun��o */
	  return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

  }
}