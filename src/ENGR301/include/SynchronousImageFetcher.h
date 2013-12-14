/*
 * SynchronousImageFetcher.h
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

#ifndef SYNCHRONOUSIMAGEFETCHER_H_
#define SYNCHRONOUSIMAGEFETCHER_H_
#include <string>
#include "common.cpp"
#include "ImageFetcher.h"


class SynchronousImageFetcher {
	public:
	SynchronousImageFetcher(std::string rgb_topic_name,std::string depth_topic_name);
	bool GetFrame(cv::Mat& rgb_frame,cv::Mat& depth_frame);

	private:
	DISALLOW_COPY_AND_ASSIGN(SynchronousImageFetcher);
	ImageFetcher rgb_handle,depth_handle;
};

#endif /* SYNCHRONOUSIMAGEFETCHER_H_ */
