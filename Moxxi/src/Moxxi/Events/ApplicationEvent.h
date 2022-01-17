#pragma once

#include "Event.h"
#include <sstream>

namespace Moxxi {
	class MOXXI_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		inline unsigned int GetWidth() { return m_Width; }
		inline unsigned int GetHeight() { return m_Height; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};


	class MOXXI_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent() {}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class MOXXI_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}