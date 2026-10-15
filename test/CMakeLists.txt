cmake_minimum_required(VERSION 3.15)
project(pdfium_test CXX)

find_package(PDFium REQUIRED)

add_executable(pdfium_test main.cpp)
target_compile_features(pdfium_test PRIVATE cxx_std_17)
target_link_libraries(pdfium_test PRIVATE pdfium)

# Link against the static CRT like PDFium itself, and turn CRT mismatch
# warnings (LNK4098) into errors so a /MD library can't slip through
set_property(TARGET pdfium_test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_options(pdfium_test PRIVATE /WX)
