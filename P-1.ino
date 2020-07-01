
int dtOn_Chang
int dtOff_Chang
void setup() {
  // put your setup code here, to run once:


}

void loop() {
  for(unsigned int i=0 ; i<dtOn*600 ; i++){
    if(dtOn != dtOn_Chang){
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
        sprintf(payload, "%s", ""); // Cleans the payload
        sprintf(payload, "{\"%s\":", VARIABLE_LABEL_SUBSCRIBE_ON); // Adds the variable label
        int sensor = dtOn;
        Serial.print("Value of Sensor is:- ");
        Serial.println(sensor);
        /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
        dtostrf(sensor, 4, 0, str_sensor);
        sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
        Serial.println("Publishing data to Ubidots Cloud");
        Serial.println(payload);
        client.publish(topic, payload);
        dtOn_Chang=dtOn;
    }
  if(dtOff != dtOff_Chang){
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
        sprintf(payload, "%s", ""); // Cleans the payload
        sprintf(payload, "{\"%s\":", VARIABLE_LABEL_SUBSCRIBE_OFF); // Adds the variable label
        int sensor = dtOn;
        Serial.print("Value of Sensor is:- ");
        Serial.println(sensor);
        /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
        dtostrf(sensor, 4, 0, str_sensor);
        sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
        Serial.println("Publishing data to Ubidots Cloud");
        Serial.println(payload);
        client.publish(topic, payload);
        dtOff_Chang=dtOff;
    }
  }
  for(unsigned int i=0 ; i<dtOff*600 ; i++){
    if(dtOn != dtOn_Chang){
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
        sprintf(payload, "%s", ""); // Cleans the payload
        sprintf(payload, "{\"%s\":", VARIABLE_LABEL_SUBSCRIBE_ON); // Adds the variable label
        int sensor = dtOn;
        Serial.print("Value of Sensor is:- ");
        Serial.println(sensor);
        /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
        dtostrf(sensor, 4, 0, str_sensor);
        sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
        Serial.println("Publishing data to Ubidots Cloud");
        Serial.println(payload);
        client.publish(topic, payload);
        dtOn_Chang=dtOn;
    }
  if(dtOff != dtOff_Chang){
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
        sprintf(payload, "%s", ""); // Cleans the payload
        sprintf(payload, "{\"%s\":", VARIABLE_LABEL_SUBSCRIBE_OFF); // Adds the variable label
        int sensor = dtOn;
        Serial.print("Value of Sensor is:- ");
        Serial.println(sensor);
        /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
        dtostrf(sensor, 4, 0, str_sensor);
        sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
        Serial.println("Publishing data to Ubidots Cloud");
        Serial.println(payload);
        client.publish(topic, payload);
        dtOff_Chang=dtOff;
    }
  }

}