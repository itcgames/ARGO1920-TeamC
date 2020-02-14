#pragma once
#include "Utilities.h"
class SplashScreen
{
public:
	SplashScreen(MenuStates* t_currentScreen);
	~SplashScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
	MenuStates* m_currentScreen;

};

