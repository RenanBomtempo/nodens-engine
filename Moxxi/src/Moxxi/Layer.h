#pragma once

#include "Moxxi/Core.h"
#include "Moxxi/Events/Event.h"
#include "Moxxi/Core/TimeStep.h"

namespace Moxxi {

	class MOXXI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender(TimeStep ts) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

