
#ifndef _LVDFILEREADER_H_
#define _LVDFILEREADER_H_

#include <VMFoundation/lvdreader.h>
#include <VMUtils/vmnew.hpp>
#include <VMUtils/ieverything.hpp>
#include <VMCoreExtension/plugin.h>
#include <VMCoreExtension/i3dblockfileplugininterface.h>

namespace vm
{
class LVDFileReader : public vm::EverythingBase<I3DBlockFilePluginInterface>
{
	std::unique_ptr<LVDReader> lvdReader;

public:
	LVDFileReader( ::vm::IRefCnt *cnt ) :
	  vm::EverythingBase<I3DBlockFilePluginInterface>( cnt )
	{
	}
	void Open( const std::string &fileName ) override
	{
		lvdReader = std::make_unique<LVDReader>( fileName );
		if ( lvdReader == nullptr ) {
			throw std::runtime_error( "failed to open lvd file" );
		}
	}
	const void *GetPage( size_t pageID ) override { return lvdReader->ReadBlock( pageID ); }
	size_t GetPageSize() const override { return lvdReader->BlockSize(); }
	size_t GetPhysicalPageCount() const override { return lvdReader->BlockCount(); }
	size_t GetVirtualPageCount() const override { return lvdReader->BlockCount(); }

	int GetPadding() const override { return lvdReader->GetBlockPadding(); }
	Size3 GetDataSizeWithoutPadding() const override { return lvdReader->OriginalDataSize(); }
	Size3 Get3DPageSize() const override
	{
		const std::size_t len = lvdReader->BlockSize();
		return Size3{ len, len, len };
	}
	int Get3DPageSizeInLog() const override { return lvdReader->BlockSizeInLog(); }
	Size3 Get3DPageCount() const override { return lvdReader->SizeByBlock(); }

private:
};


}  // namespace 


class LVDFileReaderFactory : public vm::IPluginFactory
{
public:
	DECLARE_PLUGIN_FACTORY( "visualman.blockdata.io" )
	std::vector<std::string> Keys() const override { return { ".lvd" }; }
	::vm::IEverything *Create( const std::string &key ) override
	{
		if ( key == ".lvd" ) {
			//return std::make_unique<LVDFileReader>();
			return VM_NEW<vm::LVDFileReader>();
		}
		return nullptr;
	}
};

VM_REGISTER_PLUGIN_FACTORY_DECL( LVDFileReaderFactory )
EXPORT_PLUGIN_FACTORY( LVDFileReaderFactory )

#endif