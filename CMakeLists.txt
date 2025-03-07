cmake_minimum_required(VERSION 3.30)
project(appmathsproj)

set(CMAKE_CXX_STANDARD 20)

# Create the executable target
add_executable(appmathsproj main.cpp)

# Specify the include directory for MatPlot++
target_include_directories(appmathsproj PRIVATE /usr/local/Cellar/matplotplusplus/1.2.2/include)

# Specify the library directory for the target
target_link_directories(appmathsproj PRIVATE /usr/local/Cellar/matplotplusplus/1.2.2/lib)

# Link the correct library (libmatplot.dylib)
target_link_libraries(appmathsproj PRIVATE matplot)