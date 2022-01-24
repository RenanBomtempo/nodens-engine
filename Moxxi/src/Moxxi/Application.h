#pragma once

#include "Core.h"

#include "Window.h"
#include "Moxxi/LayerStack.h"
#include "Moxxi/Events/Event.h"
#include "Moxxi/Events/ApplicationEvent.h"

#include "Moxxi/imgui/ImGuiLayer.h"
#include "Moxxi/Renderer/Shader.h"
#include "Moxxi/Renderer/Buffer.h"
#include "Moxxi/Renderer/VertexArray.h"

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
		
		bool m_Running = true;
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_Shader2;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}