#ifndef SERIALIZER_REGISTRY_HPP_
#define SERIALIZER_REGISTRY_HPP_

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <memory>

class SerializerRegistry
{
public:
	using CreatorFunc = std::function<std::shared_ptr<void>(const std::vector<uint8_t> &)>;

	void registerType(const std::string &name, CreatorFunc func)
	{
		registry_[name] = func;
	}

	std::shared_ptr<void> create(const std::string &name, const std::vector<uint8_t> &data) const
	{
		auto it = registry_.find(name);
		if (it != registry_.end())
		{
			return it->second(data);
		}
		return nullptr;
	}

private:
	std::unordered_map<std::string, CreatorFunc> registry_;
};

#endif // SERIALIZER_REGISTRY_HPP_
