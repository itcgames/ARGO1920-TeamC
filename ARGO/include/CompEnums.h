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
	ColliderCircle = 8
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
	const int COLLIDER_AABB_ID = static_cast<int>(ComponentType::ColliderAABB);
	const int COLLIDER_CIRCLE_ID = static_cast<int>(ComponentType::ColliderCircle);
}