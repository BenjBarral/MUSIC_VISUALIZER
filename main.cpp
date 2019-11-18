//
//  main.cpp
//
//
//  Created by Benjamin Barral.
//

/// C++
#include <cstdio>
#include <cstring>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <ctime>
/// OpenCv
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/// Sound : AQUILA
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <aquila/transform.h>

#include "SoundProcessor.h"
#include "Visualizer.h"

using namespace cv;
using namespace std;


/// IMAGE PROCESSING PARAMETERS AND VARIABLES
// Image parameters : display resolution, processing resolution
const int disp_width = 500, disp_height = 512, proc_width = 500,  proc_height = 64;

// Time parameters
clock_t clock_start;
int frame_counter = 0;

/// MUSIC and RHYTHM DETECTION PARAMETERS AND VARIABLES
// Sound interaction : triggers the effect in reaction to the beats
bool sound_interaction_mode = true; // set to false if you wanna trigger the effects manually instead of automatically with rythm detection
bool play_music = true;        // set to false if you don't want to hear the music
const int deltaPlay=5;  // for key shift
// Path of the audio file
string path_track = "../lahaina.wav"; //XX.wav";

//SOUND DETECTION TOOLS initialization
// Fourier spectrum variables
int num_frequencies = 2048;
Vec3b mainColor(138,43,226);
float divide_by = 15.;
double global_max_intensity = -1;
int speed_window = 2;

int main()
{
    /// SOUND : Initialize input file (to get samples for the Fft)
    sf::InputSoundFile f;
    if (!f.openFromFile(path_track)) {
        cout << "Can't open the file." << endl;
        return -1;
    }

    double* freq_intensities;
    SoundProcessor sound_processor(num_frequencies);
    double spectrum_band_values[num_frequencies];
    
    /// SOUND : Initialize music (to hear the track)
    sf::Music music;
    if (!music.openFromFile(path_track)) {
        cout << "Can't open the file." << endl;
        return -1;
    }
    
    /// VISUALIZATION
    Visualizer visualizer(speed_window, proc_height, proc_width, disp_height, disp_width, mainColor);
    
    /// VIDEO : Create windows
    namedWindow( "Sound visualizer", CV_WINDOW_AUTOSIZE );
    
    /// SOUND : Play music
    if (play_music){
        music.play();
    }
    
    int key = waitKey(3);
    
    
    while (key != 32) {
        clock_t begin = clock();
        
        if (sound_interaction_mode) {
            if (key==3){
                music.setPlayingOffset(sf::seconds(music.getPlayingOffset().asSeconds() + deltaPlay));
            }
            if (key==2){
                music.setPlayingOffset(sf::seconds(music.getPlayingOffset().asSeconds() - deltaPlay));
            }
            /// SOUND : Grab the samples that are currently played in the music
            f.seek(music.getPlayingOffset());
            /// SOUND : extract fourier spectrum
            sound_processor.ComputeSpectrum(f,spectrum_band_values,global_max_intensity);
        }
        
        /// TIME : Initialize time to count Delaunay's execution time
        clock_start = clock();
        
        freq_intensities = spectrum_band_values;
        
        
        for (int j = 0; j < proc_height; j++){
            Vec3b pixel_value;
            freq_intensities[j] /= (global_max_intensity/divide_by);
        }
        
        Mat show_im(proc_height,2*proc_width,CV_8UC3);
        Vec3b currColor = mainColor;
        
        /*for (int k = 0; k < 3; k++) {
            currColor.val[k] =(int)((double)(mainColor.val[k]) + 30. * cos(2*M_PI * (double)frame_counter / 20.) );
        }*/
        
        
        visualizer.UpdateFrame(show_im, freq_intensities,currColor);
        
        imshow( "Delaunay", show_im);
        frame_counter += 1;
        
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "Time : " << elapsed_secs << endl;
        /// Exit if space bar is pressed
        key = waitKey(3);
    }
}
