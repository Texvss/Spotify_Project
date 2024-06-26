cmake_minimum_required(VERSION 3.5)

project(project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Sql)
# find_package(CURL REQUIRED)
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Sql REQUIRED)
# # find_package(Qt${QT_VERSION_MAJOR}  REQUIRED COMPONENTS Core Gui Sql REQUIRED)
# find_package(Qt${QT_VERSION_MAJOR}  REQUIRED COMPONENTS Sql REQUIRED)


set(PROJECT_SOURCES
        main.cpp

        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        spotify.h
        spotify.cpp
        mainwindow.cpp
        trackview.h trackview.cpp trackview.ui
        lyrics.h lyrics.cpp lyrics.ui
        login.h login.cpp login.ui
        src.qrc
        liked.h liked.cpp liked.ui


















    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(project
            ${PROJECT_SOURCES}
        )
    endif()
endif()




# target_link_libraries(project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# target_link_libraries(project PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
# target_include_directories(project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Sql)

target_include_directories(project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.project)
endif()
set_target_properties(project PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(project)
endif()
