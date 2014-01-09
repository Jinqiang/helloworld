#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
 
#include "opencv2\opencv.hpp"
#include <opencv2\nonfree\features2d.hpp>
 
using namespace std;
using namespace cv;
 
void useSurfDetector(Mat img_1, Mat img_2, vector<DMatch> good_matches);
void useBriskDetector(Mat img_1, Mat img_2, vector<DMatch> good_matches);
int main()
{   // test git hub change
	Mat img_1, img_2;
  //image = imread("lena.jpg",1);
	img_1= imread("img1.ppm", CV_LOAD_IMAGE_GRAYSCALE );
	img_2= imread("img12.jpg",CV_LOAD_IMAGE_GRAYSCALE );
	if(img_1.empty()||img_2.empty())
	{
	    cout <<  "Could not open or find the image" << std::endl ;
        return -1;
	}
 
	vector<DMatch> good_matches	;
	useSurfDetector(img_1, img_2, good_matches);
 
	useBriskDetector(img_1, img_2, good_matches);
 
	// imwrite("Lena SURF Matches.jpg",img_matches);
	waitKey(0);
	return 0;
}
 
 
void useSurfDetector(Mat img_1, Mat img_2, vector<DMatch> good_matches)
{
	good_matches.clear();
	double t = (double)getTickCount();
 
	vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptor_1, descriptor_2;
 
	int minHessian = 400;
	SurfFeatureDetector detector(minHessian);
	// Step -1, Detect keypoints using SURF detector
	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);
	// Step -2, Calculate descriptors (feature vector)
	SurfDescriptorExtractor extractor;
	extractor.compute(img_1,keypoints_1,descriptor_1);
	extractor.compute(img_2,keypoints_2,descriptor_2);
 
	//step - 3, Matching descriptor vectors with a brute force mathcher
 	
 	BFMatcher matcher(NORM_L2);
 
	vector<DMatch> matches;
	matcher.match(descriptor_1, descriptor_2,matches);
    // quick calcualation of max and min distances between keypoints
	t = ((double)getTickCount() - t)/getTickFrequency();
	
	double max_dist=0; double min_dist = 1000;
	for (int i =0; i < descriptor_1.rows;i++)
	{
		double dist = matches[i].distance;
		if(max_dist<dist) max_dist = dist;
		if(min_dist>dist) min_dist = dist;
	}
 	cout << "SURF " << endl;
	cout << " max dist " <<  max_dist << endl;
	cout <<" max dist " << min_dist << endl;
	cout << " SURF Time (senconds):  " << t<<endl;
	for (int i=0;i<descriptor_1.rows;i++)
	{
		if( matches[i].distance<3*min_dist)
			good_matches.push_back(matches[i]);
	}
 
	// Draw Good Matches
	Mat img_goodmatches;
	drawMatches(img_1,keypoints_1,img_2,keypoints_2,good_matches,img_goodmatches,Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow( "Good Matches SURF", img_goodmatches );
/*	for( int i = 0; i < good_matches.size(); i++ )
	{ 
		printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); 
	}*/
 
};
 
void useBriskDetector(Mat img_1, Mat img_2, vector<DMatch> good_matches)
{
	good_matches.clear();
	double t = (double)getTickCount();
    vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptor_1, descriptor_2;
 
	int Thresh = 60;
	int Octave = 4;
	float PatternScales=1.0f;
	cv::BRISK briskDetector(Thresh, Octave,PatternScales);
	
	briskDetector.create("Feature2D.BRISK");
 
	// Step -1, Detect keypoints using SURF detector
	briskDetector.detect(img_1, keypoints_1);
	briskDetector.detect(img_2, keypoints_2);
	// Step -2, Calculate descriptors (feature vector)
	briskDetector.compute(img_1,keypoints_1,descriptor_1);
	briskDetector.compute(img_2,keypoints_2,descriptor_2);
	//step - 3, Matching descriptor vectors with a brute force mathcher
  	BFMatcher matcher(NORM_HAMMING);
 	vector<DMatch> matches;
	matcher.match(descriptor_1, descriptor_2,matches);
	
	t = ((double)getTickCount() - t)/getTickFrequency();
	
 
	// quick calcualation of max and min distances between keypoints
	double max_dist=0; double min_dist = 1000;
	for (int i =0; i < descriptor_1.rows;i++)
	{
		double dist = matches[i].distance;
		if(max_dist<dist) max_dist = dist;
		if(min_dist>dist) min_dist = dist;
	}
	cout << "BRISK " << endl;
	cout << " max dist " <<  max_dist << endl;
	cout <<" max dist " << min_dist << endl;
	cout << " Brisk Time (senconds):  " << t<<endl;
	min_dist = min_dist + (max_dist- min_dist)* 0.3;
	for (int i=0;i<descriptor_1.rows;i++)
	{
		if( matches[i].distance< min_dist)
			good_matches.push_back(matches[i]);
	}
		// Draw Good Matches
	Mat img_goodmatches;
	drawMatches(img_1,keypoints_1,img_2,keypoints_2,good_matches,img_goodmatches,Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow( "Good Matches BRISK", img_goodmatches );
/*	for( int i = 0; i < good_matches.size(); i++ )
	{ 
		printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); 
	}*/
 
}