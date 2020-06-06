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

        double xPos = totalXWhite / whitePixelCount; //Calculate average x position of the line

        if (blackPixelCount <= 10) {
            if ((xPos > cameraView.width/2 && (xPos - 10 < cameraView.width/2)) || (xPos < cameraView.width/2 && (xPos + 10 > cameraView.width/2))) { //Check if the robot is pointing generally at the line
                vLeft = 40;
                vRight = 40;
            } else if (xPos < cameraView.width/2) { //If the robot if pointing left of the line turn right
               vLeft = 0;
               vRight = 10;
            } else if (xPos > cameraView.width/2) { //If the robot is pointing right of the line turn left
                vLeft = 10;
                vRight = 0;
            } else { //If the robot cannot find a line, spin fast to try and aquire it
                vLeft = 0;
                vRight = 50;
            }
        } else {
            vLeft = 0;
            vRight = 0;
        }


        setMotors(vLeft, vRight); //Set the velocity of the motors
        usleep(10000); //Sleep for a time as to not spam the server
    }
}


