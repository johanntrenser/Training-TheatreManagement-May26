cmake_minimum_required(VERSION 3.16)

project(Trenser_QuickBookUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required for CMAKE_INSTALL_BINDIR and CMAKE_INSTALL_LIBDIR variables
include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.10)

# Backend paths
set(BACKEND_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../Trenser.QuickBook")
set(BACKEND_SRC "${BACKEND_ROOT}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Backend as an imported target
add_library(Backend STATIC IMPORTED)
set_target_properties(Backend PROPERTIES
    IMPORTED_CONFIGURATIONS "Debug;Release"
    IMPORTED_LOCATION_DEBUG   "${BACKEND_ROOT}/x64/Debug/Trenser.QuickBook.lib"
    IMPORTED_LOCATION_RELEASE "${BACKEND_ROOT}/x64/Release/Trenser.QuickBook.lib"
    INTERFACE_INCLUDE_DIRECTORIES
        "${BACKEND_SRC}/Controller;${BACKEND_SRC}/Services;${BACKEND_SRC}/Models;${BACKEND_SRC}/DataStore;${BACKEND_SRC}/SharedMemory;${BACKEND_SRC}/Utilities;${BACKEND_SRC}/Factories"
)

qt_add_executable(appTrenser_QuickBookUI
    main.cpp
)

qt_add_qml_module(appTrenser_QuickBookUI
    URI Trenser_QuickBookUI
    VERSION 1.0
    QML_FILES
        Main.qml
        views/Login.qml
        # Add future QML views here, e.g.:
        # views/Login.qml
    SOURCES
        adapter/ControllerAdapter.h
        adapter/ControllerAdapter.cpp
        adapter/EnumsAdapter.h
        QML_FILES views/MainMenu.qml
        QML_FILES views/Register.qml
        QML_FILES views/Dashboard.qml
        QML_FILES views/CustomButton.qml views/UserManagement.qml views/UserProfile.qml
        QML_FILES views/ChangePassword.qml
        QML_FILES views/TheatreManagement.qml
        QML_FILES views/NotificationsView.qml
        QML_FILES views/MovieManagement.qml
        QML_FILES views/CustomButton.qml
        QML_FILES views/SystemLogs.qml
        QML_FILES views/BookingManagement.qml
        QML_FILES views/CustomerBookingsView.qml
        QML_FILES views/CustomerBookingsView.qml
        QML_FILES views/ShowManagement.qml
        QML_FILES views/CustomerBrowseAndBook.qml
        QML_FILES views/CustomerTicketsView.qml
        QML_FILES views/TicketManagement.qml
        QML_FILES views/CustomerBrowseAndBook.qml

        # Add future C++ models here, e.g.:
        # models/MovieListModel.h
        # models/MovieListModel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTrenser_QuickBookUI PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTrenser_QuickBookUI
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appTrenser_QuickBookUI PRIVATE
    adapter
)

target_link_libraries(appTrenser_QuickBookUI
    PRIVATE Qt6::Quick
    Backend
    Qt6::QuickControls2
)

install(TARGETS appTrenser_QuickBookUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
