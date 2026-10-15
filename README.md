# pdfium-static

The prebuilt binaries from [bblanchon/pdfium-binaries](https://github.com/bblanchon/pdfium-binaries) only come as DLLs and static builds aren't planned. Their build scripts do support static builds however, so this repository essentially runs those scripts with `PDFium_BUILD_TYPE=static`, pinned to a specific commit of pdfium-binaries.

## Building

Start the **Build** workflow from the Actions tab. It takes the following inputs:

- `branch` is the PDFium branch, e.g. `chromium/7390`. Leave it empty to build the branch of the latest Chrome release.
- `version` ends up in `VERSION` and `PDFiumConfig.cmake`. It's resolved automatically, when `branch` is empty.
- `target_cpu` is a space separated list of `x64`, `x86` and `arm64`, by default `x64 x86`
- `config` is `release` (`/MT`), `debug` (`/MTd`) or `both`. Debug builds have iterator debugging enabled, so they link with a default MSVC Debug configuration.
- `release` attaches the packages to a GitHub release, see below
- `binaries_ref` is the commit of pdfium-binaries to take the build scripts from

Every CPU and configuration is built in its own job, which uploads a zip named like `pdfium-win-x64-static-mt.zip` as artifact. It contains `include/`, `lib/pdfium.lib`, `PDFiumConfig.cmake`, `args.gn` and the licenses.

Since workflow artifacts expire after a while, the zips also get attached to a release named after the PDFium branch, e.g. `chromium/8107`. With the defaults that's four zips, x64 and x86 each with `/MT` and `/MTd`. If the release already exists, it's reused and zips with the same name are replaced, as such you can also add arm64 later on. Note that releases are only created for `chromium/*` branches, a build of `main` only ends up as workflow artifact.

## Using the library

Point `PDFium_DIR` to the extracted artifact and link against the `pdfium` target:

```cmake
find_package(PDFium REQUIRED)
target_link_libraries(my_app PRIVATE pdfium)
set_property(TARGET my_app PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
```

Note that PDFium is compiled with clang-cl against the MSVC STL of the `windows-2022` runner, as such you need the same or a newer MSVC toolset. Keep in mind, that the release library only links with release configurations and the debug library only with debug configurations.

## Testing

`test/` contains a small program, which the workflow links against the staged library and runs on `hello_world.pdf`. It links with `/WX`, so a CRT mismatch fails the build instead of just being a warning.
