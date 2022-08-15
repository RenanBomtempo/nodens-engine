#pragma once

#include "Nodens/Core.h"

namespace Nodens {
	class NODENS_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}