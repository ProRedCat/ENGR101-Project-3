#include "robot.hpp"
#include <iostream>

using namespace std;

struct colorVars{
	double totalXWhite 		= 0.0;
	int whitePixelCount 		= 0;
	int blackPixelCount 		= 0;
	int rightWhiteCount		= 0;
	int leftWhiteCount 		= 0;
	void clear(){
		totalXWhite 		= 0.0;
		whitePixelCount 	= 0;
		blackPixelCount 	= 0;
		rightWhiteCount		= 0;
		leftWhiteCount 		= 0;
	}
}camVARS;

enum loopType{
	//Determines which part of the view to read
	TOTAL,
	RIGHT,
	LEFT
};

void camLoop(loopType LT, bool &Right, bool &Left){
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
				else if (get_pixel(cameraView, x, y, 3) < 5) { //Check if pixel is black
					camVARS.blackPixelCount++; //Increase blackPixelCount
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
		Right = false;
	}
	if(camVARS.leftWhiteCount > 0){
		Left = true;
	}else{
		Left = false;
	}
}

void senseDirection(double &XPOS, double &Right, double &Left){
	if ((XPOS > cameraView.width/2 && (XPOS - 5 < cameraView.width/2)) || (XPOS < cameraView.width/2 && (XPOS + 5 > cameraView.width/2))) { //Check if the robot is pointing generally at the line
		Left 	= 40;
		Right 	= 40;
	} else if (XPOS < cameraView.width/2) { //If the robot is pointing right of the line turn left
		Left 	= 0;
		Right 	= 10;
		} else if (XPOS > cameraView.width/2) { //If the robot is pointing left of the line turn right
			Left 	= 10;
			Right 	= 0;
		} else { //If the robot cannot find a line, spin fast to try and acquire it
			Left 	= -50;
			Right 	= 50;
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
       camLoop(TOTAL, turnRight, turnLeft);
       //Takes the white pixel count to the right.
       camLoop(RIGHT, turnRight, turnLeft);
       //Takes the white pixel count to the left.
       camLoop(LEFT, turnRight, turnLeft);

       double xPos = camVARS.totalXWhite / camVARS.whitePixelCount; //Calculate average x position of the line
       camVARS.clear();

        //blackPixelCount <= 10 is checking if we have hit the flag yet.
        if (camVARS.blackPixelCount <= 10) {
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
					bool containsWhite = false;
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
    }
}


