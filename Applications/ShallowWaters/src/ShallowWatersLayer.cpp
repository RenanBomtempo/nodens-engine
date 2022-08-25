#include "ShallowWatersLayer.h"
#include "ALG.h"
#include <imgui/imgui.h>

ShallowWatersLayer::ShallowWatersLayer()
	: Layer("ShallowWatersLayer"),
	m_CameraPosition(0.0f, 0.0f, 0.0f),
	m_Camera(-2, 2, -2, 2),
	m_Light(),
	m_LightDirection(0, -1, 0),
	m_LightOrientation(0, 180, 0),
	m_LightColor(1.0f, 1.0f, 1.0f),
	m_ClearColor(0.0, 0.0, 0.0, 1)
{
	InitFlatShader();
	InitSquareMesh();
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

	// Update simulation
	m_ShallowWaters.OnUpdate(ts);

	// Render
	Moxxi::RenderCommand::SetClearColor(m_ClearColor);
	Moxxi::RenderCommand::Clear();
	Moxxi::RenderCommand::SetPolygonMode(Moxxi::RendererProps::PolygonMode::Wireframe);

	Moxxi::Renderer::BeginScene(m_Camera, m_Light);

	Moxxi::Renderer::SubmitIndexed(m_FlatShader, m_SquareVA, glm::mat4(1.0f));

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
} // ShallowWatersLayer::OnImGuiRender

void ShallowWatersLayer::InitFlatShader()
{
	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec3 aNormal;

			out vec3 FragPos;
			out vec3 Normal;

			uniform mat4 uModelMatrix;
			uniform mat4 uViewProjection;

			void main ()
			{
				FragPos = vec3(uModelMatrix * vec4(aPosition, 1.0));

				gl_Position = uViewProjection * vec4(FragPos, 1.0);
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 FragColor;

			in vec3 FragPos;
			in vec3 Normal;

			void main()
			{
				FragColor = vec4(1.0, 1.0, 1.0, 1.0);
			}
		)";

	m_FlatShader.reset(Moxxi::Shader::Create(vertexSrc, fragmentSrc));
}

void ShallowWatersLayer::InitSquareMesh()
{
	// VAOs
	m_SquareVA.reset(Moxxi::VertexArray::Create());

	// VBO
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
	};

	Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, sizeof(vertices)));
	Moxxi::BufferLayout layout = {
		{ Moxxi::ShaderDataType::Float3, "aPosition" }
	};
	vertexBuffer->SetLayout(layout);
	m_SquareVA->AddVertexBuffer(vertexBuffer);

	// EBO
	uint32_t indices[5] = {
		0, 1, 2, 3, 0
	};
	Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 5));
	m_SquareVA->SetIndexBuffer(indexBuffer);
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
	float newHeight = event.GetHeight();
	float newWidth = event.GetWidth();

	m_Camera.SetSides(-newWidth / 360, newWidth / 360, -newHeight / 360, newHeight / 360);
	return true;
} // ShallowWatersLayer::OnMouseMovedEvent