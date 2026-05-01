Dependencies:
- cmake
- clang++
- pkgconfig
- autoconf
- automake
- autoconf-archive
- libtool


In order to build the software, the followings steps are recommended:
- Clone the repository into a userspace folder. Don't forget to clone the submodules as well!
- Run the command `cmake --preset [NAME OF PRESET]`
- Run the command `cmake --build build --preset [NAME OF PRESET]`
- To install, run the command `cmake --install ./build/release/ --prefix install` 
- In some environments, specifying the -DCMAKE_TOOLCHAIN_FILE, -DCMAKE_CXX_COMPILER and _DCMAKE_MAKE_PROGRAM might be necessary.
