# Linux Instructions

### Build

`docker build --output ./out -t libui-app:latest .`

### Run

`./out/App-x86_64.AppImage`

# Mac Instructions

### Install Prerequisites

brew install meson cmake ninja

### Clone and build libui-ng

From the project root
1. `git clone https://github.com/libui-ng/libui-ng.git`
1. `cd libui-ng`
1. `meson setup build --buildtype=release -Dtests=false`
1. `ninja -C build`
1. `cd ..`

### Build this project

`cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build`

### Run

1. `./build/app`
