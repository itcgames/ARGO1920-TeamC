#include "stdafx.h"
#include "ResourceManager.h"

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id, const std::string& t_filename)
{
	// Create and load t_resource
	std::unique_ptr<Resource> t_resource(new Resource());
	if (!t_resource->loadFromFile(t_filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + t_filename);

	// If loading successful, insert t_resource to map
	insertResource(t_id, std::move(t_resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id, const std::string& t_filename, const Parameter& t_secondParam)
{
	// Create and load t_resource
	std::unique_ptr<Resource> t_resource(new Resource());
	if (!t_resource->loadFromFile(t_filename, t_secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + t_filename);

	// If loading successful, insert t_resource to map
	insertResource(t_id, std::move(t_resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id)
{
	auto found = m_resourceMap.find(t_id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id) const
{
	auto found = m_resourceMap.find(t_id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier t_id, std::unique_ptr<Resource> t_resource)
{
	// Insert and check success
	auto inserted = m_resourceMap.insert(std::make_pair(t_id, std::move(t_resource)));
	assert(inserted.second);
}