#include "../common.h"
#include "../app/includes.h"
#include "../system/event_loop.h"

#include "test.h"
#include "game_screen.h"

enum
{
	k_gameStateMainMenu = 1,
	k_gameStateGame
};

void android_main(android_app* application)
{
	pegas::EventLoop eventLoop(application);
	pegas::AndroidGameApplication gameApplication(application);

	pegas::GameStatePtr mainMenu(new pegas::DefaultGameState(k_gameStateMainMenu));
	pegas::GameStatePtr gameScreen(new pegas::DefaultGameState(k_gameStateGame));

	pegas::BaseScreenLayerPtr testLayer(new pegas::GameScreen());
	//pegas::BaseScreenLayerPtr testLayer(new pegas::TestScreen());
	((pegas::DefaultGameState*)mainMenu.get())->pushLayer(testLayer);

	pegas::GameStateManager* gameStateManager = gameApplication.getGameStateManager();
	gameStateManager->addGameState(mainMenu);
	gameStateManager->addGameState(gameScreen);
	gameStateManager->changeState(k_gameStateMainMenu);

	eventLoop.run(&gameApplication, &gameApplication);
}
