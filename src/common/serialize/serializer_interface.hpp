#ifndef SERIALIZER_INTERFACE_HPP_
#define SERIALIZER_INTERFACE_HPP_

#include <vector>

class ISerializer
{
public:
	virtual std::vector<unsigned int> serialize(const void *obj) = 0;
	virtual void deserialize(const std::vector<unsigned int> &data, void *obj) = 0;
	virtual ~ISerializer() = default;
};

#endif // SERIALIZER_INTERFACE_HPP_
