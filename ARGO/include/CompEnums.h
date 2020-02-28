#pragma once
enum class ComponentType
{
	Health = 0,
	Transform = 1,
	Ai = 2,
	Input = 3,
	Visual = 4,
	Colour = 5,
	Force = 6,
	ColliderAABB = 7,
	ColliderCircle = 8,
	Timer = 9,
	Primitive = 10,
	ParticleEmitter = 11,
	Tag = 12,
	Text = 13,
	Command = 14,
	Tile = 15,
	FireRate = 16,
	PickUp = 17,
	FlowField = 18,
	HUD = 19,
	LightField = 20,
	Weapon = 21,
	Pathing = 22,
	FSM = 23,
	Count
};

//when adding new component type, add it to enum class and then to this
namespace COMPONENT_ID
{
	const int HEALTH_ID = static_cast<int>(ComponentType::Health);
	const int TRANSFORM_ID = static_cast<int>(ComponentType::Transform);
	const int AI_ID = static_cast<int>(ComponentType::Ai);
	const int INPUT_ID = static_cast<int>(ComponentType::Input);
	const int VISUAL_ID = static_cast<int>(ComponentType::Visual);
	const int COLOUR_ID = static_cast<int>(ComponentType::Colour);
	const int FORCE_ID = static_cast<int>(ComponentType::Force);
	const int PRIMITIVE_ID = static_cast<int>(ComponentType::Primitive);
	const int PARTICLE_ID = static_cast<int>(ComponentType::ParticleEmitter);
	const int TEXT_ID = static_cast<int>(ComponentType::Text);
	const int COLLIDER_AABB_ID = static_cast<int>(ComponentType::ColliderAABB);
	const int COLLIDER_CIRCLE_ID = static_cast<int>(ComponentType::ColliderCircle);
	const int TIMER_ID = static_cast<int>(ComponentType::Timer);
	const int COMMAND_ID = static_cast<int>(ComponentType::Command);
	const int TAG_ID = static_cast<int>(ComponentType::Tag);
	const int TILE_ID = static_cast<int>(ComponentType::Tile);
	const int FIRE_RATE_ID = static_cast<int>(ComponentType::FireRate);
	const int FLOW_FIELD_ID = static_cast<int>(ComponentType::FlowField);
	const int PICK_UP_ID = static_cast<int>(ComponentType::PickUp);
	const int HUD_ID = static_cast<int>(ComponentType::HUD);
	const int LIGHT_FIELD_ID = static_cast<int>(ComponentType::LightField);
	const int WEAPON_ID = static_cast<int>(ComponentType::Weapon);
	const int PATHING_ID = static_cast<int>(ComponentType::Pathing);
	const int FSM_ID = static_cast<int>(ComponentType::FSM);
}