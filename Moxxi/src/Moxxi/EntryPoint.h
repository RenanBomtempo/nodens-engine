#pragma once
#ifdef MX_PLATFORM_WINDOWS

extern Moxxi::Application* Moxxi::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = Moxxi::CreateApplication();
	app->Run();
	delete app;
}

#endif // MX_PLATFORM_WINDOWS
