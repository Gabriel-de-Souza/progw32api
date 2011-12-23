//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog01.cpp - Programa Windows estilo "Hello World!"
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// WinMain() -> Fun��o principal
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  MessageBox(NULL, "Esse � o Primeiro Programa-Exemplo!", "Prog 01", MB_OK);
  return(0);
}
