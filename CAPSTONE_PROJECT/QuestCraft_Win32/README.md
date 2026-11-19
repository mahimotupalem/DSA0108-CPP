# QuestCraft – Zero-Dependency Windows C++17 Capstone Project

**Storyline:** *The Lost Kingdom – The Quest for the Magical Crystal*  
This version uses **pure C++17 + built-in Windows Graphics (`windows.h` / `gdi32`)** — **no SFML, no MSYS2, no external libraries, and no DLL setup required**.

---

## Quick Start (2 Steps)

### Step 1: Ensure a C++ Compiler (`g++`) is Installed
If `g++ --version` works in your terminal, skip straight to **Step 2**.  
If `g++` is not recognized, run this **single command** in PowerShell to install MinGW-w64 automatically:
```powershell
winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
```
*(Close and reopen VS Code after installation finishes.)*

### Step 2: Open in VS Code & Run
1. Extract `QuestCraft_Win32.zip` and open the `QuestCraft_Win32` folder in **VS Code**.
2. Press **`Ctrl + Shift + B`** (or double-click **`run.bat`**).
3. It compiles `build\QuestCraft.exe` and opens the **QuestCraft** 2D graphical game window immediately!
