/*
 * main.cpp
    Copyright (C) 2013  Timothy Sweet

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ros/ros.h>
#include "../include/ImageFetcher.h"
#include <opencv2/opencv.hpp>
#include <sensor_msgs/image_encodings.h>
#include <vector>
#include <sstream>
#include <iostream>

#define PUCK_SIZE 25

typedef std::vector<cv::Point> Region;
	typedef std::vector<Region> RegionList;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "ENGR301");
  ImageFetcher input_stream("in");
  int sequence_number;
  bool measuring = false;

  cv::Mat frame;
  cv::Mat frame_copy;
  RegionList contours;
  int max_size,max_at;
  int max_height;

  double scale_factor = 1;
  int setpoint = 480;
  std::string print;
  double pendulum_height = 0;

  while(ros::ok()) {
	  ros::spinOnce();
	  sequence_number = input_stream.GetFrame(frame);
	  if(sequence_number!=0) {
		  if(measuring) {
			  //initialize it to a reasonably large value to remove noise
			  max_size = 2000;
			  max_at = -1;

			  //get blobs from the image
			  //need to copy the frame because findContours destroys it :(
			  frame.copyTo(frame_copy);
			  cv::findContours(frame_copy,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
			  for(int i=0;i<contours.size();i++) {
				  if(cv::contourArea(contours[i]) > max_size) {
					  max_size = cv::contourArea(contours[i]);
					  max_at = i;
				  }
			  }
			  //now I know where the biggest contour is
			  if(max_at!=-1) {
				  //find the bottom of this contour
				  cv::Rect bound = cv::boundingRect(contours[max_at]);
				  //note that in the camera frame up is negative
				  if(bound.y+bound.height < max_height) {
					  //this is the new pinacle
					  max_height = bound.y+bound.height-PUCK_SIZE;
				  }
			  }
			  cv::line(frame,cv::Point(0,max_height),cv::Point(frame.cols-1,max_height),cv::Scalar(255,0,0),2,8);
			  pendulum_height = (setpoint-max_height)*scale_factor;

		  }
		  std::stringstream text;
		  text << pendulum_height;
		  cv::putText(frame,text.str(),cv::Point(150,max_height-10),CV_FONT_HERSHEY_SIMPLEX,3,cv::Scalar(255,0,0),2,8);
		  cv::line(frame,cv::Point(0,setpoint),cv::Point(frame.cols-1,setpoint),cv::Scalar(255),2,8);
		  std::cout<<"Height: "<<pendulum_height<<std::endl;
		  imshow("window",frame);
	  }
	  switch(cv::waitKey(1)) {
	  case 'm':
		  measuring = !measuring;
		  max_height = setpoint;
		  break;
	  case 'w':
		  //adjust setpoint up
		  setpoint +=1;
		  std::cout<<"setpoint: "<<setpoint<<std::endl;
		  break;
	  case 's':
		  //adjust setpoint donw
		  setpoint -=1;
		  std::cout<<"setpoint: "<<setpoint<<std::endl;
		  break;
	  case 't':
		  //train on a piece of paper
		  //get contours in the frame and find the biggest one
		  std::cout<<"finding contours"<<std::endl;
		  cv::findContours(frame,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		  std::cout<<"found contours"<<std::endl;
		  max_at = -1;
		  for(int i=0;i<contours.size();i++) {
			  if(cv::contourArea(contours[i]) > max_size) {
				  std::cout<<"finding area"<<std::endl;
				  max_size = cv::contourArea(contours[i]);
				  max_at = i;
			  }
		  }
		  std::cout<<"found max"<<std::endl;
		  //now I know where the biggest contour is
		  if(max_at!=-1) {
			  //find the bound of this contour
			  std::cout<<"calcing bound: "<<contours[max_at]<<std::endl;
			  cv::Rect bound = cv::boundingRect(contours[max_at]);
			  std::cout<<"calced bound"<<std::endl;
			  //I know the piece of paper is 8.5 inches wide and the width of this blob
			  //so now I have a scale factor
			  scale_factor = 0.2196/bound.width;
			  std::cout<<"bound width: "<<bound.width<<std::endl;
			  std::cout<<"scale factor: "<<scale_factor<<std::endl;
		  }
		  break;
	  }
  }

  return 0;
}
