#include "robot.hpp"
#include <cmath>

using namespace std;

double mapValue(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {
    if (initClientRobot() != 0) {
        std::cout << " Error initializing robot" << std::endl;
    }
    double vLeft = 0;
    double vRight = 0;

    while (true) {
        takePicture();

        double totalX = 0;
        double totalY = 0;
        int pixelCount = 0;
        for (int x = 0; x < cameraView.width; x++) {
            for (int y = 0; y < cameraView.height; y++) {
                if(get_pixel(cameraView, y, x, 3) > 230){
                    totalX += y;
                    totalY += x;
                    pixelCount++;
                }
            }
        }

        double xPos = totalX/pixelCount;
        double yPos = totalY/pixelCount;

        cout<<xPos<<" xPos; "<<yPos<<" yPos"<<endl;

        if((yPos > 50 && (yPos - 5 < 50)) || (yPos < 50 && (yPos + 5 > 50))){
            vLeft = 40;
            vRight = 40;
        }
        else if(yPos < 50){
            vLeft = 0;
            vRight = 5;
        }
        else if(yPos > 50){
            vLeft = 5;
            vRight = 0;
        }

        setMotors(vLeft, vRight);
        //std::cout << " vLeft=" << vLeft << "  vRight=" << vRight << std::endl;
        usleep(10000);
    } //while

} // main


