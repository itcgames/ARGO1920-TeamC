#pragma once
#include "Utilities.h"
class OptionsScreen
{
public:
	OptionsScreen(MenuStates* t_currentScreen);
	~OptionsScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
	MenuStates* m_currentScreen;

};

