FROM debian:bookworm-slim AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get install -y ca-certificates \
  && rm -rf /var/lib/apt/lists/*

COPY dockerc_files/ca_certs/ ./custom_certs
COPY add_ca_certs.sh ./
RUN ./add_ca_certs.sh && update-ca-certificates

# ---- basic build tools ----
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    wget \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# libui-ng dependencies (GTK backend): meson, ninja-build, gcc, pkg-config, libgtk-3-dev, libglib2.0-dev, libpango1.0-dev, libcairo2-dev, libx11-dev, libxext-dev, libxrender-dev, libxi-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev
# AppImage dependencies: wget, file, patchelf, desktop-file-utils, libfuse2, libgtk-3-dev

RUN apt-get update && apt-get install -y \
    meson \
    ninja-build \
    gcc \
    pkg-config \
    libgtk-3-dev \
    libglib2.0-dev \
    libpango1.0-dev \
    libcairo2-dev \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxi-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    wget \
    file \
    patchelf \
    desktop-file-utils \
    libfuse2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# ---- fetch libui-ng ----
RUN git clone https://github.com/libui-ng/libui-ng.git

WORKDIR /app/libui-ng

# ---- build libui-ng itself ----
RUN meson setup build --buildtype=release -Ddefault_library=static -Dtests=false -Dexamples=false \
    && ninja -C build

# ---- AppImage build tooling ----
WORKDIR /tools

RUN wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20251107-1/linuxdeploy-x86_64.AppImage && \
    chmod +x linuxdeploy-x86_64.AppImage && \
    ./linuxdeploy-x86_64.AppImage --appimage-extract

# ---- your app ----
WORKDIR /app

COPY CMakeLists.txt .
COPY src ./src

# assume you link libui-ng via add_subdirectory or installed build
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -Dlibui-ng_DIR=/app/libui-ng/build && \
    cmake --build build && \
    strip -s build/app

COPY AppDir ./AppDir
RUN mkdir -p AppDir/usr/bin && \
    cp build/app AppDir/usr/bin/

RUN /tools/squashfs-root/AppRun \
    --appdir=AppDir \
    -d AppDir/app.desktop \
    -i AppDir/app.png \
    --exclude-library="*/gio/modules/*" \
    --output appimage

RUN cd / && ls /app

# ---- export binary ----
FROM scratch AS export
COPY --from=builder /app/App-x86_64.AppImage /
