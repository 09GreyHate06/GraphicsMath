#include "Input.h"

namespace GM
{
	bool Input::GetKey(const Window* window, const KeyCode key)
	{
		if (!window->GetState().isFocus) return false;

		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}

	bool Input::GetKey(HWND window, const KeyCode key)
	{
		if (window != GetFocus()) return false;

		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}

	bool Input::GetMouseButton(const Window* window, const MouseCode button)
	{
		if (!window->GetState().isFocus) return false;

		return (GetAsyncKeyState(button) & 0x8000) != 0;
	}

	bool Input::GetMouseButton(HWND window, const MouseCode button)
	{
		if (window != GetFocus()) return false;

		return (GetAsyncKeyState(button) & 0x8000) != 0;
	}

	Vector Input::GetMousePos(const Window* window)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(window->GetNativeWindow(), &p);
		return { static_cast<float>(p.x), static_cast<float>(p.y), 0.0f, 0.0f };
	}

	Vector Input::GetMousePos(HWND window)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(window, &p);
		return { static_cast<float>(p.x), static_cast<float>(p.y), 0.0f, 0.0f };
	}

	float Input::GetMouseX(const Window* window)
	{
		return GetMousePos(window).x;
	}

	float Input::GetMouseX(HWND window)
	{
		return GetMousePos(window).x;
	}

	float Input::GetMouseY(const Window* window)
	{
		return GetMousePos(window).y;
	}

	float Input::GetMouseY(HWND window)
	{
		return GetMousePos(window).y;
	}
}

