#include <iostream>
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;

const char* keys =
        "{ help h |                          | Print help message. }"
        "{ input1 | ../data/box.png          | Path to input image 1. }"
        "{ input2 | ../data/box_in_scene.png | Path to input image 2. }";


int main( int argc, char* argv[] )
{
    CommandLineParser parser( argc, argv, keys );
    Mat img_object = imread( parser.get<String>("input1"), IMREAD_GRAYSCALE );
    Mat img_scene = imread( parser.get<String>("input2"), IMREAD_GRAYSCALE );
    if ( img_object.empty() || img_scene.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        parser.printMessage();
        return -1;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute( img_object, noArray(), keypoints_object, descriptors_object );
    detector->detectAndCompute( img_scene, noArray(), keypoints_scene, descriptors_scene );

    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );

    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.75f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    //-- Draw matches
    Mat img_matches;
    drawMatches( img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( size_t i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, RANSAC );

    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point2f(0, 0);
    obj_corners[1] = Point2f( (float)img_object.cols, 0 );
    obj_corners[2] = Point2f( (float)img_object.cols, (float)img_object.rows );
    obj_corners[3] = Point2f( 0, (float)img_object.rows );
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform( obj_corners, scene_corners, H);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line( img_matches, scene_corners[0] + Point2f((float)img_object.cols, 0),
          scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f((float)img_object.cols, 0),
          scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f((float)img_object.cols, 0),
          scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f((float)img_object.cols, 0),
          scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );

    //-- Show detected matches
    imshow("Good Matches & Object detection", img_matches );



    //! [AKAZE]
     std::vector<KeyPoint> kpts1, kpts2;
     Mat desc1, desc2;

     Ptr<AKAZE> akaze = AKAZE::create();
     akaze->detectAndCompute(img_object, noArray(), kpts1, desc1);
     akaze->detectAndCompute(img_scene, noArray(), kpts2, desc2);
     //! [AKAZE]

     //! [2-nn matching]
     BFMatcher brute_force_matcher(NORM_HAMMING);
     std::vector< std::vector<DMatch> > nn_matches;
     brute_force_matcher.knnMatch(desc1, desc2, nn_matches, 2);
     //! [2-nn matching]

     //! [ratio test filtering]
     const float inlier_threshold = 2.5f; // Distance threshold to identify inliers with homography check
     const float nn_match_ratio = 0.8f;   // Nearest neighbor matching ratio
     std::vector<KeyPoint> matched1, matched2;
     for(size_t i = 0; i < nn_matches.size(); i++) {
         DMatch first = nn_matches[i][0];
         float dist1 = nn_matches[i][0].distance;
         float dist2 = nn_matches[i][1].distance;

         if(dist1 < nn_match_ratio * dist2) {
             matched1.push_back(kpts1[first.queryIdx]);
             matched2.push_back(kpts2[first.trainIdx]);
         }
     }
     //! [ratio test filtering]

     //! [homography check]
     std::vector<DMatch> new_good_matches;
     std::vector<KeyPoint> inliers1, inliers2;
     for(size_t i = 0; i < matched1.size(); i++) {
         Mat col = Mat::ones(3, 1, CV_64F);
         col.at<double>(0) = matched1[i].pt.x;
         col.at<double>(1) = matched1[i].pt.y;

         col = H * col;
         col /= col.at<double>(2);
         double dist = sqrt( pow(col.at<double>(0) - matched2[i].pt.x, 2) +
                             pow(col.at<double>(1) - matched2[i].pt.y, 2));

         if(dist < inlier_threshold) {
             int new_i = static_cast<int>(inliers1.size());
             inliers1.push_back(matched1[i]);
             inliers2.push_back(matched2[i]);
             new_good_matches.push_back(DMatch(new_i, new_i, 0));
         }
     }
     //! [homography check]

     double inlier_ratio = inliers1.size() / (double) matched1.size();

     std::cout << "# Inliers Ratio : " << inlier_ratio << endl;

     const double epsilon = 0.099;
     if( fabs(inlier_ratio) < epsilon )
     {
        std::cout << " No Match : the two images are not similar! " << std::endl;
     }
     else
     {
        std::cout << " Match : the two images are similar! " << std::endl;
     }

    waitKey();
    return 0;
}
#else
int main()
{
    std::cout << "This code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif
