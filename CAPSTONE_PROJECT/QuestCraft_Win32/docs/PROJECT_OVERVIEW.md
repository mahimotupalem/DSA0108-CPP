# Project Overview – QuestCraft

## 1. Title & Domain
**QuestCraft – Interactive Puzzle Adventure and Decision Challenge Game Using C++**  
Domain: Desktop 2D Interactive Game Development & Object-Oriented Software Engineering (C++17 + SFML).

## 2. Problem Statement & Motivation
Educational and cognitive puzzle games often lack narrative continuity, meaningful branching choices, and persistent analytics. QuestCraft integrates memory, mathematical logic, pattern recognition, deductive mystery solving, and consequence-driven decision-making into a cohesive five-chapter fantasy quest (*The Lost Kingdom – The Quest for the Magical Crystal*).

## 3. Core Architecture & OOP Design
- **Encapsulation:** Player state, scoring rules, hints, and CSV persistence are isolated inside dedicated classes (`Player`, `ScoreManager`, `HintSystem`, `SaveManager`).
- **Inheritance & Polymorphism:** `BasePuzzleGenerator` defines a pure virtual `generate(Difficulty, int)` interface implemented by `MemoryPuzzleGenerator`, `LogicPuzzleGenerator`, `PatternPuzzleGenerator`, and `MysteryPuzzleGenerator`.
- **Persistence:** Standard C++17 `<filesystem>` and `<fstream>` manage `players.csv`, `progress.csv`, `leaderboard.csv`, and `game_history.csv` with automatic creation and startup restoration.
