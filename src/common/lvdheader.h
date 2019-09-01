
#ifndef _LVDHEADER_H_
#define _LVDHEADER_H_
#include <cstdint>
#include <memory>
#include "common.h"

#define LVD_HEADER_BUF_ORIGIN_OFFSET  0

#define LVD_HEADER_MAGIC_FILED_SIZE  4

#define LVD_DATA_WIDTH_FIELD_SIZE  4

#define LVD_DATA_HEIGHT_FIELD_SIZE  4

#define LVD_DATA_DEPTH_FIELD_SIZE  4

#define LVD_DATA_BLOCK_LENGTH_IN_LOG_FILED_SIZE  4

#define LVD_DATA_PADDING_FIELD_SIZE  4

#define LVD_DATA_ORIGINAL_WIDTH_FIELD_SIZE 4

#define LVD_DATA_ORIGINAL_HEIGHT_FIELD_SIZE 4

#define LVD_DATA_ORIGINAL_DEPTH_FIELD_SIZE 4

#define LVD_HEADER_MAGIC_FILED_OFFSET  (LVD_HEADER_BUF_ORIGIN_OFFSET)

#define LVD_DATA_WIDTH_FIELD_OFFSET  ((LVD_HEADER_MAGIC_FILED_OFFSET) + (LVD_HEADER_MAGIC_FILED_SIZE ))

#define LVD_DATA_HEIGHT_FIELD_OFFSET  ((LVD_DATA_WIDTH_FIELD_OFFSET) + (LVD_DATA_WIDTH_FIELD_SIZE ))

#define LVD_DATA_DEPTH_FIELD_OFFSET  ((LVD_DATA_HEIGHT_FIELD_OFFSET) + (LVD_DATA_HEIGHT_FIELD_SIZE))

#define LVD_BLOCK_LOG_FILED_OFFSET  ((LVD_DATA_DEPTH_FIELD_OFFSET) + (LVD_DATA_DEPTH_FIELD_SIZE))

#define LVD_BLOCK_PADDING_FIELD_OFFSET (( LVD_BLOCK_LOG_FILED_OFFSET  )+ (LVD_DATA_BLOCK_LENGTH_IN_LOG_FILED_SIZE))



#define LVD_DATA_ORIGINAL_WIDTH_FIELD_OFFSET  ((LVD_BLOCK_PADDING_FIELD_OFFSET) + (LVD_DATA_PADDING_FIELD_SIZE ))

#define LVD_DATA_ORIGINAL_HEIGHT_FIELD_OFFSET (( LVD_DATA_ORIGINAL_WIDTH_FIELD_OFFSET) +(LVD_DATA_ORIGINAL_WIDTH_FIELD_SIZE ))

#define LVD_DATA_ORIGINAL_DEPTH_FIELD_OFFSET  ((LVD_DATA_ORIGINAL_HEIGHT_FIELD_OFFSET) + (LVD_DATA_ORIGINAL_HEIGHT_FIELD_SIZE))

#define LVD_HEADER_SIZE ((LVD_DATA_ORIGINAL_DEPTH_FIELD_OFFSET) +(LVD_DATA_ORIGINAL_DEPTH_FIELD_SIZE ))

namespace ysl
{

	class COMMON_EXPORT_IMPORT LVDHeader
	{
		std::unique_ptr<unsigned char[]> buf;
		static constexpr int BufSize = 64;
	public:
		uint32_t magicNum;
		uint32_t dataDim[3];
		uint32_t blockLengthInLog;
		uint32_t padding;
		uint32_t originalDataDim[3];
	public:
		LVDHeader();
		int HeaderSize()const;
		void Decode(unsigned char * buf);
		unsigned char * Encode();
	};
}


#endif