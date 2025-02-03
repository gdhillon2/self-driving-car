# Self Driving Line Following Car

## Overview
This project implements a self-driving car that follows a line, avoids obstacles, and can re-find the line after deviation. The car uses a combination of motor controllers, sensors, and multithreading to navigate its environment autonomously.

## Features
- Line Following: Uses front and rear line sensors to stay on track.
- Obstacle Avoidance: Detects obstacles and moves around them using ultrasonic sensors.
- Re-Finding the Line: After avoiding an obstacle, the car returns to the original track.
- Multithreaded Sensor Handling: Utilizes separate threads for sensor readings to improve responsiveness.
- Motor Control: Controls four motors for precise movement and turning.
