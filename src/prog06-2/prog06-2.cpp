//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog06-2.cpp - DIB Section
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <math.h>

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog06-2"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 06-2"
// Largura da janela 
#define WINDOW_WIDTH		328
// Altura da janela 
#define WINDOW_HEIGHT		267

//--------------------------------------------------------------------------
// Vari�veis globais
//--------------------------------------------------------------------------
HDC hMemDC = NULL; // DC de mem�ria
HBITMAP hBmp = NULL; // Bitmap compat�vel

//--------------------------------------------------------------------------
// Prot�tipo das fun��es
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void ManipulaPixels(void);
void Contraste(DIBSECTION, float);

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
  
  // Manipula pixels do bitmap apenas pela primeira vez que � mostrada na tela
  static bool bPVez = true;

  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {
    case WM_CREATE: // Janela foi criada
    {
      // Cria DC de mem�ria (hMemDC � global)
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

			// Cria DIB Section, carregando bitmap do arquivo "frutas_24bit.bmp" (hBmp � global)
			hBmp = (HBITMAP)LoadImage(NULL, "frutas_24bit.bmp", IMAGE_BITMAP, 320, 240, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

      // Caso n�o encontre o arquivo, mostra mensagem de erro
      if(!hBmp)
				MessageBox(hWnd, "N�o foi poss�vel carregar arquivo \"frutas_24bit.bmp\"", "Erro!", MB_ICONERROR);

			// Seleciona bitmap no DC de mem�ria (configura DC de mem�ria)
      SelectObject(hMemDC, hBmp);

			// Libera DC de v�deo
			ReleaseDC(hWnd, hDC);

      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;

    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
      // Obt�m DC de v�deo
      hDC = BeginPaint(hWnd, &psPaint);

			if(hBmp)
			{
				// Manipula os pixels do bitmap somente uma vez
				if(bPVez)
				{
					ManipulaPixels();
					bPVez = false;
				}

				// Faz transfer�ncia de bits entre os DC's de mem�ria e v�deo
				BitBlt(hDC, 0, 0, 320, 240, hMemDC, 0, 0, SRCCOPY);
			}

      // Libera DC de v�deo
      EndPaint(hWnd, &psPaint);

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

void ManipulaPixels(void)
{
  DIBSECTION dibSect; // Informa��es do bitmap DIB Section
  unsigned char *pImgBits; // Ponteiro para bits do bitmap

  // Preenche estrutura DIBSECTION com informa��es do bitmap
  // e faz pImgBits apontar para os bits do bitmap
  GetObject((HBITMAP)hBmp, sizeof(DIBSECTION), (LPVOID)&dibSect);
  pImgBits = (unsigned char *)dibSect.dsBm.bmBits;

  // Converte RGB para tons de cinza
  for(int i = 0; i < dibSect.dsBmih.biSizeImage; i += 3)
  {
    // Obt�m os valores RGB dos pontos da imagem
    int R = pImgBits[i + 2]; // Red (Vermelho)
    int G = pImgBits[i + 1]; // Green (Verde)
    int B = pImgBits[i];     // Blue (Azul)

    // Calcula luminance / brilho / intensidade de luz
    R = (R << 6) + (R << 3) + (R << 2) + 1; 		// R * 77
    G = (G << 7) + (G << 4) + (G << 2) + (G << 1); 	// G * 150
    B = (B << 4) + (B << 3) + (B << 2) + 1; 		// B * 29
    int L = (R + G + B) >> 8; // L = (R + G + B) / 256

    // Converte para tons de cinza	
    pImgBits[i + 2] = L; // Red (Vermelho)
    pImgBits[i + 1] = L; // Green (Verde)
    pImgBits[i]     = L; // Blue (Azul)
  }

  Contraste(dibSect, 0.2);

  // Cria um arquivo para depura��o / visualizar dados do DIB Section
  FILE *fp = fopen("bitmap_info.txt", "w");
  fprintf(fp, "DIBSECTION::BITMAP::bmType = %d\n", dibSect.dsBm.bmType);
  fprintf(fp, "DIBSECTION::BITMAP::bmWidth = %d\n", dibSect.dsBm.bmWidth);
  fprintf(fp, "DIBSECTION::BITMAP::bmHeight = %d\n", dibSect.dsBm.bmHeight);
  fprintf(fp, "DIBSECTION::BITMAP::bmWidthBytes = %d\n", dibSect.dsBm.bmWidthBytes);
  fprintf(fp, "DIBSECTION::BITMAP::bmPlanes = %d\n", dibSect.dsBm.bmPlanes);
  fprintf(fp, "DIBSECTION::BITMAP::bmBitsPixel = %d\n", dibSect.dsBm.bmBitsPixel);
  fprintf(fp, "DIBSECTION::BITMAP::bmBits = %d\n", dibSect.dsBm.bmBits);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biSize = %d\n", dibSect.dsBmih.biSize);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biWidth = %d\n", dibSect.dsBmih.biWidth);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biHeight = %d\n", dibSect.dsBmih.biHeight);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biPlanes = %d\n", dibSect.dsBmih.biPlanes);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biBitCount = %d\n", dibSect.dsBmih.biBitCount);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biCompression = %d\n", dibSect.dsBmih.biCompression);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biSizeImage = %d\n", dibSect.dsBmih.biSizeImage);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biXPelsPerMeter = %d\n", dibSect.dsBmih.biXPelsPerMeter);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biYPelsPerMeter = %d\n", dibSect.dsBmih.biYPelsPerMeter);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biClrUsed = %d\n", dibSect.dsBmih.biClrUsed);
  fprintf(fp, "DIBSECTION::BITMAPINFOHEADER::biClrImportant = %d\n", dibSect.dsBmih.biClrImportant);
  fprintf(fp, "DIBSECTION::dsBitfields[0] = %d\n", dibSect.dsBitfields[0]);
  fprintf(fp, "DIBSECTION::dsBitfields[1] = %d\n", dibSect.dsBitfields[1]);
  fprintf(fp, "DIBSECTION::dsBitfields[2] = %d\n", dibSect.dsBitfields[2]);
  fclose(fp);
}

void Contraste(DIBSECTION dibSect, float fGamma)
{
  // Ponteiro para bits do bitmap
  unsigned char *pImgBits = (unsigned char *)dibSect.dsBm.bmBits;

  // Configura o contraste da imagem
  for(int i = 0; i < dibSect.dsBmih.biSizeImage; i += 3)
  {
    // Obt�m os valores RGB dos pontos da imagem
    int R = pImgBits[i + 2]; // Red (Vermelho)
    int G = pImgBits[i + 1]; // Green (Verde)
    int B = pImgBits[i];     // Blue (Azul)

    // Aplica novo contraste
    R = 255 * pow(R / 255.0, fGamma);
    G = 255 * pow(G / 255.0, fGamma);
    B = 255 * pow(B / 255.0, fGamma);

    // Atualiza os pontos da imagem
    pImgBits[i + 2] = R; // Red (Vermelho)
    pImgBits[i + 1] = G; // Green (Verde)
    pImgBits[i]     = B; // Blue (Azul)
  }
}