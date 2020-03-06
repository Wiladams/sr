#pragma once

/*
	mmap is the rough equivalent of the mmap() function on Linux
	This basically allows you to memory map a file, which means you 
	can access a pointer to the file's contents without having to 
	go through IO routines.

	Usage:
	local m = mmap(filename)

	local bs = binstream(m:getPointer(), #m)
*/

#include "w32.hpp"

static const int ERROR_ACCESS_DENIED = 5;
static const int ERROR_ALREADY_EXISTS = 183;
static const int ERROR_INVALID_HANDLE = 6;
static const int ERROR_INVALID_PARAMETER = 87;

static const int GENERIC_READ  = 0x80000000;
static const int GENERIC_WRITE = 0x40000000;

static const int OPEN_EXISTING = 3;
static const int OPEN_ALWAYS   = 4;

static const int FILE_ATTRIBUTE_ARCHIVE = 0x20;
static const int FILE_FLAG_RANDOM_ACCESS = 0x10000000;
static const int FILE_BEGIN            = 0;

//local FILE_MAP_EXECUTE	= 0;
static const int FILE_MAP_READ		= 0x04;
static const int FILE_MAP_WRITE	= 0x02;
//local FILE_MAP_TARGETS_INVALID = 0
static const int FILE_MAP_ALL_ACCESS = 0xf001f;

static const int PAGE_READONLY         = 0x02; 
static const int PAGE_READWRITE        = 0x4;



class mmap
{

    HANDLE filehandle;
    HANDLE maphandle;


public:
    size_t size;
    uint8_t * data;
    
    mmap(const char * filename, int newsize=0)
        : size(0),
        data(nullptr),
        filehandle(INVALID_HANDLE_VALUE),
        maphandle(INVALID_HANDLE_VALUE)
    {	
        // Open file
        //print("mmap:__new - ", filename)
        filehandle = CreateFileA(filename,
            GENERIC_READ, 
            0, 
            nullptr,
            OPEN_EXISTING, 
            (FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_RANDOM_ACCESS), 
            nullptr);
    
    //print("    File Handle: ", filehandle)
    
        if (filehandle == INVALID_HANDLE_VALUE) {
            // BUGBUG - throw exception
		    //error("Could not create/open file for mmap: "..tostring(ffi.C.GetLastError()))
            return;
        }
	
        // Set file size if new
        // print("GET File Size")

        bool exists = filehandle != INVALID_HANDLE_VALUE;
        if (exists) {
		    size_t fsize = GetFileSize(filehandle, nullptr);
            //printf("    Size: ", fsize)
		    if (fsize == 0) {
			    // Windows will error if mapping a 0-length file, fake a new one
			    exists = false;
                return;
            } else {
			    size = fsize;
            }
	    } else {
		    return;
	    }

	    // Open mapping
        maphandle = CreateFileMappingA(filehandle, nullptr, PAGE_READONLY, 0, size, nullptr);
        //printf("CREATE File Mapping: ", maphandle)
	    
        if (maphandle == nullptr) {
		    //error("Could not create file map: "..tostring(ffi.C.GetLastError()))
            return ;
        }
	
	    // Open view
	    data = (uint8_t *)MapViewOfFile(maphandle, FILE_MAP_READ, 0, 0, 0);
	    //print("MAP VIEW: ", m.map)
	    if (data == nullptr) {
            // throw exception
		    //error("Could not map: "..tostring(ffi.C.GetLastError()))
            return ;
        }
    }

    bool isValid() {return maphandle != INVALID_HANDLE_VALUE;}

    uint8_t * getPointer(){return data;}

    size_t length()
	{
        return size;
    }
    
    bool close()
	{
        if (data != nullptr) {
		    UnmapViewOfFile(data);
		    data = nullptr;
        }

	    if (maphandle != INVALID_HANDLE_VALUE) {
		    CloseHandle(maphandle);
		    maphandle = INVALID_HANDLE_VALUE;
        }

	    if (filehandle != INVALID_HANDLE_VALUE) {
		    CloseHandle(filehandle);
		    filehandle = INVALID_HANDLE_VALUE;
        }

        return true;
    }

    virtual ~mmap() {
        close();
    }

};
