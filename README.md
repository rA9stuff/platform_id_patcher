`Mach-O` files include a header called `LC_BUILD_VERSION`, which contains a byte that defines the target platform for which the binary is intended. You can refer to `mach-o/loader.h` for the definitions corresponding to different devices. This patcher changes the platform id from `01` to `02`.

> [!WARNING]  
> While macOS and iOS share many libraries, subtle differences in the code can lead to issues when running binaries that have been patched in this way.   
