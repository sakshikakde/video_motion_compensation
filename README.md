# Motion Compensation using Affine warp
## Introduction
The aim of this project is to compensate for the motion in a video. Intutively, we foirst detect the motion and then apply a transformation that is opposite to the detected motion. The fooling are the steps followed:
* Detect feature points 
* Track those feature points
* Estimate motion
* Apply inverse transformation 

I used Lucas-kanade tracter to track the feature points.

## Building and running the code
* Clone the github repository       
`git clone https://github.com/sakshikakde/video_motion_compensation.git`

* Go to the repositiory folder `cd video_motion_compensation` and create a build directory `mkdir build`. Go to the build directory `cd build`.
* Run the following commands to build the project.       
'cmake ..'     
'make '

* The executable will be generated in the build folder. To run the program, use the following command.     
'./VideoStabilization ../dataset/motion_video2.mp4'

## Results 
### Original input video

![Original video](https://github.com/sakshikakde/video_motion_compensation/blob/main/readme/motion_video2.gif)


### Motion compensated video
![Result](https://github.com/sakshikakde/video_motion_compensation/blob/main/readme/motion_compensated_video.gif)

## Improvement scope
I am using an affine warp for this example. As it can be seen, the output is a little noisy. It can be used by using projective transformations.


## Reference
https://learnopencv.com/video-stabilization-using-point-feature-matching-in-opencv/
