# MotionGloveInterface

Work in progress! Not usable yet.

This is a Graphical UserInterface for creating Processing Graphs to process motion sensor data and osc data in general.

It is written with Qt.

## Something

To show an input device you must send a specific osc-message to the application:
port: 55600
oscaddress: /glove/ping [name] [sender-ip] [senderlisten-port]

e.g. (with liblo installed, from terminal)
oscsend localhost 55600 /glove/ping ssi fl 127.0.0.1 51100

