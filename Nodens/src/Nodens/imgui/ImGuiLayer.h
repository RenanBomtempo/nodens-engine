#pragma once

#include "Nodens/Layer.h"
#include "Nodens/Events/ApplicationEvent.h"
#include <Nodens/Events/MouseEvent.h>
#include <Nodens/Events/KeyEvent.h>

namespace Nodens {
	class NODENS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f; 
		//
	};
}