#pragma once
#include "NativeWindow.h"

#include "Event/Event.h"
#include "GMException.h"
#include <functional>

namespace GM
{
	struct WindowDesc
	{
		uint32_t width = 0;
		uint32_t height = 0;
		std::string name = "";
		std::string winClassName = "";

		WindowDesc(uint32_t width, uint32_t height, const std::string& name, const std::string& winClassName)
			: width(width), height(height), name(name), winClassName(winClassName) { }

		WindowDesc()
			: WindowDesc(1280u, 720u, "Graphics Math", "GraphicsMath") { }
	};

	struct WindowState
	{
		bool shouldClose = false;
		bool isMinimized = false;
		bool isFocus = true;

		WindowState() = default;
		WindowState(bool shouldClose, bool isMinimized, bool isFocus)
			: shouldClose(shouldClose), isMinimized(isMinimized), isFocus(isFocus) { }
	};

	class Window
	{
		using EventCallbackFn = std::function<void(Event&)>;

	private:
		class WindowClass
		{
		public:
			WindowClass(const std::string& windowClassName);
			~WindowClass();

			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;

			const std::string& GetName();
			HINSTANCE GetInstance();

		private:
			const std::string m_className;
			HINSTANCE m_hInst;
		};


	public:
		Window(const WindowDesc& desc);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void SetName(const std::string& name);
		void SetEventCallback(const EventCallbackFn& callback);

		const WindowDesc& GetDesc() const;
		const WindowState& GetState() const;
		HWND GetNativeWindow() const;

		void Close();

		static void PollEvents();
		static void HideCursor();
		static void ShowCursor();

		operator HWND();

	private:
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMessageCaller(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		WindowDesc m_desc;
		WindowState m_state;
		WindowClass m_windowClass;
		HWND m_hWnd;
		EventCallbackFn m_eventCallbackFn;







		// Exception Stuff
	public:
		class Exception : public GMException
		{
			using GMException::GMException;
		public:
			static std::string TranslateErrorCode(HRESULT hr);
		};

		class HRException : public Exception
		{
		public:
			HRException(int line, const std::string& file, HRESULT hr);
			virtual const char* what() const override;

			HRESULT GetErrorCode() const { return m_hr; }
			std::string GetErrorDescription() const { return TranslateErrorCode(m_hr); }
			virtual const char* GetType() const override { return "Window Exception"; }

		private:
			HRESULT m_hr;
		};
	};
}

#define GM_WND_EXCEPT(hr) GM::Window::HRException(__LINE__, __FILE__, (hr))
#define GM_WND_LAST_EXCEPT() GM_WND_EXCEPT(GetLastError())