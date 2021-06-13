#include "video_stabilization.h"

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }


    VideoCapture cap(argv[1]);
    int n_frames = int(cap.get(CAP_PROP_FRAME_COUNT)); 
    int w = int(cap.get(CAP_PROP_FRAME_WIDTH));
    int h = int(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    VideoWriter out("../results/motion_compensated_video.mp4", cv::VideoWriter::fourcc('M','J','P','G'), fps, Size(2 * w, h));


    Mat curr, curr_gray;
    Mat prev, prev_gray;
    Mat out_frame;

    // Read first frame and Convert it to grayscale
    cap >> prev;
    cvtColor(prev, prev_gray, COLOR_BGR2GRAY);
;

    for(int i = 1; i < n_frames-1; i++)
    {
        vector <Point2f> prev_pts, curr_pts;
        goodFeaturesToTrack(prev_gray, prev_pts, 200, 0.01, 30);
        bool success = cap.read(curr);
        if(!success) break; 

        // Convert to grayscale
        cvtColor(curr, curr_gray, COLOR_BGR2GRAY);

        // Calculate optical flow (i.e. track feature points)
        vector <uchar> status;
        vector <float> err;
        calcOpticalFlowPyrLK(prev_gray, curr_gray, prev_pts, curr_pts, status, err);

        // Filter only valid points
        auto prev_it = prev_pts.begin(); 
        auto curr_it = curr_pts.begin(); 
        for(size_t k = 0; k < status.size(); k++) 
        {
            if(status[k]) 
            {
            prev_it++; 
            curr_it++; 
            }
            else 
            {
            prev_it = prev_pts.erase(prev_it);
            curr_it = curr_pts.erase(curr_it);
            }
        }

        // Find transformation matrix
        Mat T = estimateRigidTransform(curr_pts, prev_pts, false); 
        print(T);

        warpAffine(curr, out_frame, T, curr.size());

        cout << "Frame: " << i << "/" << n_frames << " -  Tracked points : " << prev_pts.size() << endl;

        hconcat(curr, out_frame, out_frame);
        imshow("motion_compensated_output", out_frame);
        out.write(out_frame);
        waitKey(10);

    }

    cap.release();
    out.release();
    // Close windows
    destroyAllWindows();
    

    return 0;
}
