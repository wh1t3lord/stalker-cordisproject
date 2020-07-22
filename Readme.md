REQUIREMENTS:
You need to install visual studio 2019 
            OR
You can use Qt if you are linux's user, but you need to supported of C++17 compiler in your system.
  
Remark: Our engine is set up for C++17 and used only this standart or above (C++20/23)

## HOW TO USE: 
1. If you don't have to build the boost library YOU MUST DO IT FIRST.
    - open your cmd/bash and navigate to boost library in cloned repo (or downloaded)
    - Example: "cd C:\Users\user\cordisproject\sdk\lib_boost"
    - Write the command: "bootstrap"
    - After, write the command: ".\b2"
    - After, write the  command: ".\b2 runtime-link=static"
    > When the boost library is built you can start from second stage of this instruction
    - Open Visual Studio or your favorite IDE (it can be just notepad)
    - Build the solution in IDE or use cmake file.

2. TO Build google test write this: "cmake ."
And add to all projects which you want to test
    - Open project configuration -> Preprocessor -> "$(xrSdkDir)googletest\googletest\;"
    - add this ONCE to any .cpp file in your project: "#include <src\gtest-all.cc>"
    - If you project is dll write this in DllMain function in section where is DLL_PROCESS_ATTACH (or just in main or WinMain function if a project is exe): "testing::initGoogleTest(); RUN_ALL_TESTS();"
    - Or you can see implemented examples with unit testing in projects xrCore, xrGame and xrEngine (all three dlls), xr_3da (exe).

3. Then you MUST DO IT: "git submodule update --init --recursive".

4. After the compilation of your solution, put all content of the folder sdk/Visual Leak Detector/bin/Win64/ to your configuration output folder (e.g. cordisproject/bin/Win64/Mixed/)
5. Write your debugging path and set up to properties of xr_3da project (Properties -> Debugging -> Working Directory) Be carefull and watch your selected path.

For example:
C:/Game/Stalker/ <- it is a not valid path 
C:/Game/Stalker/bin/ <- it is a valid path

6. Copy and paste the folder cordisproject/res/gamedata/ to your root folder of game.

7. Run the xr_3da.

[RU]
## Как использовать

Для Windows подойдёт Visual Studio 2019. Для Linux Qt или любой другой в вашем понимании IDE. 

1. Когда вы перешли в корневой каталог, вы должны скомпилировать boost библиотеку, для этого переходим по "sdk/lib_boost". В консоли пишем bootstrap, затем .\b2, после .\b2 runtime-link=static.
2. Если вы хотите использовать googletest в вашем проекте, либо в любом другом, для этого вы должны указать в настройках проекта Configuration -> Preprocessor -> "$(xrSdkDir)googletest\googletest\;". Затем в любом .cpp файле указать включение файла реализации gtest-all.cc, то есть #include <src\gtest-all.cc>, НО ТОЛЬКО ОДИН ЕДИНСТВЕННЫЙ РАЗ. Самое использование сводится к "testing::initGoogleTest(); RUN_ALL_TESTS();", где вы должны это указать в любом месте в котором посчитаете нужным, обычно это выступает точка входа в приложение/библиотеку. Для большего понимания, вы можете уже сразу же посмотреть настроенные проекты под googletest - это xrCore, xrGame, xrEngine, xr_3da.
3. Выполнить: "git submodule update --init --recursive".
4. После компиляции решения, положите в папку Mixed или любой другой конфигурации (по пути, к примеру cordisproject/bin/Win64/Mixed/) всё из папки sdk/Visual Leak Detector/bin/Win64/
5. Указать для xr_3da в Configuration -> Debugging -> Working Directory; ВНИМАНИЕ СТРОГО УКАЗАТЬ ЧТО ИЗ КАКОЙ-ТО ПАПКИ. То есть нельзя просто указать корневой каталог, а принудительно указать то запуск происходит из каталога допустим bin. 

К примеру имеем Игру по пути: C:/Game/Stalker/ <- не действительный путь 
А нужно: C:/Game/Stalker/bin/ <- теперь правильно

6. Поставить из папки cordisproject/res/gamedata/ в корневой каталог игры.

7. Запуск через IDE.