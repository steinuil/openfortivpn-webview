cmake_minimum_required(VERSION 3.1.0)

project(openfortivpn-webview VERSION 1.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)

add_executable(openfortivpn-webview
  mainwindow.cpp
  main.cpp
)

target_link_libraries(openfortivpn-webview Qt5::WebEngineWidgets)

install(TARGETS openfortivpn-webview DESTINATION bin)
