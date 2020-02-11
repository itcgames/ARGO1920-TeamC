#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::~RenderSystem()
{
	BaseSystem::~BaseSystem();
}

void RenderSystem::update(Entity& t_entity)
{
	//some sort of update to visual components here
	//maybe shaders/blending?
}

void RenderSystem::render(SDL_Renderer* t_renderer, Entity& t_entity)
{
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));

	if (posComp != nullptr)
	{
		VisualComponent* visComp = static_cast<VisualComponent*>(t_entity.getComponent(ComponentType::Visual));
		TextComponent* textComp = static_cast<TextComponent*>(t_entity.getComponent(ComponentType::Text));
		if (visComp != nullptr)
		{
			renderTextures(visComp, posComp->getPos().x, posComp->getPos().y, t_renderer);
		}
		if (textComp != nullptr)
		{
			renderText(t_renderer, posComp, textComp);
		}
		else if (!textComp && !visComp)
		{
			ColourComponent* colComp = static_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));
			renderPrimitives(t_renderer, posComp, colComp);
		}
	}
}

void RenderSystem::renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	//hard-coded primitive size
	//fite me
	SDL_Rect rect;
	rect.x = t_posComp->getPos().x - 25;
	rect.y = t_posComp->getPos().y - 25;
	rect.w = 50;
	rect.h = 50;

	rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
	rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;

	Colour colour;
	//set colour from the component
	if (t_colComp)
	{
		colour = t_colComp->getColour();
	}
	//else make it red
	else
	{
		colour.red = 255;
	}

	SDL_SetRenderDrawColor(t_renderer, colour.red, colour.green, colour.blue, colour.alpha);
	SDL_RenderFillRect(t_renderer, &rect);

	//reset the renderer to previous colour
	SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
}

void RenderSystem::renderTextures(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer, SDL_Rect* t_clip, double t_angle, SDL_Point* t_center, SDL_RendererFlip t_flip)
{
	SDL_Rect renderQuad = { t_textureLeftPos, t_textureTopPos, t_visComp->getWidth(), t_visComp->getHeight() };

	//Set clip rendering dimensions
	if (t_clip != NULL)
	{
		renderQuad.w = t_clip->w;
		renderQuad.h = t_clip->h;
	}

	renderQuad.x = renderQuad.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
	renderQuad.y = renderQuad.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	//Render to screen
	SDL_RenderCopyEx(t_renderer, t_visComp->getTexture(), t_clip, &renderQuad, t_angle, t_center, t_flip);
}

void RenderSystem::renderText(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	SDL_Rect rect = t_textComp->getRect();
	rect.x = t_posComp->getPos().x;
	rect.y = t_posComp->getPos().y;

	if (!t_textComp->hasStatisPos())
	{
		rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
		rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	}

	SDL_SetRenderDrawColor(t_renderer, t_textComp->getColour().r, t_textComp->getColour().g, t_textComp->getColour().b, t_textComp->getColour().a);

	SDL_RenderCopy(t_renderer, t_textComp->getTexture(), NULL, &rect);

	//reset the renderer to previous colour
	SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
}

void RenderSystem::setFocus(glm::vec2 t_point)
{
	m_focusPoint = t_point;
}
