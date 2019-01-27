This is a Bluemix project involving an MQTT client running on an Arduino-compatible ESP8266 chip. 

The MQTT client connects to Wifi and pushes telemetric data into the cloud at the frequency of 1 second. The client connects to an MQTT broker in the Bluemix cloud and publishes data to a Websocket server. The Websocket server sends the data to a web GUI running a Websocket client.

The Node.js cloud logic code for NODE-Red and the GUI will be added shortly.
