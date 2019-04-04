
#ifndef _VIRTUALVOLUMEHIERACHY_H_
#define _VIRTUALVOLUMEHIERACHY_H_

#include "../memory/lineararray.h"
#include "../volume/largevolumecache.h"
#include <list>
#include "gpupagetable.h"

namespace ysl
{
	struct PageDirectoryEntryAbstractIndex
	{
		using internal_type = int;
		const internal_type x, y, z;
		PageDirectoryEntryAbstractIndex(internal_type x_ = -1,
			internal_type y_ = -1,
			internal_type z_ = -1)
			:x(x_), y(y_), z(z_) {}
	};

	struct PageTableEntryAbstractIndex
	{
		using internal_type = int;
		internal_type x, y, z;
		PageTableEntryAbstractIndex(internal_type x_ = -1,
			internal_type y_ = -1,
			internal_type z_ = -1) :
			x(x_), y(y_), z(z_) {}

	};

	struct PhysicalMemoryBlockIndex			// DataBlock start in 3d texture
	{
		using internal_type = int;
		const internal_type x, y, z;
		PhysicalMemoryBlockIndex(internal_type x_ = -1,
			internal_type y_ = -1,
			internal_type z_ = -1) :
			x(x_), y(y_), z(z_) {}
	};
	enum EntryFlag { Empty = 0, Unmapped = 2, Mapped = 1 };


	class PageTableManager
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
		const Size3 physicalMemoryBlock;
		const Size3 virtualMemoryBlock;
		const Size3 blockSize;
		CPUVolumeDataCache * const cacheData;
		std::shared_ptr<GPUPageTableDataCache> texPageTable;

		void InitGPUPageTable();
		void InitCPUPageTable(const Size3& blockDim);
		void InitLRUList(const Size3& physicalMemoryBlock, const Size3& page3DSize);
	public:
		using size_type = std::size_t;
		PageTableManager(const Size3& physicalMemoryBlock, CPUVolumeDataCache * virtualData): physicalMemoryBlock(physicalMemoryBlock),
		virtualMemoryBlock(virtualData->BlockDim()),
		blockSize({ (std::size_t)virtualData->BlockSize(), (std::size_t)virtualData->BlockSize(),  (std::size_t)virtualData->BlockSize() }),
		cacheData(virtualData)
		{

			InitCPUPageTable(virtualMemoryBlock);
			InitLRUList(physicalMemoryBlock, blockSize);
			InitGPUPageTable();
		}
		std::vector<PhysicalMemoryBlockIndex> UpdatePageTable(const std::vector<VirtualMemoryBlockIndex>& missedBlockIndices);
		Linear3DArray<PageTableEntry> & PageTable(int level);
		Size3 PageTableSize();
		Size3 BlockSize()const;
		void UpdatePageTable();
		CPUVolumeDataCache* VirtualData();
	};
}

#endif