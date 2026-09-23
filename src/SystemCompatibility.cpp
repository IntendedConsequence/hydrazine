/*! \file SystemCompatibility.h
	\date Monday August 2, 2010
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for hacked code required to assist windows 
		compilaiton
*/

#ifndef SYSTEM_COMPATIBILITY_CPP_INCLUDED
#define SYSTEM_COMPATIBILITY_CPP_INCLUDED

// Hydrazine includes
#include <hydrazine/SystemCompatibility.h>

#if defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
    // Windows / MinGW
    #include <windows.h>
    // Optional: for demangling if you really need it
    // #include <dbghelp.h>   // UnDecorateSymbolName
#else
    // Linux / other Unix
    #include <GL/glx.h>
    #include <unistd.h>
    #include <sys/sysinfo.h>
    #include <cxxabi.h>
#endif

namespace hydrazine
{
	unsigned int getHardwareThreadCount()
	{
	#if defined(__APPLE__)
		// ... existing Apple code ...
	#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;
	#else
		return sysconf(_SC_NPROCESSORS_ONLN);
	#endif
	}

	std::string getExecutablePath(const std::string& executableName)
	{
		return executableName;
	}

	long long unsigned int getFreePhysicalMemory()
	{
	#if defined(__APPLE__)
		// ... existing Apple code ...
	#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx(&status);
		return status.ullAvailPhys;
	#else
		return get_avphys_pages() * getpagesize();
	#endif
	}

	bool isAnOpenGLContextAvailable()
	{
	#if defined(__APPLE__)
		return false; // TODO
	#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
		return wglGetCurrentContext() != nullptr;
	#else
		return glXGetCurrentContext() != nullptr;
	#endif
	}

	bool isMangledCXXString(const std::string& string)
	{
		return string.find("_Z") == 0;
	}

	std::string demangleCXXString(const std::string& string)
	{
	#if defined(__APPLE__)
		return string; // TODO
	#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
		// Simple stub – real demangling on Windows needs DbgHelp
		return string;
	#else
		int status = 0;
		char* demangled = abi::__cxa_demangle(string.c_str(), 0, 0, &status);
		std::string name = (status == 0 && demangled) ? demangled : string;
		free(demangled);
		return name;
	#endif
	}

}

#endif

