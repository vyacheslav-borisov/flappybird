#ifndef PEGAS_RESOURCE_H_
#define PEGAS_RESOURCE_H_


namespace pegas
{
	class Resource
	{
	public:
		Resource(const std::string& path);
		virtual ~Resource();

		std::string getPath() const { return m_path; }
		virtual off_t getLength();
		virtual const void* bufferize();

		virtual status load() = 0;
		virtual void unload();
		virtual status read(void* buffer, size_t count) = 0;

	protected:
		std::string m_path;
		char* m_buffer;
	};

	class FileResource: public Resource
	{
	public:
		FileResource(const std::string& path);

		status load();
		void unload();
		status read(void* buffer, size_t count);

	private:
		std::ifstream m_inputStream;
	};


	class AssetResource: public Resource
	{
	public:
		AssetResource(android_app* application, const std::string& path);

		virtual off_t getLength();
		virtual const void* bufferize();

		virtual status load();
		virtual void unload();
		virtual status read(void* buffer, size_t count);

	private:
		AAssetManager* m_assetManager;
		AAsset* m_asset;
	};
}

#endif /* RESOURCE_H_ */
