#pragma once
#include "Core/Window.h"
#include "MouseCodes.h"
#include "KeyCodes.h"

#include "Math/Types.h"

namespace GM
{
	class Input
	{
	public:
		static bool GetKey(const Window* window, const KeyCode key);
		static bool GetKey(HWND window, const KeyCode key);
		static bool GetMouseButton(const Window* window, const MouseCode button);
		static bool GetMouseButton(HWND window, const MouseCode button);
		static Vector GetMousePos(const Window* window);
		static Vector GetMousePos(HWND window);
		static float GetMouseX(const Window* window);
		static float GetMouseX(HWND window);
		static float GetMouseY(const Window* window);
		static float GetMouseY(HWND window);

	private:
		Input() = default;
	};
}