
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#endif /*_WIN32*/

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>


#include "audio/audioManager.hpp"
#include "input/inputManager.hpp"
#include "log/logmanager.hpp"

#include "message/message.hpp"
#include "message/TEXEngineInterface.hpp"

#include "physics/PhysicsManager.hpp"

#include "port/Timer.h"

#include "renderer/renderManager.hpp"

#include "ui/uiManager.hpp"
#include "ui/label.hpp"
#include "ui/uiImage.hpp"

#include "util/ncf.hpp"
#include "util/nstring.hpp"
#include "GameEventCodes.h"
#include "GameLogicManager.hpp"

#ifdef _WIN32

HWND hWnd = NULL;
HDC hDC = NULL;
HGLRC hRC = NULL;
HINSTANCE hInstance;

#endif /*_WIN32*/

bool active = true;

const double MEDIUM_FIRE_RATE = 0.225;

#ifdef	_WIN32
	const double SLOW_FIRE_RATE = 0.45;
#else
	const double SLOW_FIRE_RATE = 0.25;
#endif

using namespace TEXEngine::UI;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;
using namespace std;

#ifdef _WIN32

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL WINAPI	console_proc(DWORD);


void KillGLWindow(void){
	ChangeDisplaySettings(NULL, 0);
	if (hWnd && !DestroyWindow(hWnd)){
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;
	}

	if (!UnregisterClass("X-Sledding3000",hInstance)){
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;	
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreen){
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	hInstance			= GetModuleHandle(NULL);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "X-Sledding3000";

	if (!RegisterClass(&wc)) {
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	


	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;				// Window Extended Style
	dwStyle = GameLogicManager::PROPER_WINDOW_ATTRIBUTES;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	
	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"X-Sledding3000",					// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								5, 5,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		//KillGLWindow(); //TODO
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}	
	GameLogicManager::Get().m_hwnd = hWnd;
	if(fullscreen)
		GameLogicManager::Get().switch_to_fullscreen(fullscreen, width, height, true);

	TEXEngine::Core::UIManager::get().aspect_modifier((float)width/(float)height);
	RenderManager::get().viewport(width, height);

	return TRUE;
}

#endif /*_WIN32*/

void game_setup(){
	TEXEngine::Core::setup();

	TEXEngine::Core::PhysicsManager::get().init();
#ifdef _DEBUG
	TEXEngine::Core::PhysicsManager::get().set_enable_debug_render(true);
#endif

	Message empty(0,0,1);

#ifdef _WIN32
	empty.add_pointer(0,reinterpret_cast<void*>(hWnd));
#endif //WIN32
	//the variables above absolutely have to be defined before the call to initialise here
	trigger_event(INITIALISE, empty);
#ifdef _WIN32
	trigger_event(INIT_KEYBOARD, empty);
	trigger_event(INIT_MOUSE, empty);
	
	Message	xbox(3,0,0);
	
	xbox.add_integral_data(2,1);
	trigger_event(INIT_XBOX360_CONTROLLER,xbox);
#endif
}

#ifdef _WIN32

LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam){
	switch (uMsg) {
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam)){					// Check Minimization State
				active=TRUE;						// Program Is Active
			}
			else{
				active=FALSE;						// Program Is No Longer Active
			}
			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)	{						// Check System Calls
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			RenderManager::get().viewport(LOWORD(lParam),HIWORD(lParam));
			TEXEngine::Core::UIManager::get().aspect_modifier((float)LOWORD(lParam)/(float)HIWORD(lParam));
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

BOOL WINAPI	console_proc( DWORD ctrl )
{
	PostQuitMessage(0);
	return TRUE;
}


HWND _find_console_handle(){
	static const int BUFF_SIZE = 1024;
	char windowTitle[BUFF_SIZE];
	char temp_windowTitle[BUFF_SIZE+5];
	HWND returnValue;

	GetConsoleTitle(windowTitle,BUFF_SIZE);
	sprintf(temp_windowTitle,"%s-temp",windowTitle);
	SetConsoleTitle(temp_windowTitle);
	Sleep(40);
	returnValue = FindWindow(NULL,temp_windowTitle);
	SetConsoleTitle(windowTitle);

	return returnValue;
}

void _redirect_io(){
	static const SHORT _max_console_lines = 2000;
	static const unsigned int _console_x_offset = 2000;
	static const unsigned int _console_y_offset = 10;

	// allocate a new console window
	if ( !AllocConsole() )	return;

	SetConsoleCtrlHandler(&console_proc,TRUE);


	CONSOLE_SCREEN_BUFFER_INFO	console_info;

	// set the maximum console lines
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
	console_info.dwSize.Y = _max_console_lines;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_info.dwSize);

	// resize the console window
	HWND	console_handle = _find_console_handle();
	RECT	console_size;
	RECT	desktop_size;
	
	GetWindowRect(console_handle,&console_size);
	GetClientRect(GetDesktopWindow(),&desktop_size);
	console_size.bottom = desktop_size.bottom - 100;
	MoveWindow(console_handle,_console_x_offset,_console_y_offset,console_size.right,console_size.bottom,TRUE);

	// get the handles to the input and output streams, and redirect them
	int		stdout_handle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT); 
	int		stdin_handle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT);
	int		stderr_handle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);

	FILE*	stdout_file = _fdopen(stdout_handle,"w");
	FILE*	stdin_file = _fdopen(stdin_handle,"r");
	FILE*	stderr_file = _fdopen(stderr_handle,"w");

	*stdout = *stdout_file;
	*stdin = *stdin_file;
	*stderr = *stderr_file;
	
	ios::sync_with_stdio();
}
#endif /*_WIN32*/

#ifdef _WIN32 //Windows main loop
int WINAPI WinMain(	HINSTANCE	hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	ShowCursor(false);
	MSG		msg;
	#ifdef _DEBUG
		_redirect_io();
	#else
		TEXEngine::Core::LogManager::get().echo(false);
	#endif
#endif /*_WIN32*/


	trigger_event(MESSAGE_MANAGER_KEEP_HISTORY,Message(0,0,0));
	GameLogicManager::Get().init_settings();

	unsigned int w = GameLogicManager::Get().get_resolution_width();
	unsigned int h = GameLogicManager::Get().get_resolution_height();

#ifdef _WIN32
	if (!CreateGLWindow("X-Sledding 3000", w, h, 32, GameLogicManager::Get().is_fullscreen())){
		return 0;
	}
#endif //_WIN32

	game_setup();
	GameLogicManager::Get().setup();

#ifdef _WIN32
	UIManager::get().aspect_modifier(static_cast<float>(w)/static_cast<float>(h));
	AudioManager::get().set_volume(GameLogicManager::Get().get_music_volume(), TEXEngine::Asset::TEXENG_AUDIO_MUSIC);
	AudioManager::get().set_volume(GameLogicManager::Get().get_sound_volume(), TEXEngine::Asset::TEXENG_AUDIO_FX);
#else
	UIManager::get().aspect_modifier(16.0f/9.0f);
#endif /*_WIN32*/

	//gameLogic->set_running(true);
	

	Timer timer;
	double last_update = 0;
	double last_fast_update = 0;
	double last_medium_update = 0;
	double last_slow_update = 0;
	Message parameters(0,1,0);



	TEXEngine::Core::LogManager::get().log_message("Game Log:\tApplication initialised");
	GameLogicManager::Get().set_running(true);
	//trigger_event(BACK_TO_MENU, parameters);
	timer.start();
	while(GameLogicManager::Get().is_running()){
		
		double difference = 0;
		double current_time = timer.get_time_in_sec();

		difference = current_time - last_update;
		last_update = current_time;
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(difference));
		trigger_event(UPDATE, parameters);

		//TEXEngine::Core::PhysicsManager::get().update(difference);

		difference = current_time - last_fast_update;
		last_fast_update = current_time;
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(difference));
		trigger_event(UPDATE_FAST, parameters);

		difference = current_time - last_medium_update;
		if ( difference > MEDIUM_FIRE_RATE )
		{
			last_medium_update = current_time;
			parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(difference));
			trigger_event(UPDATE_MEDIUM,parameters);
		}

		difference = current_time - last_slow_update;
		if ( difference > SLOW_FIRE_RATE )
		{
			last_slow_update = current_time;
			parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(difference));
			trigger_event(UPDATE_SLOW,parameters);
		}

#ifdef _WIN32
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				GameLogicManager::Get().set_running(false);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif //_WIN32

#ifdef _DEBUG
		static bool createdDebugLabel = false;
		if(!createdDebugLabel){
			//TEXEngine::Core::UIManager::get().create_label(29999, 3, "DEBUG", 0.0f, 0.0f, 0.4f, 0.05f);
			TEXEngine::Core::UIManager::get().create_label(29999, 3, "DEBUG", 1.6f, 0.0f, 0.4f, 0.05f);
			TEXEngine::Core::UIManager::get().attach_to_root(29999, 1000);
			createdDebugLabel = true;
		}
#endif
#ifdef _WIN32
		//this is just for the mouse
		UIImage* img = TEXEngine::Core::UIManager::get().get_image(9100);
		static bool imgHack = false;
		if(img && !imgHack){
			img->set_x(1.0f);
			img->set_y(1.0f);
			imgHack = true;
		}
#endif
				
		// Draw The Scene.  
		if (active)	
		{
			TEXEngine::Core::RenderManager::get().render_scene();
			TEXEngine::Core::UIManager::get().render_ui();
			TEXEngine::Core::RenderManager::get().submit_frame();
		}

	}

	// Dump of the engine log
	TEXEngine::Core::LogManager::get().log_message("Game Log:\tShutting down the application..");

	// This is needed, for cleanup and to avoid exception problems
	Message tempMsg(0,0,1);

	std::string filename("x_sledding_3000.log");
	tempMsg.add_pointer(0,static_cast<void*>(&filename));
	trigger_event(EXIT, tempMsg);

#ifdef _WIN32
	// Shutdown
	KillGLWindow();
#endif //_WIN32

	
#ifdef _WIN32
	return (msg.wParam);
#else
	return 0;
#endif //_WIN32
}

