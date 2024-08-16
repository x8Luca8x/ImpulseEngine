#include "Windows/WindowsFilesystem.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

#pragma push_macro("DeleteFile")
#undef DeleteFile

#pragma push_macro("MoveFile")
#undef MoveFile

#pragma push_macro("CopyFile")
#undef CopyFile

#pragma push_macro("CreateDirectory")
#undef CreateDirectory

#pragma push_macro("CreateFile")
#undef CreateFile

#include <ShlObj.h>

int32 SpecialDirectoryToCSIDL(ESpecialDirectory Directory)
{
	switch (Directory)
	{
	case ESpecialDirectory::Documents:
		return CSIDL_MYDOCUMENTS;
	case ESpecialDirectory::Programs:
		return CSIDL_PROGRAM_FILES;
	case ESpecialDirectory::ProgramsX86:
		return CSIDL_PROGRAM_FILESX86;
	case ESpecialDirectory::ProgramData:
		return CSIDL_COMMON_APPDATA;
	case ESpecialDirectory::Desktop:
		return CSIDL_DESKTOP;
	case ESpecialDirectory::Downloads:
		return CSIDL_PERSONAL;
	case ESpecialDirectory::Music:
		return CSIDL_MYMUSIC;
	case ESpecialDirectory::Pictures:
		return CSIDL_MYPICTURES;
	case ESpecialDirectory::Videos:
		return CSIDL_MYVIDEO;
	case ESpecialDirectory::User:
		return CSIDL_PROFILE;
	case ESpecialDirectory::AppData:
		return CSIDL_APPDATA;
	}

	return -1;
}

bool FWindowsFilesystem::IsFileValid(FFileHandle Handle)
{
	return Handle != NULL && Handle != INVALID_HANDLE_VALUE;
}

FFileHandle FWindowsFilesystem::OpenFile(const FString& Filename, EFileAccess Access)
{
	DWORD access = 0;
	DWORD shareMode = 0;
	DWORD creationDisposition = 0;
	DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	if ((Access & FILE_Read) == FILE_Read)
	{
		access = GENERIC_READ;
		shareMode = FILE_SHARE_READ;
		creationDisposition = OPEN_EXISTING;
	}

	if ((Access & FILE_Write) == FILE_Write)
	{
		access |= GENERIC_WRITE;
		shareMode |= FILE_SHARE_WRITE;
		creationDisposition = OPEN_EXISTING;
	}

	if ((Access & FILE_Append) == FILE_Append)
	{
		access |= FILE_APPEND_DATA;
		shareMode |= FILE_SHARE_READ;
		creationDisposition = OPEN_EXISTING;
	}

	if ((Access & FILE_Truncate) == FILE_Truncate)
	{
		access |= GENERIC_WRITE;
		shareMode |= FILE_SHARE_WRITE;
		creationDisposition = TRUNCATE_EXISTING;
	}

	return CreateFileW(*Filename, access, shareMode, NULL, creationDisposition, flagsAndAttributes, NULL);
}

void FWindowsFilesystem::CloseFile(FFileHandle Handle)
{
	CloseHandle(Handle);
}

bool FWindowsFilesystem::WriteData(FFileHandle Handle, const void* Data, uint64 Size)
{
	DWORD bytesWritten = 0;

	const bool bSuccess = WriteFile(Handle, Data, static_cast<DWORD>(Size), &bytesWritten, (LPOVERLAPPED)nullptr);

	return bSuccess && bytesWritten == Size;
}

bool FWindowsFilesystem::ReadData(FFileHandle Handle, void* Data, uint64 Size, const FGenericReadFileOptions& Options)
{
	OVERLAPPED overlapped{};
	overlapped.Offset = static_cast<DWORD>(Options.Offset);

	DWORD bytesRead = 0;
	const bool bSuccess = ReadFile(Handle, Data, static_cast<DWORD>(Size), &bytesRead, &overlapped);

	return bSuccess && bytesRead == Size;
}

FString FWindowsFilesystem::GetSpecialDirectory(ESpecialDirectory Directory)
{
	const int32 csidl = SpecialDirectoryToCSIDL(Directory);
	if (csidl == -1)
		return FString();

	TCHAR path[MAX_PATH];
	if (SHGetFolderPath(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, path) != S_OK)
		return FString();

	return path;
}

uint64 FWindowsFilesystem::GetFileSize(FFileHandle Handle)
{
	DWORD size = 0;
	return ::GetFileSize(Handle, &size);
}

bool FWindowsFilesystem::IsFile(const FString& Path)
{
	DWORD attributes = GetFileAttributesW(*Path);
	return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FWindowsFilesystem::IsDirectory(const FString& Path)
{
	DWORD attributes = GetFileAttributesW(*Path);
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FWindowsFilesystem::CreateFile(const FString& Filename)
{
	FFileHandle handle = CreateFileW(*Filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!IsFileValid(handle))
		return false;

	CloseHandle(handle);
	return true;
}

bool FWindowsFilesystem::CreateDirectory(const FString& Directory)
{
	return CreateDirectoryW(*Directory, NULL);
}

#pragma pop_macro("DeleteFile")
#pragma pop_macro("MoveFile")
#pragma pop_macro("CopyFile")
#pragma pop_macro("CreateDirectory")
#pragma pop_macro("CreateFile")

#endif