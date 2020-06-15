#include "robot.hpp"

// checks for white pixels in the camera view
int whiteChecker (){
	takePicture(); 
	int noWhite = -1;
	int countBlack = 0;
	for (int y = 0; y < cameraView.width; y++) { 
			for (int x = 0; x < cameraView.height; x++) {
			  if (get_pixel(cameraView, x, y, 3) > 230) {
				  noWhite = y;
			  }
			  if (get_pixel(cameraView, x, y, 3) < 15) {
				  countBlack++; // if black pixel is found
			  }
			  if (countBlack > 1800){
				  return noWhite = -2;
			  }  
		}
	}
	return noWhite;
}

int main() {
    if (initClientRobot() != 0) { //Check if the robot was not initialised
        std::cout << " Error initializing robot" << std::endl;
    }
    
    double vLeft=0;
    double vRight=0;
    double conSpeed = 50; // constant speed

    while (true) { //Loop continuously
        
        // variables that might change after every movement while true
        double difference = 0;
        double middle = cameraView.width/2;
        
        int noWhite = whiteChecker();
        std::cout<<"noWhite: "<<noWhite;
        
		while (noWhite== -1){
			double vRight = conSpeed;
			double vLeft = 0;
			setMotors(vLeft, vRight); //Set the velocity of the motors 
			noWhite = whiteChecker();
		}
		// if white is in the middle of view then right and left velocities must be equal
		if (noWhite == -2) {
			vRight = 0;
			vLeft = 0;
		}
		else if (noWhite - middle == 0) {
				vRight = conSpeed;
				vLeft = vRight;
			}
			else { // calculates distance away from middle and uses for velocity equation
				difference = noWhite - middle;
				vRight = conSpeed -difference;
				vLeft = conSpeed +difference;
			}
			
        setMotors(vLeft, vRight);
        }
	}

