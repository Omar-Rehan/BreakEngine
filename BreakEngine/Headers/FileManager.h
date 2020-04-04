#pragma once

#include "physfs.h"

class File {
private:
	PHYSFS_File* m_pFile;

public:
	File() = default;
	~File() = default;

	bool OpenRead(const char* dir);
	bool OpenWrite(const char* dir);
	bool OpenAppend(const char* dir);
	bool Close();

	int Flush();
	bool Check();
	PHYSFS_sint64 Tell();
	int Seek(PHYSFS_uint64 pos);
	PHYSFS_sint64 GetFileLength();

	void Read(void* pBuffer, size_t size, PHYSFS_uint64 count, PHYSFS_uint64* numOfReadBytes);
	PHYSFS_sint64 Write(const void* buffer, PHYSFS_uint64 size);
};

class FileManager {
public:
	FileManager() = default;
	~FileManager() = default;

	void Initialize();
	void Terminate();
	
	File* CreateNewFile();
	void DestroyFile(File* pFile);

	void AddSearchDirectory(const char* dir);
	void Mount(const char* dir);

	bool Exists(const char* dir);


private:
	std::list<File*> m_pFiles;
};