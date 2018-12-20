#include "WinAPIwindow.h"

WinAPIwindow::WinAPIwindow(std::string name_, unsigned screenWidth_, unsigned screenHeight_, unsigned posX_, unsigned posY_, double fov_, bool fullscreen_) {
	windowName = name_;
	screenWidth = screenWidth_;
	screenHeight = screenHeight_;
	posX = posX_;
	posY = posY_;
	fov = fov_;
	fullscreen = fullscreen_;
	hwnd=nullptr;
	hDC=nullptr;
	hRC=nullptr;
	hInstance=nullptr;

	unsigned n = windowName.size();
	className = new char[n + 7];
	strcpy(className, windowName.c_str());
	strcpy(className + n, "_CLASS");
}

//ustawienie w�a�ciwo�ci i stworzenie okna [pobranie uchwytu okna HWND i kontekstu urz�dzenia HDC]
int WinAPIwindow::SetupWindowSettings() {
	InitializeWindowClassEx();				//inicjalizacja klasy okna WinAPI

	if (!RegisterClassEx(&(windowClass)))			//rejestracja klasy okna
		return 0;

	if (!CreateNewWindow())							//stworzenie nowego okna
		return 0;

	return 1;
}

//stworzenie nowego okna [stworzenie samego nowego uchwytu i kontekstu urz�dzenia bez nowej klasy]
int WinAPIwindow::CreateNewWindow() {
	if (fullscreen)
		SetupFullscreenSettings();					//ustawienie w�a�ciwo�ci dla trybu pe�noekranowego

	ShowCursor(TRUE);								//ukrycie kursora

	if (!Create())							//stworzenie okna aplikacji
		return 0;

	//SetCapture(hwnd);

	return 1;
}

int WinAPIwindow::CreateGraphicContext(HWND hwnd_) {
	hwnd = hwnd_;
	hDC = GetDC(hwnd);
	if (!hDC)
		return 0;
	SetupPixelFormat();
	hRC = wglCreateContext(hDC);
	if (!hRC)
		return 0;

	//wglMakeCurrent(hDC, hRC);

	return 1;
}

//funkcja s�u��ca do zwolnienia kontekstu grafiki OpenGL dla kontekstu urz�dzenia WinAPI
void WinAPIwindow::ReleaseGraphicContext() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void WinAPIwindow::DeleteGraphicsContext() {
	wglDeleteContext(hRC);
}

//funkcja wy�wietlaj�ca i aktualizuj�ca okno WinAPI
void WinAPIwindow::ShowWinAPIwindow() {
	ShowWindow(hwnd, SW_SHOW);						//wy�wietlenie i aktualizacja okna WinAPI
	UpdateWindow(hwnd);
}

//funkcja ustawiaj�ca dane opcje dla ekranu apliakcji OpenGL
int WinAPIwindow::SetupOpenGLScreenSettings() {
	//nadanie wymiaru okna OpenGL
	//glViewport(0, 0, screenWidth, screenHeight);

	//glMatrixMode(GL_PROJECTION);						//okre�lenie macierzy rzutowania
	//glLoadIdentity();									//zresetowanie macierz rzutowania

														//wyznaczenie proporcji obrazu i pola widzenia
	//gluPerspective(fov, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.01f, 1000.0f);

	//glMatrixMode(GL_MODELVIEW);							//wybranie macierzy widoku modelu
	//glLoadIdentity();									//zresetowanie macierzy widoku
	
	return 1;
}

void WinAPIwindow::UpdateProjectionMatrix() {
	projection = glm::perspective(glm::radians(static_cast<float>(fov)), static_cast<float>(screenWidth)/ static_cast<float>(screenHeight),0.1f, 1000.0f);
}

//inicjalizacja klasy okna WNDCLASSEX
void WinAPIwindow::InitializeWindowClassEx() {
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
}

//okreslenie podstawowego stylu okna w zale�no�ci od trybu wy�wietlania [pe�noekranowy/okienkowy]
DWORD WinAPIwindow::GetWindowStyle() {
	DWORD dwStyle;									//podstawowy styl okna

	if (fullscreen)
		dwStyle = WS_POPUP;
	else
		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU;

	return dwStyle;
}

//okre�lenie rozszerzonego trybu okna w zale�no�ci od trybu wy�wietlania [pe�noekranowy/okeinkowy]
DWORD WinAPIwindow::GetWindowExStyle() {
	DWORD dwExStyle;								//rozszerzony styl okna

	if (fullscreen)
		dwExStyle = WS_EX_APPWINDOW;
	else
		dwExStyle = NULL;

	return dwExStyle;
}

//stworzenie okna aplikacji oraz pobranie jego uchwytu [HWND]
int WinAPIwindow::Create() {
	DWORD dwStyle, dwExStyle;						//style okna [podstawowy - dwStyle i rozszerzony - dwExStyle]

	dwStyle = GetWindowStyle();						//pobranie podstawowego stylu okna
	dwExStyle = GetWindowExStyle();					//pobranie rozszerzonego stylu okna

													//stworzenie okna aplikacji oraz pobranie jego uchwytu
	CreateWindowEx(dwExStyle,
		className,
		windowName.c_str(),
		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		posX, posY,
		screenWidth, screenHeight,
		NULL,
		NULL,
		hInstance,
		this);

	if (!hwnd)										//sprawdzenie czy okno zosta�o stworzone
		return 0;

	return 1;
}

//ustawienie w�a�ciwo�ci okna dla trybu pe�noekranowego [stworzenie struktur DEVMODE i RECT]
void WinAPIwindow::SetupFullscreenSettings() {
	DWORD dwStyle, dwExStyle;						//style okna

	dwStyle = GetWindowStyle();						//pobranie styl�w okna
	dwExStyle = GetWindowExStyle();

	//sktruktura DEVMODE zawieraj�ca informacj� o inicjalizacji i konfiguracji urz�dzenia wyj�ciowego [ekranu]
	DEVMODE dmScreenSettings;
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = screenWidth;
	dmScreenSettings.dmPelsHeight = screenHeight;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	//zmiana trybu wy�wietlania na pe�noekranowy
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		fullscreen = false;

	///struktura RECT okre�laj�ca prostok�t zawarto�ci okna w systemie Windows
	RECT windowRect;
	windowRect.top = 0;								//g�rny, lewy naro�nik okna
	windowRect.left = 0;
	windowRect.bottom = screenHeight;				//dolny, prawy naro�nik okna
	windowRect.right = screenWidth;

	//funkcja wyznaczaj�ca rozmiar okna systemu Windows na podstawie zadanych rozmiar�w prostok�ta zawarto�ci [struktura RECT]
	//pozwala to aplikacji OpenGL wykorzytsa� maksymalnie rozmiar okna systemu
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
}

//wybranie i usatwienie foramtu pikseli
void WinAPIwindow::SetupPixelFormat() {
	int nPixelFormat;									//indeks formatu pikseli

														//struktura okre�laj�ca u�ywany foramt pikseli
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),					//rozmiar struktury
		1,												//domy�lna wersja
		PFD_DRAW_TO_WINDOW |							//grafika w oknie
		PFD_SUPPORT_OPENGL |							//grafika OpenGL
		PFD_DOUBLEBUFFER,								//podw�jne buforowanie
		PFD_TYPE_RGBA,									//tryb kolor�w RGBA
		32,												//32-bitowy opis kolor�w
		0, 0, 0, 0, 0, 0,								//nie specyfikuje bit�w kolor�w
		0,												//bez bufora alfa
		0,												//ignoruje bit przesuni�cia
		0,												//bez bufora akumulacji
		0, 0, 0, 0,										//ignoruje bity akumulacji
		32,												//32-bitowy bufor Z
		0,												//bez bufora powielania
		0,												//bez bufor�w pomocniczych
		PFD_MAIN_PLANE,									//g��wna p�aszczyzna rysowania
		0,												//zarezerwowane
		0, 0, 0 };										//ignoruje maski warstw

														//wybiera najodpowiedniejszy format pikseli z dostepnych
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	//okre�la format pikseli dla kontekstu urz�dzenia
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void WinAPIwindow::Cursor(bool show) { 
	if (show) ShowCursor(TRUE); 
	else ShowCursor(FALSE); 
}

WinAPIwindow::~WinAPIwindow() {
	delete[]className;
}
