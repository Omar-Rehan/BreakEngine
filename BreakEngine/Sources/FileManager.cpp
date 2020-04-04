#include "pch.h"
#include "FileManager.h"


void FileManager::Initialize() {
	PHYSFS_init(NULL);
}
void FileManager::Terminate() {
	for (auto& e : m_pFiles) delete e;
	m_pFiles.clear();

	PHYSFS_deinit();
}

File* FileManager::CreateNewFile() {
	m_pFiles.emplace_front(new File());
	return m_pFiles.front();
}
void FileManager::DestroyFile(File* pFile) {
	std::list<File*>::iterator it = std::find(m_pFiles.begin(), m_pFiles.end(), pFile);
	if (it == m_pFiles.end()) {
		std::cout << "cannot find file in list" << std::endl;
		return;
	}
	delete (*it);
	m_pFiles.erase(it);
}

void FileManager::AddSearchDirectory(const char* dir) {
	if (!PHYSFS_addToSearchPath(dir, NULL)) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
	}
}
void FileManager::Mount(const char* dir) {
	if (!PHYSFS_mount(dir, NULL, 1)) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
	}
}

bool FileManager::Exists(const char* dir) {
	return PHYSFS_exists(dir);
}




bool File::OpenRead(const char* dir) {
	if (Check()) {
		std::cout << "A file is already open!" << std::endl;
		return false;
	}

	m_pFile = PHYSFS_openRead(dir);
	if (m_pFile == nullptr) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
		return false;
	}

	return true;
}
bool File::OpenWrite(const char* dir) {
	if (Check()) {
		std::cout << "A file is already open!" << std::endl;
		return false;
	}

	m_pFile = PHYSFS_openWrite(dir);
	if (m_pFile == nullptr) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
		return false;
	}

	return true;
}
bool File::OpenAppend(const char* dir) {
	if (Check()) {
		std::cout << "A file is already open!" << std::endl;
		return false;
	}

	m_pFile = PHYSFS_openAppend(dir);
	if (m_pFile == nullptr) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
		return false;
	}

	return true;
}
bool File::Close() {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return false;
	}

	PHYSFS_close(m_pFile);
	m_pFile = nullptr;
	return true;
}

int File::Flush() {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return NULL;
	}

	int success = PHYSFS_flush(m_pFile);
	if (success == NULL) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
	}
	return success;
}
bool File::Check() {
	return (m_pFile != nullptr);
}
PHYSFS_sint64 File::Tell() {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return -1;
	}

	int success = PHYSFS_tell(m_pFile);
	if (success == -1) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
	}
	
	return success;
}
int File::Seek(PHYSFS_uint64 pos) {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return NULL;
	}

	int success = PHYSFS_seek(m_pFile, pos);
	if (!success) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
	}
	return success;
}
PHYSFS_sint64 File::GetFileLength() {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return NULL;
	}

	return PHYSFS_fileLength(m_pFile);
}

void File::Read(void* buffer, size_t size, PHYSFS_uint64 count, PHYSFS_uint64* numOfReadBytes) {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return;
	}
	
	PHYSFS_uint64 fileLength = GetFileLength();
	PHYSFS_uint64 readBytes = PHYSFS_readBytes(m_pFile, buffer, fileLength);
	if (readBytes == -1) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
		return;
	}

	if (numOfReadBytes != nullptr) *numOfReadBytes = readBytes;
}
PHYSFS_sint64 File::Write(const void* buffer, PHYSFS_uint64 count) {
	if (!Check()) {
		std::cout << "There is no open file!" << std::endl;
		return NULL;
	}

	PHYSFS_sint64 numOfWrittenBytes = PHYSFS_writeBytes(m_pFile, buffer, count);
	if (numOfWrittenBytes == -1) {
		std::cout << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << std::endl;
		return NULL;
	}

	return numOfWrittenBytes;
}