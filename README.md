Marc Rover — Pathfinding Robot Simulator in C 🤖

This academic project simulates a virtual exploration robot named **Marc Rover** navigating a grid-like planetary map using advanced data structures in C.

🧭 Project Description

The goal is to design an intelligent system that allows the rover to move across a terrain map while avoiding obstacles and minimizing movement cost. The rover's environment is modeled as a decision tree, where each node represents a possible state.

📦 Data Structures Used

- Node: stores position, terrain type, movement cost, and available directions
- Tree: represents possible movement paths from the starting point
- Queue: stores upcoming positions to visit (BFS-style)
- Stack: tracks the current path for backtracking

🔍 Features

- Recursive decision tree generation (`createTreeRecursivity`)
- Pathfinding to the base using minimal cost (`pathToBase`)
- Queue-based exploration of the environment (`enqueue`, `dequeue`)
- Stack-based tracking of movement history
- Full dynamic memory management with pointer safety

⚠️ Challenges Encountered

- Dynamic memory allocation and preventing memory leaks
- Pointer handling for tree traversal and node linkage
- Combating combinatorial explosion by limiting tree depth

🧠 Learnings

This project strengthened understanding of:
- Tree traversal
- Memory management in C
- Recursive algorithms
- Basic pathfinding strategies

🌟 Future Improvements

- Integration of advanced algorithms (e.g., A* search)
- Optimized memory footprint for large-scale maps
- Visual simulation interface

🚀 Author

Project by Paul-Alex Yao 
           Zoubir Sanhoun
           Raymond Sureshkumar
Efrei Paris — Academic Coursework Project
