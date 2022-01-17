#include "Moxxi/Layer.h"
#include "Moxxi/Events/Event.h"

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
		float m_Time = 0.0f;

	};
}