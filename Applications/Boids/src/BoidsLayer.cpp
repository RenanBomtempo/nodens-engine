#include "BoidsLayer.h"
#include <imgui/imgui.h>

BoidsLayer::BoidsLayer()
	: Layer("BoidsLayer"),
		m_CameraPosition(0.0f, 8.0f, 0.0f),
		m_CameraMode(CameraMode::Free),
		m_CameraRotation(-25.0f, -90.f, 0.0f),
		m_Light(),
		m_LightDirection(0, -1, 0),
		m_LightOrientation(0, 180, 0),
		m_LightColor(1.0f, 1.0f, 1.0f),
		m_ClearColor(0.5, 0.5, 0.5, 1)
{
	InitCameras();
	InitFlatShader();
	InitDomainMesh();
	InitTowerMesh();
	InitBoidsMesh();
} // BoidsLayer::BoidsLayer

void BoidsLayer::OnUpdate(Moxxi::TimeStep ts)
{
	m_ElapsedTime += ts;
	//MX_TRACE("m_ElapsedTime: {0}s", m_ElapsedTime);

	ProcessInputs(ts);

	// Update scene
	Moxxi::PerspectiveCamera& activeCamera = m_Cameras[(int)m_CameraMode];
	if (m_CameraMode == CameraMode::Free)
		activeCamera.SetPosition(m_CameraPosition);

	activeCamera.SetRotation(m_CameraRotation);
	activeCamera.SetFOV(m_CameraFOV);
	
	if (m_FollowBoid)
		activeCamera.LookAt(m_Boids.m_TargetBoid.m_Position);
	else
		activeCamera.LookAt(m_Boids.m_AveragePosition);

	m_LightDirection = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightOrientation.z), glm::vec3(0, 0, 1));
	m_Light.SetDirection(m_LightDirection);
	m_Light.SetColor(m_LightColor);
	
	m_Boids.OnUpdate(ts);

	// Render
	Moxxi::RenderCommand::SetClearColor(m_ClearColor);
	Moxxi::RenderCommand::Clear();
	Moxxi::Renderer::BeginScene(activeCamera, m_Light);
	Moxxi::RenderCommand::SetFrontFaceOrientation(Moxxi::RendererProps::FrontFaceOrientation::Clockwise);
	Moxxi::Renderer::SubmitArray(m_FlatShader, m_DomainVA, glm::mat4(1.0f), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	Moxxi::Renderer::SubmitArray(m_FlatShader, m_TowerVA, glm::translate(glm::mat4(1.0f), glm::vec3(0,3,0)), glm::vec4(0.7f, 0.2f, 0.2f, 1.0f));
	Moxxi::RenderCommand::SetFrontFaceOrientation(Moxxi::RendererProps::FrontFaceOrientation::CounterClockwise);
	for (auto& boid : m_Boids.m_Flock)
		Moxxi::Renderer::SubmitArray(m_FlatShader, m_BoidVA, boid.GetModelMatrix(), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
	Moxxi::Renderer::SubmitArray(m_FlatShader, m_BoidVA, m_Boids.GetTargetBoid().GetModelMatrix(), glm::vec4(0.2f, 0.2f, 0.9f, 1.0f));
	//Moxxi::Renderer::SubmitArray(m_FlatShader, m_BoidVA, glm::translate(glm::mat4(1.0f), m_Boids.m_ControllerPosition), glm::vec4(0.8f, 0.2f, 0.9f, 1.0f));
	Moxxi::Renderer::EndScene();
} // BoidsLayer::OnUpdate

void BoidsLayer::OnImGuiRender(Moxxi::TimeStep ts)
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
	ImGui::Text("No Boids: %d", m_Boids.m_Flock.size());
	ImGui::End();

	// Boids ---------------------------------------------------------------
	ImGui::Begin("Boids parameters", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PushItemWidth(120);
	ImGui::Text("Behaviour");
	ImGui::SliderFloat("Separation Factor", &m_Boids.m_SeparationFactor, 0, 0.5f);
	ImGui::SliderFloat("Alignement Factor", &m_Boids.m_AlignementFactor, 0, 0.5f);
	ImGui::SliderFloat("Cohesion Factor",   &m_Boids.m_CohesionFactor,   0, 0.5f);
	ImGui::SliderFloat("Follow Factor",   &m_Boids.m_FollowFactor,   0, 0.5f);
	ImGui::Text("Speed");
	ImGui::SliderFloat("Max Speed", &m_Boids.m_SpeedMultiplier, 1, 5);
	ImGui::SliderFloat("Min Speed", &m_Boids.m_MinSpeed, 1, m_Boids.m_SpeedMultiplier);
	ImGui::Text("Awareness");
	ImGui::SliderFloat("Search Radius", &m_Boids.m_SearchRadius, 1, 8);
	ImGui::SliderFloat("FOV", &m_Boids.m_FOV, 0, 180);
	ImGui::End();

	// Camera Transform ----------------------------------------------------
	ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Follow");
	if (ImGui::Button("Follow Flock"))
		m_FollowBoid = false;
	if (ImGui::Button("Follow Boid"))
		m_FollowBoid = true;
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

} // BoidsLayer::OnImGuiRender

void BoidsLayer::InitCameras()
{
	m_Cameras[(int)CameraMode::Tower].m_UseTarget = true;
	m_Cameras[(int)CameraMode::Tower].SetPosition({0, 8, 0});
	m_Cameras[(int)CameraMode::FollowFlockBack].m_UseTarget = true;
	m_Cameras[(int)CameraMode::FollowFlockBack].SetPosition({ 0, 8, 0 });
	m_Cameras[(int)CameraMode::FollowFlockSide].m_UseTarget = true;
	m_Cameras[(int)CameraMode::FollowFlockSide].SetPosition({ 0, 8, 0 });
}

void BoidsLayer::InitFlatShader()
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
void BoidsLayer::InitTowerMesh()
{
	// VAOs
	m_TowerVA.reset(Moxxi::VertexArray::Create());

	// VBO
	float vertices[] = {
		-1.2f, -3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //0
		 1.2f, -3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //1
		-1.2f,  3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //3
							  
		-1.2f,  3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //3
		 1.2f, -3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //1
		 1.2f,  3.0f, -1.2f,  0.0f, 0.0f, -1.0f, //2
							  
		 1.2f, -3.0f, -1.2f,  1.0f, 0.0f, 0.0f, //1
		 1.2f, -3.0f,  1.2f,  1.0f, 0.0f, 0.0f, //5
		 1.2f,  3.0f, -1.2f,  1.0f, 0.0f, 0.0f, //2
							  
		 1.2f,  3.0f, -1.2f,  1.0f, 0.0f, 0.0f, //2
		 1.2f, -3.0f,  1.2f,  1.0f, 0.0f, 0.0f, //5
		 1.2f,  3.0f,  1.2f,  1.0f, 0.0f, 0.0f, //6
							  
		 1.2f, -3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //5
		-1.2f, -3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //4
		 1.2f,  3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //6
						
		 1.2f,  3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //6
		-1.2f, -3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //4
		-1.2f,  3.0f,  1.2f,  0.0f, 0.0f, 1.0f, //7
						 
		-1.2f, -3.0f,  1.2f,  -1.0f, 0.0f, 0.0f, //4
		-1.2f, -3.0f, -1.2f,  -1.0f, 0.0f, 0.0f, //0
		-1.2f,  3.0f,  1.2f,  -1.0f, 0.0f, 0.0f, //7
						
		-1.2f,  3.0f,  1.2f,  -1.0f, 0.0f, 0.0f, //7
		-1.2f, -3.0f, -1.2f,  -1.0f, 0.0f, 0.0f, //0
		-1.2f,  3.0f, -1.2f,  -1.0f, 0.0f, 0.0f, //3
						
		-1.2f,  3.0f, -1.2f,  0.0f, 1.0f, 0.0f, //3
		 1.2f,  3.0f, -1.2f,  0.0f, 1.0f, 0.0f, //2
		-1.2f,  3.0f,  1.2f,  0.0f, 1.0f, 0.0f, //7
						
		-1.2f,  3.0f,  1.2f,  0.0f, 1.0f, 0.0f, //7
		 1.2f,  3.0f, -1.2f,  0.0f, 1.0f, 0.0f, //2
		 1.2f,  3.0f,  1.2f,  0.0f, 1.0f, 0.0f  //6
	};
	Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, sizeof(vertices)));
	Moxxi::BufferLayout layout = {
		{ Moxxi::ShaderDataType::Float3, "aPosition" },
		{ Moxxi::ShaderDataType::Float3, "aNormal" }
	};
	vertexBuffer->SetLayout(layout);
	m_TowerVA->AddVertexBuffer(vertexBuffer);

	// EBO
	//uint32_t indices[6 * 5] = {
	//	0, 1, 3,
	//	3, 1, 2,
	//	1, 5, 2, 
	//	2, 5, 6,
	//	5, 4, 6, 
	//	6, 4, 7,
	//	4, 0, 7, 
	//	7, 0, 3,
	//	3, 2, 7, 
	//	7, 2, 6,
	//	4, 5, 0, 
	//	0, 5, 1
	//};
	//Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
	//indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 6 * 6));
	//m_TowerVA->SetIndexBuffer(indexBuffer);
} // BoidsLayer::InitDomainMesh

void BoidsLayer::InitDomainMesh()
{
	// VAOs
	m_DomainVA.reset(Moxxi::VertexArray::Create());

	// VBO
	float vertices[] = {
		-DOMAIN_SIZE, 0, -DOMAIN_SIZE, 0, 1, 0,
		 DOMAIN_SIZE, 0, -DOMAIN_SIZE, 0, 1, 0,
		-DOMAIN_SIZE, 0,  DOMAIN_SIZE, 0, 1, 0,
		 
		 DOMAIN_SIZE, 0, -DOMAIN_SIZE, 0, 1, 0,
		 DOMAIN_SIZE, 0,  DOMAIN_SIZE, 0, 1, 0,
		-DOMAIN_SIZE, 0,  DOMAIN_SIZE, 0, 1, 0
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
} // BoidsLayer::InitDomainMesh

void BoidsLayer::InitBoidsMesh()
{
	// VAOs
	m_BoidVA.reset(Moxxi::VertexArray::Create());

	// VBO
	/*float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};*/

	float vertices[] = {
		 0.000000f, -0.469336f,  1.00000f, 0.840173f, -0.485084f,-0.242492f,
		 0.000000f,  0.000000f, -1.00000f, 0.840173f, -0.485084f,-0.242492f,
		 0.866025f,  0.234668f,  1.00000f, 0.840173f, -0.485084f,-0.242492f,

		 0.000000f, -0.469336f,  1.000000f, 0.000000f, 0.000000f, 1.000000f,
		 0.866025f,  0.234668f,  1.000000f, 0.000000f, 0.000000f, 1.000000f,
		-0.866025f,  0.234668f,  1.000000f, 0.000000f, 0.000000f, 1.000000f,

		 0.866025f,  0.234668f,  1.000000f, 0.000000f, 0.970148f, -0.242512f,
		 0.000000f,  0.000000f, -1.000000f, 0.000000f, 0.970148f, -0.242512f,
		-0.866025f,  0.234668f,  1.000000f, 0.000000f, 0.970148f, -0.242512f,

		-0.866025f,  0.234668f,  1.000000f, -0.840173f, -0.485084f, -0.242492f,
		 0.000000f,  0.000000f, -1.00000f, -0.840173f, -0.485084f, -0.242492f,
		 0.000000f, -0.469336f,  1.000000f, -0.840173f, -0.485084f, -0.242492f

		// 0.000000f, -0.469336f, 1.000000f, | 0.840173f, -0.485084f,-0.242492f,
		// 0.000000f, 0.000000f, -1.00000f, | 0.000000f, 0.000000f, 1.000000f,
		// 0.866025f, 0.234668f, 1.000000f,  | 0.000000f, 0.970148f, -0.242512f,
		// -0.866025f, 0.234668f, 1.000000f, | -0.840173f, -0.485084f, -0.242492f,

	};
	Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, sizeof(vertices)));
	Moxxi::BufferLayout layout = {
		{ Moxxi::ShaderDataType::Float3, "aPosition" },
		{ Moxxi::ShaderDataType::Float3, "aNormal"   }
	};
	vertexBuffer->SetLayout(layout);
	m_BoidVA->AddVertexBuffer(vertexBuffer);

} // BoidsLayer::InitBoidsMesh

void BoidsLayer::ProcessInputs(Moxxi::TimeStep ts)
{
	static float boost = 3;
	static uint32_t boostKey = MX_KEY_LEFT_SHIFT;
	Moxxi::PerspectiveCamera& activeCamera = m_Cameras[(int)m_CameraMode];

	if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_ADD))
		m_Boids.m_Flock.push_back(Boid(m_Boids.m_Flock.size()));
	else if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_SUBTRACT) && m_Boids.m_Flock.size() >= 1)
		m_Boids.m_Flock.pop_back();

	if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_0))
		m_CameraMode = CameraMode::Free;
	else if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_1))
		m_CameraMode = CameraMode::Tower;
	else if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_2))
		m_CameraMode = CameraMode::FollowFlockBack;
	else if (Moxxi::Input::IsKeyPressed(MX_KEY_KP_3))
		m_CameraMode = CameraMode::FollowFlockSide;

	if (m_CameraMode == CameraMode::Free)
	{
		if (Moxxi::Input::IsKeyPressed(MX_KEY_W))
			m_CameraPosition += m_CameraMoveSpeed * ts * activeCamera.GetFront()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_S))
			m_CameraPosition -= m_CameraMoveSpeed * ts * activeCamera.GetFront()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_A))
			m_CameraPosition -= m_CameraMoveSpeed * ts * activeCamera.GetRight()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_D))
			m_CameraPosition += m_CameraMoveSpeed * ts * activeCamera.GetRight()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_LEFT_CONTROL))
			m_CameraPosition -= m_CameraMoveSpeed * ts * activeCamera.GetUp()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_SPACE))
			m_CameraPosition += m_CameraMoveSpeed * ts * activeCamera.GetUp()
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_LEFT))
			m_CameraRotation.y -= m_CameraSensitivity * ts * 150
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_RIGHT))
			m_CameraRotation.y += m_CameraSensitivity * ts * 150
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_UP))
			m_CameraRotation.x += m_CameraSensitivity * ts * 150
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_DOWN))
			m_CameraRotation.x -= m_CameraSensitivity * ts * 150
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (m_CameraRotation.x > 89.f)
			m_CameraRotation.x = 89.f;
		if (m_CameraRotation.x < -89.f)
			m_CameraRotation.x = -89.f;
	}
	else {
		if (Moxxi::Input::IsKeyPressed(MX_KEY_W))
			m_Boids.m_ControllerPosition += m_CameraSensitivity * ts * activeCamera.GetFront() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_S))
			m_Boids.m_ControllerPosition -= m_CameraSensitivity * ts * activeCamera.GetFront() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_A))
			m_Boids.m_ControllerPosition -= m_CameraSensitivity * ts * activeCamera.GetRight() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_D))
			m_Boids.m_ControllerPosition += m_CameraSensitivity * ts * activeCamera.GetRight() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (Moxxi::Input::IsKeyPressed(MX_KEY_LEFT_CONTROL))
			m_Boids.m_ControllerPosition -= m_CameraSensitivity * ts * activeCamera.GetUp() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_SPACE))
			m_Boids.m_ControllerPosition += m_CameraSensitivity * ts * activeCamera.GetUp() * 10.f
			* (Moxxi::Input::IsKeyPressed(boostKey) ? boost : 1.0f);

		if (m_CameraRotation.x > 89.f)
			m_CameraRotation.x = 89.f;
		if (m_CameraRotation.x < -89.f)
			m_CameraRotation.x = -89.f;
	}
} // BoidsLayer::ProcessInputs

void BoidsLayer::OnEvent(Moxxi::Event& event)
{
	Moxxi::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Moxxi::WindowResizeEvent>(MX_BIND_EVENT_FN(BoidsLayer::OnWindowResizeEvent));
} // BoidsLayer::OnEvent

bool BoidsLayer::OnWindowResizeEvent(Moxxi::WindowResizeEvent& event)
{
	m_Cameras[(int)CameraMode::Free].SetResolution(event.GetWidth(), event.GetHeight());
	m_Cameras[(int)CameraMode::Tower].SetResolution(event.GetWidth(), event.GetHeight());
	m_Cameras[(int)CameraMode::FollowFlockBack].SetResolution(event.GetWidth(), event.GetHeight());
	m_Cameras[(int)CameraMode::FollowFlockSide].SetResolution(event.GetWidth(), event.GetHeight());
	return true;
} // BoidsLayer::OnMouseMovedEvent