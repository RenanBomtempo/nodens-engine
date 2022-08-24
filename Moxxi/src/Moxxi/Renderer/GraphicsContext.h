#pragma once

#include "Moxxi/Core.h"

namespace Moxxi {
	class MOXXI_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}