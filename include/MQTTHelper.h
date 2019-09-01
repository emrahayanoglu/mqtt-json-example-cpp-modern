//
// Created by Emrah Ayanoglu on 31.08.19.
//

#ifndef MQTT_HELPER_MQTTHELPER_H
#define MQTT_HELPER_MQTTHELPER_H

#include <mqtt/callback.h>
#include <mqtt/async_client.h>

#include <set>

class MQTTHelper: public virtual mqtt::callback, public virtual mqtt::iaction_listener{
public:
    MQTTHelper() = default;
    ~MQTTHelper() = default;

    // callback virtual functions
    void connected(const std::string& cause) override ;
    void connection_lost(const std::string& cause) override ;
    void message_arrived(mqtt::const_message_ptr msg) override ;
    void delivery_complete(mqtt::delivery_token_ptr tok) override ;


    //iaction_listener virtual functions
    void on_failure(const mqtt::token& asyncActionToken) override ;
    void on_success(const mqtt::token& asyncActionToken) override ;

    //connection functions
    bool isConnected();
    bool connect(std::vector<std::string>& subsList);
    bool reconnect();
    bool disconnect();

    //publish message
    bool publish(std::string& topic, std::string& payload, bool retain=false, int qos=1);

private:
    std::shared_ptr<mqtt::async_client> cliPtr;
    mqtt::connect_options connOpts;
    std::set<std::string> subscriptionList;
};

#endif //MQTT_HELPER_MQTTHELPER_H
