//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog07-2.cpp - Janelas n�o-retangulares
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog07-2"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 07-2"
// Largura da janela 
#define WINDOW_WIDTH		200
// Altura da janela 
#define WINDOW_HEIGHT		394

//--------------------------------------------------------------------------
// Prot�tipo das fun��es
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void WindowSkin(HWND, HDC, HBITMAP);

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
		   WS_POPUP | WS_VISIBLE, // N�o utiliza barra de t�tulo no programa
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

	// Identificadores do bitmap e DC de mem�ria
	static HBITMAP hBmp = NULL;
	static HDC hMemDC = NULL;
  
  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {
    case WM_CREATE: // Janela foi criada
    {
      // Cria DC de mem�ria (hMemDC � global)
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

			// Cria DIB Section, carregando bitmap do arquivo "skin.bmp"
			hBmp = (HBITMAP)LoadImage(NULL, "skin.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			// Modifica janela do programa
			WindowSkin(hWnd, hMemDC, hBmp);

			// Libera DC de v�deo
			ReleaseDC(hWnd, hDC);

      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;

    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
      // Obt�m DC de v�deo
      hDC = BeginPaint(hWnd, &psPaint);

			// Faz transfer�ncia de bits entre os DC's de mem�ria e v�deo
			BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hMemDC, 0, 0, SRCCOPY);

      // Libera DC de v�deo
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

		case WM_LBUTTONDOWN: // Bot�o esquerdo do mouse pressionado
		{
			// Se a posi��o y do cursor do mouse for menor que 100
			if(HIWORD(lParam) < 100)
				// Envia mensagem WM_NCLBUTTONDOWN, indicando que foi um clique na barra de t�tulo
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
			
			return(0);
		} break;

    case WM_CLOSE: // Janela foi fechada
    {
      // Deleta bitmap
      DeleteObject(SelectObject(hMemDC, hBmp));

      // Deleta DC de mem�ria
      DeleteDC(hMemDC);

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

		default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que n�o foram verificadas na fun��o */
			return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    } break;

  }
}

//--------------------------------------------------------------------------
// WindowSkin() -> Cria janela no formato de uma figura
//--------------------------------------------------------------------------
void WindowSkin(HWND hWnd, HDC hMemDC, HBITMAP hBmp)
{
	// Cria regi�o no formato da figura
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	HRGN hRgnTemp = hRgn;

	// Cor transparente
	COLORREF rgbTransparente = RGB(255, 0, 255);

	// Salva o primeiro ponto n�o-transparente da posi��o x da imagem
	int ix = 0;

	// Seleciona bitmap no DC de mem�ria (configura DC de mem�ria)
	SelectObject(hMemDC, hBmp);

	// Varre o bitmap para obter cores e o formato da figura
	for(int y = 0; y <= WINDOW_HEIGHT; y++)
	{
		for(int x = 0; x <= WINDOW_WIDTH; x++)
		{
			// Verifica se a cor do ponto atual � transparente
			if(GetPixel(hMemDC, x, y) != rgbTransparente)
			{
				// Se n�o for, salva o primeiro ponto n�o-transparente da posi��o x da imagem
				ix = x;

				// E incrementa a posi��o x at� achar um ponto que � transparente
				// ou at� chegar ao final da imagem (na horizontal)
				while((x <= WINDOW_WIDTH) && (GetPixel(hMemDC, x, y) != rgbTransparente))
					x++;

				// Depois cria uma regi�o tempor�ria onde:
				// x = primeiro ponto n�o-transparente at� �ltimo ponto n�o-transparente
				// y = posi��o y atual at� pr�ximo y
				hRgnTemp = CreateRectRgn(ix, y, x, y + 1);

				// Combina a regi�o atual (hRgn) com a regi�o tempor�ria (hRgnTemp)
				CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
			}
		}
	}

	// Depois que varreu todo o bitmap, verificando seu formato (de acordo com
	// as partes transparentes e n�o-transparentes da imagem), define o formato
	// da janela conforme a regi�o atual (hRgn).
	SetWindowRgn(hWnd, hRgn, TRUE);
}