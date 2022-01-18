#include "Moxxi.h"

#include "imgui/imgui.h"

class ExampleLayer : public Moxxi::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"){}

	void OnUpdate() override
	{
		if (Moxxi::Input::IsKeyPressed(MX_KEY_TAB))
			MX_TRACE("Tab is pressed");
	}
	
	void OnEvent(Moxxi::Event& event) override
	{
		
	}
};

class Boids : public Moxxi::Application
{
public:
	Boids() 
	{
		PushLayer(new ExampleLayer());
	}
	~Boids() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new Boids();
}