#pragma once

#include "Moxxi/Application.h"
#include "Moxxi/Layer.h"
#include "Moxxi/Log.h"

#include "Moxxi/Core/TimeStep.h"

#include "Moxxi/Input.h"
#include "Moxxi/KeyCodes.h"
#include "Moxxi/MouseButtonCodes.h"

#include "Moxxi/imgui/ImGuiLayer.h"

//------Renderer-------------------
#include "Moxxi/Renderer/Renderer.h"
#include "Moxxi/Renderer/RenderCommand.h"

#include "Moxxi/Renderer/Shader.h"
#include "Moxxi/Renderer/Buffer.h"
#include "Moxxi/Renderer/VertexArray.h"

#include "Moxxi/Renderer/PointLight.h"
#include "Moxxi/Renderer/DirectionalLight.h"

#include "Moxxi/Renderer/OrthographicCamera.h"
#include "Moxxi/Renderer/PerspectiveCamera.h"
//----------------------------------

//------Entry Point-----------------
#ifdef MAIN_APPLICATION_FILE
	#include "Moxxi/EntryPoint.h"
#endif
//----------------------------------
// 
