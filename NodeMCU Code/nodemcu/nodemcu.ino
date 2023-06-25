/* F5 TEST FOR ESP2PY
 * Written by Junicchi
 * https://github.com/KebabLord/esp_to_python
 * It simply increases and returns a variable everytime a python req came */

#include "ESP_MICRO.h" //Include the micro library 
int testvariable = 0;
void setup(){
  Serial.begin(9600); // Starting serial port for seeing details
  start("Ligma","RandomPassword");  // EnAIt will connect to your wifi with given details
  Serial.println(WiFi.localIP());
}
void loop(){
  waitUntilNewReq();  //Waits until a new request from python come
  /* increases index when a new request came*/
  testvariable += 1;
  
  returnThisInt(testvariable); //Returns the data to python
  
}