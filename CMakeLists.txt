# This is a build file for Huggle (used with cmake)
# You will need to update it by hand!
cmake_minimum_required (VERSION 2.8.7)
PROJECT(huggle_thanks)
#  cmake <folder> -DQT5_BUILD=true
option(QT5_BUILD "Build huggle using the Qt5 framework" FALSE)
SET(CMAKE_CXX_FLAGS "-std=c++11")
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
include_directories(../../)
#### Qt 4 and 5 ####
if(QT5_BUILD)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5Xml REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)
    set(QT_INCLUDES
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5Xml_INCLUDE_DIRS}
        ${Qt5Svg_INCLUDE_DIRS}
    )
    include_directories(${QT_INCLUDES})
else()
    find_package(Qt4 REQUIRED)
endif()

file(GLOB thanks_srcx
    "src/*.cpp"
)
file (GLOB thanks_resx
    "src/*.qrc"
)
file (GLOB thanks_uixx
    "src/*.ui"
)

SET(thanks_SOURCES ${thanks_srcx})
SET(thanks_FORMS ${thanks_uixx})
SET(thanks_RESOURCES ${thanks_resx})
if(QT5_BUILD)
    QT5_WRAP_CPP(thanks_HEADERS_MOC ${thanks_HEADERS})
    QT5_WRAP_UI(thanks_FORMS_HEADERS ${thanks_FORMS})
    QT5_ADD_RESOURCES(thanks_RESOURCES_RCC ${thanks_RESOURCES})
else()
    QT4_WRAP_CPP(thanks_HEADERS_MOC ${thanks_HEADERS})
    QT4_WRAP_UI(thanks_FORMS_HEADERS ${thanks_FORMS})
    QT4_ADD_RESOURCES(thanks_RESOURCES_RCC ${thanks_RESOURCES})
endif()
if (NOT QT5_BUILD)
  INCLUDE(${QT_USE_FILE})
endif()
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS( -DQT_USE_QSTRINGBUILDER )
ADD_LIBRARY(huggle_thanks SHARED ${thanks_SOURCES} ${thanks_HEADERS_MOC} ${thanks_FORMS_HEADERS} ${thanks_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(huggle_thanks ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(huggle_thanks huggle_core huggle_ui)
if (QT5_BUILD)
    qt5_use_modules(huggle_thanks Core Gui Widgets Xml)
endif()
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
if (LINUX_SNAP)
    INSTALL(TARGETS huggle_thanks DESTINATION bin/extensions)
else()
    INSTALL(TARGETS huggle_thanks DESTINATION share/huggle/extensions)
endif()

