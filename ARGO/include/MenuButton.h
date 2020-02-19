#pragma once
 class MenuButton
{
public:
	MenuButton(glm::vec2 t_position, glm::vec2 t_size, SDL_Texture* t_texture);
	MenuButton();
	~MenuButton();

	void init(glm::vec2 t_position, glm::vec2 t_size, SDL_Texture* t_texture);
	void setPosition(glm::vec2 t_position);
	glm::vec2 getPosition();

	void setIsSelected(bool t_isSelected);
	bool getIsSelected();

	void render(SDL_Renderer* t_renderer);

private:
	

	glm::vec2 m_position;
	glm::vec2 m_size;
	SDL_Texture* m_buttonTexture;
	bool m_isSelected{ false };
};

