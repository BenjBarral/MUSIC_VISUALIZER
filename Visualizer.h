//
//  Visualizer.h
//  audioVisualizer
//
//  Created by Benjamin Barral on 11/11/2019.
//

#ifndef Visualizer_h
#define Visualizer_h


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

static float min_brightness = 0.3;

class Visualizer{
    
private:
    int speed_window;
    Mat curr_im, prev_im;
    Vec3b mainColor;
    int disp_width,disp_height,proc_height, proc_width;
public:
    Visualizer(const int& speed, const int& p_height, const int& p_width,
               const int& d_height, const int& d_width, Vec3b soundColor){
        speed_window = speed;
        mainColor = soundColor;
        
        disp_width = d_width;
        disp_height = d_height;
        proc_height =  p_height;
        proc_width = p_width;
        
        curr_im = Mat::zeros(proc_height,proc_width,CV_8UC3);
        prev_im = Mat::zeros(proc_height,proc_width,CV_8UC3);
    }
    
    void UpdateFrame(Mat& show_im, const double* freq_intensities, Vec3b currColor){
        curr_im.copyTo(prev_im);
        for (int i = speed_window; i < proc_width; i++){
            prev_im.col(i-speed_window).copyTo(curr_im.col(i));
        }
        for (int j = 0; j < proc_height; j++){
            Vec3b pixel_value;
            double brightness = freq_intensities[j];
            if (brightness > min_brightness) {
                for (int k = 0; k < 3; k++) {
                    int color = (int)(brightness * (double)(currColor.val[k]));
                    if (color > 255) color = 255;
                    pixel_value.val[k] = color;
                }
            }
            for (int i = 0; i < speed_window; i++){
                curr_im.at<Vec3b>(proc_height - 1 - j,i) = pixel_value;
            }
        }
        
        for (int j = 0; j < proc_height; j++){
            for (int i = 0; i < proc_width; i++){
                show_im.at<Vec3b>(j,i) = curr_im.at<Vec3b>(j,0);
                show_im.at<Vec3b>(j,i+proc_width) = curr_im.at<Vec3b>(j,i);
            }
        }
        
        resize(show_im, show_im, Size(2*disp_width,disp_height));
    }
    
};

#endif /* Visualizer_h */
