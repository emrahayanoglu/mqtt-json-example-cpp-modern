//
// Created by Emrah Ayanoglu on 31.08.19.
//
#include "MQTTHelper.h"
#include <nlohmann/json.hpp>
#include <JSONHelper.h>

void MQTTHelper::connected(const std::string &cause) {
    std::cout << "MQTT is connected" << std::endl;
    std::cout << cause << std::endl;

    //Subscribe on topics on the list
    for (auto& it : subscriptionList) {
        std::cout << "Subscribed to: " << it << std::endl;
        cliPtr->subscribe(it, 1);
    }
}

void MQTTHelper::connection_lost(const std::string &cause) {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    reconnect();
}

void MQTTHelper::delivery_complete(mqtt::delivery_token_ptr tok) {

}

void MQTTHelper::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "Topic: " << msg->get_topic() << std::endl;
    std::cout << "Payload: " << msg->get_payload_str() << std::endl;
    nlohmann::json jsonPayload;
    if(!JSONHelper::fromPayload(msg->get_payload_str(), jsonPayload)){
        return;
    }
    std::cout << jsonPayload;
}

void MQTTHelper::on_failure(const mqtt::token &asyncActionToken) {

}

void MQTTHelper::on_success(const mqtt::token &asyncActionToken) {

}

bool MQTTHelper::publish(std::string &topic, std::string &payload, bool retain, int qos) {
    if(!isConnected())
        return false;

    try {
        cliPtr->publish(topic, payload, qos, retain);
        return true;
    }
    catch (const mqtt::exception& ex){
        std::cout << ex.get_message() << std::endl;
        return false;
    }
}

bool MQTTHelper::connect(std::vector<std::string> &subsList) {
    if(isConnected())
        return false;

    //Add all subscriptions
    for(auto& topic : subsList){
        if(subscriptionList.find(topic) == subscriptionList.end()){
            subscriptionList.insert(topic);
        }
    }

    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    cliPtr = std::make_shared<mqtt::async_client>("tcp://localhost:1883", "async_subcribe_cpp");
    cliPtr->set_callback(*this);

    try {
        cliPtr->connect(connOpts);
    }
    catch (const mqtt::exception &ex) {
        std::cout << ex.get_message() << std::endl;
        return false;
    }

    return true;
}

bool MQTTHelper::isConnected(){
    if(!cliPtr || !cliPtr->is_connected())
        return false;

    return true;
}

bool MQTTHelper::reconnect() {
    if(!isConnected())
        return false;

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        cliPtr->connect(connOpts);
        return true;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return false;
    }
}

bool MQTTHelper::disconnect() {
    if(!isConnected())
        return false;

    try {
        cliPtr->disconnect()->wait();
    }
    catch (const mqtt::exception &ex) {
        std::cout << ex.get_message() << std::endl;
        return false;
    }
    return true;
}



