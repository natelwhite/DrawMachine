### Install Dependencies
- SDL3 has dependencies that must be installed before building this project. The dependencies you must install depends on the system
#### Windows
- Install mingw-w64-ucrt toolchain:
	- Download: https://github.com/msys2/msys2-installer/releases/download/2024-12-08/msys2-x86_64-20241208.exe
	- From MSYS2 terminal: `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`
#### Linux
- Ubuntu 18.04:
	- `sudo apt-get install build-essential git make \`
	- `pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \`
	- `libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \`
	- `libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev \`
	- `libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \`
	- `libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev`
- Ubuntu 22.04+ can also add `libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev` to that command line.
- Fedora 35:
	- `sudo yum install gcc git-core make cmake \`
	- `alsa-lib-devel pulseaudio-libs-devel nas-devel pipewire-devel \`
	- `libX11-devel libXext-devel libXrandr-devel libXcursor-devel libXfixes-devel \`
	- `libXi-devel libXScrnSaver-devel dbus-devel ibus-devel \`
	- `systemd-devel mesa-libGL-devel libxkbcommon-devel mesa-libGLES-devel \`
	- `mesa-libEGL-devel vulkan-devel wayland-devel wayland-protocols-devel \`
	- `libdrm-devel mesa-libgbm-devel libusb-devel libdecor-devel \`
	- `pipewire-jack-audio-connection-kit-devel \`
- Fedora 39+ can also add `liburing-devel` to that command line.
- For any other build issues potentially related to linux, see: https://wiki.libsdl.org/SDL3/README/linux.
### Build
- First, clone the repo and it's submodules
	- `git clone --recurse-submodules https://natelwhite/DrawMachine.git`
- Then, while in `./DrawMachine/`:
	- Generate build system: `cmake -B build`
		- Optionally use cmake: `cmake -B build -G Ninja`
	- Execute build system: `cmake --build build`
- The DrawMachine executable should now be in ./DrawMachine/build/.

