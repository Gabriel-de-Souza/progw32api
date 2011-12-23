//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog05-8.cpp - Simples programa de desenho
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog05-8"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 05-8"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

// Tipo de desenho
#define LINHA		'1'
#define RETANGULO	'2'
#define ELIPSE		'3'

//--------------------------------------------------------------------------
// Vari�veis globais
//--------------------------------------------------------------------------
// Armazena tipo de desenho
char cTipo = '1';

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
  wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcl.lpszMenuName = NULL;
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

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

  // Canetas e pinc�is
  HPEN hPen = NULL;
  HPEN hPenOld = NULL;
  HBRUSH hBrush = NULL;
  HBRUSH hBrushOld = NULL;

  // Armazena coordenadas (x, y) do cursor do mouse
  static POINT ptMouseInicio;
  static POINT ptMouseFim;
  
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

		  // Mostra informa��o na tela
		  TextOut(hDC, 8, 8, "1: Linha, 2: Ret�ngulo, 3: Elipse", strlen("1: Linha, 2: Ret�ngulo, 3: Elipse"));

			// Libera DC e valida �rea
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

		case WM_CHAR: // Obt�m o c�digo ASCII da tecla pressionada
		{
		  // Verifica tecla pressionada e modifica tipo de desenho (vari�vel cTipo)
			switch((char)wParam)
			{
				case LINHA:
				{
					cTipo = LINHA;
				} break;

				case RETANGULO:
				{
					cTipo = RETANGULO;
				} break;

				case ELIPSE:
				{
					cTipo = ELIPSE;
				} break;
			}

			return(0);
		} break;

		case WM_LBUTTONDOWN: // Bot�o esquerdo do mouse pressionado
		{
			// Obt�m coordenada (x, y) atual do cursor do mouse
			ptMouseInicio.x = LOWORD(lParam);
			ptMouseInicio.y = HIWORD(lParam);

			return(0);
		} break;

		case WM_LBUTTONUP: // Bot�o esquerdo do mouse foi solto
		{
		  // Obt�m coordenada (x, y) atual do cursor do mouse
			ptMouseFim.x = LOWORD(lParam);
			ptMouseFim.y = HIWORD(lParam);

			// Obt�m identificador do DC
			hDC = GetDC(hWnd);

			// Cria e seleciona nova caneta no DC e salva caneta antiga
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
			hPenOld = (HPEN)SelectObject(hDC, hPen);

			// Cria e seleciona novo pincel no DC e salva pincel antigo
			hBrush = CreateSolidBrush(RGB(128, 0, 0));
			hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
  
			// Verifica tipo de desenho
			switch(cTipo)
			{
				// Desenha linha
				case LINHA:
				{
					MoveToEx(hDC, ptMouseInicio.x, ptMouseInicio.y, NULL);
					LineTo(hDC, ptMouseFim.x, ptMouseFim.y);
				} break;

				// Desenha ret�ngulo
				case RETANGULO:
				{
					Rectangle(hDC, ptMouseInicio.x, ptMouseInicio.y, ptMouseFim.x, ptMouseFim.y);
				} break;

				// Desenha elipse
				case ELIPSE:
				{
					Ellipse(hDC, ptMouseInicio.x, ptMouseInicio.y, ptMouseFim.x, ptMouseFim.y);
				}	break;
			}

			// Restaura pincel antigo e deleta novo pincel
			SelectObject(hDC, hBrushOld);
			DeleteObject(hBrush);

			// Restaura caneta antiga e deleta nova caneta
			SelectObject(hDC, hPenOld);
			DeleteObject(hPen);

			// Libera DC
			ReleaseDC(hWnd, hDC);

			return(0);
		} break;

		default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que n�o foram verificadas na fun��o */
		  return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

  }
}