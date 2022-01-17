#include "Moxxi.h"

class ExampleLayer : public Moxxi::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"){}

	void OnUpdate() override
	{
		MX_INFO("ExampleLayer::Update");
	}
	
	void OnEvent(Moxxi::Event& event) override
	{
		MX_INFO("{0}", event);
	}
};

class Boids : public Moxxi::Application
{
public:
	Boids() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Moxxi::ImGuiLayer());
	}
	~Boids() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new Boids();
}