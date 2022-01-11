#include "Moxxi.h"

class Boids : public Moxxi::Application
{
public:
	Boids() {}
	~Boids() {}
};

Moxxi::Application* Moxxi::CreateApplication()
{
	return new Boids();
}