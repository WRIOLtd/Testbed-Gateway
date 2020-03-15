# Testbed-Gateway

Talk to Mqtt Broker and sensor on /dev/ttyUSB0, Act As middle-ware between Gateway Server and Sensors

Third Party libs used:

- Mqtt Client has been implemented on mosquitto lib, To successfully compile this gateway
	- First libs using command(sudo apt-get install libmosquitto-dev libmosquittopp-dev libssl-dev)
	- Issue Command ("make") in this directory