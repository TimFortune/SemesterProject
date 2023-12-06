#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
using namespace std;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "Beagle2"
#define TOPIC       "TFortune/feeds/weather.temperature"
#define AUTHMETHOD  "TFortune"
#define AUTHTOKEN   ""
#define QOS         1
#define TIMEOUT     10000L
//#define LED_GPIO    "/sys/class/gpio/gpio60"
#define LED_USR    "/sys/class/leds/beaglebone:green:usr0"
#define THRESHOLD   73

/*    testing with external led first
// Use this function to control the LED 
void writeGPIO(string filename, string value){
   fstream fs;
   string path(LED_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}
*/

void writeLED(string filename, string value){
   fstream fs;
   string path(LED_USR);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

void removeTrigger(){
	writeLED("/trigger", "none");
}

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);

    if (message->payloadlen == 0) {
        printf("Empty payload\n");
        return 0;
   }

    printf("   message: ");
    payloadptr = (char*) message->payload;
    float temperature = atof(payloadptr);
    printf("The temperature is %f\n", temperature);

  if (temperature < THRESHOLD)	{ 
	removeTrigger();
	writeLED("/brightness", "1");
}	else if(temperature > THRESHOLD){
	removeTrigger();
	writeLED("/brightness", "0");
}	else	{
	   	cout << "Invalid command!" << endl;
}    

//temperature > THRESHOLD ? writeGPIO("value", "1") : writeGPIO("value", "0");		    
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    writeLED("direction", "out");

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    
    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

