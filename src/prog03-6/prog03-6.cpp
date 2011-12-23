//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog03-6.cpp - Menus pop-up
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

// Cabe�alho dos recursos
#include "prog03-6-res.h"

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog03-6"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 03-6"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

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
//  wcl.lpszMenuName = MAKEINTRESOURCE(IDM_MEUMENU); // Carrega menu do recurso (m�todo 1)
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE)); // Carrega �cone do recurso

  // Cria identificador para o menu do recurso
  HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MEUMENU));

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
		   NULL, // Ou passa o identificador hMenu, carregando menu do recurso (m�todo 2)
		   hInstance,
		   NULL);

    // Verifica se a janela foi criada
    if(hWnd)
    {
	  // Define menu da janela (m�todo 3)
	  SetMenu(hWnd, hMenu);

	  // Deixa item 1 do menu 1 indispon�vel
	  EnableMenuItem(hMenu, ID_DO_MENU1_ITEM1, MF_BYCOMMAND | MF_GRAYED);

	  // Checa o item 2 do menu 1
	  CheckMenuItem(hMenu, ID_DO_MENU1_ITEM2, MF_BYCOMMAND | MF_CHECKED);

	  // Checa o item 2 do menu 2 como r�dio
	  CheckMenuRadioItem(hMenu, ID_DO_MENU2_ITEM1, ID_DO_MENU2_ITEMX, ID_DO_MENU2_ITEM2, MF_BYCOMMAND);

	  // Carrega teclas de atalho e cria identificador para elas
  	  HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDA_MEUATALHO));

      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

	  // Armazena dados da mensagem que ser� obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem n�o for WM_QUIT,
      // obt�m mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
		// Verifica se tecla de atalho foi ativada, se for, n�o executa
		// TranslateMessage() nem DispatchMessage(), pois j� foram processadas
	    if(!TranslateAccelerator(hWnd, hAccel, &msg))
		{
          // Traduz teclas virtuais ou aceleradoras (de atalho)
          TranslateMessage(&msg);

          // Envia mensagem para a fun��o que processa mensagens (WindowProc)
          DispatchMessage(&msg);
		}
      }

	  // Destr�i o menu e libera mem�ria
	  DestroyMenu(hMenu);

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
      /* Devemos avisar manualmente ao Windows que a janela j� foi atualizada, pois n�o � um processo autom�tico. Se isso n�o for feito, o Windows n�o ir� parar de enviar a mensagem WM_PAINT ao programa. */

      // O c�digo abaixo avisa o Windows que a janela j� foi atualizada.
      hDC = BeginPaint(hWnd, &psPaint);
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

	case WM_RBUTTONUP:
	{
	  // Obt�m coordenadas (x, y) do cursor do mouse
	  POINT ptMouse = { LOWORD(lParam), HIWORD(lParam) };

	  // Converte coordenadas do cursor do mouse para coordenadas da tela
	  // Necess�rio para TrackPopupMenu()
	  ClientToScreen(hWnd, &ptMouse);

	  // Obt�m o identificador do menu do programa
	  HMENU hMenuDoProg = GetMenu(hWnd);

	  // Obt�m o identificador do sub-menu 1
	  HMENU hMenu1 = GetSubMenu(hMenuDoProg, 0);

	  // Obt�m o identificador do sub-menu 2
	  HMENU hMenu2 = GetSubMenu(hMenuDoProg, 1);

	  // Cria o identificador para um novo menu pop-up
	  HMENU hMenu = CreatePopupMenu();

	  // Adiciona o sub-menu 1 para o novo menu pop-up
	  AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu1, "Menu 1");

	  // Adiciona o sub-menu 2 para o novo menu pop-up
	  AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu2, "Menu 2");

	  // Mostra o menu pop-up na posi��o do cursor do mouse
	  TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, 0, hWnd, NULL);

	  return(0);
	} break;

	case WM_COMMAND: // Item do menu, tecla de atalho ou controle ativado
	{
      // Verifica bit menos significativo de wParam (ID�s)
	  switch(LOWORD(wParam))
	  {
	    case ID_DO_MENU1_ITEM1:
		{
		  MessageBox(hWnd, "Menu 1... Item 1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU1_ITEM2:
		{
		  MessageBox(hWnd, "Menu 1... Item 2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU1_ITEMX:
		{
		  MessageBox(hWnd, "Menu 1... Item X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM1:
		{
		  MessageBox(hWnd, "Menu 2... Item 1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM2:
		{
		  MessageBox(hWnd, "Menu 2... Item 2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEMX:
		{
		  MessageBox(hWnd, "Menu 2... Item X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_1:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.1", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_2:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.2", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case ID_DO_MENU2_ITEM3_X:
		{
		  MessageBox(hWnd, "Menu 2... Item 3... Item 3.X", "Menu selecionado", MB_OK | MB_ICONINFORMATION);
		} break;
	    case IDA_ATALHO_ESC:
		{
		  // Destr�i a janela
          DestroyWindow(hWnd);
		} break;
	  }

	  return(0);
	} break;

	default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que n�o foram verificadas na fun��o */
	  return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

  }
}