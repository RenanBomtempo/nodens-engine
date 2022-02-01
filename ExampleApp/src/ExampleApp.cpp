#include "Moxxi.h"

#include "imgui/imgui.h"

class ExampleLayer : public Moxxi::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
	{
		m_VertexArray.reset(Moxxi::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f
		};

		Moxxi::Ref<Moxxi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Moxxi::VertexBuffer::Create(vertices, sizeof(vertices)));
		Moxxi::BufferLayout layout = {
			{ Moxxi::ShaderDataType::Float3, "a_Position" },
			{ Moxxi::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Moxxi::Ref<Moxxi::IndexBuffer> indexBuffer;
		indexBuffer.reset(Moxxi::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// ------
		m_SquareVA.reset(Moxxi::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		Moxxi::Ref<Moxxi::VertexBuffer> squareVB;
		squareVB.reset(Moxxi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Moxxi::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Moxxi::Ref<Moxxi::IndexBuffer> squareIB;
		squareIB.reset(Moxxi::IndexBuffer::Create(squareIndices, 6));
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

		m_Shader.reset(Moxxi::Shader::Create(vertexSrc, fragmentSrc));

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

		m_Shader2.reset(Moxxi::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Moxxi::TimeStep ts) override
	{
		//MX_TRACE("Delta time: {0}s ({1})", ts.GetSeconds(), ts.GetMilliseconds());

		if (Moxxi::Input::IsKeyPressed(MX_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		
		if (Moxxi::Input::IsKeyPressed(MX_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		
		if (Moxxi::Input::IsKeyPressed(MX_KEY_A))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (Moxxi::Input::IsKeyPressed(MX_KEY_D))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		Moxxi::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Moxxi::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Moxxi::Renderer::BeginScene(m_Camera);

		Moxxi::Renderer::Submit(m_Shader2, m_SquareVA);
		Moxxi::Renderer::Submit(m_Shader, m_VertexArray);

		Moxxi::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Moxxi::Event& event) override
	{
	}

private:
	Moxxi::Ref<Moxxi::Shader> m_Shader;
	Moxxi::Ref<Moxxi::VertexArray> m_VertexArray;

	Moxxi::Ref<Moxxi::VertexArray> m_SquareVA;
	Moxxi::Ref<Moxxi::Shader> m_Shader2;

	Moxxi::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotateSpeed = 1.0f;


};

class ExampleApp : public Moxxi::Application
{
public:
	ExampleApp()
	{
		PushLayer(new ExampleLayer());
	}
	~ExampleApp() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new ExampleApp();
}