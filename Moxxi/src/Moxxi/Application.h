#pragma once

#include "Core.h"

#include "Window.h"
#include "Moxxi/LayerStack.h"
#include "Moxxi/Events/Event.h"
#include "Moxxi/Events/ApplicationEvent.h"

#include "Moxxi/imgui/ImGuiLayer.h"
#include "Moxxi/Renderer/Shader.h"
#include "Moxxi/Renderer/Buffer.h"

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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}