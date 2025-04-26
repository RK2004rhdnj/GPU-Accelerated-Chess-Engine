# GPU-Accelerated Chess Engine

A high-performance chess engine that leverages GPU acceleration for improved move generation and evaluation. This engine implements modern chess programming techniques while utilizing parallel processing capabilities of GPUs for enhanced performance.

## Features

- **GPU-Accelerated Move Generation**: Utilizes parallel processing for efficient move generation
- **Multiple Protocol Support**: 
  - UCI (Universal Chess Interface) protocol implementation
  - XBoard protocol support
- **Advanced Search Algorithms**: Implements efficient search techniques for optimal move selection
- **Position Evaluation**: Sophisticated evaluation function for accurate position assessment
- **Opening Book Support**: Includes polyglot opening book implementation
- **Performance Testing**: Built-in performance testing capabilities

## Project Structure

The engine is organized into several key components:

- `board.c`: Core board representation and state management
- `movegen.c`: Move generation logic
- `search.c`: Search algorithms implementation
- `evaluate.c`: Position evaluation function
- `uci.c`: UCI protocol implementation
- `xboard.c`: XBoard protocol implementation
- `polybook.c`: Opening book handling
- `bitboards.c`: Bitboard operations
- `pvtable.c`: Principal variation table implementation

## Building the Project

To build the project, use the provided Makefile:

```bash
make
```

This will compile the engine and create the executable.

## Usage

### UCI Mode
To run the engine in UCI mode:
```bash
./blunderbuss
```

### XBoard Mode
To run the engine in XBoard mode:
```bash
./blunderbuss -xboard
```

## Performance Testing

The engine includes performance testing capabilities:
```bash
./blunderbuss -perft
```
