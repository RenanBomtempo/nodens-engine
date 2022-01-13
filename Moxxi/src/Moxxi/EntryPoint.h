#pragma once
#ifdef MX_PLATFORM_WINDOWS

extern Moxxi::Application* Moxxi::CreateApplication();

int main(int argc, char** argv) 
{
	Moxxi::Log::Init();
	MX_CORE_INFO("Initialized Core Log!");
	MX_INFO("Initialized Client Log!");

	auto app = Moxxi::CreateApplication();
	app->Run();
	delete app;
}

#endif // MX_PLATFORM_WINDOWS
