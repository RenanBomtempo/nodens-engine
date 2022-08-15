#pragma once

#include "Core.h"

#include "Window.h"

#include "Nodens/LayerStack.h"

#include "Nodens/Events/Event.h"
#include "Nodens/Events/ApplicationEvent.h"

#include "Nodens/imgui/ImGuiLayer.h"
#include "Nodens/Core/TimeStep.h"

namespace Nodens {

	class Application
	{
	public:
		Application();
		Application(const WindowProps& props);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		bool m_Running = true;
		
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}