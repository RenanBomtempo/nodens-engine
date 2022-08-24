#define MAIN_APPLICATION_FILE
#include "Moxxi.h"
#include "BoidsLayer.h"
#include "Boids.h"

// -----------------------------------------------------------------------------
class BoidsApp : public Moxxi::Application
{
public:
	BoidsApp(const Moxxi::WindowProps props)
		: Application(props)
	{
		PushLayer(new BoidsLayer());
	}
	~BoidsApp() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new BoidsApp(Moxxi::WindowProps("Boids", 1920, 1080, false));
}	