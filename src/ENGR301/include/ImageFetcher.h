/*
 * ImageFetcher.h
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
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <stereo_msgs/DisparityImage.h>
#include <sensor_msgs/image_encodings.h>

#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
/*
 * grabs a frame on a specified topic and provides it as a cv::Mat
 */
class ImageFetcher
{
	private:
		//transport handles
		ros::NodeHandle handle_;
		image_transport::ImageTransport transport_;
		image_transport::Subscriber subscription_;

		//pointers to raw Mats
		cv_bridge::CvImagePtr raw_frame_ptr_;

		void FrameCallback(const sensor_msgs::ImageConstPtr& msg);
		bool updated_;
		void convertMsgToCvImagePtr(const sensor_msgs::ImageConstPtr& msg, cv_bridge::CvImagePtr& raw_ptr);

		int sequence_number_;
	public:
		/*
		 * Returns sequence number, or 0 if there is no frame
		 * Potentially discards the first frame
		 */
		int GetFrame(cv::Mat& frame);
		inline bool IsUpdated() {return updated_;}
		ImageFetcher(const std::string& topic_name);
};
