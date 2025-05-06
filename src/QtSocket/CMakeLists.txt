cmake_minimum_required(VERSION 3.16)

project(QtSocket VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appQtSocket
    main.cpp
)

qt_add_qml_module(appQtSocket
    URI QtSocket
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES SocketClient.qml
        QML_FILES MainMenu.qml
        SOURCES SocketClientController.h SocketClientController.cpp
        QML_FILES SocketServer.qml
        SOURCES SocketServerController.h SocketServerController.cpp
        SOURCES
        SOURCES ServerResponseFile.h ServerResponseFile.cpp
        SOURCES ServerResponseData.h ServerResponseData.cpp
        SOURCES ServerResponseProvider.h ServerResponseProvider.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQtSocket PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQtSocket
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtSocket
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appQtSocket
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
