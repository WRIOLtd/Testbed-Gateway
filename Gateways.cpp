#include "mqtt.h"
#include "tools-utils.h"
#include <pthread.h> 


#define CLIENT_ID "Client_ID"
#define BROKER_ADDRESS "127.0.0.1"	// put your mqtt broker IP Here
#define MQTT_PORT 1883;
#define MQTT_TOPIC "/iot/zolertia/cmd"

class mqtt_client *iot_client;


void *SensorPolling(void *vargp) 
{ 
	printf("Starting polling from sensors\n");
	char reply_command[500];
	char json_str[500];
	char topic_str[500];
	int ret=0;
	
	while (1){
		//pthread_mutex_lock( &mqtt_mutex );
		strcpy(json_str,"{");
		memset(reply_command,0,sizeof(char)*500);
		ret = SerialCommand("READSENVAL\n",reply_command);
		//strcat(json_str,"\"Temperature\":\"");
		if(strlen(reply_command)>0){
			strcat(json_str,reply_command);	
			strcpy(topic_str,"/iot/zolertia/data");		
		}
		else{
			strcpy(topic_str,"/iot/zolertia/error");
			if(ret == FAILEDONACCESS){
				strcat(json_str,"SERIALACCESSERROR");				
			}
			else{
				strcat(json_str,"SERIALREADWRITEERROR");			
			//strcat(json_str,"\"Temperature\": \"29285\",\"BatteryVal\": \"3290\",\"NodeId\": \"19625\",\"RssiVal\": \"-102\",\"TXLevel\": \"3\",\"LqiVal\": \"-87\",\"ChaVal\": \"26\",\"MinChaVal\": \"11\",\"MaxChaVal\": \"26\",\"PanIdVal\": \"-51\",\"RxModeVal\": \"3\",\"TxModeVal\": \"1\",\"BleBuffVal\": \"0\"");							
			}
		}
		//strcat(json_str,"\"");			
        
		//printf("temperature %s\n",reply_command);
		strcat(json_str,"}");
		printf("publishing message %s\n",json_str);
		iot_client->publishmsg(topic_str,json_str);				
        	stringlog("publishing message %s\n",0,json_str);
		/*double vm, rss;
		mem_usage(vm, rss);
		cout << "Virtual Memory: " << vm << " Resident set size: " << rss << endl;*/
		//pthread_mutex_unlock( &mqtt_mutex );
    	sleep(5); 
	}     
    return NULL; 
} 

int main(int argc, char *argv[])
{
    //class mqtt_client *iot_client;
    int rc;

    char client_id[15];// = { [41] = '\1' }; // make the last character non-zero so we can test based on it later //CLIENT_ID;
    srand(time(0));
    char host[] = BROKER_ADDRESS;
    int port = MQTT_PORT;

    InitLog();
    pthread_t thread_id;
    rand_str(client_id, sizeof client_id);
    printf("Client id: %s\n",client_id);

    mosqpp::lib_init();

    if (argc > 1)
        strcpy (host, argv[1]);

    iot_client = new mqtt_client(client_id, host, port);
    pthread_create(&thread_id, NULL, SensorPolling, NULL);
    while(1)
    {
        rc = iot_client->loop();
        if (rc)
        {
            iot_client->reconnect();
        }
        /*else
            iot_client->subscribe(NULL, MQTT_TOPIC);*/
    }

    mosqpp::lib_cleanup();

    return 0;
}

