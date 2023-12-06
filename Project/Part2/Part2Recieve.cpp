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
#define CLIENTID   "Beagle1"
#define TOPIC      "TFortune/feeds/weather.cross"
#define AUTHMETHOD "TFortune"
#define AUTHTOKEN  "aio_zGNc943dtLcpW9cXfCKmhcXQVpyJ"
#define QOS        1
#define TIMEOUT    10000L
#define GPIO_PIN   46 
#define GPIO_PATH "/sys/class/gpio/"

//const std::string  gpioPath = "/sys/class/gpio/gpio46";
//const std::string  gpioPathValue = "/sys/class/gpio/gpio46/value";
/*
void exportPin(int pin) {
    std::ofstream exportFile(gpioPath + "export");
    exportFile << pin;
    exportFile.close();
}

void setDirection(int pin, const std::string& direction) {
    std::ofstream directionFile(gpioPath + "gpio" + std::to_string(pin) + "/direction");
    directionFile << direction;
    directionFile.close();
}

int readValue(int pin) {
    std::ifstream valueFile(gpioPathValue + "gpio" + std::to_string(pin) + "/value");
    int value;
    valueFile >> value;
    valueFile.close();
    return value;
}

void writeValue(int pin, int value) {
    std::ofstream valueFile(gpioPathValue + "gpio" + std::to_string(pin) + "/value");
    valueFile << value;
    valueFile.close();
}

*/
int main(int argc, char* argv[]) {
	GPIO inGPIO(46);
	inGPIO.setDirection(INPUT);
    //exportPin(GPIO_PIN);
    //setDirection(GPIO_PIN, "in");

	 inGPIO.getValue();

    
	printf("\n Value of pin = %d", inGPIO.getValue() );
	printf("\n Status of pin = %d\n", inGPIO.getDirection() );
   
   
   char str_payload[100];          // Set your max message size here
   MQTTClient client;
   cout << "Starting Beagle board MQTT Adafruit Publish Example" << endl;
   MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
   MQTTClient_message pubmsg = MQTTClient_message_initializer;
   MQTTClient_deliveryToken token;
   MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
   opts.keepAliveInterval = 20;
   opts.cleansession = 1;
   opts.username = AUTHMETHOD;
   opts.password = AUTHTOKEN;
   int rc;
   if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
      cout << "Failed to connect, return code " << rc << endl;
      return -1;
   }
   sprintf(str_payload, "%d", inGPIO.getValue());   // <--------- this is where value gets sent
   pubmsg.payload = str_payload;
   pubmsg.payloadlen = strlen(str_payload);
   pubmsg.qos = QOS;
   pubmsg.retained = 0;
   MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
   cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
        " seconds for publication of " << str_payload <<
        " \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
   rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
   if(rc == MQTTCLIENT_SUCCESS) {
     cout << "Message with token " << (int)token << " delivered." << endl;
   }
   else {
     cout << "Did not complete with error code: " << rc << endl;
     // MQTTCLIENT_SUCCESS 0           MQTTCLIENT_FAILURE -1
     // MQTTCLIENT_DISCONNECTED -3     MQTTCLIENT_MAX_MESSAGES_INFLIGHT -4
     // MQTTCLIENT_BAD_UTF8_STRING -5  MQTTCLIENT_NULL_PARAMETER -6
     // MQTTCLIENT_TOPICNAME_TRUNCATED -7   MQTTCLIENT_BAD_STRUCTURE -8
     // MQTTCLIENT_BAD_QOS   -9        MQTTCLIENT_SSL_NOT_SUPPORTED   -10
   }
   MQTTClient_disconnect(client, 10000);
   MQTTClient_destroy(&client);
   return rc;
}