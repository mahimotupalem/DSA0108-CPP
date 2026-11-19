cmake_minimum_required(VERSION 3.16)
project(QuestCraft_Win32 VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QuestCraft WIN32
    src/main.cpp
    src/WinUI.cpp
    src/Player.cpp
    src/GameMenu.cpp
    src/LevelManager.cpp
    src/PuzzleEngine.cpp
    src/DecisionSystem.cpp
    src/HintSystem.cpp
    src/ScoreManager.cpp
    src/Leaderboard.cpp
    src/SaveManager.cpp
)
target_include_directories(QuestCraft PRIVATE src)
target_link_libraries(QuestCraft PRIVATE gdi32 user32 msimg32)
