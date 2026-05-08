#include <iostream>
#define HEIGHT 8
#define WIDTH 8

using namespace std;

int findMaxPixel(int a[][WIDTH], int h, int w) {
    int tmpMax = a[0][0];

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (a[i][j] > tmpMax) {
                tmpMax = a[i][j];
            }
        }
    }
    return tmpMax;
}

int main() {
    int image[HEIGHT][WIDTH] = {
        { 10,  30,  55,  80, 120, 160, 200, 230},
        { 20,  45,  70, 100, 140, 180, 220, 210},
        { 35,  60,  90, 130, 170, 210, 240, 190},
        { 50,  80, 115, 150, 190, 230, 255, 170},
        { 40,  65, 100, 140, 175, 215, 235, 150},
        { 25,  50,  80, 115, 155, 195, 210, 130},
        { 15,  35,  60,  90, 130, 165, 185, 110},
        {  5,  20,  40,  70, 105, 140, 160,  90}
    };

    int maxPixel = findMaxPixel(image, HEIGHT, WIDTH);

    cout << "최대 화소 밝기 : " << maxPixel << endl;

    return 0;
}