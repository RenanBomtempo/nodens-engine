#pragma once

#include "Nodens/Application.h"
#include "Nodens/Layer.h"
#include "Nodens/Log.h"

#include "Nodens/Core/TimeStep.h"

#include "Nodens/Input.h"
#include "Nodens/KeyCodes.h"
#include "Nodens/MouseButtonCodes.h"

#include "Nodens/imgui/ImGuiLayer.h"

//------Renderer-------------------
#include "Nodens/Renderer/Renderer.h"
#include "Nodens/Renderer/RenderCommand.h"

#include "Nodens/Renderer/Shader.h"
#include "Nodens/Renderer/Buffer.h"
#include "Nodens/Renderer/VertexArray.h"

#include "Nodens/Renderer/OrthographicCamera.h"
//----------------------------------

//------Entry Point-----------------
#ifdef MAIN_APPLICATION_FILE
	#include "Nodens/EntryPoint.h"
#endif
//----------------------------------
// 
