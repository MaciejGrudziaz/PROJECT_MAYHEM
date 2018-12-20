#include "DIRECTINPUT.h"

//inicjalizacja zmiennych statycznych
DIRECTINPUT_KEYBOARD* DIRECTINPUT::m_pKeyboard = NULL;
DIRECTINPUT_MOUSE* DIRECTINPUT::m_pMouse = NULL;
LPDIRECTINPUT8 DIRECTINPUT::m_pDI = NULL;

//--------------------------------------------------------------------------------------------------------------
//-----------------------------------------------MAKRO FAILED(..)-----------------------------------------------
//----------------zwraca warto�� 'TRUE' je�li operacja wywo�ana wewn�trz makra nie powiod�a si�-----------------
//------------------------------jesli operacj powiod�a si� zwraca warto�� 'FALSE'-------------------------------
//--------------------------------------------------------------------------------------------------------------
//**************************************************************************************************************
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------KLAWIATURA--------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

//konstruktor dla systemu wej�cia klawiatury [pobiera wska�nik do obiekty systemu wej�cia oraz uchwyt okna WinAPI]
DIRECTINPUT_KEYBOARD::DIRECTINPUT_KEYBOARD(LPDIRECTINPUT8 pDI, HWND hwnd)
{
	//stworzenie obiektu klawiatury
	pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL);

	//ustawienie formatu danych [domy�lny]
	m_pDIDev->SetDataFormat(&c_dfDIKeyboard);

	//ustawienie trybu pracy [apliakcja korzysta z systemu wejscia je�li jest na pierwszym planie i nie koliduje z innymi]
	m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//pobranie dost�pu do systemu wej�cia
	m_pDIDev->Acquire();

	//wyczyszczenie tablicy sygna��w wej�ciwoych
	Clear();
}

//destruktor 
DIRECTINPUT_KEYBOARD::~DIRECTINPUT_KEYBOARD()
{
	//je�li obiekt klawaitury zosta� stworzony
	if (m_pDIDev)
	{
		//zwolnienie systemu wej�cia klawiatury
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

//od�wie�enia sygna��w wej�ciowych z klawiatury
bool DIRECTINPUT_KEYBOARD::Update()
{
	//pobranie stanu klawiatury i zapisanie go do do tablicy 'm_keys'
	if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
	{
		//je�li operacja nie powiod�a si� to pobieramy dost�p do sysemtu wej�cia z klawiatury
		if (FAILED(m_pDIDev->Acquire()))
		{
			return false;
		}
		//ponowana pr�ba wczytania stanu kalwiatury i zapisania go do tablicy 'm_keys'
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
		{
			return false;
		}
	}
	return true;
}

//pobranie dost�pu do systemu wej�cia z klawiatury
bool DIRECTINPUT_KEYBOARD::Acquire()
{
	//wyczyszczenie tablicy sygna��w wej�ciowych
	Clear();
	return (!FAILED(m_pDIDev->Acquire()));
}

//zwolnienie dostepu do systemu wejscia z klawiatury
bool DIRECTINPUT_KEYBOARD::Unacquire()
{
	//wyczyszczenie tablicy sygna��w wej�ciowych
	Clear();
	return (!FAILED(m_pDIDev->Unacquire()));
}

//--------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------MYSZ-----------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

//konstruktor dla systemu wej�cia myszy [pobiera wska�nik dla interefejsu systemu g��wnego wejscia Direct Input, uchwyt okna apliakcji WinAPI oraz 
//znacznik czy mysz ma by� na wy��czno�� aplikacji czy nie]
DIRECTINPUT_MOUSE::DIRECTINPUT_MOUSE(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive)
{
	//stworznie obiektu dla systemu wej�cia myszy
	pDI->CreateDevice(GUID_SysMouse, &m_pDIDev, NULL);

	//usatwinie formatu danych [domy�lny]
	m_pDIDev->SetDataFormat(&c_dfDIMouse);

	//ustawienie flag w zale�no�ci od zancznika wy��czno�ci
	DWORD flags;
	//apliakcja u�ywa myszy gdy jest na pierwszym planie, na wy�aczno�� oraz dezaktywuje dzia�anie przycisku Windows
	if (isExclusive) flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	//aplikacja u�ywa myszy gdy jest na piewszym planie oraz gdy nie koliduje z innymi aplikacjami
	else flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

	//usatwienie trybu pracy w zale�no�ci od warto�ci flag
	m_pDIDev->SetCooperativeLevel(hwnd, flags);

	//pobranie dost�pu do systemu wej�cia myszy
	m_pDIDev->Acquire();

	//pobranie stanu myszy oraz zapisanie go do struktry m_state
	m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
}

//destruktor
DIRECTINPUT_MOUSE::~DIRECTINPUT_MOUSE()
{
	//je�li obiekt myszy istnieje
	if (m_pDIDev)
	{
		//zwolnij dost�p do systemu wej�cia myszy
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

//uaktualnienie danych dla sygna��w systemu wej�cia myszy
bool DIRECTINPUT_MOUSE::Update()
{
	//pobranie stanu urz�dzenia i zapisanie go do struktury 'm_state'
	if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
	{
		//je�li operacja nie powiod�a si� nasepuje pr�ba pobrania dostepu do systemu wejs�ia myszy
		if (FAILED(m_pDIDev->Acquire()))
		{
			return false;
		}
		//ponowna pr�ba pobrania stanu urz�dzenia i zapisania go do struktury 'm_state'
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
		{
			return false;
		}
	}

	return true;
}


//pobranie dost�pu do systemu wej�cia myszy
bool DIRECTINPUT_MOUSE::Acquire()
{
	return (!FAILED(m_pDIDev->Acquire()));
}

//zwolnienie dost�pu do systemu wejscia myszy
bool DIRECTINPUT_MOUSE::Unacquire()
{
	return (!FAILED(m_pDIDev->Unacquire()));
}

//--------------------------------------------------------------------------------------------------------------
//-------------------------------------------------DIRECTINPUT--------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

//inicjalizacja systemu wej�cia DirectInput oraz jego sk�adowych [klawiatury, myszy]
//jako argumenty przyjmuje znacznik czy system wej�cia ma by� 'ekskluzywny' dla aplikacji oraz flagi jakie sytemy wej�cia wczyta� [mysz/klawiatur�]
bool DIRECTINPUT::Initialize(bool isExclusive, DWORD flags)
{
	//stworzenie g��wnego systemu wej�cia Direct Input okre�lonego wska�nikiem 'm_pDI' dla okna instancji 'WinAPIwindow::hInstance'
	if (FAILED(DirectInput8Create(WinAPIwindowManager::GethInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL))) return false;
	//je�li flaga obs�ugi klawiatury jest ustawiona
	if (flags & IS_USEKEYBOARD)
	{
		//stw�rz nowy obiekt systemu wej�cia dla klawiatury
		m_pKeyboard = new DIRECTINPUT_KEYBOARD(m_pDI, WinAPIwindowManager::GetMainWindow()->GetHWND());
		//je�li operacja si� nie powiod�� zwr�� komunikat b��du
		if (m_pKeyboard == NULL) return false;
	}
	//je�li flaga obs�ugi myszy jest ustawiona
	if (flags & IS_USEMOUSE)
	{
		//stw�rz nowy obiekt systemu wej�cia dla myszy
		m_pMouse = new DIRECTINPUT_MOUSE(m_pDI, WinAPIwindowManager::GetMainWindow()->GetHWND(), isExclusive);
		//je�li operacja nie powiod�a si� zwr�� b��d
		if (m_pMouse == NULL) return false;
	}
	//pobierz dost�p do wszystkich obiekt�w systemu wej�cia
	AcquireAll();
	//od�wie� stan sygna��w wej�ciowych system�w wej�cia
	Update();

	return true;
}

//koniec pracy systemu wej�cia ['zamytkanie' obecnego systemu wej�cia]
bool DIRECTINPUT::Shutdown()
{
	//zwolnij dost�p do wszystkich system�w wej�cia w systemie
	UnacquireAll();
	//jesli obiekt klawiatury istnieje
	if (m_pKeyboard)
	{
		//usuni�cie obiektu klawiatury
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}
	//je�li obiekt myszy istnieje
	if (m_pMouse)
	{
		//usuni�cie obiektu myszy
		delete m_pMouse;
		m_pMouse = NULL;
	}
	//zwolnienie dost�pu do g��wnego systemu wej�cia
	if (FAILED(m_pDI->Release())) return false;

	return true;
}

//uaktulanienie stan�w sygna��w system�w wej�cia
bool DIRECTINPUT::Update()
{
	if (m_pKeyboard) m_pKeyboard->Update();

	if (m_pMouse) m_pMouse->Update();

	return true;
}

//pobranie dost�pu do wszystkich obecnych system�w wej�cia
void DIRECTINPUT::AcquireAll()
{
	if (m_pKeyboard) m_pKeyboard->Acquire();

	if (m_pMouse) m_pMouse->Acquire();
}

//zwolnienie dost�pu do wszystkich obecnych system�w wej�cia
void DIRECTINPUT::UnacquireAll()
{
	if (m_pKeyboard) m_pKeyboard->Unacquire();

	if (m_pMouse) m_pMouse->Unacquire();
}

int DIRECTINPUT::GetMouseMovevementAction(int keyCode) {
	int dx, dy;
	if (m_pMouse) {
		m_pMouse->GetMovement(dx, dy);
		if (keyCode == DIK_MOUSE_UP && dy < 0)
			return abs(dy);
		else if (keyCode == DIK_MOUSE_DOWN && dy > 0)
			return dy;
		else if (keyCode == DIK_MOUSE_LEFT && dx < 0)
			return abs(dx);
		else if (keyCode == DIK_MOUSE_RIGHT && dx > 0)
			return dx;
		else return 0;

	}
	else return 0;
}
