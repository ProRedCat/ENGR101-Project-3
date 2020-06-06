#include "robot.hpp"
#include <iostream>

using namespace std;

int main() {
    if (initClientRobot() != 0) { //Check if the robot was not initialised
        std::cout << " Error initializing robot" << std::endl;
    }

    //Velocities for the robot wheels
    double vLeft = 0;
    double vRight = 0;

	bool turningRight = false;
	bool turningLeft = false;

    while (true) { //Loop continuously
       takePicture(); //Take a picture to update the robots view

       //Variables for finding average y position of the line
       double totalXWhite = 0;
       int whitePixelCount = 0;

       int blackPixelCount = 0;
       
       
       for (int y = cameraView.height - 20; y < cameraView.height; y++) { //Loop through the y pixels from the height - 20 to height
            for (int x = cameraView.width/3; x < cameraView.width - (cameraView.width - ((cameraView.width/3) * 2)); x++) { //Loop through the x pixels between 1/3 camera width and 2/3 camera width
                if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					totalXWhite += x; //Add x to totalXWhite
					whitePixelCount++; //Incease the whitePixelCount by 1
				}
				else if (get_pixel(cameraView, x, y, 3) < 5) { //Check if pixel is black
                    blackPixelCount++; //Increase blackPixelCount
				}
            }
        }
		
		bool turnRight = false;
		int rightWhiteCount = 0;
		for (int y = cameraView.height - 10; y < cameraView.height; y++) { //Loop through the y pixels from the height - 10 to height
            for (int x = cameraView.width - (cameraView.width/3); x < cameraView.width; x++) { //Loop through the x pixels between 2/3 camera width and camera width
                if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					rightWhiteCount++;
				}
            }
        }
        
        bool turnLeft = false;
		int leftWhiteCount = 0;
		for (int y = cameraView.height - 10; y < cameraView.height; y++) { //Loop through the y pixels from the height - 10 to height
            for (int x = 0; x < cameraView.width/3; x++) { //Loop through the x pixels between 0 camera width and 1/3 camera width
                if(get_pixel(cameraView, y, x, 3) > 230){ //Check if the pixel is white
					leftWhiteCount++;
				}
            }
        }
       
        
        
        if(rightWhiteCount > 0){
			turnRight = true;
		}
		else{
			turningRight = false;
		}
		
		if(leftWhiteCount > 0){
			turnLeft = true;
		}
		else{
			turningLeft = false;
		}

        double xPos = totalXWhite / whitePixelCount; //Calculate average x position of the line

        if (blackPixelCount <= 10) {
            if ((xPos > cameraView.width/2 && (xPos - 5 < cameraView.width/2)) || (xPos < cameraView.width/2 && (xPos + 5 > cameraView.width/2))) { //Check if the robot is pointing generally at the line
                vLeft = 40;
                vRight = 40;
            } else if (xPos < cameraView.width/2) { //If the robot if pointing right of the line turn left
               vLeft = 0;
               vRight = 10;
            } else if (xPos > cameraView.width/2) { //If the robot is pointing left of the line turn right
                vLeft = 10;
                vRight = 0;
            } else { //If the robot cannot find a line, spin fast to try and aquire it
                vLeft = 0;
                vRight = 150;
            }
            
            
			if(turnRight == true && turnLeft == true){ //If the robot can turn both left and right turn left
				if(turningRight == false){
					turningLeft = true;
					vLeft = 0;
					vRight = 10;
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
					
					if(!containsWhite){ //Check if there is not a path infront of the robot, if so turn left 
						turningRight = true;
						vLeft = 10;
						vRight = 0;
					}
				}
				if(turnLeft && turningRight == false){ //Robot pivots on left wheel 
					turningLeft = true;
					vLeft = 0;
					vRight = 10;
				}
			}			
        } else { //The robot has reached the flag
            vLeft = 0;
            vRight = 0;
        }
		
        setMotors(vLeft, vRight); //Set the velocity of the motors
        usleep(10000); //Sleep for a time as to not spam the server
    }
}


