# At-server

Modem emulation server.

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

3. Build

```bash
make all
```

## Example usage

1. Run socat

```bash
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

```text
2026/09/04 01:07:30 socat[76159] N PTY is /dev/pts/2
2026/09/04 01:07:30 socat[76159] N PTY is /dev/pts/3
```

2. Run at server

```bash
./build/at_server --rules rules.cfg --tty /dev/pts/2
```

3. Send command

```bash
echo -e "AT\r" > /dev/pts/3
```

```bash
cat /dev/pts/3
```

```text
mshandruk@saturn:~$ cat /dev/pts/3
OK
```

## Development

Install dev-tools:

```bash
sudo apt install -y clang-format clang-tidy make
```

1. Format check:

```bash
make format
```

2. Static analysis:

```bash
make lint
```

3. Debug build with tests:

```bash
make tests
```

4. Run tests:

```bash
./build/tests
```
