
#ifndef _CACHETEXTURE_H_
#define _CACHETEXTURE_H_
#include "texture.h"
#include <vector>
#include "largevolumecache.h"
#include "oocprimitive.h"

namespace ysl
{
	namespace vm
	{
		struct BlockDescriptor
		{
		private:
			PhysicalMemoryBlockIndex value;
			VirtualMemoryBlockIndex key;
		public:
			BlockDescriptor() = default;
			BlockDescriptor(const PhysicalMemoryBlockIndex & value,VirtualMemoryBlockIndex key) :value(value),key(key) {}
			const PhysicalMemoryBlockIndex & Value()const { return value; }
			const VirtualMemoryBlockIndex & Key()const { return key; }
		};

		enum EntryFlag { Empty = 0, Unmapped = 2, Mapped = 1 };

		class MappingTableManager
		{

		public:
			struct PageDirEntry
			{
				int x, y, z, w;
			};
			struct PageTableEntry
			{
				int x, y, z, w;
			};
		private:
			//Linear3DArray<PageDirEntry> PageDir;
			Linear3DArray<PageTableEntry> pageTable;
			std::list<std::pair<PageTableEntryAbstractIndex, PhysicalMemoryBlockIndex>> g_lruList;
			//const Size3 physicalMemoryBlock;
			//const Size3 virtualMemoryBlock;
			//const Size3 blockSize;
			//CPUVolumeDataCache * const cacheData;
			//const std::shared_ptr<CPUVolumeDataCache> cacheData;
			void InitCPUPageTable(const Size3& blockDim);
			void InitLRUList(const Size3& physicalMemoryBlock, const Size3& page3DSize);
			void InitLRUList(const Size3& physicalMemoryBlock);
		public:
			using size_type = std::size_t;
			//PageTableManager(const Size3 & physicalMemoryBlock, std::shared_ptr<CPUVolumeDataCache> virtualData): physicalMemoryBlock(physicalMemoryBlock),
			//virtualMemoryBlock(virtualData->BlockDim()),
			//blockSize(virtualData->BlockSize()),
			//cacheData(virtualData)
			//{

			//	InitCPUPageTable(virtualMemoryBlock);
			//	InitLRUList(physicalMemoryBlock, blockSize);
			//	InitGPUPageTable();
			//}

			//MappingTableManager(std::shared_ptr<CPUVolumeDataCache> virtualData)
			//	//cacheData(std::move(virtualData)),
			//	//gpuCacheData(std::move(physicalData))
			//{

			//	//InitCPUPageTable(TODO:);    // block size
			//	//InitLRUList(TODO:,TODO:);	// blocks zie ,page table size}
			//}

			/**
			 * \brief 
			 * \param virtualSpaceSize virtual space size
			 */
			MappingTableManager(const Size3 & virtualSpaceSize,const Size3 & physicalSpaceSize)
			{
				InitCPUPageTable(virtualSpaceSize);
				InitLRUList(physicalSpaceSize);
			}


			const void * GetData()const { return pageTable.Data(); }

			/**
			 * \brief Translates the virtual space address to the physical address and update the mapping table by LRU policy
			 */
			std::vector<PhysicalMemoryBlockIndex> UpdatePageTable(const std::vector<VirtualMemoryBlockIndex>& missedBlockIndices);
		};

		class VISUALMAN_EXPORT_IMPORT OutOfCoreVolumeTexture :public IOutOfCoreAdapter			// Dest
		{
		public:
			explicit OutOfCoreVolumeTexture(const std::string & fileName);

			void OnDrawCallStart(OutOfCorePrimitive* p) override;
			void OnDrawCallFinished(OutOfCorePrimitive* p) override;

			Ref<Texture> GetVolumeTexture() { return volumeData; }
			Ref<const Texture> GetVolumeTexture()const { return volumeData; }

			Ref<BufferObject> GetAtomicCounterBuffer() { return atomicCounterBuffer; }
			Ref<const BufferObject> GetAtomicCounterBuffer()const { return atomicCounterBuffer; }

			Ref<BufferObject> GetBlockIDBuffer() { return blockIdBuffer; }
			Ref<const BufferObject> GetBlockIDBuffer()const { return blockIdBuffer; }

			Ref<BufferObject> GetHashBuffer() { return duplicateRemoveHash; }
			Ref<const BufferObject> GetHashBuffer()const { return duplicateRemoveHash; }

			Ref<Texture> GetMappingTableTexture() { return mappingTable; }
			Ref<const Texture> GetMappingTableTexture()const { return mappingTable; }

			Vec3i DataResolution()const { return Vec3i(cpuVolumeData->OriginalDataSize()); }
			Vec3i DataResolutionWithPadding()const { return Vec3i(cpuVolumeData->BlockDim()*cpuVolumeData->BlockSize()); }
			Vec3i Padding()const {return Vec3i( cpuVolumeData->Padding(),cpuVolumeData->Padding(),cpuVolumeData->Padding()); }
			Vec3i VirtualBlockDim()const { return Vec3i(cpuVolumeData->BlockDim()); }
			Vec3i BlockSize()const { return Vec3i(cpuVolumeData->BlockSize());}

			void BindToOutOfCorePrimitive(Ref<OutOfCorePrimitive> oocPrimitive);
			~OutOfCoreVolumeTexture();
		private:
			static constexpr int pboCount = 3;
			static constexpr GLbitfield mapping_flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
			static constexpr GLbitfield	storage_flags = GL_DYNAMIC_STORAGE_BIT | mapping_flags;

			void SetSubTextureDataUsePBO(const std::vector<BlockDescriptor> & data);

			Size3 EvalTextureSize(const Size3 & hint)const;
			size_t EvalIDBufferSize(const Size3 & hint)const;

			void CreatePBOs(int bytes);

			double time = 0;
			size_t totalBlocks = 0;

			std::array<void*, pboCount> pboPtrs;
			std::array<void*, pboCount> offset;
			Ref<BufferObject> pbos;
			std::array<GLsync, pboCount> fences;


			size_t bytes = 0;
			Ref<Texture> volumeData;
			Ref<CPUVolumeDataCache> cpuVolumeData;
			Ref<BufferObject> atomicCounterBuffer;
			Ref<BufferObject> duplicateRemoveHash;
			Ref<BufferObject> blockIdBuffer;
			std::vector<int> blockIdLocalBuffer;
			Ref<Texture> mappingTable;
			Ref<MappingTableManager> mappingTableManager;
			// CPU Volume Cache
		};
	}
}

#endif