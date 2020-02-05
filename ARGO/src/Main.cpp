#include <stdafx.h>
#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));

	Game game;
	game.run();

	return EXIT_SUCCESS;
}