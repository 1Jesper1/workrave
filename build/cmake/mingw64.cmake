set(SYS_ROOT_PREFIX "")

set(TOOLCHAIN_ROOT "/usr/i686-w64-mingw64")
set(SYS_ROOT "${SYS_ROOT_PREFIX}/usr/i686-w64-mingw64/sys-root/mingw/")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER   i686-w64-mingw64-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw64-g++)
set(CMAKE_RC_COMPILER  i686-w64-mingw64-windres)

set (WINE wine)
set (ISCC "/build/inno/app/ISCC.exe")

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_ROOT} ${SYS_ROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(${TOOLCHAIN_ROOT}/include)
include_directories(${SYS_ROOT}/include)

set(ENV{PKG_CONFIG_LIBDIR} "${SYS_ROOT}/lib/pkgconfig")
set(ENV{PKG_CONFIG_PATH} "")
set(ENV{PKG_CONFIG_SYSROOT_DIR} "${SYS_ROOT_PREFIX}")
