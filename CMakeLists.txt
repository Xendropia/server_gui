cmake_minimum_required(VERSION 3.12)

project(Chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure the current directory is included in the include paths
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if (MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

# List of modules to use (for Qt6)
set(Qt6Modules Core Widgets LinguistTools)

# Find necessary Qt6 packages
find_package(Qt6 COMPONENTS ${Qt6Modules} REQUIRED)

# Collect source, header, resource, and UI files
file(GLOB_RECURSE SRC_FILES sources/*.cpp)
file(GLOB_RECURSE HEADERS_FILES sources/*.h)
file(GLOB_RECURSE RC_FILES sources/*.qrc)
file(GLOB_RECURSE UI_FILES sources/*.ui)

# Enable automatic handling of moc, rcc, and uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Specify translation files
set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/my_ru.ts)
file(GLOB_RECURSE TS_SOURCES "*.cpp" "*.h" ".ui")

# Set up translation handling with lupdate
add_custom_target(translations)
foreach(_ts_file ${TS_FILES})
  get_filename_component(_ts_name ${_ts_file} NAME_WE)
  add_custom_command(
    OUTPUT "${_ts_file}"
    DEPENDS ${TS_SOURCES}
    COMMAND ${Qt6_LUPDATE_EXECUTABLE} -recursive
    ${CMAKE_CURRENT_SOURCE_DIR} -ts ${_ts_file}
  )
  add_custom_target(translate_${_ts_name} DEPENDS ${_ts_file})
  add_dependencies(translations translate_${_ts_name})
endforeach()

# Add translation files
qt6_add_translation(QM_FILES ${TS_FILES})

# Create the executable
add_executable(Chat ${QM_FILES} ${SRC_FILES} ${HEADERS_FILES} ${RC_FILES} ${UI_FILES})

# Link libraries to the target
target_link_libraries(Chat PUBLIC Qt6::Core Qt6::Widgets)
