cmake_minimum_required(VERSION 3.16)
project(HospitalManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Sql Core Gui Charts REQUIRED)

add_executable(HospitalManagementSystem
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/ui/MainWindow.cpp
    src/ui/Dashboard.cpp
    src/ui/PatientManager.cpp
    src/ui/RoomManager.cpp
    src/ui/DoctorManager.cpp
    src/ui/AdmissionManager.cpp
    src/ui/SettingsManager.cpp
)

target_link_libraries(HospitalManagementSystem PRIVATE Qt6::Widgets Qt6::Sql Qt6::Core Qt6::Gui Qt6::Charts)

file(COPY src/styles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
