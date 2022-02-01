#pragma once

#include "mxpch.h"

#include "Moxxi/Core.h"
#include "Moxxi/Events/Event.h"

namespace Moxxi {
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProps(const std::string& title = "Moxxi Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{

		}
	};

	class MOXXI_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
			// Type alias. Identical to 'typedef std::function<void(Event&)> EventCallbackFn'

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}