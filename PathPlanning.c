#pragma config(Sensor, in1,    leftLineFollower, sensorLineFollower)
#pragma config(Sensor, in2,    centerLineFollower, sensorLineFollower)
#pragma config(Sensor, in3,    rightLineFollower, sensorLineFollower)
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  compassSensor,  sensorVirtualCompass)
#pragma config(Sensor, dgtl6,  touchSensor,    sensorTouch)
#pragma config(Sensor, dgtl8,  sonarSensor,    sensorSONAR_cm)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop, reversed, encoderPort, dgtl1)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, encoderPort, dgtl3)

// The dimensions of the grid.
const int gridWidth = 10;
const int gridHeight = 10;

// The robot's starting position.
int startX = 0;
int startY = 0;

// The robot's current position.
int currentX = 0;
int currentY = 0;

// The position of the goal.
int goalX = 5;
int goalY = 5;

int distancePerSquare = 100;
int openCells = gridWidth * gridHeight;

// A cell is a coordinate, with an x and y value.
typedef struct {
	int x;
	int y;
} cell;

bool grid[gridHeight][gridWidth];	// 
bool open[gridHeight][gridWidth];	// 
cell path[gridWidth * gridHeight];	// The path the robot will take.
int pathIndex = 0;					// The number of cells in the robots path.

// Returns the distance between two points.
int genericDistance(int x1, int y1, int x2, int y2) 
{
	return abs(x2 - x1) + abs(y2 - y1);
}

// The path finding function.
bool bestFirst_route() 
{
	int distance = 1000;	// 
	int nodeX = -1;			// 
	int nodeY = -1;			// 

	// Loop while there are still open cells left to explore.
	while (openCells > 0) 
	{
		// The cell where the robot currently is is not open, because the robot is currently there.
		open[currentY][currentX] = false;

		// Checks to see if the robot is at the goal or not.
		if (currentX == goalX && currentY == goalY) 
		{
			return true;
		}

		// 
		distance = 1000;

		// 
		if (currentX - 1 >= 0 && open[currentX - 1][currentY]) 
		{
			nodeX = currentX - 1;
			nodeY = currentY;
			distance = genericDistance(currentX - 1, currentY, goalX, goalY);
		}

		// 
		if (currentY - 1 >= 0 && open[currentX][currentY - 1] && genericDistance(currentX, currentY - 1, goalX, goalY) < distance) 
		{
			nodeX = currentX;
			nodeY = currentY - 1;
			distance = genericDistance(currentX, currentY - 1, goalX, goalY);
		}

		// 
		if (currentX + 1 < gridHeight && open[currentX + 1][currentY] && genericDistance(currentX + 1, currentY, goalX, goalY) < distance) 
		{
			nodeX = currentX + 1;
			nodeY = currentY;
			distance = genericDistance(currentX + 1, currentY, goalX, goalY);
		}

		// 
		if (currentY + 1 < gridHeight && open[currentX][currentY + 1] && genericDistance(currentX, currentY + 1, goalX, goalY) < distance) 
		{
			nodeX = currentX;
			nodeY = currentY + 1;
			distance = genericDistance(currentX, currentY + 1, goalX, goalY);
		}

		// 
		if (nodeX == -1 || nodeY == -1) 
		{
			// Step backwards.
			pathIndex = pathIndex - 1;
			currentX = path[pathIndex - 1].x;
			currentY = path[pathIndex - 1].y;
			continue;
		}

		// 
		currentX = nodeX;
		currentY = nodeY;

		// Update the path
		path[pathIndex].x = currentX;
		path[pathIndex].y = currentY;

		// Since we have a new cell in the path, the amount of cells to travel has increased by 1.
		pathIndex = pathIndex + 1;
	}

	return false;
}

void motors_off() 
{
	// Turn the motors off.
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
}

void move_forward(int amount) 
{
	int speed = 50;

	nMotorEncoder[leftEncoder] = 0;
	nMotorEncoder[rightEncoder] = 0;

	// Move while we have not traveled over the specified amount.
	while ((nMotorEncoder[leftEncoder] > -amount) && (nMotorEncoder[rightEncoder] > -amount) && (nMotorEncoder[leftEncoder] < amount) && (nMotorEncoder[rightEncoder] < amount)) 
	{
		motor[rightMotor] = speed;	// Motor on port2 is run at half (63) power forward
		motor[leftMotor]  = speed;	// Motor on port3 is run at half (63) power forward
		wait1Msec(1);
	}

	// Turn off the motors.
	motors_off();
}

void turn_right(int ms, int power) 
{
	// Rotate the robot by having the wheels spin in opposite directions.
	motor[rightDrive] = power;	// Motor on port2 is run at half (63) power forward
	motor[leftDrive]  = -power;	// Motor on port3 is run at half (63) power forward
	wait1Msec(ms);
}

void turn_right(int amount) 
{
	nMotorEncoder[leftEncoder] = 0;
	nMotorEncoder[rightEncoder] = 0;

	// Turn right for a specified amount.
	while (nMotorEncoder[leftEncoder] > -amount && nMotorEncoder[rightEncoder] > -amount) 
	{
		turn_right(1, 50);
	}

	// Reverse power shortly.
	turn_right(15, motorSpeed);

	motors_off();
}

task main()
{
	// Initialize the grid.
	for (int y = 0; y < gridHeight; y++) 
	{
		for (int x = 0; x < gridWidth; x++) 
		{
			grid[y][x] = true;
		}
	}

	int facingDirection = 1;
	int cellDirection = 1;

	// 
	if (bestFirst_route()) 
	{
		// Follow path.
		currentX = startX;
		currentY = startY;

		for (int x = 0; x < pathIndex; x++) 
		{
			if (path[x].x > currentX) 
			{
				cellDirection = 1;
			}
			else if (path[x].y > currentY) 
			{
				cellDirection = 2;
			}
			else if (path[x].x < currentX) 
			{
				cellDirection = 3;
			}
			else if (path[x].y < currentY) 
			{
				cellDirection = 4;
			}

			// If the robot is not facing the correct direction, rotate to face the correct direction.
			while (facingDirection != cellDirection) 
			{
				// Turn to face cell.
				turnRight(100);
				facingDirection++;
			}

			move_forward(100);
		}
	}
}												        // Program ends, and the robot stops
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
