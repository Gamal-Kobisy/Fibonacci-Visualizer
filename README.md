# Fibonacci Recursion Tree Visualizer

An interactive Qt-based C++ application that demonstrates how recursion unfolds in the Fibonacci sequence through dynamic tree visualization.

## Features

- **Interactive Visualization**: Watch the Fibonacci recursion tree build in real-time
- **Node-by-Node Animation**: Each recursive call is represented as a node in the tree
- **Memoization Support**: Toggle memoization to see how caching reduces redundant calculations
- **Adjustable Animation Speed**: Control the visualization speed from slow to fast
- **Statistics Display**: View total function calls, memoized calls, and efficiency gains
- **Color-Coded Nodes**:
  - Light Blue: Base cases (F(0), F(1))
  - Peach: Computed nodes
  - Light Green: Memoized nodes (cached results)
- **Visual Indicators**: Dashed lines show connections to memoized results

## Educational Value

This application helps learners understand:
- How recursion works in practice
- The concept of overlapping subproblems
- The benefits of memoization/dynamic programming
- How function call stacks grow during recursion
- The difference between naive recursion and optimized approaches

## Requirements

- Qt 5.15+ or Qt 6.x
- CMake 3.16+
- C++17 compatible compiler
- Linux, macOS, or Windows

## Building from Source

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install qt6-base-dev cmake build-essential

# Or for Qt5:
# sudo apt-get install qtbase5-dev cmake build-essential

# Build
mkdir build
cd build
cmake ..
cmake --build .

# Run
./bin/FibonacciVisualizer
```

### macOS

```bash
# Install dependencies (using Homebrew)
brew install qt cmake

# Build
mkdir build
cd build
cmake ..
cmake --build .

# Run
./bin/FibonacciVisualizer
```

### Windows

```bash
# Install Qt from https://www.qt.io/download
# Install CMake from https://cmake.org/download/

# Build (in Developer Command Prompt)
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run
.\bin\Release\FibonacciVisualizer.exe
```

## Usage

1. **Enter a Number**: Use the spin box to select a Fibonacci number (0-15)
2. **Toggle Memoization**: Check the "Use Memoization" box to enable caching
3. **Click Visualize**: Start the animation
4. **Adjust Speed**: Use the slider to control animation speed
5. **Observe**: Watch how the recursion tree builds and see the statistics

### Tips

- Start with small numbers (5-7) to understand the pattern
- Compare the same number with and without memoization
- Notice how memoization significantly reduces redundant calculations for larger numbers
- The statistics panel shows efficiency gains when using memoization

## Implementation Details

### Architecture

The application consists of three main components:

1. **FibonacciTree** (`fibonaccitree.h/cpp`): Manages the recursion tree data structure
   - Builds the tree recursively
   - Tracks function calls and memoization
   - Supports both naive and memoized approaches

2. **TreeVisualizer** (`treevisualizer.h/cpp`): Handles the visual rendering
   - Draws nodes and edges
   - Manages animation timing
   - Calculates optimal node positioning

3. **MainWindow** (`mainwindow.h/cpp`): Provides the user interface
   - Controls for input and settings
   - Statistics display
   - Animation controls

### Algorithm

The naive Fibonacci implementation has O(2^n) time complexity due to redundant calculations. With memoization, the complexity reduces to O(n) by caching results.

## Screenshots

(Screenshots would be added here showing the application in action)

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Gamal Kobisy

## Acknowledgments

This project was created to help students and developers understand recursion, memoization, and dynamic programming concepts through interactive visualization.