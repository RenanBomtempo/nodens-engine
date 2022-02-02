/*******************************************************/
/* PROGRAMA: main.cpp                                  */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 96/08/14                                  */
/* MODIFICAÇÃO: OPENGL E C++ EM ABRIL DE 2004          */
/*              DENISE BURGARELLI DUCZMAL              */
/*              RODNEY JOSUÉ BIEZUNER                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h> 
#include "funcoes.h"
#include "PerfTimer.h"

#include <iostream>
using std::cout;
using std::endl;
using std::ios;

#include <fstream>
using std::ofstream;

#define null 0
/*************variaveis externas ***************************** */
int flagpeano = 0;
int flagglobal = 0;
int flagproblem;
int flagonda_inicio = 0;
double cotaref = 1.0; //3.0
double cotades = 1.0; //3.0
double cotalaplace = 0.0001;//0.1
double cotadeslap = 0.0008;//0.06;
double cotaref_term = 10.0;
double cotades_term = 10.0;

void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //set white background color
	glClear(GL_COLOR_BUFFER_BIT);//clear a tela

	defineTableOfColors(); //define a tabela das cores usadas

	glMatrixMode(GL_PROJECTION);//set coordinate system
	glLoadIdentity();
	gluOrtho2D(0.0, 600.0, 0.0, 600.0);

}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);      //clear a tela

	flagproblem = 0;      //Escolhe o problema a ser rodado 
				   //            Laplace [flagproblem = 0], 
				   //               Onda [flagproblem = 1] 
				   //              Calor [flagproblem = 2]
				   //      Termoacústico [flagproblem = 3]
	Dchange_peano(1);      //Mostra (1) ou nao (0) a curva de Peano
	init();               //Inicializa a malha de acordo com o problema

	CPerfTimer Perftemp;
	//Perftemp.Start();    

	play(flagproblem);

	//Perftemp.Stop();

	//cout << "Tempo de Processamento (segundos): " << Perftemp.Elapsedms()/1000 << endl;

	//cout << "\nEnd of program." << endl;

	glFlush();//send all output to display
}

int main(int argc, char** argv)
{
	myInit();

	glutDisplayFunc(myDisplay);


	glutMainLoop();

	return(0);
}

#define MAIN_APPLICATION_FILE
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