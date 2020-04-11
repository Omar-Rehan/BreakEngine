#include "pch.h"
#include "Application.h"


int main() {
	Application app;
	app.Initialize();
	app.GameLoop();
	app.Terminate();
	return 0;
}