# A* Pathfinding Visualizer

This repository contains two implementations of the A* (A-Star) Pathfinding Algorithm:
- **Python** (using `pygame` for visualization)
- **C++** (using [SFML](https://www.sfml-dev.org/) for visualization)

The A* algorithm is widely used for finding the shortest path between two points on a grid or graph, especially in game development and robotics.

---

## Overview of the A* Algorithm

### How It Works
The A* algorithm combines features of Dijkstra’s algorithm and a heuristic-based search to efficiently compute the shortest path from a start point to a goal point. It uses:
- **g_score**: The cost from the start node to the current node.
- **h_score** (heuristic): An estimated cost from the current node to the goal (using Manhattan distance in this implementation).
- **f_score**: The sum of `g_score` and `h_score` which guides the selection of the next node.

### Time Complexity
- **Worst-Case:**  
  In the worst-case scenario (e.g., when the heuristic provides little to no guidance), A* behaves similarly to Dijkstra’s algorithm:
  - **O(N log N)**, where N is the number of nodes (using a binary heap for the priority queue).

- **Average-Case:**  
  With an admissible and consistent heuristic (such as Manhattan distance for grid-based movement), A* usually expands far fewer nodes:
  - Approximately **O(b^d)**, where _b_ is the branching factor (neighbors per node) and _d_ is the depth of the shortest path.

- **Best-Case:**  
  In an ideal scenario where the goal is reached very quickly:
  - **O(1)** (rare in practical scenarios).

---

## Implementations

### Python Implementation
- **Language:** Python 3
- **Visualization:** `pygame`
- **Usage:**
  1. Install dependencies:
     ```bash
     pip install pygame
     ```
  2. Run the application:
     ```bash
     python astar.py
     ```
- **Controls:**
  - **Left Click:** Set start (orange), end (turquoise), or barriers (black)
  - **Right Click:** Remove a cell (reset)
  - **Spacebar:** Execute the algorithm
  - **C:** Clear the grid

### C++ Implementation
- **Language:** C++
- **Visualization:** SFML
- **Usage:**
  1. Install SFML (ensure it matches your compiler version)
  2. Compile with a command similar to:
     ```bash
     g++ astar.cpp -o astar -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
     ```
  3. Run the executable:
     ```bash
     ./astar.exe
     ```
- **Controls:** (similar to the Python version)
  - **Left Click:** Place start, end, and obstacles
  - **Right Click:** Remove obstacles or reset cells
  - **Spacebar:** Start the algorithm
  - **C:** Clear the grid

---

