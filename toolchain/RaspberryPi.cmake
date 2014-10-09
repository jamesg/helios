set(CMAKE_SYSTEM_NAME Linux)

set(ATLAS_RPI_ROOTFS "/home/james/raspberrypi/filesystem")

# Assuming the Raspberry Pi filesystem is mounted at ${ATLAS_RPI_ROOTFS} .
set(BOOST_DIR "${ATLAS_RPI_ROOTFS}/usr")
set(Boost_INCLUDE_DIR "${ATLAS_RPI_ROOTFS}/usr/include")
set(Boost_LIBRARY_DIR "${ATLAS_RPI_ROOTFS}/usr/lib")
set(CMAKE_FIND_ROOT_PATH "${ATLAS_RPI_ROOTFS}")

# Use programs from the host, libraries and headers from the target.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# For standard libraries like libicu and libdl.
link_directories("${ATLAS_RPI_ROOTFS}/usr/lib/arm-linux-gnueabihf")

# Options to arm-linux-gnueabihf-g++, --sysroot is required to pick up
# libraries.
set(CMAKE_C_FLAGS "--sysroot=${ATLAS_RPI_ROOTFS}")
set(CMAKE_CXX_FLAGS "-std=c++0x --sysroot=${ATLAS_RPI_ROOTFS}")

# These are from https://github.com/raspberrypi/tools.git .
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

