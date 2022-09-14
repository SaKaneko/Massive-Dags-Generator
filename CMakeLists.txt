cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17) # C++17...
set(CMAKE_CXX_COMPILER g++)

project(MassiveDagGenerator CXX)

set(MDG_src
    src/main.cpp
)
set(GraphView_src
    src/graphview.cpp
)

## Define the executable
add_executable(MDG-run ${MDG_src})
add_executable(Graph-View ${GraphView_src})

install(TARGETS MDG-run
        DESTINATION bin)