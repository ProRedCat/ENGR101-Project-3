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
        double totalY = 0;
        int pixelCount = 0;

        //Image is rotated so y is width and x is height
        for (int y = 0; y < cameraView.width; y++) { //Loop through all y pixels
            for (int x = 0; x < cameraView.height; x++) { //Loop through all x pixes
                if (get_pixel(cameraView, x, y, 3) > 230) {
                    totalY += y; //Add the y value for finding average y position
                    pixelCount++; //Increase pixel count for finding average y position
                }
            }
        }

        double yPos = totalY / pixelCount; //Calculate average y position of line

        if ((yPos > 75 && (yPos - 10 < 75)) || (yPos < 75 && (yPos + 10 > 75))) { //Check if the robot is pointing generally at the line
            vLeft = 40;
            vRight = 40;
        } else if (yPos < 75) { //If the robot if pointing left of the line turn right
            vLeft = 5;
            vRight = 10;
        } else if (yPos > 75) { //If the robot is pointing right of the line turn left
            vLeft = 10;
            vRight = 5;
        } else { //If it can't find any rotate on the spot in hopes of finding it
            vLeft = 0;
            vRight = 40;
        }

        setMotors(vLeft, vRight); //Set the velocity of the motors
        usleep(10000); //Sleep for a time as to not spam the server
    }
}


