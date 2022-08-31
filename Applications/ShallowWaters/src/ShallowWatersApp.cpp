#define MAIN_APPLICATION_FILE
#include "Moxxi.h"
#include "ShallowWatersLayer.h"

// -----------------------------------------------------------------------------
class ShallowWatersApp : public Moxxi::Application
{
public:
	ShallowWatersApp(const Moxxi::WindowProps props)
		: Application(props)
	{
		PushLayer(new ShallowWatersLayer());
	}
	~ShallowWatersApp() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new ShallowWatersApp(Moxxi::WindowProps("ShallowWaters", 720, 720, false));
}	