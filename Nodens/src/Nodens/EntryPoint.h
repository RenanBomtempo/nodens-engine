#pragma once
#ifdef ND_PLATFORM_WINDOWS

extern Nodens::Application* Nodens::CreateApplication();

int main(int argc, char** argv) 
{
	Nodens::Log::Init();

	auto app = Nodens::CreateApplication();
	app->Run();
	delete app;
}

#endif // ND_PLATFORM_WINDOWS
