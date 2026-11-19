@echo off
setlocal
echo ============================================================
echo   QuestCraft Win32 Enhanced Build (C++17 - Zero External Libs)
echo ============================================================

where g++ >nul 2>&1
if errorlevel 1 (
    echo [ERROR] No C++ compiler ^(g++^) found on this PC!
    echo Run this ONE command in PowerShell to install a C++ compiler:
    echo    winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
    echo Then close and reopen VS Code / terminal and run build.bat again.
    pause
    exit /b 1
)

if not exist "build" mkdir build

g++ -std=c++17 -O2 -mwindows -Wall -Wextra ^
    -I"src" ^
    src\main.cpp ^
    src\WinUI.cpp ^
    src\Player.cpp ^
    src\GameMenu.cpp ^
    src\LevelManager.cpp ^
    src\PuzzleEngine.cpp ^
    src\DecisionSystem.cpp ^
    src\HintSystem.cpp ^
    src\ScoreManager.cpp ^
    src\Leaderboard.cpp ^
    src\SaveManager.cpp ^
    -lgdi32 -luser32 -lmsimg32 ^
    -o build\QuestCraft.exe

if errorlevel 1 (
    echo [BUILD FAILED] See compiler output above.
    pause
    exit /b 1
)

xcopy /E /I /Y "data" "build\data" >nul
echo [SUCCESS] Built enhanced build\QuestCraft.exe with ZERO external DLLs!
