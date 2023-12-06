// Transmission code

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "GPIO.h"
#include "GPIO.cpp"
using namespace std;
using namespace exploringBB;



#define ADDRESS    "tcp://io.adafruit.com"
#define CLIENTID   "Beagle2"
#define TOPIC      "TFortune/feeds/weather.cross"
#define AUTHMETHOD "TFortune"
#define AUTHTOKEN  "aio_zGNc943dtLcpW9cXfCKmhcXQVpyJ"
#define QOS        1
#define TIMEOUT    10000L
#define GPIO_PIN   60 
#define LED_GPIO  "/sys/class/gpio/gpio60"
#define GPIO_PATH "/sys/class/gpio/"

//const std::string  gpioPath = "/sys/class/gpio/gpio46";
//const std::string  gpioPathValue = "/sys/class/gpio/gpio46/value";

void writeGPIO(string filename, string value){
   fstream fs;
   string path(LED_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
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
    float cross = atof(payloadptr);
    printf("The Appliances are %f\n", cross);

  if (cross > 0){ 
	printf("Led ON \n");
        system("echo 1 > /sys/class/gpio/gpio60/value");
	
	//writeGPIO("value", "1");

}	else {
	printf("Led OFF \n");
	
	system("echo 0 > /sys/class/gpio/gpio60/value");

	//writeGPIO("value", "0");
}    
	    
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
    

    writeGPIO("direction", "out");

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

