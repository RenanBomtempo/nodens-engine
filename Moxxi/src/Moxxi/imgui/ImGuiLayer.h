#include "Moxxi/Layer.h"
#include "Moxxi/Events/ApplicationEvent.h"
#include <Moxxi/Events/MouseEvent.h>
#include <Moxxi/Events/KeyEvent.h>

namespace Moxxi {
	class MOXXI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		// Mouse
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		// Keys
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//void OnKeyTypedEvent(KeyTypedEvent& e);
		// Window
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;

	};
}