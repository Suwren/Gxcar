//
// Created by wsc on 2022/11/17.
//

#ifndef ENGINEERING_TRAINING_HEADFILE_H
#define ENGINEERING_TRAINING_HEADFILE_H

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <wiringSerial.h>
#include <wiringPi.h>

using namespace std;
using namespace cv;

#define blue_low Scalar(100,43,46)
#define blue_up Scalar(124,255,255)
#define green_low Scalar(35,43,46)
#define green_up Scalar(90,255,255)
#define red_low1 Scalar(0,43,46)
#define red_low2 Scalar(156,43,46)
#define red_up1 Scalar(5,255,255)
#define red_up2 Scalar(180,255,255)


void Read_Code(Mat &src,String &data);//读取二维码信息，src：输入图片，data：二维码信息
void readcode(Mat& src, String& data, vector<string>& dat, String& data_show);
void Color_Detected(Mat &srcImg,Mat &dstImg,char Color);
void Get_centroid(Mat &binaryImg,Point2f &centroid);
void line_detect(Mat &img, double &distance, double &angle);
void catch_object(Mat& img, vector<string> data, int fd, VideoCapture video);
void Color_Detected_circle(Mat &srcImg,Mat &dstImg,char Color);
void objectplace(Mat& src,Mat& the,String& message);
void objectplace_2(Mat& src,Mat& thre,String& message);
void objectplace_H(Mat& src,String& message);
void objectplace_H_2(Mat& src,String& message);
#endif //ENGINEERING_TRAINING_HEADFILE_H
