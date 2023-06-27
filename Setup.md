# Installation
1. Install the wsServer library to ~.
2. Install the pthread library if it is not already provided:
```
sudo apt-get install libpthread-stubs0-dev
```

# Setup
1. Follow the wiring diagram to do the setup
2. Set the environment variable $SRC_ROOT
2. Install the wsServer library to ~.
4. Install wiringPi to standard headers
4. Setup ip address
5. `source` the set_environment_variables.sh script
7. Start the Python server in the web/static directory
8. make and then run ./server


Raspberry Pi:

1. Wait to receive GPIO LDR.
2. If LDR received, wait for sound [above THRESHOLD] for [MAX_TIME].
If sound not received, GOTO 1.
3. If sound received, send time delay to server.

Aims to minimize processing done on the Pi, so it could be substituted
for a smaller circuit.

-------------------
Design specifics:

Assumes Raspbian OS is installed.
Dependent on the OS for time.h, and WirePi library.

GPIO pins:
- Connect LDR to ...
- Connect Microphone to ...
-------------------


Later idea:

-> Account for time calling functions e.g. messing up the delay readings.
-> Stuff to monitor battery levels.