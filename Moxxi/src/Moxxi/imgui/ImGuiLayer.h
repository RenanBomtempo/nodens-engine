#pragma once

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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender(TimeStep ts) override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f; 
		//
	};
}