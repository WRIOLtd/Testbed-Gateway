#ifndef SIMPLECLIENT_MQTT_H
#define SIMPLECLIENT_MQTT_H

#include <mosquittopp.h>
#include <cstring>
#include <cstdio>

#define MAX_PAYLOAD 500
#define DEFAULT_KEEP_ALIVE 60
#define DEBUG

class mqtt_client : public mosqpp::mosquittopp
{
public:
    mqtt_client (const char *id, const char *host, int port);
    ~mqtt_client();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void publishmsg(const char* topic,const char* messagedata);
};

#endif //SIMPLECLIENT_MQTT_H
