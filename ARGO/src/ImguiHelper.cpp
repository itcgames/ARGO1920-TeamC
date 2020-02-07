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

std::string ImGuiHelper::GetDecimalPercision(int t_decimalPercision, bool t_isFloat)
{
	return "%." + std::to_string(t_decimalPercision) + (t_isFloat ? "f" : "");
}

void ImGuiHelper::InputFloat2(glm::vec2& t_vector, std::string t_title)
{
	float vector[2] = { t_vector.x, t_vector.y };
	ImGui::InputFloat2(t_title.c_str(), vector);
	t_vector = glm::vec2(vector[0], vector[1]);
}

void ImGuiHelper::InputFloat3(glm::vec3& t_vector, std::string t_title)
{
	float vector[3] = { t_vector.x, t_vector.y, t_vector.z };
	ImGui::InputFloat2(t_title.c_str(), vector);
	t_vector = glm::vec3(vector[0], vector[1], vector[2]);
}

