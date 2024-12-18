cmake_minimum_required(VERSION 3.16)

project(MegaLines VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Quick Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Sql)

set(PROJECT_SOURCES
  main.cpp

  bridge/formbridgebase.cpp
  bridge/formbridgemanager.cpp
  bridge/bridgemain.cpp
  bridge/bridgepagegame.cpp
  bridge/bridgepagetitle.cpp

  ctl/gamestoragecontroler.cpp
  ctl/playgroundfieldcontroler.cpp

  model/playgroundfieldmodel.cpp

  objects/entity.cpp
)

set(PROJECT_HEADERS
  bridge/formbridgebase.hpp
  bridge/formbridgemanager.hpp
  bridge/bridgemain.hpp
  bridge/bridgepagegame.hpp
  bridge/bridgepagetitle.hpp

  ctl/gamestoragecontroler.hpp
  ctl/playgroundfieldcontroler.hpp

  model/playgroundfieldmodel.hpp

  objects/entity.hpp

  util/position.hpp
)

set(PROJECT_ASSETS
  assets.qrc
  qml.qrc
)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Assets" FILES ${PROJECT_ASSETS})

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
  ${PROJECT_ASSETS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ./)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Sql
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
