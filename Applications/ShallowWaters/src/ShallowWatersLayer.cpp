#include "ShallowWatersLayer.h"
#include "ALG.h"
#include <imgui/imgui.h>

ShallowWatersLayer::ShallowWatersLayer()
	: Layer("ShallowWatersLayer"),
	m_CameraPosition(0.0f, 0.0f, 0.0f),
	m_Camera(-2, 2, -2, 2),
	m_ClearColor(0.0, 0.0, 0.0, 1)
{
	m_Grid.Initialize();

	InitFlatShader();
	InitSquareWireframeMesh();
	InitSquareFillMesh();
	InitMHCMesh();

} // ShallowWatersLayer::ShallowWatersLayer


void ShallowWatersLayer::OnUpdate(Moxxi::TimeStep ts)
{
	m_ElapsedTime += ts;

	ProcessInputs(ts);

	// Simulate;

	DrawGrid();
} // ShallowWatersLayer::OnUpdate

void ShallowWatersLayer::DrawGrid()
{
	// Update scene
	m_Camera.SetPosition(m_CameraPosition);

	// Prepare scene
	Moxxi::RenderCommand::SetLineWdith(m_LineWidth);
	Moxxi::RenderCommand::SetPointSize(m_PointSize);
	Moxxi::RenderCommand::SetClearColor(m_ClearColor);
	Moxxi::RenderCommand::Clear();

	// Render Scene
	Moxxi::Renderer::BeginScene(m_Camera);

	// Render MHC
	glm::vec3 mhc_position(-.5, -.5, 0);
	glm::mat4 mhc_transform(1);

	mhc_transform = glm::scale(mhc_transform, glm::vec3(2));
	mhc_transform = glm::translate(mhc_transform, mhc_position);

	if (m_MHCPoints) {
		// MHC Points
		Moxxi::RenderCommand::SetPolygonMode(Moxxi::RendererProps::PolygonMode::Point);
		Moxxi::Renderer::SubmitIndexed(
			m_FlatShader, m_MHCVA, mhc_transform, { 1,1,0,1 });
	}
	if (m_MHCLines) {
		Moxxi::RenderCommand::SetPolygonMode(Moxxi::RendererProps::PolygonMode::Wireframe);
		Moxxi::Renderer::SubmitIndexedLines(
			m_FlatShader, m_MHCVA, mhc_transform);
	}

	auto cell = m_Grid.FirstCell();
	while (cell != nullptr)
	{
		glm::vec3 cellPosition = {
			(cell->Center().x - .5) * 2,
			(cell->Center().y - .5) * 2,
			0
		};

		glm::mat4 cellTransform(1);
		cellTransform = glm::translate(cellTransform, cellPosition);
		cellTransform = glm::scale(cellTransform, glm::vec3(cell->SideLength()));

		if (m_Wireframe) {
			Moxxi::RenderCommand::SetPolygonMode(Moxxi::RendererProps::PolygonMode::Wireframe);
			// Render wireframe square
			Moxxi::Renderer::SubmitIndexedLines(
				m_FlatShader, m_SquareWireframeVA, cellTransform);
		}

		if (m_Fill) {
			Moxxi::RenderCommand::SetPolygonMode(Moxxi::RendererProps::PolygonMode::Fill);
			// Render filled square
			Moxxi::Renderer::SubmitIndexed(
				m_FlatShader, m_SquareFillVA, cellTransform, { cellPosition + glm::vec3(.5, .5, 0.5), 1 });
		}

		cell = cell->Next();
	}

	Moxxi::Renderer::EndScene();
} // ShallowWatersLayer::DrawGrid

void ShallowWatersLayer::OnImGuiRender(Moxxi::TimeStep ts)
{
	// Perfomance window--------------------------------------------------------
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

	// Options menu ------------------------------------------------------------
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PushItemWidth(90);

	ImGui::Text("Cell count: %u", m_Grid.CellCount());
	ImGui::SliderFloat("Line width", &m_LineWidth, 0, 3);
	ImGui::SliderFloat("Point Size", &m_PointSize, 1, 8);
	ImGui::Checkbox("Wireframe", &m_Wireframe);
	ImGui::Checkbox("Fill", &m_Fill);
	ImGui::Checkbox("MHC Lines", &m_MHCLines);
	ImGui::Checkbox("MHC Points", &m_MHCPoints);

	if (ImGui::Button("Refine"))
	{
		m_Grid.RefineGrid();
		InitMHCMesh();
	}
	if (ImGui::Button("Coarsen"))
	{
		m_Grid.CoarsenGrid();
		InitMHCMesh();
	}

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
			
			uniform vec4 uObjectColor;
			in vec3 FragPos;
			in vec3 Normal;

			void main()
			{
				FragColor = uObjectColor;
			}
		)";

	m_FlatShader.reset(Moxxi::Shader::Create(vertexSrc, fragmentSrc));
}

void ShallowWatersLayer::InitSquareWireframeMesh()
{
	// VAO
	m_SquareWireframeVA.reset(Moxxi::VertexArray::Create());

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
	m_SquareWireframeVA->AddVertexBuffer(vertexBuffer);

	// EBO
	uint32_t indices[5] = {
		0, 1, 2, 3, 0
	};
	Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 5));
	m_SquareWireframeVA->SetIndexBuffer(indexBuffer);
} // ShallowWatersLayer::InitDomainMesh

void ShallowWatersLayer::InitSquareFillMesh()
{
	// VAO
	m_SquareFillVA.reset(Moxxi::VertexArray::Create());

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
	m_SquareFillVA->AddVertexBuffer(vertexBuffer);

	// EBO
	uint32_t indices[6] = {
		0, 1, 2, 2, 3, 0
	};
	Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 6));
	m_SquareFillVA->SetIndexBuffer(indexBuffer);
} // ShallowWatersLayer::InitDomainMesh

void ShallowWatersLayer::InitMHCMesh()
{
	// VAO
	m_MHCVA.reset(Moxxi::VertexArray::Create());

	// VBO
	uint32_t cell_count = m_Grid.CellCount();
	float* vertices = new float[3 * cell_count];

	uint32_t cell_counter = 0;
	auto cell = m_Grid.FirstCell();
	while (cell != nullptr)
	{
		auto next = cell->Next();
		vertices[3 * cell_counter] = cell->Center().x;
		vertices[3 * cell_counter + 1] = cell->Center().y;
		vertices[3 * cell_counter + 2] = 0;
		cell_counter++;
		cell = next;
	}

	Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, 3 * cell_count * sizeof(float)));
	Moxxi::BufferLayout layout = {
		{ Moxxi::ShaderDataType::Float3, "aPosition" }
	};
	vertexBuffer->SetLayout(layout);
	m_MHCVA->AddVertexBuffer(vertexBuffer);

	// EBO
	uint32_t* indices = new uint32_t[2 * (cell_count - 1)];

	for (uint32_t i = 0; i < cell_count; i++)
	{
		indices[2 * i] = i;
		indices[2 * i + 1] = i+1;
	}

	Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 2 * (cell_count - 1)));
	m_MHCVA->SetIndexBuffer(indexBuffer);
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