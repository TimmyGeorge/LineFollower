This project contains the source code for a line following robot designed for the autonomous transportation of payloads.

This robot was built using the following components:
- 3 QTR-8RC infrared sensors
- An Arduino
- An ESP32
- 2 TB6612FNG motor drivers
- 2 Motors with encoders (for speed measurement)
- 1 Ultrasonic sensor

Other than the interfaces provided by the Arduino IDE, no external libraries were used to highlight my knowledge of using synchronous I/O & interrupts to interface with actuators/sensors.

The robot uses PID control to ensure smooth navigation and quadratic interpolation for line position estimation.

See the YouTube video [here](https://youtu.be/zLgiPtKdl_k?si=FkQfM7-O2s-zYjav) for the demonstration of this line follower
