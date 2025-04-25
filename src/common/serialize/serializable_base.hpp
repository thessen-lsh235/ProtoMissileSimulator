#ifndef SERIALIZABLE_BASE_HPP_
#define SERIALIZABLE_BASE_HPP_

#include <vector>
#include <stdint.h>

template <typename Derived>
class Serializable
{
public:
	std::vector<uint8_t> toBytes() const
	{
		return static_cast<const Derived *>(this)->serializeImpl();
	}

	void fromBytes(const std::vector<uint8_t> &data)
	{
		static_cast<Derived *>(this)->deserializeImpl(data);
	}

	virtual ~Serializable() = default;
};

#endif // SERIALIZABLE_BASE_HPP_
