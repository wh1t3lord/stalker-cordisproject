#pragma once

#ifndef DEBUG
#define MASTER_GOLD
#endif // DEBUG

#include "xrCore_benchmark_macros.h"

#if !defined(_CPPUNWIND)
#error Please enable exceptions...
#endif

#ifndef _MT
#error Please enable multi-threaded library...
#endif

#ifdef NDEBUG
#define XRAY_EXCEPTIONS 0
#define LUABIND_NO_EXCEPTIONS
#else
#define XRAY_EXCEPTIONS 1
#endif

#if !defined(DEBUG) && (defined(_DEBUG) || defined(MIXED))
#define DEBUG
#endif

// Warnings
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4251) // object needs DLL interface
#pragma warning(disable : 4345)
//#pragma warning (disable : 4530 ) // C++ exception handler used, but unwind semantics are not enabled

#ifdef XR_X64
#pragma warning(disable : 4512)
#endif

#pragma warning(disable : 4714) // __forceinline not inlined

#ifndef DEBUG
#pragma warning(disable : 4189) // local variable is initialized but not referenced
#endif // frequently in release code due to large amount of VERIFY

// Our headers
#include "xrDebug.h"
//#include "vector.h"

#ifdef DEBUG
#include <vld.h>
#endif

#include "clsid.h"
//#include "Threading/Lock.hpp"
#include "xrMemory.h"

//#include "_stl_extensions.h"
#include "_std_extensions.h"
#include "xrCommon/xr_vector.h"
#include "xrCommon/xr_set.h"
#include "xrsharedmem.h"
#include "xrstring.h"
#include "xr_resource.h"
#include "Compression/rt_compressor.h"
#include "xr_shared.h"
#include "string_concatenations.h"
#include "_flags.h"
#include "Cordis/Cordis_Functions.h"
#include <tuple>
// stl ext
struct XRCORE_API xr_rtoken
{
    shared_str name;
    int id;

    xr_rtoken(pcstr _nm, int _id)
    {
        name = _nm;
        id = _id;
    }

    void rename(pcstr _nm) { name = _nm; }
    bool equal(pcstr _nm) const { return (0 == xr_strcmp(*name, _nm)); }
};

#include "xr_shortcut.h"

using RStringVec = xr_vector<shared_str>;
using RStringSet = xr_set<shared_str>;
using RTokenVec = xr_vector<xr_rtoken>;

#include "FS.h"
#include "log.h"
#include "xr_trims.h"
#include "xr_ini.h"
#ifdef NO_FS_SCAN
#include "ELocatorAPI.h"
#else
#include "LocatorAPI.h"
#endif
#include "FileSystem.h"
#include "FTimer.h"
#include "fastdelegate.h"
#ifdef WINDOWS
#include "intrusive_ptr.h"
#endif

#include "net_utils.h"

#pragma region Cordis BOOST Includes 
#include <boost/regex.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#pragma endregion


#pragma region Cordis Google Test
// TODO: воспользоваться потом boost_unit_test_framework
#include <gtest/gtest.h>
#pragma endregion

#pragma region Cordis Log Section

#ifdef CRD_LOG_FULL
// @ default message
// Color: Gray in log  ouput
#define MESSAGE(text_message, ...) { /*xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] "##text_message, file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__);*/ }

// @ used for deallocation or something else rare and indeed vital information in log 
// Color: Dark Blue in log ouput
#define MESSAGEI(text_message, ...) { xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] information -> "##text_message, file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__); }

// @ used for warnings
// Color: Yellow in log output
#define MESSAGEW(text_message, ...) { xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] WARNING: "##text_message, file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__); }
// @ version of MESSAGEW macros, but with Return statement at the end
// Color: Yellow in log ouput
#define MESSAGEWR(text_message, ...){ xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] WARNING: "##text_message" Return ...", file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__); }

// @ for errors
// Color: Red in log output
#define MESSAGEE(text_message, ...) { xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] ERROR: "##text_message, file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__); R_ASSERT(false); }
// @ version of MESSAGEE, but with Return statement at the end
// Color: Red in log output
#define MESSAGEER(text_message, ...) { xr_string file_name = __FILE__; Msg("[%s|" __FUNCTION__ "|%d] ERROR: "##text_message" Return ...", file_name.substr(file_name.rfind("\\")+1).c_str(), __LINE__, __VA_ARGS__); R_ASSERT(false); }
#else
#define MESSAGE(text_message, ...) {}
#define MESSAGEI(text_message, ...) {}
#define MESSAGEW(text_message, ...) {}
#define MESSAGEWR(text_message, ...) {}
#define MESSAGEE(text_message, ...) {}
#define MESSAGEER(text_message, ...) {}
#endif

#pragma endregion

#pragma region Cordis Includes
#include <regex>
#include "Cordis_ScriptDataStructures.h"

// TODO: заменить на более новую библиотеку - это уже legacy
#include <tbb/tbb.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include "TaskManager.h"
#pragma endregion

// destructor
template <class T>
class destructor
{
    T* ptr;

public:
    destructor(T* p) { ptr = p; }
    ~destructor() { xr_delete(ptr); }
    T& operator()() { return *ptr; }
};

// ***** The Core definition *****
class XRCORE_API xrCore
{
    const char* buildDate = "";
    u32 buildId = 0;
    bool UseSDK = false;

public:
    string64 ApplicationName;
    CTimer m_Timer;
    string_path ApplicationPath;
    string_path WorkingPath;
    string64 UserName;
    string64 CompName;
    char* Params;
    DWORD dwFrame;
    bool PluginMode;
    bool bRunningOutsideDirectory = true;
    void Initialize(pcstr ApplicationName, pcstr commandLine = nullptr, LogCallback cb = nullptr, bool init_fs = true,
        pcstr fs_fname = nullptr, bool plugin = false);
    void _destroy();
    const char* GetBuildDate() const { return buildDate; }
    u32 GetBuildId() const { return buildId; }
    static constexpr pcstr GetBuildConfiguration();
    inline void ActivateSDK(void) noexcept
    {
        if (!this->UseSDK)
            this->UseSDK = true;
    }

    inline bool CanUseSDK(void) noexcept { return this->UseSDK; }

private:
    void CalculateBuildId();
};

extern XRCORE_API xrCore Core;