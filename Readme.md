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
    When the boost library is built you can start from second stage of this instruction
    2. Open Visual Studio or your favorite IDE (it can be just notepad)
    3. Build the solution in IDE or use cmake file.
    
        
TO Build google test write this: "cmake -DBUILD_SHARED_LIBS=ON ."