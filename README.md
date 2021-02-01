# Linux System Monitor
Create a Linux system monitor similar to htop. 

<img src="https://github.com/Doegstra/Linux-System-Monitor/blob/incorporate-feedback/images/system-monitor.gif" width="600"/>

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

To install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/Doegstra/Linux-System-Monitor.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
<img src="https://github.com/Doegstra/Linux-System-Monitor/blob/incorporate-feedback/images/system-monitor.gif" width="600"/>
