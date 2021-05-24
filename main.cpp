#include "stdafx.h"
#include "Scene.h"

//void updateInput(Renderer& renderer) {}

//void handleInput(InputHandle& input) {}

//void updateAI() {}

//void updatePhysics(const float dt) {}

//void updateAudio(AudioHandle& audio) {}

//void draw(float interpol, Renderer& renderer) {}

void update(Scene& scene) {
	scene.update();
}
void draw(const float interpol, Scene& scene) {
	scene.draw(interpol);
}

void game() {
	////auto renderer = std::make_unique<Renderer>();
	////auto audio = std::make_unique<AudioHandle>();

	std::unique_ptr<sf::Clock> timer;
	float dt = 0.0f;
	float currentfps = 0;
	sf::Clock fpstimer = sf::Clock();

	timer = std::make_unique<sf::Clock>();
	double accumulator = 0;

	//auto input = std::make_unique<InputHandle>();
	auto scene = std::make_unique<Scene>();

	while (Settings::getSetting<bool>("running"))
	{

		double deltaTime = timer->getElapsedTime().asSeconds();
		accumulator += deltaTime;
		timer->restart();

		const float targetDT = Settings::getSetting<float>("targetDT");


		if (Settings::getSetting<TimeStepType>("TimeStepType") == TimeStepType::Variable) {
			//variable timestep
			update(*scene);
			accumulator = 0;
		}
		else {
			//fixed time step
			while (accumulator >= targetDT) {
				update(*scene);
				accumulator -= targetDT;
			}
		}


		//draw
		draw((float)accumulator / targetDT, *scene);
	}
}

void stop() {
#ifdef _DEBUG
	Logger::get() << "Game Closing\n";
#endif
	Settings::setSetting<bool>("running", false);
}


int main(int argc, char* argv[])
{

	std::cout.flush();

#ifdef _DEBUG
	Logger::get() << "Running in debug mode\n"
		<< "Current working directory: " << fs::current_path().generic_string() << "\n";
#endif

#ifdef _WIN32
	Logger::get() << "Current OS is: windows\n"
		<< "Windows debug version will check for memory leaks\n";
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	game();
	return 0;
}