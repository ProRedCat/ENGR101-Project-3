#include "robot.hpp"

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
        double totalYWhite = 0;
        int whitePixelCount = 0;

        int blackPixelCount = 0;

        //Image is rotated so y is width and x is height
        for (int y = 0; y < cameraView.width; y++) { //Loop through all y pixels
            for (int x = cameraView.height - 20; x < cameraView.height; x++) { //Loop through all x pixes
                if (get_pixel(cameraView, x, y, 3) > 230) {
                    totalYWhite += y; //Add the y value for finding average y position
                    whitePixelCount++; //Increase pixel count for finding average y position
                }
                else if(get_pixel(cameraView, x, y, 3) < 5){
                    blackPixelCount++;
                }
            }
        }

        double yPos = totalYWhite / whitePixelCount; //Calculate average y position of line

        if(blackPixelCount <= 10){
            if ((yPos > 75 && (yPos - 10 < 75)) || (yPos < 75 && (yPos + 10 > 75))) { //Check if the robot is pointing generally at the line
                vLeft = 40;
                vRight = 40;
            } else if (yPos < 75) { //If the robot if pointing left of the line turn right
                vLeft = 5;
                vRight = 20;
            } else if (yPos > 75) { //If the robot is pointing right of the line turn left
                vLeft = 20;
                vRight = 5;
            }else{
                vLeft = 0;
                vRight = 20;
            }
        }
        else{
            vLeft = 0;
            vRight = 0;
        }


        setMotors(vLeft, vRight); //Set the velocity of the motors
        usleep(10000); //Sleep for a time as to not spam the server
    }
}


