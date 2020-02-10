#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

enum class Textures
{
	Tile,
	Player,
	Enemy1,
	Enemy2,
	Projectile,
	Test
};

enum class Audio
{
	GunShot,
	Explosion,
	Test
};


template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier t_id, const std::string& t_filename);

	template <typename Parameter>
	void load(Identifier t_id, const std::string& t_filename, const Parameter& t_secondParam);

	Resource& get(Identifier t_id);
	const Resource& get(Identifier id) const;

private:
	void insertResource(Identifier t_id, std::unique_ptr<Resource> t_resource);

	std::map<Identifier, std::unique_ptr<Resource>>	m_resourceMap;
};