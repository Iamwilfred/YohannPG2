cmake_minimum_required(VERSION 3.16)

# ------------------------------------------------------------------
#  Projet
# ------------------------------------------------------------------
project(yohann1Pg2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
#  Qt 5  (passe à Qt6 si besoin)
# ------------------------------------------------------------------
find_package(Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)

# ------------------------------------------------------------------
#  JSON for Modern C++
# ------------------------------------------------------------------
find_package(nlohmann_json 3.2.0 REQUIRED)

# ------------------------------------------------------------------
#  AUTOMOC / AUTOUIC / AUTORCC
# ------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ------------------------------------------------------------------
#  Sources et en-têtes
# ------------------------------------------------------------------
set(Sources
    main.cpp

    travelagency.cpp
    booking.cpp
    rentalcarreservation.cpp
    hotelbooking.cpp
    flightbooking.cpp
    trainticket.cpp
    travelagencyui.cpp
    travel.cpp
    customer.cpp
    airport.cpp

    icons.qrc               # ressources Qt
)

set(Headers
    travelagency.h
    booking.h
    rentalcarreservation.h
    hotelbooking.h
    flightbooking.h
    trainticket.h
    travelagencyui.h
    travel.h
    customer.h
    airport.h
)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})

# ------------------------------------------------------------------
#  Include path local (facultatif si tout est en racine)
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------------
#  Link
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::WebEngineWidgets
    nlohmann_json::nlohmann_json
)

# ------------------------------------------------------------------
#  Installation (optionnel)
# ------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
