#include "../common.h"

#include "resource.h"
#include "log.h"

#include <sys/stat.h>

namespace pegas
{
	/****************************************************************************
	 * 	Resource class implementation
	 ****************************************************************************/
	Resource::Resource(const std::string& path)
		:m_path(path), m_buffer(NULL)
	{
		LOGI("Resource constructor, %s", path.c_str());
	}

	Resource::~Resource()
	{
		LOGI("Resource destructor");

		unload();
	}

	void Resource::unload()
	{
		LOGI("Resource::unload: %s", m_path.c_str());

		if(m_buffer)
		{
			delete[] m_buffer;
			m_buffer = NULL;
		}
	}

	off_t Resource::getLength()
	{
		LOGI("Resource::getLength");

		struct stat fileStatus;

		if(stat(m_path.c_str(), &fileStatus) >= 0)
		{
			return fileStatus.st_size;
		}

		return -1;
	}

	const void* Resource::bufferize()
	{
		LOGI("Resource::bufferize");

		if(m_buffer != NULL)
		{
			return m_buffer;
		}

		off_t size = getLength();
		if(size < 0)
		{
			LOGW("size < 0");
			return NULL;
		}


		m_buffer = new char[size];
		if(read(m_buffer, size) == STATUS_OK)
		{
			return m_buffer;
		}

		LOGW("buffer reading failed");

		return NULL;
	}

	/************************************************************************************
	 *  FileResource class implementation
	 **********************************************************************************/
	FileResource::FileResource(const std::string& path)
		:Resource(path)
	{
		LOGI("FileResource constructor: %s", path.c_str());
	}

	status FileResource::load()
	{
		LOGI("FileResource::load: %s", m_path.c_str());

		m_inputStream.open(m_path.c_str(), std::ios::in|std::ios::binary);

		return m_inputStream.is_open() ? STATUS_OK : STATUS_KO;
	}

	void FileResource::unload()
	{
		LOGI("FileResource::unload: %s", m_path.c_str());

		if(m_inputStream.is_open())
		{
			m_inputStream.close();
		}

		Resource::unload();
	}

	status FileResource::read(void* buffer, size_t count)
	{
		LOGI("FileResource::read, buffer size: %d", count);

		m_inputStream.read((char*)buffer, count);

		return (!m_inputStream.fail()) ? STATUS_OK : STATUS_KO;
	}



	/************************************************************************
	 * 	AssetResource class implementation
	 ***********************************************************************/
	AssetResource::AssetResource(android_app* application, const std::string& path)
		:Resource(path),
		 m_assetManager(application->activity->assetManager),
		 m_asset(NULL)
	{
		LOGI("AssetResource constructor, %s", path.c_str());
	}

	status AssetResource::load()
	{
		LOGI("AssetResource::load, %s", m_path.c_str());

		m_asset = AAssetManager_open(m_assetManager, m_path.c_str(), AASSET_MODE_UNKNOWN);

		return (m_asset != NULL) ? STATUS_OK : STATUS_KO;
	}

	void AssetResource::unload()
	{
		LOGI("AssetResource::unload, %s", m_path.c_str());

		if (m_asset != NULL)
		{
			AAsset_close(m_asset);
			m_asset = NULL;
		}

		Resource::unload();
	}

	status AssetResource::read(void* buffer, size_t count)
	{
		LOGI("AssetResource::read, buffer size: %d", count);

		int32_t readCount = AAsset_read(m_asset, buffer, count);

		return (readCount == count) ? STATUS_OK : STATUS_KO;
	}

	off_t AssetResource::getLength() {
		return AAsset_getLength(m_asset);
	}

	const void* AssetResource::bufferize() {
		return AAsset_getBuffer(m_asset);
	}
}





