#include "stdafx.h"
#include "resource.h"
#if defined(WINDOWS)
#include "StickyKeyFilter.hpp"
#elif defined(LINUX)
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#endif
#include "xrEngine/main.h"
#include "xrEngine/splash.h"
#include <SDL.h>

#include <src/gtest-all.cc>

int entry_point(pcstr commandLine)
{
    splash::show(true);

    Cordis::TaskManager::getInstance().initialize();

    R_ASSERT3(SDL_Init(SDL_INIT_VIDEO) == 0, "Unable to initialize SDL", SDL_GetError());

    Cordis::TaskManager::getInstance().getCore()->run([&]() {
		xrDebug::Initialize();

		if (strstr(commandLine, "-dedicated"))
			GEnv.isDedicatedServer = true;

#ifdef WINDOWS
		StickyKeyFilter filter;
		if (!GEnv.isDedicatedServer)
			filter.initialize();
#endif
   });

    pcstr fsltx = "-fsltx ";
    string_path fsgame = "";
    if (strstr(commandLine, fsltx))
    {
        const u32 sz = xr_strlen(fsltx);
        sscanf(strstr(commandLine, fsltx) + sz, "%[^ ] ", fsgame);
    }
    Core.m_Timer.Start();
    Core.Initialize("Cordis Project", commandLine, nullptr, true, *fsgame ? fsgame : nullptr);

    const auto result = RunApplication();

    Core._destroy();

    SDL_Quit();
    return result;
}

#if defined(WINDOWS)
int StackoverflowFilter(const int exceptionCode)
{
    if (exceptionCode == EXCEPTION_STACK_OVERFLOW)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
}

/*
int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prevInst, char* commandLine, int cmdShow)
{    
    return entry_point(commandLine);
}*/

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	char* commandLine = nullptr;
	int i;
	if (argc > 1)
	{
		size_t sum = 0;
		for (i = 1; i < argc; ++i)
			sum += strlen(argv[i]) + strlen(" \0");

		commandLine = xr_alloc<char>(sum);
		ZeroMemory(commandLine, sum);

		for (i = 1; i < argc; ++i)
		{
			strcat(commandLine, argv[i]);
			strcat(commandLine, " ");
		}
	}
	else
		commandLine = strdup("");

 
	int result = entry_point(commandLine);

    if (commandLine && argc > 1)
	    xr_free(commandLine);

    return result;
}

#elif defined(LINUX)
int main(int argc, char *argv[])
{
    int result = EXIT_FAILURE;

    try
    {
        char* commandLine = nullptr;
        int i;
        if(argc > 1)
        {
            size_t sum = 0;
            for(i = 1; i < argc; ++i)
                sum += strlen(argv[i]) + strlen(" \0");

            commandLine = (char*)xr_malloc(sum);
            ZeroMemory(commandLine, sum);

            for(i = 1; i < argc; ++i)
            {
                strcat(commandLine, argv[i]);
                strcat(commandLine, " ");
            }
        }
        else
            commandLine = strdup("");

        result = entry_point(commandLine);

        xr_free(commandLine);
    }
    catch (const std::overflow_error& e)
    {
        _resetstkoflw();
        FATAL_F("stack overflow: %s", e.what());
    }
    catch (const std::runtime_error& e)
    {
        FATAL_F("runtime error: %s", e.what());
    }
    catch (const std::exception& e)
    {
        FATAL_F("exception: %s", e.what());
    }
    catch (...)
    {
    // this executes if f() throws std::string or int or any other unrelated type
    }

    return result;
}
#endif
