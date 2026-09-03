# At-server

## Requirements

- **OS:** Linux (tested on Ubuntu 24.04)
- **Compiler:** GCC (g++) 13.3.0 or higher
- **Standard:** C++11

## Getting Started

1. Install common deps:

```bash
sudo apt update
sudo apt install -y build-essential git
```

2. Clone repository:

```bash
git clone https://github.com/mshandruk/at-server.git
cd at-server
```

## Development

Install dev-tools:

```bash
sudo apt install -y clang-format clang-tidy
```

1. Format check:

```bash
clang-format -i src/*
```

2. Static analysis:

```bash
clang-tidy src/* -- -std=c++11
```

3. Debug build with tests:

```bash
mkdir -p build
g++ -std=c++11 src/PatternMatcher.cpp src/RulesLoader.cpp src/Tests.cpp -o build/tests
```

4. Run tests:

```bash
./build/tests
```
