#include "Game.h"



int main() 
{
	auto mainWindow = std::make_shared<GLWindow>(800, 600); // make the window.
	mainWindow->initialise(); 

	auto game = std::make_unique<Game>();
	game->generateGame(mainWindow); // create the game.

    while (!mainWindow->shouldClose()) 
    {
		glfwPollEvents();
		game->updateGame(mainWindow);
		mainWindow->swapBuffer();
    }
    return EXIT_SUCCESS;
}