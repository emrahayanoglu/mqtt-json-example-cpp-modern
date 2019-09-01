#include <iostream>
#include <spdlog/spdlog.h>
#include "MQTTHelper.h"

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Hello World");

    std::vector<std::string> subsList = {"#"};

    std::string topic = "hello/world";
    std::string payload = R"({"hello": "world"})";

    MQTTHelper mqttHelper;
    mqttHelper.connect(subsList);
    mqttHelper.publish(topic, payload, false);

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(5));

        mqttHelper.publish(topic, payload, false);
    }

    return 0;
}