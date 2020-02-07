#pragma once
class ImGuiHelper
{
public:
	static void InputText(std::string& t_string, std::string t_inputTitle);
	static void InputTextWithHint(std::string& t_string, std::string t_inputTitle, std::string t_hint);
	static std::string GetDecimalPercision(int t_decimalPercision, bool t_isFloat = false);
	static void InputFloat2(glm::vec2& t_vector, std::string t_title);
	static void InputFloat3(glm::vec3& t_vector, std::string t_title);
private:
};

