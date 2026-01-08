cmake_minimum_required(VERSION 3.16)
project(Application VERSION 1.0.0 LANGUAGES CXX)

# Find WxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base webview)
include(${wxWidgets_USE_FILE})

# Add executable
set(SOURCES
    src/main.cpp
)
add_executable(Application ${SOURCES})
target_link_libraries(Application ${wxWidgets_LIBRARIES})

# Web assets build
set(WEB_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/web)

add_custom_target(web-assets ALL
    COMMAND npm install
    COMMAND npm run build
    WORKING_DIRECTORY ${WEB_BUILD_DIR}
)

# Make sure the executable depends on web assets
add_dependencies(Application web-assets)

# Copy built web assets to executable directory
add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${WEB_BUILD_DIR}/dist
        $<TARGET_FILE_DIR:Application>/web-assets
)
