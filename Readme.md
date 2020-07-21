REQUIREMENTS:
You need to install visual studio 2019 
            OR
You can use Qt if you are linux's user, but you need to supported of C++17 compiler in your system.
  
Remark: Our engine is set up for C++17 and used only this standart or above (C++20/23)

HOW TO USE: 
    1. If you don't have to build the boost library YOU MUST DO IT FIRST.
        1.1 open your cmd/bash and navigate to boost library in cloned repo (or downloaded)
            Example: "cd C:\Users\user\cordisproject\sdk\lib_boost"
        1.2 Write the command: "bootstrap"
        1.3 After, write the command: ".\b2"
        1.4 After 1.3, write the  command: ".\b2 runtime-link=static"
    When the boost library is built you can start from second stage of this instruction
    2. Open Visual Studio or your favorite IDE (it can be just notepad)
    3. Build the solution in IDE or use cmake file.
    
        
TO Build google test write this: "cmake ."
And add to all projects which you want to test
1. Open project configuration -> Preprocessor -> "$(xrSdkDir)googletest\googletest\;"
2. add this ONCE to any .cpp file in your project: "#include <src\gtest-all.cc>"
3. If you project is dll write this in DllMain function in section where is DLL_PROCESS_ATTACH (or just in main or WinMain function if a project is exe): "testing::initGoogleTest(); RUN_ALL_TESTS();"
4. Or you can see implemented examples with unit testing in projects xrCore, xrGame and xrEngine (all three dlls), xr_3da (exe).

[RU]
Для Windows подойдёт Visual Studio 2019. Для Linux Qt или любой другой в вашем понимании IDE. 

1. Когда вы перешли в корневой каталог, вы должны скомпилировать boost библиотеку, для этого переходим по "sdk/lib_boost". В консоли пишем bootstrap, затем .\b2, после .\b2 runtime-link=static.
2. Если вы хотите использовать googletest в вашем проекте, либо в любом другом, для этого вы должны указать в настройках проекта Configuration -> Preprocessor -> "$(xrSdkDir)googletest\googletest\;". Затем в любом .cpp файле указать включение файла реализации gtest-all.cc, то есть #include <src\gtest-all.cc>, НО ТОЛЬКО ОДИН ЕДИНСТВЕННЫЙ РАЗ. Самое использование сводится к "testing::initGoogleTest(); RUN_ALL_TESTS();", где вы должны это указать в любом месте в котором посчитаете нужным, обычно это выступает точка входа в приложение/библиотеку. Для большего понимания, вы можете уже сразу же посмотреть настроенные проекты под googletest - это xrCore, xrGame, xrEngine, xr_3da.