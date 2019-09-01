cmake_minimum_required(VERSION 3.14)
project(mqtt_helper)

find_package(PahoMqttCpp)
find_package(spdlog)
find_package(nlohmann_json)

set(CMAKE_CXX_STANDARD 17)

add_executable(mqtt_helper main.cpp src/MQTTHelper.cc src/JSONHelper.cc)
target_include_directories(mqtt_helper PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE PahoMqttCpp::paho-mqttpp3)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)