#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main(){

    //Read image
    Mat image = imread("../assets/putin.jpg");

    Mat result;

    //Reference points for X-Axis
    float originalValue[] = {0,50,100,150,200,255};
    //Changed points on Y-axis for red and blue channels
    float redValue[] = {0,80,150,190,220,255};
    float blueValue[] = {0,20,40,75,150,255};

    //split into channels
    vector<Mat> channels(3);
    split(image, channels);

    Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();

    //Linear Interpolation applied to get values for all the points on X-Axis
    for(int i=0; i< 256; i++){
        int j=0;
        float xval = (float) i;
        while(xval > originalValue[j]){
            j++;
        }
        if(xval == originalValue[j]){
            lut[i] = redValue[j];
            continue;
        }
        float slope = ((float)(redValue[j] - redValue[j-1]))/((float)(originalValue[j] - originalValue[j-1]));
        float constant = redValue[j] - slope * originalValue[j];
        lut[i] = slope*xval + constant;
    }

    //Applying mapping and check for underflow/overflow in Red channel
    LUT(channels[2], lookupTable, channels[2]);
    min(channels[2], 255, channels[2]);
    max(channels[2], 0, channels[2]);

    Mat lookupTable2(1, 256, CV_8U);
    uchar* lut2 = lookupTable2.ptr();
    
     //Linear Interpolation applied to get values for all the points on X-Axis
    for(int i=0; i< 256; i++){
        int j=0;
        float xval = (float) i;
        while(xval > originalValue[j]){
            j++;
        }
        if(xval == originalValue[j]){
            lut2[i] = blueValue[j];
            continue;
        }
        float slope = ((float)(blueValue[j] - blueValue[j-1]))/((float)(originalValue[j] - originalValue[j-1]));
        float constant = blueValue[j] - slope * originalValue[j];
        lut2[i] = slope*xval + constant;
    }
    
    LUT(channels[0], lookupTable2, channels[0]);
    min(channels[0], 255, channels[0]);
    min(channels[0], 0, channels[0]);

    //Merge the channels
    merge(channels, result);

    //create windows to display images
    namedWindow("image", WINDOW_NORMAL);
    namedWindow("result", WINDOW_NORMAL);

    //display images
    imshow("image", image);
    imshow("result", result);

    //press esc to exit the program
    waitKey(0);

    //close all the opened windows
    destroyAllWindows();

    return 0;
}