
#include "buffer.h"
#include <stdexcept>

namespace ysl
{
	//LocalBuffer::LocalBuffer(size_type x, const uint8_t* data): LocalBuffer(x, nullptr, true)
	//{
	//	this->data = (uint8_t*)AllocAligned<uint8_t>(x);
	//	if (this->data == nullptr)
	//	{
	//		throw std::runtime_error("Bad Alloc");
	//	}
	//	if (this->data)
	//		memcpy(this->data, data, x * sizeof(uint8_t));
	//}
	//bool LocalBuffer::Resize(std::size_t x)
	//{
	//	if (own)
	//	{
	//		if (x == bytes)
	//			return false;

	//		auto* ptr = (uint8_t*)AllocAligned<uint8_t>(x);
	//		if (this->data != nullptr)
	//		{
	//			if (x > bytes)
	//			{
	//				memcpy(ptr, data, bytes);
	//			}
	//			else if (x < bytes)
	//			{
	//				memcpy(ptr, data, x);
	//			}
	//		}

	//		Clear();
	//		bytes = x;
	//		data = ptr;
	//		return true;
	//	}
	//	return false;
	//}

	//void LocalBuffer::Clear()
	//{
	//	if (own)
	//		FreeAligned(data);
	//	data = nullptr;
	//	bytes = 0;
	//	own = true;
	//}
}
