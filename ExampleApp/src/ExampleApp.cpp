#define MAIN_APPLICATION_FILE
#include "Nodens.h"

#include "imgui/imgui.h"

class ExampleLayer : public Nodens::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
	{
		m_VertexArray.reset(Nodens::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f
		};

		Nodens::Ref<Nodens::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Nodens::VertexBuffer::Create(vertices, sizeof(vertices)));
		Nodens::BufferLayout layout = {
			{ Nodens::ShaderDataType::Float3, "a_Position" },
			{ Nodens::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Nodens::Ref<Nodens::IndexBuffer> indexBuffer;
		indexBuffer.reset(Nodens::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// ------
		m_SquareVA.reset(Nodens::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		Nodens::Ref<Nodens::VertexBuffer> squareVB;
		squareVB.reset(Nodens::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Nodens::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Nodens::Ref<Nodens::IndexBuffer> squareIB;
		squareIB.reset(Nodens::IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main ()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(Nodens::Shader::Create(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			out vec3 v_Position;

			void main ()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position, 1.0);
			}
		)";

		m_Shader2.reset(Nodens::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Nodens::TimeStep ts) override
	{
		//ND_TRACE("Delta time: {0}s ({1})", ts.GetSeconds(), ts.GetMilliseconds());

		if (Nodens::Input::IsKeyPressed(ND_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Nodens::Input::IsKeyPressed(ND_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		
		if (Nodens::Input::IsKeyPressed(ND_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Nodens::Input::IsKeyPressed(ND_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		
		if (Nodens::Input::IsKeyPressed(ND_KEY_A))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (Nodens::Input::IsKeyPressed(ND_KEY_D))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		Nodens::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Nodens::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Nodens::Renderer::BeginScene(m_Camera);

		Nodens::Renderer::SubmitIndexed(m_Shader2, m_SquareVA);
		Nodens::Renderer::SubmitIndexed(m_Shader, m_VertexArray);

		Nodens::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Nodens::Event& event) override
	{
	}

private:
	Nodens::Ref<Nodens::Shader> m_Shader;
	Nodens::Ref<Nodens::VertexArray> m_VertexArray;

	Nodens::Ref<Nodens::VertexArray> m_SquareVA;
	Nodens::Ref<Nodens::Shader> m_Shader2;

	Nodens::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotateSpeed = 1.0f;
};

class ExampleApp : public Nodens::Application
{
public:
	ExampleApp() : Application(Nodens::WindowProps("Example Application", 1920, 1080, false))
	{
		PushLayer(new ExampleLayer());
	}
	~ExampleApp() {}
};

Nodens::Application* Nodens::CreateApplication()
{
	return new ExampleApp();
}