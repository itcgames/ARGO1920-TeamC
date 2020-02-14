#pragma once
#include "Utilities.h"
class MenuScreen
{
public:
	MenuScreen(MenuStates* t_currentScreen);
	~MenuScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
	MenuStates* m_currentScreen;

};

