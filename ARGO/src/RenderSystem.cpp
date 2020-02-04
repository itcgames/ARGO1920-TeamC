#include "stdafx.h"
#include "RenderSystem.h"

void RenderSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	std::vector<Component*> comps = t_e->getComponents();
	for (int i = 0; i < comps.size(); i++)
	{
		if (comps[i]->getType() == Component::ComponentType::Position)
		{
			m_posComps.push_back(static_cast<PositionComponent*>(comps[i]));

			//set fill colour
			Uint8 rgb[3];

			switch (t_e->getId())
			{
			case 0:
				rgb[0] = 0;
				rgb[1] = 255;
				rgb[2] = 0;
				break;
			case 1:
				rgb[0] = 255;
				rgb[1] = 0;
				rgb[2] = 0;
				break;
			case 2:
				rgb[0] = 187;
				rgb[1] = 140;
				rgb[2] = 83;
				break;
			case 3:
				rgb[0] = 128;
				rgb[1] = 128;
				rgb[2] = 128;
				break;
			default:
				rgb[0] = 255;
				rgb[1] = 0;
				rgb[2] = 255;
				break;
			}

			for (int i = 0; i < 3; i++)
			{
				m_rgbVec.push_back(rgb[i]);
			}
		}
	}
	comps.clear();
}

void RenderSystem::render(SDL_Renderer* t_renderer)
{
	for (int i = 0; i < m_posComps.size(); i++)
	{
		if (m_posComps[i] != nullptr)
		{
			Uint8 prevRGBA[4];
			SDL_GetRenderDrawColor(t_renderer, &prevRGBA[0], &prevRGBA[1], &prevRGBA[2], &prevRGBA[3]);

			SDL_Rect rect;
			rect.x = m_posComps[i]->getPos().x - 25;
			rect.y = m_posComps[i]->getPos().y - 25;
			rect.w = 50;
			rect.h = 50;

			SDL_SetRenderDrawColor(t_renderer, m_rgbVec[i * 3], m_rgbVec[i * 3 + 1], m_rgbVec[i * 3 + 2], 255);
			SDL_RenderFillRect(t_renderer, &rect);

			//reset the renderer to previous colour
			SDL_SetRenderDrawColor(t_renderer, prevRGBA[0], prevRGBA[1], prevRGBA[2], prevRGBA[3]);
		}
	}
}
