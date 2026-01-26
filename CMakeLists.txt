cmake_minimum_required(VERSION 3.26)
project(IntelliDrive VERSION 0.1.0)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

# Ensure SFML builds as shared libraries
set(SFML_BUILD_SHARED_LIBS TRUE CACHE BOOL "Build SFML as shared libraries" FORCE)

# Build SFML
add_subdirectory(vendors/sfml/ vendors/sfml_build/)

# Find ArrayFire
find_package(ArrayFire REQUIRED)

# Explicitly list all source files
set(SOURCE_FILES
        src/main.cpp
        src/CarChooseState.cpp
        src/LevelCreator.cpp
        src/LevelSelectState.cpp
        src/Tile.cpp
        src/GameState.cpp
        src/GameStateParent.cpp
        src/AiGameState.cpp
        src/SettingsState.cpp
        src/MetricsManager.cpp
        src/DropDown.cpp
        src/Game.cpp
        src/Car.cpp
        src/ResourceManager.cpp
        src/MenuState.cpp
        src/PauseState.cpp
        src/DeathState.cpp
        src/VariableManager.cpp
        vendors/ai/Utility/Utility.cpp
        vendors/ai/NeuralNetwork/NeuralNetwork.cpp
)

set(HEADER_FILES
        include/CarChooseState.h
        include/LevelCreator.h
        include/LevelSelectState.h
        include/Tile.h
        include/AiGameState.h
        include/SettingsState.h
        include/MetricsManager.h
        include/DropDown.h
        include/Game.h
        include/Car.h
        include/ResourceManager.h
        include/MenuState.h
        include/PauseState.h
        include/DeathState.h
        include/GameStateParent.h
        include/VariableManager.h
        vendors/ai/Utility/Utility.h
        vendors/ai/NeuralNetwork/NeuralNetwork.h
)

# Add the main executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/vendors/sfml/include/
        ${ArrayFire_INCLUDE_DIRS} # Include ArrayFire
)

# Include directories
include_directories(${AF_PATH}/include)

# Link directories (if necessary)
link_directories(${AF_PATH}/lib)

# Link SFML and ArrayFire libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        ${ArrayFire_LIBRARIES} # Link ArrayFire
)

# Define the resource directory
set(RESOURCE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

# Create the resource directory if it doesn't exist
file(MAKE_DIRECTORY ${RESOURCE_DIR})

# Get all files in the resources directory
file(GLOB RESOURCE_FILES "${PROJECT_SOURCE_DIR}/resources/*")

# Add custom command to copy resource files
foreach(resource ${RESOURCE_FILES})
    get_filename_component(resource_name ${resource} NAME)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${RESOURCE_DIR}/${resource_name}"
            COMMENT "Copying ${resource} to ${RESOURCE_DIR}/${resource_name}"
    )
endforeach()

# Copy necessary SFML DLLs after the build (for Windows)
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:sfml-graphics>"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-graphics.dll"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:sfml-window>"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-window.dll"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:sfml-system>"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-system.dll"
    )
endif()

# Optional: Set properties for Visual Studio to use multithreaded DLL runtime
if (MSVC)
    foreach(flag_var
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/MDd" "/MD" ${flag_var} "${${flag_var}}")
    endforeach()
endif()
