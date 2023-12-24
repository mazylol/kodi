cmake_minimum_required(VERSION 3.22)
project(kodi)

# enable ccache
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

set(ENV_FILE_SOURCE ${CMAKE_SOURCE_DIR}/.env)
set(ENV_FILE_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/.env)
configure_file(${ENV_FILE_SOURCE} ${ENV_FILE_DESTINATION} COPYONLY)

include(FetchContent)

# dpp
FetchContent_Declare(dpp
        GIT_REPOSITORY https://github.com/brainboxdotcc/DPP
        GIT_TAG v10.0.28
)
FetchContent_MakeAvailable(dpp)

# cpp-dotenv
FetchContent_Declare(cpp_dotenv
        GIT_REPOSITORY https://github.com/adeharo9/cpp-dotenv
        GIT_TAG v1.0.0-alpha
)
FetchContent_MakeAvailable(cpp_dotenv)

# nlohmann_json::nlohmann_json
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# fmt
FetchContent_Declare(fmt URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz)
FetchContent_MakeAvailable(fmt)

add_executable(${PROJECT_NAME}
        main.cpp
        Commands.cpp
        Types.cpp
        Types.h
        Commands.h
)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying assets folder to build directory"
)

add_custom_target(copy_languages
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/languages
        ${CMAKE_BINARY_DIR}/languages
        COMMENT "Copying languages folder to build directory"
)

add_custom_target(copy_people
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/people
        ${CMAKE_BINARY_DIR}/people
        COMMENT "Copying languages folder to build directory"
)

add_dependencies(${PROJECT_NAME} copy_assets copy_languages copy_people)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
        dpp
        cpp_dotenv
        fmt::fmt
        nlohmann_json::nlohmann_json
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)