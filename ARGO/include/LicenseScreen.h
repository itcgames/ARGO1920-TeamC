#pragma once
#include "Utilities.h"
class LicenseScreen
{
public:
	LicenseScreen(MenuStates* t_currentScreen);
	~LicenseScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
	MenuStates* m_currentScreen;

};

