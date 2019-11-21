# A real-time minimalist sound visualizer

An application for music visualization in real-time, using OpenCV and the Aquila sound processing library.
The user needs to input a `.wav` audio file.

![](images-and-videos/giphy.gif)

## Usage
### Dependencies : 
Libraries required
-[OpenCV](https://opencv.org/releases/)
-[Aquila + SFML]( https://aquila-dsp.org/download/) (signal processing)

You can build using CMake (and modify the CMakeLists.txt)

### Running and interacting with the program
Run the ```visualizer``` target.
Put the path of the track you want to play in the ```path_track``` variable (WAV file).

Use left and right arrows to manipulate the audio track.
