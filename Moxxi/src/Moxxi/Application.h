#pragma once

#include "Core.h"

#include "Window.h"

#include "Moxxi/LayerStack.h"

#include "Moxxi/Events/Event.h"
#include "Moxxi/Events/ApplicationEvent.h"

#include "Moxxi/imgui/ImGuiLayer.h"
#include "Moxxi/Core/TimeStep.h"

namespace Moxxi {

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
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}