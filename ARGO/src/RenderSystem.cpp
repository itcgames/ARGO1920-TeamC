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
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	VisualComponent* visComp = static_cast<VisualComponent*>(t_entity.getComponent(ComponentType::Visual));
	FSMComponent* FSMComp = static_cast<FSMComponent*>(t_entity.getComponent(ComponentType::FSM));
	if (transformComp != nullptr)
	{
		if (inView(transformComp) || transformComp->getAlwaysOnscreen())
		{

			TextComponent* textComp = static_cast<TextComponent*>(t_entity.getComponent(ComponentType::Text));

			if (FSMComp != nullptr)
			{
				glm::vec2 renderPosition = transformComp->getPos();
				ColliderCircleComponent* circleColliderComp = static_cast<ColliderCircleComponent*>(t_entity.getComponent(ComponentType::ColliderCircle));
				if (circleColliderComp)
				{
					renderPosition -= glm::vec2(circleColliderComp->getRadius(), circleColliderComp->getRadius());
				}
				renderTexture(FSMComp->getFSM().getCurrent()->getTexture(), renderPosition.x, renderPosition.y, t_renderer, transformComp->getRotation(), FSMComp->getFSM().getCurrent()->getFrame());
			}

			if (visComp != nullptr)
			{
				glm::vec2 renderPosition = transformComp->getPos();
				ColliderCircleComponent* circleColliderComp = static_cast<ColliderCircleComponent*>(t_entity.getComponent(ComponentType::ColliderCircle));
				if (circleColliderComp)
				{
					renderPosition -= glm::vec2(circleColliderComp->getRadius(), circleColliderComp->getRadius());
				}
				renderTexture(visComp, renderPosition.x, renderPosition.y, t_renderer, transformComp->getRotation());
			}

			PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(t_entity.getComponent(ComponentType::Primitive));
			ParticleEmitterComponent* emitComp = static_cast<ParticleEmitterComponent*>(t_entity.getComponent(ComponentType::ParticleEmitter));
			if (textComp != nullptr)
			{
				renderText(t_renderer, transformComp, textComp);
			}

			if (!textComp && !visComp && !emitComp && primComp)
			{
				ColourComponent* colComp = static_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));
				//we dont need to check if colComp is null as its handled in the function
				renderPrimitives(t_renderer, transformComp, colComp, primComp);
			}


			if (emitComp && primComp)
			{
				ColourComponent* colComp = static_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));
				if (colComp)
				{
					renderParticles(t_renderer, emitComp, primComp, colComp, transformComp);
				}
			}
		}
	}
}
void RenderSystem::renderLight(SDL_Renderer* t_renderer, Entity& t_entity)
{
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));

	if (transformComp != nullptr)
	{
		if (inView(transformComp))
		{
			Uint8 prevRGBA[4];
			SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

			SDL_Rect rect;
			rect.x = transformComp->getPos().x;
			rect.y = transformComp->getPos().y;
			rect.w = Utilities::TILE_SIZE;
			rect.h = Utilities::TILE_SIZE;
			rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
			rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
			Uint8 alpha = static_cast<lightFieldComponent*>(t_entity.getComponent(ComponentType::LightField))->getWeight();
			if (alpha > 245)
			{
				alpha = 245;
			}

			SDL_SetRenderDrawColor(t_renderer, 20, 0, 0, alpha);
			SDL_RenderFillRect(t_renderer, &rect);
			SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
		}
	}
}
void RenderSystem::renderPrimitives(SDL_Renderer* t_renderer, TransformComponent* t_posComp, ColourComponent* t_colComp, PrimitiveComponent* t_primitive)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	SDL_Rect rect;
	rect.x = t_posComp->getPos().x;
	rect.y = t_posComp->getPos().y;
	rect.w = t_primitive->getSize().x;
	rect.h = t_primitive->getSize().y;

	if (!t_primitive->getStaticPosition())
	{
		rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
		rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	}

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


void RenderSystem::renderParticles(SDL_Renderer* t_renderer, ParticleEmitterComponent* t_emitter, PrimitiveComponent* t_primitive, ColourComponent* t_colComp, TransformComponent* t_posComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	SDL_Rect rect;
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


	for (int i = 0; i < t_emitter->getMaxParticles(); i++)
	{
		if (t_emitter->getParticleAlive(i))
		{
			SDL_SetRenderDrawColor(t_renderer, t_emitter->getParticleColour(i).x, t_emitter->getParticleColour(i).y, t_emitter->getParticleColour(i).z, t_emitter->getParticleColour(i).a);
			rect.w = t_emitter->getParticleSize(i);
			rect.h = t_emitter->getParticleSize(i);
			rect.x = t_emitter->getParticlePosition(i).x;
			rect.y = t_emitter->getParticlePosition(i).y;

			rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
			rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
			SDL_RenderFillRect(t_renderer, &rect);
		}
	}
	//reset the renderer to previous colour
	SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
}

void RenderSystem::renderTexture(VisualComponent* t_visComp, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer, double t_angle, SDL_Rect* t_clip, SDL_Point* t_center, SDL_RendererFlip t_flip)
{
	SDL_Rect renderQuad = { t_textureLeftPos, t_textureTopPos, t_visComp->getWidth(), t_visComp->getHeight() };

	//Set clip rendering dimensions
	if (t_clip != NULL)
	{
		renderQuad.w = t_clip->w;
		renderQuad.h = t_clip->h;
	}


	glm::vec2 offset = glm::vec2(0, 0);
	if (t_visComp->getOffset() != glm::vec2(0,0))
	{
		float theta = glm::radians(t_angle);
		float cs = cos(theta);
		float sn = sin(theta);
		offset.x = t_visComp->getOffset().x * cs - t_visComp->getOffset().y * sn;
		offset.y = t_visComp->getOffset().x * sn + t_visComp->getOffset().y * cs;
	}

	if (!t_visComp->getStaticPosition())
	{
		renderQuad.x = renderQuad.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x + offset.x;
		renderQuad.y = renderQuad.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y + offset.y;
	}
	//Render to screen
	SDL_RenderCopyEx(t_renderer, t_visComp->getTexture(), t_clip, &renderQuad, t_angle, t_center, t_flip);
}

void RenderSystem::renderTexture(SDL_Texture* t_tex, int t_textureLeftPos, int t_textureTopPos, SDL_Renderer* t_renderer, double t_angle, SDL_Rect* t_clip, SDL_Point* t_center, SDL_RendererFlip t_flip)
{
	int width;
	int height;
	SDL_QueryTexture(t_tex, NULL, NULL, &width, &height);
	SDL_Rect renderQuad = { t_textureLeftPos, t_textureTopPos, width, height };

	//Set clip rendering dimensions
	if (t_clip != NULL)
	{
		renderQuad.w = t_clip->w;
		renderQuad.h = t_clip->h;
	}
	renderQuad.x = renderQuad.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
	renderQuad.y = renderQuad.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	//Render to screen
	SDL_RenderCopyEx(t_renderer, t_tex, t_clip, &renderQuad, t_angle, t_center, t_flip);
}

void RenderSystem::renderText(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	SDL_Rect rect;
	rect.x = t_posComp->getPos().x;
	rect.y = t_posComp->getPos().y;
	rect.w = t_textComp->getWidth();
	rect.h = t_textComp->getHeight();

	if (!t_textComp->hasStaticPos())
	{
		rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
		rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	}

	if (t_textComp->getColour().r != 255 && t_textComp->getColour().g != 255 && t_textComp->getColour().b != 255 && t_textComp->getColour().a != 255)
	{
		SDL_SetRenderDrawColor(t_renderer, t_textComp->getColour().r, t_textComp->getColour().g, t_textComp->getColour().b, t_textComp->getColour().a);
	}

	SDL_RenderCopy(t_renderer, t_textComp->getTexture(), NULL, &rect);

	if (t_textComp->getColour().r != 255 && t_textComp->getColour().g != 255 && t_textComp->getColour().b != 255 && t_textComp->getColour().a != 255)
	{
		//reset the renderer to previous colour
		SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
	}
}

void RenderSystem::renderHUD(SDL_Renderer* t_renderer, TransformComponent* t_posComp, TextComponent* t_textComp, VisualComponent* t_visComp, PrimitiveComponent* t_primitiveComp, HUDComponent* t_hudComp)
{
	Uint8 prevRGBA[4];
	SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

	SDL_Rect rect;
	rect.x = t_posComp->getPos().x;
	rect.y = t_posComp->getPos().y;
	rect.w = t_textComp->getWidth();
	rect.h = t_textComp->getHeight();

	if (!t_textComp->hasStaticPos())
	{
		rect.x = rect.x + Utilities::SCREEN_WIDTH / 2 - m_focusPoint.x;
		rect.y = rect.y + Utilities::SCREEN_HEIGHT / 2 - m_focusPoint.y;
	}

	if (t_textComp->getColour().r != 255 && t_textComp->getColour().g != 255 && t_textComp->getColour().b != 255 && t_textComp->getColour().a != 255)
	{
		SDL_SetRenderDrawColor(t_renderer, t_textComp->getColour().r, t_textComp->getColour().g, t_textComp->getColour().b, t_textComp->getColour().a);
	}

	SDL_RenderCopy(t_renderer, t_textComp->getTexture(), NULL, &rect);

	if (t_textComp->getColour().r != 255 && t_textComp->getColour().g != 255 && t_textComp->getColour().b != 255 && t_textComp->getColour().a != 255)
	{
		//reset the renderer to previous colour
		SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
	}
}

bool RenderSystem::inView(TransformComponent* t_posComp)
{
	float left, right, top, down;
	left = m_focusPoint.x - Utilities::SCREEN_WIDTH * OFF_SCREEN_SCALAR;
	right = m_focusPoint.x + Utilities::SCREEN_WIDTH * OFF_SCREEN_SCALAR;
	top = m_focusPoint.y - Utilities::SCREEN_HEIGHT * OFF_SCREEN_SCALAR;
	down = m_focusPoint.y + Utilities::SCREEN_HEIGHT * OFF_SCREEN_SCALAR;

	//returns false if object is outside of the window, true if it is in view
	return left < t_posComp->getPos().x && right > t_posComp->getPos().x &&
		top < t_posComp->getPos().y && down > t_posComp->getPos().y;
}

void RenderSystem::setFocus(glm::vec2 t_point)
{
	m_focusPoint = t_point;
}

glm::vec2& RenderSystem::getFocus()
{
	return m_focusPoint;
}

void RenderSystem::operator=(const RenderSystem& t_otherRenderSystem)
{
	m_focusPoint = t_otherRenderSystem.m_focusPoint;
}
