# ENEE408I_Team4

Breakdown of directories:

ENEE408I_Team4/Avoidance_and_Serial_Navagation/ - This directory contains the necessary header files and code for the Arduino that listens for serial commands and avoids obstacles

ENEE408I_Team4/Basic_Navagation/ - This directory contains the necessary header files and code for the Arduino that navigates around the room avoiding obstacles, not listening for any commands

ENEE408I_Team4/alexa:jetson_coms/ - This directory contains the file FlaskAskDecideTask.py, meant to be run on the Jetson to listen for Alexa commands and send the corresponding serial command

ENEE408I_Team4/jetson:arduino_coms/serial_test.py - This directory contains the file serial_test.py, which was used to test sending serial commands from the jetson to the Arduino, but was not used in integration

ENEE408I_Team4/Debug_arduino_code - This directory contains a few short programs useful for debugging navagation code, including one that can be used to adjust motor speeds and one that we used to see our robot going in each direction - foward, back, left, and right.  It also contains alexa_nav_test, which we used as a simple program to test response to Alexa commands by having the robot navigate around the room in response to receiving the serial command 'n'.

ENEE408I_Team4/face-recognition-opencv/ - This directory contains the python scripts and datasets needed to run face recognition programs.  encode_faces.py goes over training data and recognize_faces_image.py, recognize_faces_video_file.py, and recognize_faces_video.py identifys faces based of trained data.
