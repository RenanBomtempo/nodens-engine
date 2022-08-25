#include "ShallowWatersLayer.h"
#include "ALG.h"
#include <imgui/imgui.h>

ShallowWatersLayer::ShallowWatersLayer()
	: Layer("ShallowWatersLayer"),
		m_CameraPosition(0.0f, 8.0f, 0.0f),
		m_CameraRotation(-25.0f, -90.f, 0.0f),
		m_Camera(-50, 50, -50, 50),
		m_Light(),
		m_LightDirection(0, -1, 0),
		m_LightOrientation(0, 180, 0),
		m_LightColor(1.0f, 1.0f, 1.0f),
		m_ClearColor(0.5, 0.5, 0.5, 1)
{
	InitCamera();
	InitFlatShader();
	InitDomainMesh();
} // ShallowWatersLayer::ShallowWatersLayer

void ShallowWatersLayer::OnUpdate(Moxxi::TimeStep ts)
{
	m_ElapsedTime += ts;
	//MX_TRACE("m_ElapsedTime: {0}s", m_ElapsedTime);

	ProcessInputs(ts);

	// Update scene
	m_Camera.SetPosition(m_CameraPosition);
	
	m_LightDirection = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.z), glm::vec3(0, 0, 1));
	m_Light.SetDirection(m_LightDirection);
	m_Light.SetColor(m_LightColor);
	
	m_ShallowWaters.OnUpdate(ts);

	// Render
	Moxxi::RenderCommand::SetClearColor(m_ClearColor);
	Moxxi::RenderCommand::Clear();
	Moxxi::Renderer::BeginScene(m_Camera, m_Light);
	Moxxi::RenderCommand::SetFrontFaceOrientation(Moxxi::RendererProps::FrontFaceOrientation::Clockwise);
	Moxxi::Renderer::SubmitArray(m_FlatShader, m_DomainVA, glm::mat4(1.0f), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	Moxxi::Renderer::EndScene();
} // ShallowWatersLayer::OnUpdate

void ShallowWatersLayer::OnImGuiRender(Moxxi::TimeStep ts)
{
	// Perfomance ----------------------------------------------------------
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	static int corner = 0;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("Performance", NULL, window_flags);
	ImGui::Text("Frametime: %.3fms", ts.GetMilliseconds());
	ImGui::Text("FPS: %.3f", 1.0f / ts);
	ImGui::End();

	// Camera Transform ----------------------------------------------------
	ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("FOV", &m_CameraFOV, 1, 150);
	ImGui::Text("Position");
	ImGui::InputFloat("X", &m_CameraPosition.x, -10, 10);
	ImGui::InputFloat("Y", &m_CameraPosition.y, -10, 10);
	ImGui::InputFloat("Z", &m_CameraPosition.z, -10, 10);
	ImGui::Text("Orientation");
	ImGui::SliderFloat("Pitch", &m_CameraRotation.x, -90, 90);
	ImGui::SliderFloat("Yaw", &m_CameraRotation.y, -180, 180);
	if (ImGui::Button("Reset"))
	{
		m_CameraPosition.x = 0;
		m_CameraPosition.y = 0;
		m_CameraPosition.z = 5;

		m_CameraRotation.x = 0;
		m_CameraRotation.y = -90;
		m_CameraRotation.z = 0;
	}
	ImGui::End();
	
	// Light ----------------------------------------------------------
	ImGui::Begin("Lighting", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Direction");
	ImGui::SliderFloat("X", &m_LightOrientation.x, 0, 360);
	ImGui::SliderFloat("Y", &m_LightOrientation.y, 0, 360);
	ImGui::SliderFloat("Z", &m_LightOrientation.z, 0, 360);
	ImGui::ColorPicker3("Light color", &m_LightColor.r);
	ImGui::ColorPicker4("Clear Color", &m_ClearColor.r);
	ImGui::End();

} // ShallowWatersLayer::OnImGuiRender

void ShallowWatersLayer::InitCamera()
{
	m_Camera.SetPosition({0, 0, 0});
}

void ShallowWatersLayer::InitFlatShader()
{
	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec3 aNormal;

			out vec3 FragPos;
			out vec3 Normal;

			uniform mat4 uNormalMatrix;
			uniform mat4 uModelMatrix;
			uniform mat4 uViewProjection;

			void main ()
			{
				FragPos = vec3(uModelMatrix * vec4(aPosition, 1.0));
				Normal = vec3(uNormalMatrix * vec4(aNormal, 0.0));

				gl_Position = uViewProjection * vec4(FragPos, 1.0);
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 FragColor;

			in vec3 FragPos;
			in vec3 Normal;

			uniform vec4 uObjectColor;
			uniform vec3 uLightDirection;
			uniform vec3 uLightColor;

			void main()
			{
				// ambient
				float ambientStrength = 0.5;
				vec3 ambient = ambientStrength * uLightColor;
  	
				// diffuse 
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(uLightDirection);
				float diff = max(dot(norm, uLightDirection), 0.0);
				vec3 diffuse = diff * uLightColor;
            
				FragColor = vec4(ambient + diffuse, 1.0) * uObjectColor;
				//FragColor = vec4(norm, 1.0);
			}
		)";

	m_FlatShader.reset(Moxxi::Shader::Create(vertexSrc, fragmentSrc));
}

void ShallowWatersLayer::InitDomainMesh()
{
	// VAOs
	m_DomainVA.reset(Moxxi::VertexArray::Create());

	// VBO
	float vertices[] = {
		-1, 0, -1, 0, 1, 0,
		 1, 0, -1, 0, 1, 0,
		-1, 0,  1, 0, 1, 0,
		 
		 1, 0, -1, 0, 1, 0,
		 1, 0,  1, 0, 1, 0,
		-1, 0,  1, 0, 1, 0
	};
	Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, sizeof(vertices)));
	Moxxi::BufferLayout layout = {
		{ Moxxi::ShaderDataType::Float3, "aPosition" },
		{ Moxxi::ShaderDataType::Float3, "aNormal" }
	};
	vertexBuffer->SetLayout(layout);
	m_DomainVA->AddVertexBuffer(vertexBuffer);

	// EBO
	/*uint32_t indices[3*2] = {
		0, 1, 2, 1, 3, 2
	};
	Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 3*2));
	m_DomainVA->SetIndexBuffer(indexBuffer);*/
} // ShallowWatersLayer::InitDomainMesh

void ShallowWatersLayer::ProcessInputs(Moxxi::TimeStep ts)
{

} // ShallowWatersLayer::ProcessInputs

void ShallowWatersLayer::OnEvent(Moxxi::Event& event)
{
	Moxxi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Moxxi::WindowResizeEvent>(MX_BIND_EVENT_FN(ShallowWatersLayer::OnWindowResizeEvent));
} // ShallowWatersLayer::OnEvent

bool ShallowWatersLayer::OnWindowResizeEvent(Moxxi::WindowResizeEvent& event)
{
	return true;
} // ShallowWatersLayer::OnMouseMovedEvent