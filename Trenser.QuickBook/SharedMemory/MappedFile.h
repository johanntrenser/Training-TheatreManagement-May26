#pragma once
#include <Windows.h>
#include <SharedStructs.h>
#include <string>
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
	int m_mappedVersion;
public:
	/*
	 * Function: MappedFile::MappedFile
	 * Description: Default constructor that initializes all handles,
	 *              pointers and metadata to their invalid or empty states.
	 * Parameters: None
	 * Returns: None
	 */
	MappedFile():
		m_fileHandle(INVALID_HANDLE_VALUE),
		m_mappingHandle(NULL),
		m_filePointer(nullptr),
		m_header(nullptr),
		m_records(nullptr),
		m_filePath(""),
		m_mappingName(""),
		m_mappedVersion(-1)
	{}

	/*
	 * Function: MappedFile::MappedFile
	 * Description: Constructs a mapped file object with the specified
	 *              file path and mapping name while initializing all
	 *              handles and pointers to their invalid states.
	 * Parameters:
	 *    filePath    - Path of the backing file to be mapped
	 *    mappingName - Name of the file mapping object
	 * Returns: None
	 */
	MappedFile(const std::string& filePath, const std::string& mappingName) :
		m_fileHandle(INVALID_HANDLE_VALUE),
		m_mappingHandle(NULL),
		m_filePointer(nullptr),
		m_header(nullptr),
		m_records(nullptr),
		m_filePath(filePath),
		m_mappingName(mappingName),
		m_mappedVersion(-1)
	{}
	
	MappedFile(const MappedFile&) = delete;
	MappedFile& operator=(const MappedFile&) = delete;

	/*
	 * Function: MappedFile::open
	 * Description: Opens the backing file, creates a file mapping,
	 *              maps it into memory and initializes the file header
	 *              if the file is being created for the first time.
	 * Parameters: None
	 * Returns:
	 *    true if the mapped file was opened successfully,
	 *    false otherwise
	 */
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
			if (!SetEndOfFile(m_fileHandle))
			{
				CloseHandle(m_fileHandle);
				m_fileHandle = INVALID_HANDLE_VALUE;
				return false;
			}
			SetFilePointer(m_fileHandle, 0, NULL, FILE_BEGIN);
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
			m_fileHandle = INVALID_HANDLE_VALUE;
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
			m_mappingHandle = NULL;
			m_fileHandle = INVALID_HANDLE_VALUE;
			return false;
		}
		m_header = reinterpret_cast<FileHeader*>(m_filePointer);
		m_records = reinterpret_cast<T*>(reinterpret_cast<char*>(m_filePointer) + sizeof(FileHeader));
		if (m_header->maximumRecords == 0)
		{
			m_header->recordCount = 0;
			m_header->maximumRecords = config::Limit::INITIAL_FILE_MAP_SIZE;
			m_header->recordSize = sizeof(T);
			m_header->version = 0;
		}
		m_mappedVersion = m_header->version;
		return true;
	}
	
	/*
	 * Function: MappedFile::addRecord
	 * Description: Adds a new record to the mapped file. If the current
	 *              capacity is exhausted, the mapped file is automatically
	 *              grown before inserting the record.
	 * Parameters:
	 *    record - The record to be added to the mapped file
	 * Returns:
	 *    true if the record was added successfully,
	 *    false otherwise
	 */
	bool addRecord(const T& record)
	{
		checkAndRemap();
		if (m_header->recordCount >= m_header->maximumRecords)
		{
			if (!grow())
			{
				return false;
			}
		}
		m_records[m_header->recordCount] = record;
		m_header->recordCount++;
		FlushViewOfFile(m_filePointer, 0);
		return true;
	}

	/*
	 * Function: MappedFile::getAllRecords
	 * Description: Retrieves all records stored in the mapped file.
	 * Parameters:
	 *    recordCount - Reference used to return the total number of records
	 * Returns:
	 *    Pointer to the first record in the mapped memory region
	 */
	T* getAllRecords(int& recordCount)
	{
		checkAndRemap();
		recordCount = m_header->recordCount;
		return m_records;
	}

	/*
	 * Function: MappedFile::findById
	 * Description: Searches the mapped file for a record with the specified ID.
	 * Parameters:
	 *    id - Identifier of the record to locate
	 * Returns:
	 *    Pointer to the matching record if found,
	 *    nullptr otherwise
	 */
	T* findById(const char* id)
	{
		checkAndRemap();
		for (int index = 0; index < m_header->recordCount; index++)
		{
			if (strcmp(reinterpret_cast<const char*>(&m_records[index]), id) == 0)
			{
				return &m_records[index];
			}
		}
		return nullptr;
	}

	/*
	 * Function: MappedFile::flush
	 * Description: Flushes all modifications in the mapped memory region
	 *              to the underlying file.
	 * Parameters: None
	 * Returns: None
	 */
	void flush()
	{
		if (m_filePointer)
		{
			FlushViewOfFile(m_filePointer, 0);
		}
	}

	/*
	 * Function: MappedFile::close
	 * Description: Flushes the mapped view, unmaps it from memory and
	 *              releases all associated file and mapping handles.
	 * Parameters: None
	 * Returns: None
	 */
	void close()
	{
		if (m_filePointer)
		{
			FlushViewOfFile(m_filePointer, 0);
			UnmapViewOfFile(m_filePointer);
			clearPointers();
		}
		if (m_mappingHandle != NULL)
		{
			CloseHandle(m_mappingHandle);
			m_mappingHandle = NULL;
		}
		if (m_fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_fileHandle);
			m_fileHandle = INVALID_HANDLE_VALUE;
		}
	}

	/*
	 * Function: getRecordCount
	 * Description: Returns the record count stored in the mapped file header.
	 * Parameters:
	 *    None
	 * Returns:
	 *    Integer record count
	 */
	int getRecordCount() const
	{
		return m_header->recordCount;
	}

	/*
	 * Function: MappedFile::~MappedFile
	 * Description: Destructor that releases all resources associated
	 *              with the mapped file by closing the mapping and file
	 *              handles and unmapping any active views.
	 * Parameters: None
	 * Returns: None
	 */
	~MappedFile()
	{
		close();
	}

private:
	/*
	* Function: MappedFile::createMapping
	* Description: Creates a file mapping object of the specified size
	*              and maps it into the process address space.
	* Parameters:
	*    fileSize - Size of the mapping to create in bytes
	* Returns:
	*    true if the mapping was created successfully,
	*    false otherwise
	*/
	bool createMapping(DWORD fileSize)
	{
		m_mappingHandle = CreateFileMappingA(
			m_fileHandle,
			NULL,
			PAGE_READWRITE,
			0,
			fileSize,
			m_mappingName.c_str());
		if (m_mappingHandle == NULL)
		{
			return false;
		}
		m_filePointer = MapViewOfFile(
			m_mappingHandle,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0
		);
		if (m_filePointer == nullptr)
		{
			CloseHandle(m_mappingHandle);
			m_mappingHandle = NULL;
			return false;
		}
		m_header = reinterpret_cast<FileHeader*>(m_filePointer);
		m_records = reinterpret_cast<T*>(reinterpret_cast<char*>(m_filePointer) + sizeof(FileHeader));
		return true;
	}

	/*
	* Function: MappedFile::checkAndRemap
	* Description: Checks whether the mapped file has been resized by
	*              another process and remaps it if the version number
	*              has changed.
	* Parameters: None
	* Returns: None
	*/
	void checkAndRemap()
	{
		if (m_header == nullptr)
		{
			return;
		}
		if (m_header->version != m_mappedVersion)
		{
			remapToCurrentSize();
		}
	}

	/*
	 * Function: MappedFile::remapToCurrentSize
	 * Description: Unmaps the current view and remaps the file using
	 *              the latest size information stored in the file header.
	 * Parameters: None
	 * Returns: None
	 */
	void remapToCurrentSize()
	{
		int newMaximumRecords = m_header->maximumRecords;
		DWORD newSize = sizeof(FileHeader) + (newMaximumRecords * sizeof(T));
		UnmapViewOfFile(m_filePointer);
		clearPointers();
		CloseHandle(m_mappingHandle);
		m_mappingHandle = NULL;
		if (createMapping(newSize))
		{
			m_mappedVersion = m_header->version;
		}
	}

	/*
	 * Function: MappedFile::grow
	 * Description: Expands the capacity of the mapped file by increasing
	 *              the number of records that can be stored and remapping
	 *              the file to reflect the new size.
	 * Parameters: None
	 * Returns:
	 *    true if the mapped file was successfully expanded,
	 *    false otherwise
	 */
	bool grow()
	{
		int oldMaximumRecords = m_header->maximumRecords;
		int newMaximumRecords = oldMaximumRecords +	(oldMaximumRecords / 2);
		DWORD newSize = sizeof(FileHeader) + (newMaximumRecords * sizeof(T));
		UnmapViewOfFile(m_filePointer);
		clearPointers();
		CloseHandle(m_mappingHandle);
		m_mappingHandle = NULL;
		DWORD position = SetFilePointer(
				m_fileHandle,
				newSize,
				NULL,
				FILE_BEGIN);
		if (position ==	INVALID_SET_FILE_POINTER &&	GetLastError() != NO_ERROR)
		{
			return false;
		}
		if (!SetEndOfFile(m_fileHandle))
		{
			return false;
		}
		SetFilePointer(
			m_fileHandle,
			0,
			NULL,
			FILE_BEGIN);
		if (!createMapping(newSize))
		{
			return false;
		}
		m_header->maximumRecords = newMaximumRecords;
		m_header->version++;
		FlushViewOfFile(m_header, sizeof(FileHeader));
		m_mappedVersion = m_header->version;
		return true;
	}

	/*
	 * Function: MappedFile::clearPointers
	 * Description: Resets all internal pointers associated with the mapped
	 *              file to nullptr.
	 * Parameters: None
	 * Returns: None
	 */
	void clearPointers()
	{
		m_filePointer = nullptr;
		m_records = nullptr;
		m_header = nullptr;
	}
};

