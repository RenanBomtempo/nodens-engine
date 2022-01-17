#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Moxxi/Layer.h"
#include "Moxxi/Events/ApplicationEvent.h"
#include "Moxxi/LayerStack.h"

namespace Moxxi {

	class MOXXI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}