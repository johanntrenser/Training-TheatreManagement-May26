#pragma once
#include <Windows.h>
#include <SharedStructs.h>
#include "ApplicationConfig.h"

template <typename T>
class MappedFile
{
private:
	HANDLE m_fileHandle;
	HANDLE m_mappingHandle;
	void* m_filePointer;
	FileHeader* m_header;
	T* m_records;
	std::string m_filePath;
	std::string m_mappingName;
public:
	MappedFile():
		m_fileHandle(INVALID_HANDLE_VALUE),
		m_mappingHandle(NULL),
		m_filePointer(nullptr),
		m_header(nullptr),
		m_records(nullptr),
		m_filePath(""),
		m_mappingName("")
	{}

	MappedFile(const std::string& filePath, const std::string& mappingName) :
		m_fileHandle(INVALID_HANDLE_VALUE),
		m_mappingHandle(NULL),
		m_filePointer(nullptr),
		m_header(nullptr),
		m_records(nullptr),
		m_filePath(filePath),
		m_mappingName(mappingName)
	{}
	
	bool open()
	{
		m_fileHandle = CreateFileA(
			m_filePath.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_fileHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		DWORD fileSize = GetFileSize(m_fileHandle, NULL);
		if (fileSize == 0)
		{
			fileSize = sizeof(FileHeader) + config::Limit::INITIAL_FILE_MAP_SIZE * sizeof(T);
			SetFilePointer(m_fileHandle, fileSize, NULL, FILE_BEGIN);
			SetEndOfFile(m_fileHandle);
		}
		m_mappingHandle = CreateFileMappingA(
			m_fileHandle,
			NULL,
			PAGE_READWRITE,
			0,
			fileSize,
			m_mappingName.c_str());
		if (m_mappingHandle == NULL)
		{
			CloseHandle(m_fileHandle);
			return false;
		}
		m_filePointer = MapViewOfFile(
			m_mappingHandle,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			0);
		if (m_filePointer == nullptr)
		{
			CloseHandle(m_mappingHandle);
			CloseHandle(m_fileHandle);
			return false;
		}
		m_header = reinterpret_cast<FileHeader*>(m_filePointer);
		m_records = reinterpret_cast<T*>((char*)m_filePointer + sizeof(FileHeader));
		if (m_header->maxRecords == 0)
		{
			m_header->recordCount = 0;
			m_header->maxRecords = config::Limit::INITIAL_FILE_MAP_SIZE;
			m_header->recordSize = sizeof(T);
		}
		return true;
	}

	void close()
	{
		if (m_filePointer)
		{
			FlushViewOfFile(m_filePointer, 0);
			UnmapViewOfFile(m_filePointer);
			m_filePointer = nullptr;
			m_records = nullptr;
			m_header = nullptr;
		}
		if (m_mappingHandle)
		{
			CloseHandle(m_mappingHandle);
			m_mappingHandle = nullptr;
		}
		if (m_fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_fileHandle);
			m_fileHandle = INVALID_HANDLE_VALUE;
		}
	}

	~MappedFile()
	{
		close();
	}
};

