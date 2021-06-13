#include <opencv2/opencv.hpp>
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>


using namespace std;
using namespace cv;
const int SMOOTHING_RADIUS = 50;

struct TransformParam
{
  TransformParam() {}
  TransformParam(double _dx, double _dy, double _da) 
  {
      dx = _dx;
      dy = _dy;
      da = _da;
  }

  double dx;
  double dy;
  double da; // angle

  void getTransform(Mat &T)
  {
    // Reconstruct transformation matrix accordingly to new values
    T.at<double>(0,0) = cos(da);
    T.at<double>(0,1) = -sin(da);
    T.at<double>(1,0) = sin(da);
    T.at<double>(1,1) = cos(da);

    T.at<double>(0,2) = dx;
    T.at<double>(1,2) = dy;
  }
};

struct Trajectory
{
    Trajectory() {}
    Trajectory(double _x, double _y, double _a) {
        x = _x;
        y = _y;
        a = _a;
    }

    double x;
    double y;
    double a; // angle
};


vector<Trajectory> cumSum(vector<TransformParam> &transforms)
{
  vector <Trajectory> trajectory; // trajectory at all frames
  // Accumulated frame to frame transform
  double a = 0;
  double x = 0;
  double y = 0;

  for(size_t i=0; i < transforms.size(); i++) 
  {
      x += transforms[i].dx;
      y += transforms[i].dy;
      a += transforms[i].da;

      trajectory.push_back(Trajectory(x,y,a));

  }

  return trajectory; 
}

vector<Trajectory> cumSum(vector <Trajectory> trajectory_prior, TransformParam transforms)
{
    vector <Trajectory> trajectory = trajectory_prior; // trajectory at all frames
    // Accumulated frame to frame transform
    double a = 0;
    double x = 0;
    double y = 0;
    if (!trajectory.empty())
    {
        Trajectory trajectory_endpoint = trajectory.back();
        a = trajectory_endpoint.a;
        x = trajectory_endpoint.x;
        y = trajectory_endpoint.y;
    }


    x += transforms.dx;
    y += transforms.dy;
    a += transforms.da;

    trajectory.push_back(Trajectory(x,y,a));
    return trajectory; 
}


vector <Trajectory> smoothenTraj(vector <Trajectory>& trajectory, int radius)
{
  vector <Trajectory> smoothed_trajectory; 
  for(size_t i=0; i < trajectory.size(); i++) {
      double sum_x = 0;
      double sum_y = 0;
      double sum_a = 0;
      int count = 0;

      for(int j=-radius; j <= radius; j++) {
          if(i+j >= 0 && i+j < trajectory.size()) {
              sum_x += trajectory[i+j].x;
              sum_y += trajectory[i+j].y;
              sum_a += trajectory[i+j].a;

              count++;
          }
      }

      double avg_a = sum_a / count;
      double avg_x = sum_x / count;
      double avg_y = sum_y / count;

      smoothed_trajectory.push_back(Trajectory(avg_x, avg_y, avg_a));
  }

  return smoothed_trajectory; 
}


void fixBorder(Mat &frame_stabilized)
{
  Mat T = getRotationMatrix2D(Point2f(frame_stabilized.cols/2, frame_stabilized.rows/2), 0, 1.04); 
  warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size()); 
}

