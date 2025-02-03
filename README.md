# Self Driving Line Following Car

## Overview
This project implements a self-driving car that follows a line, avoids obstacles, and can re-find the line after deviation. The car uses a combination of motor controllers, sensors, and multithreading to navigate its environment autonomously.

## Features
- Line Following: Uses front and rear line sensors to stay on track.
- Obstacle Avoidance: Detects obstacles and moves around them using ultrasonic sensors.
- Re-Finding the Line: After avoiding an obstacle, the car returns to the original track.
- Multithreaded Sensor Handling: Utilizes separate threads for sensor readings to improve responsiveness.
- Motor Control: Controls four motors for precise movement and turning.

## How It Works
1. The car waits for a button press to start.
2. It continuously reads sensor data:

    - If both front line sensors detect the line, it moves forward.
    - If one front sensor loses the line, it performs a soft turn.
    - If both front sensors lose the line, it executes a hard turn based on previous sensor data.

3. If an obstacle is detected:

    - The car shifts left to avoid it.
    - Moves forward until it clears the obstacle.
    - Shifts right back onto the line.

4. The car continues until manually stopped.
