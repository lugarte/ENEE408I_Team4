# ENEE408I_Team4

Breakdown of directories:

ENEE408I_Team4/Avoidance_and_Serial_Navagation/ - This directory contains the necessary header files and code for the Arduino that listens for serial commands and avoids obstacles

ENEE408I_Team4/Basic_Navagation/ - This directory contains the necessary header files and code for the Arduino that navigates around the room avoiding obstacles, not listening for any commands

ENEE408I_Team4/alexa:jetson_coms/ - This directory contains the file FlaskAskDecideTask.py, meant to be run on the Jetson to listen for Alexa commands and send the corresponding serial command

ENEE408I_Team4/jetson:arduino_coms/serial_test.py - This directory contains the file serial_test.py, which was used to test sending serial commands from the jetson to the Arduino, but was not used in integration
