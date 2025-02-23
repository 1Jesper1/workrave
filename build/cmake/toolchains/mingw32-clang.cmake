set(SYS_ROOT_PREFIX "")
set(GCC_VERSION 11.2.1)
set(TOOLCHAIN_ROOT "/usr/i686-w64-mingw32")
set(SYS_ROOT "${SYS_ROOT_PREFIX}/usr/i686-w64-mingw32/sys-root/mingw/")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_RC_COMPILER llvm-rc)
set(CMAKE_ASM_MASM_COMPILER uasm)

set(CMAKE_SYSTEM_PROCESSOR i686)
set(CMAKE_C_COMPILER_TARGET i686-w64-mingw32)
set(CMAKE_CXX_COMPILER_TARGET i686-w64-mingw32)
set(CMAKE_ASM_COMPILER_TARGET i686-w64-mingw32)

set (WINE wine)

set(CMAKE_SYSROOT ${SYS_ROOT})
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_ROOT} ${SYS_ROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(ENV{PKG_CONFIG_LIBDIR} "${SYS_ROOT}/lib/pkgconfig")
set(ENV{PKG_CONFIG_PATH} "")
set(ENV{PKG_CONFIG_SYSROOT_DIR} "${SYS_ROOT_PREFIX}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "--sysroot=${SYS_ROOT} -L/usr/lib/gcc/${CMAKE_C_COMPILER_TARGET}/${GCC_VERSION}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT ${CMAKE_EXE_LINKER_FLAGS_INIT})

set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
    /usr/lib64/clang/13.0.0/include
    ${SYS_ROOT}/include/c++/
    ${SYS_ROOT}/include/c++/${CMAKE_CXX_COMPILER_TARGET}
    ${SYS_ROOT}/${CMAKE_C_COMPILER_TARGET}/include
    ${SYS_ROOT}/include
)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_STANDARD_INCLUDE_DIRECTORIES})
