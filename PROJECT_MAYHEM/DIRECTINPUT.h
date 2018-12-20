#pragma once
#include <dinput.h>
#include "WinAPIwindowManager.h"

#define IS_USEKEYBOARD 0x01
#define IS_USEMOUSE 0x02
#define DIK_MOUSE_UP 0xF0
#define DIK_MOUSE_DOWN 0xF1
#define DIK_MOUSE_LEFT 0xF2
#define DIK_MOUSE_RIGHT 0xF3

//----------------------------------------------------------------------------------------------------
//--------------------------------------OBS�UGA SYSTEMU WEJ�CIA---------------------------------------
//--------------------obs�uga systemu wej�cia przy u�yciu biblioteki DirectX Input--------------------
//--------------------------------obs�uga wej�cia z myszy i klawiatury--------------------------------
//----------------------------------------------------------------------------------------------------

//wej�cie z klawiatury
class DIRECTINPUT_KEYBOARD
{
	LPDIRECTINPUTDEVICE8 m_pDIDev;										//wska�nik dla interfejsu wej�cia poprzez Direct Input
	unsigned char m_keys[256];											//tablica przetrzymuj�ca aktualne warto�ci wej�cia z klawiatury
public:
	DIRECTINPUT_KEYBOARD(LPDIRECTINPUT8 pDI, HWND hwnd);				//konstruktor
	~DIRECTINPUT_KEYBOARD();											//destruktor

	bool KeyDown(int key) {												//metoda zwracaj�ca aktualny stan wci�ni�cia przycisku okre�lonego kodem 'key'
		if (m_keys[key]) return true;
		else return false;
	}
	bool KeyUp(int key) {												//metoda zwracja�ca aktualny stan puszczenia przycisk okre�lonego kodem 'key'
		if (m_keys[key]) return false;
		else return true;
	}

	bool Update();														//uaktualnienie tablicy wej��

	void Clear() { ZeroMemory(m_keys, 256 * sizeof(char)); }			//wyczyszczenie z pami�ci tablicy 'm_keys'

	bool Acquire();														//pobranie dost�pu do systemu wej�cia (klawiatury)
	bool Unacquire();													//zwolnienie dost�pu do systemu wej�cia (klawiatury)
};

//wej�cie z myszy
class DIRECTINPUT_MOUSE
{
	LPDIRECTINPUTDEVICE8 m_pDIDev;										//wska�nik dla interfejsu wej�cia Direct Input
	DIMOUSESTATE m_state;												//struktura opisuj�ca stan myszy							
public:
	//konstruktor
	DIRECTINPUT_MOUSE(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
	~DIRECTINPUT_MOUSE();												//desktruktor

																		//funkcje zwracaj�ce stan przycisk�w myszy
	bool ButtonDown(int button) { return (m_state.rgbButtons[button] & 0x80) ? true : false; }
	bool ButtonUp(int button) { return (m_state.rgbButtons[button] & 0x80) ? false : true; }
	//zwraca przesuni�cie k�ka myszki [pozycja wzgl�dna]
	int GetWheelMovement() { return  m_state.lZ; }
	//zwraca przemieszczenie myszy [pozycja wzgl�dna]
	void GetMovement(int &dx, int &dy) { dx = m_state.lX; dy = m_state.lY; }

	bool Update();														//od�wie�enie stanu myszy

	bool Acquire();														//pobranie dost�pu do systemu wej�cia [myszy]
	bool Unacquire();													//zwolnienie dost�pu do systemu wej�cia [myszy]
};

class DIRECTINPUT
{
	DIRECTINPUT();														//konstruktor domy�lny

	static DIRECTINPUT_KEYBOARD* m_pKeyboard;							//wska�nik na obiekt klawiatury
	static DIRECTINPUT_MOUSE* m_pMouse;									//wska�nik na obiekt myszy
	static LPDIRECTINPUT8 m_pDI;										//wska�nik dla interfejsu Direct Input

public:
	//inicjalizacja systemu wej�cia Direct Input
	static bool Initialize(bool isExclusive = true, DWORD flags = IS_USEKEYBOARD | IS_USEMOUSE);
	static bool Shutdown();												//zamkni�cie/zwolnienie systemu wej�cia 

	static void AcquireAll();											//pobranie wszystkich element�w systemu wej�cia
	static void UnacquireAll();											//zwolnienie wszystkich element�w systemu wej�cia

	static DIRECTINPUT_KEYBOARD* GetKeyboard() { return m_pKeyboard; }	//zwr�cenie wska�nika na obiekt klawiatury
	static DIRECTINPUT_MOUSE* GetMouse() { return m_pMouse; }			//zwr�cenie wska�nika na obiekt myszy

	static bool Update();												//aktualizowanie system�w wej�cia

																		//funkcje sprawdzaj�ce stan wci�ni�cia danego klawisza na klawiaturze
	static bool KeyDown(int key) { return (m_pKeyboard && m_pKeyboard->KeyDown(key)); }
	static bool KeyUp(int key) { return (m_pKeyboard && m_pKeyboard->KeyUp(key)); }

	//funkcje sprawdzaj�ce stan wci�ni�cia danego przycisku na myszy
	static bool ButtonDown(int button) { return (m_pMouse && m_pMouse->ButtonDown(button)); }
	static bool ButtonUp(int button) { return (m_pMouse && m_pMouse->ButtonUp(button)); }

	//sprawdzenie akcji myszy
	static int GetMouseMovevementAction(int keyCode);
	//pobranie przemiszczenia myszy [pozycji wzgl�dnej]
	static void GetMouseMovement(int &dx, int &dy) { if (m_pMouse) m_pMouse->GetMovement(dx, dy); }
	//pobranie przemieszczenia rolki myszy
	static int GetMouseWheelMovement() { return (m_pMouse) ? m_pMouse->GetWheelMovement() : 0; }
};