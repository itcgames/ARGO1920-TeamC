#pragma once
#include "Utilities.h"
class CreditsScreen
{
public:
	CreditsScreen(MenuStates* t_currentScreen);
	~CreditsScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
	MenuStates* m_currentScreen;
};

