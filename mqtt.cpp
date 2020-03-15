#include "mqtt.h"
#define PUBLISH_TOPIC "EXAMPLE_TOPIC"

#ifdef DEBUG
#include <iostream>
#endif

#include "tools-utils.h"

mqtt_client::mqtt_client(const char *id, const char *host, int port) : mosquittopp(id)
{
    int keepalive = DEFAULT_KEEP_ALIVE;
    connect(host, port, keepalive);
}

mqtt_client::~mqtt_client()
{
}

void mqtt_client::on_connect(int rc)
{
    if (!rc)
    {
	subscribe(NULL, "/iot/zolertia/cmd");
        #ifdef DEBUG
            std::cout << "Connected - code " << rc << std::endl;
        #endif
    }
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    #ifdef DEBUG
        std::cout << "Subscription succeeded." << std::endl;
    #endif
}

void mqtt_client::publishmsg(const char* topic,const char* messagedata)
{
    publish(NULL, topic, strlen(messagedata), messagedata);
    #ifdef DEBUG
        std::cout << "publish succeeded." << std::endl;
    #endif
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
    int payload_size = MAX_PAYLOAD + 1;
    char buf[payload_size];

    if(!strcmp(message->topic, "/iot/zolertia/cmd"))
    {
        memset(buf, 0, payload_size * sizeof(char));

        /* Copy N-1 bytes to ensure always 0 terminated. */
        memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));

        #ifdef DEBUG
            std::cout << buf << std::endl;
        #endif

        /*// Examples of messages for M2M communications...
        if (!strcmp(buf, "STATUS"))
        {
            snprintf(buf, payload_size, "This is a Status Message...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Status Request Recieved." << std::endl;
            #endif
        }

        if (!strcmp(buf, "ON"))
        {
            snprintf(buf, payload_size, "Turning on...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn on." << std::endl;
            #endif
        }

        if (!strcmp(buf, "OFF"))
        {
            snprintf(buf, payload_size, "Turning off...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn off." << std:: endl;
            #endif
        }*/

	//MQTT::Message &message = md.message;
	char reply_command[500];
	char json_str[500];
	char topic_str[500];
	int ret=0;
	//pthread_mutex_lock( &mqtt_mutex );
	//printf("Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n", 
	//		++arrivedcount, message.qos, message.retained, message.dup, message.id);
	//printf("Payload %.*s\n", (int)message.payloadlen, (char*)message.payload);

	//stringlog("Message arrived: Payload %.*s\n", 0,(int)message.payloadlen, (char*)message.payload);

	if(strcmp("on",(char *)buf)==0){
		system("sudo /share/yepkit-USB-hub/ykushcmd -u 1 &");		
		sleep(3);
		publishmsg("/iot/zolertia/reply","{\"Node_id\": \"0\",\"Command\": \"on\",\"Value\": \"on\",\"Result\": \"Success\"}");
	}
	if(strcmp("off",(char *)buf)==0){
		system("sudo /share/yepkit-USB-hub/ykushcmd -d 1 &");
		sleep(3);
		publishmsg("/iot/zolertia/reply","{\"Node_id\": \"0\",\"Command\": \"off\",\"Value\": \"off\",\"Result\": \"Success\"}");
	}    
	if(strncmp("SETPANID=",(char *)buf,strlen("SETPANID="))==0 || strncmp("SETCH=",(char *)buf,strlen("SETCH="))==0 || strncmp("SETTX=",(char *)buf,strlen("SETTX="))==0){
		strcpy(json_str,"{");
		memset(reply_command,0,sizeof(char)*500);
		ret=SerialCommand((char *)buf,reply_command);
		//strcat(json_str,"\"Temperature\":\"");
		if(strlen(reply_command)>0){
			strcat(json_str,reply_command);	
			strcpy(topic_str,"/iot/zolertia/reply");		
		}
		else{
			strcpy(topic_str,"/iot/zolertia/error");
			if(ret == FAILEDONACCESS){
				strcat(json_str,"SERIALACCESSERROR");
			}
			else
				strcat(json_str,"SERIALREADWRITEERROR");						
		}
		//strcat(json_str,"\"");			
        
		//printf("temperature %s\n",reply_command);
		strcat(json_str,"}");
		printf("publishing message %s\n",json_str);		
		publishmsg(topic_str,json_str);		
	}
	//pthread_mutex_unlock( &mqtt_mutex );
    }
}
