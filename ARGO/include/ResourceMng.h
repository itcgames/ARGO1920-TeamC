//#pragma once
//
//#include <unordered_map>
//#include <string>
//#include <memory>
//
//
////Credit to CaioIcy for creating this ResourceManager
////https://github.com/CaioIcy
//
///**
//* @brief Resource management class template.
//*
//* Usage:
//* @code
//* auto some_music = ResourceHandler<Music>.Instance().Get("path/to/music.ogg");
//* @endcode
//*
//* @todo Figure out if there is a way to restrict at compile-time the classes that can be used
//* 	with this.
//*/
//template <typename T>
//class ResourceManager final {
//
//private:
//	using TypePtr = std::shared_ptr<T>; /**< std::shared_ptr alias for type T */
//	using TypeMap = std::unordered_map<std::string, TypePtr>; /**< std::unordered_map
//		alias for std::string as keys and TypePtr as values */
//	struct ConstructorTag {}; /**< Private tag needed to use the constructor. */
//
//public:
//	/**
//	* @brief Singleton implementation for ResourceManager.
//	*
//	* @return The instance for a ResourceManager<T>.
//	*/
//	static ResourceManager<T>& Instance();
//
//	/**
//	* @brief The constructor.
//	*
//	* Since the std::make_unique in ResourceManager<T>::Instance() can't access a private
//	* constructor, we make this constructor public but need a ConstructorTag parameter.
//	* ConstructorTag is an empty struct only accessible by this class, since it is private.
//	* This way the constructor can only actually be used in a private setting.
//	*/
//	explicit ResourceManager(const ConstructorTag& private_tag);
//
//	/**
//	* @brief The destructor.
//	*
//	* @note Warns about resources being deleted with references still pointing to them.
//	*/
//	~ResourceManager();
//
//	// Not copyable and not moveable.
//	ResourceManager(const ResourceManager&) = delete;
//	ResourceManager& operator=(const ResourceManager&) = delete;
//	ResourceManager(ResourceManager&&) = delete;
//	ResourceManager& operator=(ResourceManager&&) = delete;
//
//	/**
//	* @brief Retrieves the resource from the resources.
//	*
//	* @param path : Path to the desired resource.
//	*
//	* @return The pointer to the wanted resource. Creates it, if not already in resources.
//	*/
//	T* Get(const std::string& path);
//
//private:
//	/**
//	* @brief Constructs the desired resource.
//	*
//	* @param path : Path to the desired resource.
//	*/
//	TypePtr Load(const std::string& path);
//
//	/**
//	* @brief Registers the resource on the resources map.
//	*
//	* @param path : Path to the resource. Will be the key value.
//	* @param resource : The shared pointer for the resource.
//	*/
//	void RegisterResource(const std::string& path, TypePtr resource);
//
//	/**
//	* @brief Unregisters the resource on the resources map.
//	*
//	* @param path : The key (path to resource) on the map.
//	*/
//	void UnregisterResource(const std::string& path);
//
//	TypeMap m_resources; /**< The map that contains all the T resources. */
//};
//
//template <typename T>
//ResourceManager<T>::ResourceManager(const ConstructorTag& private_tag) :
//	m_resources{}
//{
//	static_cast<void>(private_tag);
//}
//
//template <typename T>
//ResourceManager<T>& ResourceManager<T>::Instance() {
//	// "C++11 mandates that the initializer for a local static variable is only run once, even
//	// in the presence of concurrency. So, assuming you’ve got a modern C++ compiler, this code
//	// is thread-safe[...]"
//	static auto instance = std::make_unique<ResourceManager<T>>(ConstructorTag{});
//	return *instance.get();
//}
//
//template <typename T>
//ResourceManager<T>::~ResourceManager() {
//	for (const auto& resource : m_resources) {
//		if (resource.second.use_count() != 1) {
//			log_warn() << "Resource deleted with use count different than 1 (" <<
//				resource.first << ").";
//		}
//	}
//}
//
//template <typename T>
//T* ResourceManager<T>::Get(const std::string& path) {
//	T* t_ptr = nullptr;
//
//	const auto it = m_resources.find(path);
//
//	// Resource is already registered.
//	if (it != m_resources.end()) {
//		t_ptr = it->second.get();
//	}
//	// Resource was not loaded yet.
//	else {
//		t_ptr = Load(path).get();
//	}
//
//	return t_ptr;
//}
//
//template <typename T>
//std::shared_ptr<T> ResourceManager<T>::Load(const std::string& path) {
//	const auto resource = std::make_shared<T>(path);
//	RegisterResource(path, resource);
//	return resource;
//}
//
//template <typename T>
//void ResourceManager<T>::RegisterResource(const std::string& path, TypePtr resource) {
//	m_resources.emplace(path, resource);
//}
//
//template <typename T>
//void ResourceManager<T>::UnregisterResource(const std::string& path) {
//	const auto it = m_resources.find(path);
//	if (it != m_resources.end()) {
//		m_resources.erase(it);
//	}
//}
