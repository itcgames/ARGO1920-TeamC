#include "stdafx.h"
#include "HUDManager.h"

HUDManager::HUDManager(Entity(&t_players)[Utilities::S_MAX_PLAYERS]):
	m_players{t_players}
{
}
/// <summary>
/// Initializes the Manager now that the Renderer is not null.
/// </summary>
/// <param name="t_renderer"></param>
void HUDManager::init(SDL_Renderer* t_renderer)
{
	m_renderer = t_renderer;
	float index = 0;
	for (auto& i : m_playerHUD) {
		setUpHUD(i, index);
		index++;
	}
}

/// <summary>
/// Gets the data and draws the HUD for each player and tracks their resources.
/// </summary>
void HUDManager::update()
{
	for (auto& hudElement : m_playerHUD) {
		//Get the layout Data
		HUDComponent* hudComp = static_cast<HUDComponent*>(hudElement.HUDLayoutData.getComponent(ComponentType::HUD));
		//Calculates the position of the HUD. The " 1 " is needed because there is a buffer between the screen edge and the first HUD.
		hudComp->setHUDPosition(glm::vec2(((Utilities::SCREEN_WIDTH * Utilities::PERCENTAGE_BUFFER_BETWEEN_HUDS) * (hudComp->getIndex() + 1)) + ((Utilities::SCREEN_WIDTH * Utilities::PERCENTAGE_SCREENSPACE_OF_HUD) * hudComp->getIndex()), hudComp->getHUDPosition().y));

		//Ammo Text
		TransformComponent* transformComp = static_cast<TransformComponent*>(hudElement.HUDAmmoText.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x + hudComp->getAmmoTextOffset().x, hudComp->getHUDPosition().y + hudComp->getAmmoTextOffset().y);
		TextComponent* textComp = static_cast<TextComponent*>(hudElement.HUDAmmoText.getComponent(ComponentType::Text));
		//Get the Ammo Componentn Here and do the changes needed to set the correct size of the bar.
		//This is a stand in until Emmett is done that.
		textComp->setText(std::string("100 / 100"));


		//Ammo Bar
		transformComp = static_cast<TransformComponent*>(hudElement.HUDAmmoBar.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x + hudComp->getAmmoOffset().x, hudComp->getHUDPosition().y + hudComp->getAmmoOffset().y);

		//Health Text
		transformComp = static_cast<TransformComponent*>(hudElement.HUDHealthText.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x + hudComp->getHealthTextOffset().x, hudComp->getHUDPosition().y + hudComp->getHealthTextOffset().y);
		textComp = static_cast<TextComponent*>(hudElement.HUDHealthText.getComponent(ComponentType::Text));
		HealthComponent* hpComp = static_cast<HealthComponent*>(m_players[hudComp->getIndex()].getComponent(ComponentType::Health));
		if (hpComp->isAlive())
		{
			textComp->setText(std::to_string(hpComp->getHealth()) + " / " + std::to_string(hpComp->getMaxHealth()));
		}
		else
		{
			textComp->setText(std::to_string(0) + " / " + std::to_string(hpComp->getMaxHealth()));
			//If the player is dead and hasn't swapped states.
			if (hudComp->getAvatarState() == AvatarState::Player)
			{
				//Removing and readding Component because the Load from File function from the Visual Component isn't Working.    
				VisualComponent* visiComp = static_cast<VisualComponent*>(hudElement.HUDAvatarIcon.getComponent(ComponentType::Visual));
				visiComp->loadFromFile("Skull.png", m_renderer);
				hudComp->setAvatarState(AvatarState::Skull);
			}
		}

		//Health Bar
		transformComp = static_cast<TransformComponent*>(hudElement.HUDHealthBar.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x + hudComp->getHealthOffset().x, hudComp->getHUDPosition().y + hudComp->getHealthOffset().y);
		//Convert the Max Health from a Const Int to a float.
		float tempMaxHealth = hpComp->getMaxHealth();
		//A percentage number between 1 and 0
		float healthPercentage = (hpComp->getHealth()/ tempMaxHealth); 
		//Scale it to the Value
		float healthBarSizeX = hudComp->getMaxHealthSize().x * healthPercentage;
		//Sets the Bar to the desired Size
		hudComp->setCurrentHealthSize(healthBarSizeX);
		PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(hudElement.HUDHealthBar.getComponent(ComponentType::Primitive));
		//Applies the size change.
		primComp->setSize(hudComp->getCurrentHealthSize());

		//Hud Texture
		transformComp = static_cast<TransformComponent*>(hudElement.HUDVisualTexture.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x, hudComp->getHUDPosition().y);

		//Avatar Texture
		transformComp = static_cast<TransformComponent*>(hudElement.HUDAvatarIcon.getComponent(ComponentType::Transform));
		transformComp->setPos(hudComp->getHUDPosition().x + hudComp->getAvatarOffset().x, hudComp->getHUDPosition().y + hudComp->getAvatarOffset().y);
	}
}
/// <summary>
/// Renders the HUD in order so they overlap in the desire manner.
/// </summary>
/// <param name="t_renderer"></param>
/// <param name="t_system"></param>
void HUDManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& i : m_playerHUD) {
		t_system->render(t_renderer, i.HUDAvatarIcon);
		t_system->render(t_renderer, i.HUDHealthBar);
		t_system->render(t_renderer, i.HUDAmmoBar);
		t_system->render(t_renderer, i.HUDVisualTexture);
		t_system->render(t_renderer, i.HUDHealthText);
		t_system->render(t_renderer, i.HUDAmmoText);
	}
}

/// <summary>
/// Creates and Sets up the HUDBlock structs. Each player has 1.
/// </summary>
/// <param name="t_hudBlock"></param>
/// <param name="t_playerIndex"></param>
void HUDManager::setUpHUD(HUDBlock& t_hudBlock, int t_playerIndex)
{
	t_hudBlock.HUDLayoutData.addComponent(new HUDComponent(t_playerIndex));
	HUDComponent* hudComp = static_cast<HUDComponent*>(t_hudBlock.HUDLayoutData.getComponent(ComponentType::HUD));

	t_hudBlock.HUDAmmoBar.addComponent(new TransformComponent(true));
	t_hudBlock.HUDAmmoBar.addComponent(new ColourComponent(static_cast<Uint8>(0), static_cast<Uint8>(125), static_cast<Uint8>(50), 255));
	t_hudBlock.HUDAmmoBar.addComponent(new PrimitiveComponent());
	PrimitiveComponent* primComp = static_cast<PrimitiveComponent*>(t_hudBlock.HUDAmmoBar.getComponent(ComponentType::Primitive));
	primComp->setSize(hudComp->getMaxAmmoSize());
	primComp->setStaticPosition(true);

	t_hudBlock.HUDHealthBar.addComponent(new TransformComponent(true));
	t_hudBlock.HUDHealthBar.addComponent(new ColourComponent(static_cast<Uint8>(255), static_cast<Uint8>(0), static_cast<Uint8>(0), 255));
	t_hudBlock.HUDHealthBar.addComponent(new PrimitiveComponent());
	primComp = static_cast<PrimitiveComponent*>(t_hudBlock.HUDHealthBar.getComponent(ComponentType::Primitive));
	primComp->setSize(hudComp->getMaxHealthSize());
	primComp->setStaticPosition(true);

	t_hudBlock.HUDVisualTexture.addComponent(new TransformComponent(true));
	t_hudBlock.HUDVisualTexture.addComponent(new VisualComponent("HUD.png", m_renderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255),true));

	t_hudBlock.HUDHealthText.addComponent(new TransformComponent(true));
	t_hudBlock.HUDHealthText.addComponent(new TextComponent(std::string("ariblk.ttf"), m_renderer, true, std::string("HI")));
	TextComponent* textComp = static_cast<TextComponent*>(t_hudBlock.HUDHealthText.getComponent(ComponentType::Text));
	textComp->setSize(hudComp->getHealthTextSize());

	t_hudBlock.HUDAmmoText.addComponent(new TransformComponent(true));
	t_hudBlock.HUDAmmoText.addComponent(new TextComponent(std::string("ariblk.ttf"), m_renderer, true, std::string("HI")));
	textComp = static_cast<TextComponent*>(t_hudBlock.HUDAmmoText.getComponent(ComponentType::Text));
	textComp->setSize(hudComp->getAmmoTextSize());

	t_hudBlock.HUDAvatarIcon.addComponent(new TransformComponent(true));
	switch (t_playerIndex)
	{
	case 0:
		t_hudBlock.HUDAvatarIcon.addComponent(new VisualComponent("RedPlayerAvatar.png", m_renderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), true));
		break;
	case 1:
		t_hudBlock.HUDAvatarIcon.addComponent(new VisualComponent("BluePlayerAvatar.png", m_renderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), true));
		break;
	case 2:
		t_hudBlock.HUDAvatarIcon.addComponent(new VisualComponent("GreenPlayerAvatar.png", m_renderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), true));
		break;
	case 3:
		t_hudBlock.HUDAvatarIcon.addComponent(new VisualComponent("YellowPlayerAvatar.png", m_renderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), true));
		break;
	}
}

void HUDManager::swapToSkullAvatar(VisualComponent* t_visComp)
{

}
