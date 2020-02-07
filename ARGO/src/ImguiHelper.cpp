#include "stdafx.h"
#include "ImguiHelper.h"

void ImGuiHelper::InputText(std::string& t_string, std::string t_inputTitle)
{
	char charStr[128];
	std::copy(t_string.begin(), t_string.end(), charStr);
	charStr[t_string.size()] = '\0';
	ImGui::InputText(t_inputTitle.c_str(), charStr, IM_ARRAYSIZE(charStr));
	t_string = charStr;
}

void ImGuiHelper::InputTextWithHint(std::string& t_string, std::string t_inputTitle, std::string t_hint)
{
	char charStr[128];
	std::copy(t_string.begin(), t_string.end(), charStr);
	charStr[t_string.size()] = '\0';
	ImGui::InputTextWithHint(t_inputTitle.c_str(), t_hint.c_str(), charStr, IM_ARRAYSIZE(charStr));
	t_string = charStr;
}
