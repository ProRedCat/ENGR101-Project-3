#include "robot.hpp"
#include <iostream>

using namespace std;

struct colorVars{
	double totalXWhite 		= 0.0;
	int whitePixelCount 	= 0;
	int rightWhiteCount		= 0;
	int leftWhiteCount 		= 0;
	void clear(){
		totalXWhite 		= 0.0;
		whitePixelCount 	= 0;
		rightWhiteCount		= 0;
		leftWhiteCount 		= 0;
	}
}camVARS;

enum loopType{
	//Determines which
	TOTAL,
	RIGHT,
	LEFT
};

bool mazeMode = false; //Boolean for the state of the robot (default is maze navigation is off, only turned true when no white pixels can be found and there are red pixels in the screen)

void camLoop(loopType LT, bool &Right, bool &Left, bool &TurningRight, bool &TurningLeft){
	//This function could be generalized but because
	//of the x cases being all unique, there is no point.
	switch(LT){
	case TOTAL:
		for (int y = cameraView.height - 20; y < cameraView.height; y++) { //Loop through the y pixels from the height - 20 to height
			for (int x = cameraView.width/3; x < cameraView.width - (cameraView.width - ((cameraView.width/3) * 2)); x++) { //Loop through the x pixels between 1/3 camera width and 2/3 camera width
				if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					camVARS.totalXWhite += x; //Add x to totalXWhite
					camVARS.whitePixelCount++; //Increase the whitePixelCount by 1
				}
			}
		}
		break;
	case RIGHT:
		for (int y = cameraView.height - 10; y < cameraView.height; y++) { //Loop through the y pixels from the height - 10 to height
			for (int x = cameraView.width - (cameraView.width/3); x < cameraView.width; x++) { //Loop through the x pixels between 2/3 camera width and camera width
				if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					camVARS.rightWhiteCount++;
				}
			}
		}
		break;
	case LEFT:
		for (int y = cameraView.height - 10; y < cameraView.height; y++) { //Loop through the y pixels from the height - 10 to height
			for (int x = 0; x < cameraView.width/3; x++) { //Loop through the x pixels between 0 camera width and 1/3 camera width
				if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					camVARS.leftWhiteCount++;
				}
			}
		}
		break;
	}

	//Decide on left or right turn.
	if(camVARS.rightWhiteCount > 0){
		Right = true;
	}else{
		TurningRight = false;
		Right = false;
	}
	if(camVARS.leftWhiteCount > 0){
		Left = true;
	}else{
		TurningLeft = false;
		Left = false;
	}
}

/*
 * Function for checking if there is black from the middle down of the camera view and between 1/4 and 3/4 width (detecting flag)
 */
bool checkBlack(){
	bool hasBlack = false;
	for (int y = cameraView.height/2.0; y < cameraView.height; y++) {  //Loop from middle to bottom
		for (int x = cameraView.width/4.0; x < (cameraView.width/4.0) * 3; x++) { //Loop between 1/4 and 3/4
			if(get_pixel(cameraView, y, x, 3) == 0){ //Check if the pixel is white
				hasBlack = true;
			}
		}
	}
	
	return hasBlack;
}

/*
 * Function for checking if there is red in the camera view
 */
bool redCheck(){
	bool hasRed = false;
	for (int y = 0; y < cameraView.height; y++) { 
		for (int x = 0; x < cameraView.width; x++) { 
			if(get_pixel(cameraView, y, x, 0) == 255 && get_pixel(cameraView, y, x, 1) == 0 && get_pixel(cameraView, y, x, 2) == 0){ //Check if the pixel is white
				hasRed = true;
			}
		}
	}
	
	return hasRed;
}


/*
 * Function to check if there is a maze wall to the left
 */
bool leftWallCheck(){
	bool hasRed = false;
	for (int y = cameraView.height/2.0 + 10; y < cameraView.height; y++) { //Loop thorugh the height of the camera
		for (int x = 0; x < 15; x++) { //Loop through the pixels from 0 to the width of the line
			if(get_pixel(cameraView, y, x, 0) == 255 && get_pixel(cameraView, y, x, 1) == 0 && get_pixel(cameraView, y, x, 2) == 0){ //Check if pixel is red
				hasRed = true;
			}
		}
	}
	
	return hasRed; //return if a line has been found
}

/*
 * Function to check if the maze wall has been shifted to the right
 */
bool shiftedLeftWallCheck(){
	bool hasRed = false;
	for (int y = cameraView.height/2.0 + 10; y < cameraView.height; y++) { //Loop thorugh the height of the camera
		for (int x = 15; x < 30; x++) { //Loop through the pixels from 0 to the width of the line
			if(get_pixel(cameraView, y, x, 0) == 255 && get_pixel(cameraView, y, x, 1) == 0 && get_pixel(cameraView, y, x, 2) == 0){ //Check if pixel is red
				hasRed = true;
			}
		}
	}
	
	return hasRed; //return if a line has been found
}

/*
 * Function to check if there is a maze wall infront of the robot
 */
bool frontWallCheck(){
	bool hasRed = false;
	for (int y = cameraView.height/2.0; y < cameraView.height/2.0 + 10; y++) { //Loop from half the height to half the height + 10 pixels
		for (int x = cameraView.width/4.0; x < (cameraView.width/4.0) * 3; x++) { //Loop through the pixels from 1/4 to 3/4
			if(get_pixel(cameraView, y, x, 0) == 255 && get_pixel(cameraView, y, x, 1) == 0 && get_pixel(cameraView, y, x, 2) == 0){ //Check if pixel is red
				hasRed = true;
			}
		}
	}
	
	return hasRed; //return if a line has been found
}

void senseDirection(double &XPOS, double &Right, double &Left){
	if ((XPOS > cameraView.width/2 && (XPOS - 5 < cameraView.width/2)) || (XPOS < cameraView.width/2 && (XPOS + 5 > cameraView.width/2))) { //Check if the robot is pointing generally at the line
		Left 	= 40;
		Right 	= 40;
	} else if (XPOS < cameraView.width/2) { //If the robot if pointing right of the line turn left
		Left 	= 0;
		Right 	= 15;
	} else if (XPOS > cameraView.width/2) { //If the robot is pointing left of the line turn right
		Left 	= 15;
		Right 	= 0;
	} else { //If the robot cannot find a line check if it has entered a maze or if it needs to turn around
		if(mazeMode){ //Check if the robot has entered the maze
			if(!leftWallCheck()){ //Check if there is not a wall to the left, if so turn left
				Left = 20;
				Right = 28;
			}
			else if(frontWallCheck()){ //Check if there is a wall infront of the robot, if so do a 90 degree turn
				Left = 40;
				Right = -40;
			}
			else if(shiftedLeftWallCheck()){ //Check if the wall is shifted over, if so turn right to correct
				Left = 28;
				Right = 20;
			}
			else{ //Else go forwards
				Left = 40;
				Right = 40;
			}
		}
		else if(redCheck()){ //If there are no white lines check if there is red to follow
			mazeMode = true;
		}
		else{ //Else there is no red to follow so turn around to aquire the line again
			Left 	= -50;
			Right 	= 50;	
		}
	}
}

//Entry point.
int main() {
    if (initClientRobot() != 0) { //Check if the robot was not initialised
        std::cout << " Error initializing robot" << std::endl;
    }

    //Velocities for the robot wheels
    double vLeft = 0;
    double vRight = 0;
    //Booleans for motor controls.
	bool turningRight = false;
	bool turningLeft = false;
    while (true) { //Loop continuously
       takePicture(); //Take a picture to update the robots view
       bool turnLeft = false;
       bool turnRight = false;
       //Takes total white value and black + white pixel count.
       camLoop(TOTAL, turnRight, turnLeft, turningRight, turningLeft);
       //Takes the white pixel count to the right.
       camLoop(RIGHT, turnRight, turnLeft, turningRight, turningLeft);
       //Takes the white pixel count to the left.
       camLoop(LEFT, turnRight, turnLeft, turningRight, turningLeft);

       double xPos = camVARS.totalXWhite / camVARS.whitePixelCount; //Calculate average x position of the line

        //blackPixelCount <= 10 is checking if we have hit the flag yet.
        if (!checkBlack()) {
        	//Detects whether it is facing away from the line, or towards it.
        	senseDirection(xPos, vRight, vLeft);
            
			if(turnRight == true && turnLeft == true){ //If the robot can turn both left and right turn left
				if(turningRight == false){
					turningLeft = true;
					vLeft 	= 0;
					vRight 	= 10;
				}
			}
			else{ //Else check if the robot should turn right or left
				if(turnRight && turningLeft == false){ //Robot pivots on right wheel
					int containsWhite = false;
					for (int y = cameraView.height - 20; y < cameraView.height - 10; y++) { //Loop through the y pixels from the height - 20 to height - 10
						for (int x = cameraView.width/3; x < cameraView.width - (cameraView.width - ((cameraView.width/3) * 2)); x++) { //Loop through the x pixels between 1/3 camera width and 2/3 camera width
							if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
								containsWhite = true; //If any white is detected, it is assumed that there is still a path infront of the robot
							}
						}
					}
					if(!containsWhite){ //Check if there is not a path in front of the robot, if so turn left
						turningRight = true;
						vLeft 	= 20;
						vRight 	= 0;
					}
				}
				if(turnLeft && turningRight == false){ //Robot pivots on left wheel 
					turningLeft = true;
					vLeft 	= 0;
					vRight 	= 10;
				}
			}			
        } else { //The robot has reached the flag
            vLeft 	= 0;
            vRight 	= 0;
        }
		
        setMotors(vLeft, vRight); //Set the velocity of the motors
        usleep(10000); //Sleep for a time as to not spam the server
        
        camVARS.clear();
    }
}


