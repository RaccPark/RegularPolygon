#pragma once
#include <conio.h>
#include "Utils.h"
#include <fstream>

using namespace std;

enum class KeyState
{
	Idle,
	Pressed,
	Stay,
	Released,
	PressedReleased
};

typedef KeyState MouseState;

class InputManager
{
	KeyState keys[256];
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	INPUT_RECORD irInBuf[128];
	char keyName[1024];

	MouseState mouse[2];

	COORD mousePosition;

	VOID ErrorExit(LPCSTR lpszMessage)
	{
		Debug::Log("%s\n", lpszMessage);

		// Restore input mode on exit.

		SetConsoleMode(hStdin, fdwSaveOldMode);

		ExitProcess(0);
	}

	VOID KeyEventProc(KEY_EVENT_RECORD ker)
	{
		Debug::Log("Key event: bkeyDown: %d, wRepeatCount: %d, keyCode: %d,  wScanCode: %d, uChar: %d, KeyState: %d",
			ker.bKeyDown, ker.wRepeatCount, ker.wVirtualKeyCode, ker.wVirtualScanCode, ker.uChar.AsciiChar, ker.dwControlKeyState);

		switch (keys[ker.wVirtualKeyCode & 0x00ff])
		{
		case KeyState::Idle:
			if (ker.bKeyDown == TRUE)
				keys[ker.wVirtualKeyCode] = KeyState::Pressed;
			break;
		case KeyState::Pressed:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::PressedReleased;
			else
				keys[ker.wVirtualKeyCode] = KeyState::Stay;
			break;
		case KeyState::Stay:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::Released;
			break;
		case KeyState::Released:
		case KeyState::PressedReleased:
			keys[ker.wVirtualKeyCode] = KeyState::Idle;
			break;
		}
	}

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
	{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
		char log[128];
		memset(log, ' ', 128);
		log[127] = '\0';
		

		switch (mer.dwEventFlags)
		{
		case 0:

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				switch (mouse[0]) {
				case MouseState::Idle:
					mouse[0] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[0] = MouseState::Stay;
					mousePosition = mer.dwMousePosition;
					break;
				}
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				switch (mouse[1]) {
				case MouseState::Idle:
					mouse[1] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[1] = MouseState::Stay;
					mousePosition = mer.dwMousePosition;
					break;
				}
				mousePosition = mer.dwMousePosition;
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					switch (mouse[i]) {
					case MouseState::Stay:
						mouse[i] = MouseState::Released;
						mousePosition = mer.dwMousePosition;
						break;
					}
				}
			}
			break;
		case DOUBLE_CLICK:
			sprintf(log, " => double click");
			break;
		case MOUSE_HWHEELED:
			sprintf(log, " => horizontal mouse wheel");
			break;
		case MOUSE_MOVED:
			mousePosition = mer.dwMousePosition;
			sprintf(log, " => mouse moved");
			break;
		case MOUSE_WHEELED:
			sprintf(log, " => vertical mouse wheel");
			break;
		default:
			sprintf(log, " => unknown");
			break;
		}
		//Debug::Log("Mouse event: (%d, %d), Button: %d, KeyState: %d, eventFlag: %d %s",
		//	mer.dwMousePosition.X, mer.dwMousePosition.Y, mer.dwButtonState, mer.dwControlKeyState, mer.dwEventFlags, log);
	}

	const char* vk2name(WORD keyCode) {
		UINT scanCode = MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC);
		LONG lParamValue = (scanCode << 16);
		GetKeyNameTextA(lParamValue, keyName, 1024);
		return keyName;
	}

public:
	InputManager() {
		DWORD fdwMode;

		for (int i = 0; i < 256; i++)
			keys[i] = KeyState::Idle;

		mouse[0] = MouseState::Idle;
		mouse[1] = MouseState::Idle;
		mousePosition = { 0, 0 };

		memset(irInBuf, 0, sizeof(INPUT_RECORD) * 128);

		// Get the standard input handle.

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");

		// Save the current input mode, to be restored on exit.

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");

		// Enable the window and mouse input events.

		fdwMode = (ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) & ~ENABLE_QUICK_EDIT_MODE ;
		if (!SetConsoleMode(hStdin, fdwMode))
			ErrorExit("SetConsoleMode");
	}

	virtual ~InputManager() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

	void readEveryFrame() {
		DWORD cNumRead = 0;

		for (int i = 0; i < 256; i++) {
			switch (keys[i]) {
			case KeyState::Released:
			case KeyState::PressedReleased:
				keys[i] = KeyState::Idle;
				break;
			case KeyState::Pressed:
				keys[i] = KeyState::Stay;
				break;
			}
		}
		for (int i = 0; i < 2; i++) {
			switch (mouse[i]) {
			case MouseState::Released:
				mouse[i] = MouseState::Idle;
				break;
			case MouseState::Pressed:
				mouse[i] = MouseState::Stay;
				break;
			}
		}

		if (GetNumberOfConsoleInputEvents(hStdin, &cNumRead) == 0)
		{
			Debug::Log("Input Reading Failure %d", GetLastError());
			return;
		}

		if (cNumRead == 0) {
			return;
		}

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			ErrorExit("ReadConsoleInput");

		//Debug::Log("cNumRead = %d (event)   ", cNumRead);
		for (int i = 0; i < (int)cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;
			}
		}
	}

	auto getKeyDown(WORD keyCode) 
	{
		bool result = keys[keyCode & 0x00ff] == KeyState::Pressed
			|| keys[keyCode & 0x00ff] == KeyState::PressedReleased;
		if (result) Debug::Log("key %s pressed [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		return result;
	}

	auto getKey(WORD keyCode) 
	{
		bool result = keys[keyCode & 0x00ff] == KeyState::Pressed
			|| keys[keyCode & 0x00ff] == KeyState::Stay
			|| keys[keyCode & 0x00ff] == KeyState::PressedReleased
			|| keys[keyCode & 0x00ff] == KeyState::Released;
		if (result) Debug::Log("key %s staying [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		
		return result;
	}

	auto getKeyUp(WORD keyCode) 
	{
		bool result = keys[keyCode & 0x00ff] == KeyState::Released
			|| keys[keyCode & 0x00ff] == KeyState::PressedReleased;
		if (result) Debug::Log("key %s released [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		return result;
	}

	auto getMouseButton(int button) const
	{
		bool result = mouse[button] == MouseState::Pressed
			|| mouse[button] == MouseState::Stay
			|| mouse[button] == MouseState::Released;
		//if (result) Debug::Log("mouse %d state staying [%x] on [%d, %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}
	auto getMouseButtonDown(int button) const
	{
		bool result = mouse[button] == MouseState::Pressed;
		//if (result) Debug::Log("mouse %d state pressed [%x] on [%d, %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}
	auto getMouseButtonUp(int button) const
	{
		bool result = mouse[button] == MouseState::Released;
		//if (result) Debug::Log("mouse %d state released [%x] on [%d, %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}

	Position getMousePosition() const
	{
		return { mousePosition.X, mousePosition.Y };
	}

};


