#include "HealthComponent.h"

HealthComponent::HealthComponent(int t_hp) :
	Component(ComponentType::Health),
	m_health(t_hp)
{

}

HealthComponent::~HealthComponent()
{
}
