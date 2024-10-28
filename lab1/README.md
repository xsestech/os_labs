# Lab 1

Variant 23

## Docs

Report for this lab is avaliable at [docs/](docs/)

## Usage
## Local prerequisites
If you want to run this code locally you will need:
You will need:
- llvm clang or gcc
- Any POSIX system
- cmake 3.25 installed
## Docker
To use this code in proper docker run this command
```bash
docker build --build-arg UID=$(id -u) . -t os_lab
docker run -it --rm -v .:/app os_lab
```
### Building
```bash
cmake -S . -B build
cmake --build build
```
## Running
```bash
export CHILD_EXEC_PATH=/build/child/child
./build/parent/parent
```
### Strace
```bash
strace -f ./build/parent/parent
```