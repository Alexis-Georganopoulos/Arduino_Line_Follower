# Arduino Line Follower


AIn this project, an An Arduino Leonardo was setup to be a line following robot. The chassis, sensors, and code were all designed/constructed/written from scratch. A simple binary controller allows the robot to use the underside IR sensors to track the black line. It can handle curves, straight lines, right-angle turns, small radius turns, and acute-angle turns. <br>
[Using the robot](#usage)

https://user-images.githubusercontent.com/106456040/236983636-2fea785a-858b-4c19-b2cb-65fcdd88ca31.mp4

## Source Code Overview
The source file [line_follower.ino](/line_follower/line_follower.ino) is composed of a typical `setup()`/`loop()` configuration found on all Arduino devices, written in C++.

Outside of these, we initialise some key global variables, and define the driver code:

- ir_current: An array to store the current IR sensor readings. It has two elements representing the left (`ir_current[0]`) and right (`ir_current[1]`) sensors.
- ir_max: An array to store the maximum IR sensor readings. This is set during the automated calibration and remains constant.
- button_value: A variable to hold the state of the **off**-board button
- go: A boolean variable that determines whether the robot is in **calibration** or **tracking** mode. It's controlled by the **on**-board button, hence the callback function.
- ir_cutoff: A percentage multiplier that controls the robots' sensitivity to ir_max.
- steady_state_speed: A constant representing the speed of the motors during the steady state when going in a straight line. Keep in mind that the motors are mounted in reverse, so this value is negative.

### Key functions

- Calibration: The code includes a calibration routine triggered by pressing the off-board button. The calibration() function performs a sweeping operation of the IR sensors to determine the maximum sensor readings. It adjusts the motor speeds during the sweeping process and stores the maximum IR sensor values. A cut-off threshold is then applied to the stored maximum values for calibration. The robot will roughly rotate back and fourth $180^o$. This makes sure it sweeps the brightest and darkest portions of the line. The whole calibration process is automated this way.

- irSweep: This performs the sweeping operation of the IR sensors and updates the maximum IR sensor readings.


### Initialization: 
The setup() function initializes the pin modes, serial communication, and assigns the button callback.

### Main Logic: 
The loop() function is responsible for the main program logic. It continuously checks the "go" variable to determine if the robot should be moving. If `go = true`, the code turns on the IR emitter and reads the IR sensor values. Based on the sensor readings, it adjusts the motor speeds to follow the line. <br>
Normally we would seperate the control system code into its own function, but its a simple binary controller so it's appropriate to leave it as-is.<br>
The value of `go` is controlled by the onboard button, hence the callback function.

If "go = false" the motors are set to zero speed, and the robot enters calibration mode. In this mode, we can press the off-board button to start the automated calibration process.



## Usage
Simply upload the code to your `Arduino Leonardo` and you're set.
Remember to download the [`prismino.h`](https://github.com/Robopoly/Robopoly_PRismino) library in the Arduino IDE.

For details on the used parts, visit the [Prisme kit](https://www.epfl.ch/campus/associations/list/robopoly/kit-prisme/) for the base parts, and the [Prisme extensions](https://www.epfl.ch/campus/associations/list/robopoly/kit-prisme-extension/) for other parts. Not, these webpages are in french.

The chassis was made arbitrarily, feel free to get creative, or derive from my work in the [images](images/) folder. <br>
The calibration of the robot is seen here:

