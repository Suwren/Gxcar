//
// Created by wsc on 2022/11/17.
//
#include "HeadFile.h"

/*
 *
 * @brief 获取物体质心
 * @param binaryImg 输入的二值化图片，图片中物体应为白色，其余东西均为黑�??????
 * @param centroid 物体的质心坐�??????
  */
void Get_centroid(Mat &binaryImg,Point2f &centroid){

    vector<vector<Point>> contours;//定义轨迹�??????
    vector<Vec4i> hierarchy;
    findContours(binaryImg,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));

    vector<Moments> mu(contours.size());
    for(int i=0;i<contours.size();i++){
        mu[i]=moments(contours[i],false);//计算�??????
    }

    vector<Point2f> mc(contours.size());
    for( int i = 0; i < contours.size(); i++ ){
        double area=contourArea(contours[i]);
       if (area < 50*50){
           continue;
       }
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        centroid=mc[i];//计算中心�??????
        circle(binaryImg,mc[i],8,Scalar(0,0,0),-1);
        //drawContours(draw,contours,i,Scalar(255,255,255),1,8);
    }

}

/*
 *
 * @brief 识别颜色
 * @param srcImg 输入图片
 * @param dstImg 输出图片
 * @param Color 目标颜色，输出图片中只有和目标颜色相同的区域为白色，其余部分均为黑色
*/
void Color_Detected(Mat &srcImg,Mat &dstImg,char Color)
{
    Mat hsv;
    cvtColor(srcImg,hsv,COLOR_BGR2HSV);
    switch (Color) {
        case '1':{//�??1
            Mat temp1,temp2;
            inRange(hsv,red_low1,red_up1,temp1);
            inRange(hsv,red_low2,red_up2,temp2);
            bitwise_or(temp1,temp2,dstImg);
            break;
        }
        case '2':{//�??2
            inRange(hsv,green_low,green_up,dstImg);
            break;
        }
        case '3':{//�??3
            inRange(hsv,blue_low,blue_up,dstImg);
            break;
        }
        default:{
            //cout<<"Color is error input"<<endl;
            break;
        }
    }
}
void Color_Detected_circle(Mat &srcImg,Mat &dstImg,char Color)
{
    Mat hsv;
    cvtColor(srcImg,hsv,COLOR_BGR2HSV);
    switch (Color) {
        case 'R':{
            inRange(hsv,Scalar(150,129,105),Scalar(190,218,167),dstImg);
            break;
        }
        case 'G':{
            inRange(hsv,Scalar(61,40,110),Scalar(91,120,255),dstImg);
            break;
        }
        case 'B':{
            inRange(hsv,Scalar(116,113,96),Scalar(139,255,130),dstImg);
            break;
        }
        case 'b':{
            inRange(hsv,Scalar(0,0,85),Scalar(255,255,255),dstImg);
            break;
        }
        default:{
            //cout<<"Color is error input"<<endl;
            break;
        }
    }
}
/*
 *
 * @brief 获取直线角度和距�??????
 * @param img 输入原始图片
 * @param distance 返回距离
 * @param angle 返回直线角度 
  */
  //第一次distance中�?170 178 183->8cm，angle中�?2.9 5 // (305-146)->(100-50) m后面第二个数
void line_detect(Mat &img, double &distance, double &angle)
{
    
	Mat src1_GRAY, src1_shink, src1_blur, src1_thres, src1_dilt, src1_erd;
	src1_shink = img(Rect(0, 0, img.cols, img.rows - 135));
	GaussianBlur(src1_shink, src1_blur, Size(5, 5), 0.5);
    for(int i = 0; i<4;i++)
   		GaussianBlur(src1_blur, src1_blur, Size(5, 5), 0.5);
	// Canny(src1_blur, src1_thres, 100, 275);

	// dilate(src1_thres, src1_dilt, getStructuringElement(MORPH_RECT, Size(3, 7)), Point(-1, -1), 13);
	// imshow("dilate", src1_dilt);
	// erode(src1_dilt, src1_erd, getStructuringElement(MORPH_RECT, Size(5, 5)), Point(-1, -1), 26);
	// imshow("erode", src1_erd);

    Color_Detected_circle(src1_blur, src1_thres, 'b');
	// cvtColor(src1_blur, src1_GRAY, COLOR_BGR2GRAY);
	// threshold(src1_GRAY, src1_thres, 90, 255, THRESH_BINARY);
    
	vector<Point> point_set;
	Point point_temp;
	for (int i = 0; i < src1_thres.rows; ++i)
	{
		for (int j = 0; j < src1_thres.cols; ++j)
		{
			if (src1_thres.at<unsigned char>(i, j) != 255)
			{
				point_temp.x = j;
				point_temp.y = i;
				point_set.push_back(point_temp);
			}
		}
	}

	if (point_set.size() >= 6)
	{
		Vec4f fitline;
		fitLine(point_set, fitline, DIST_L2, 0, 0.01, 0.01);

		//求出直线上的两个�??????
		double k_line = fitline[1] / fitline[0];
		Point p1(0, k_line * (0 - fitline[2]) + fitline[3]);
		Point p2(src1_thres.cols - 1, k_line * (src1_thres.cols - 1 - fitline[2]) + fitline[3]);
        distance=(p1.y + p2.y) / 2.0;
        angle=atan(k_line) / atan(1) * 45 / 45 * 100-2.9;
		cout << "distance: " << distance << endl;
		cout << "angle: " << angle << endl;
		
		imwrite("../img.jpg", img);
		// 显示拟合出的直线
		line(img, p1, p2, Scalar(0, 0, 255), 2);
		imshow("原图+拟合结果", img);
        //imwrite("../img.jpg", img);
		imshow("src1_thres", src1_thres);
        //imwrite("../src1_thres.jpg", src1_thres);
		waitKey(2);
	}
}

/*
 *
 * @brief 分割字符�??????
 * @param str 要分割的字符�??????
 * @param pattern 分隔�??????
  */
vector<string> split(const string& str, const char pattern)
{
	vector<string> res;
	stringstream input(str);   //读取str到字符串流中
	string temp;
	//使用getline函数从字符串流中读取,遇到分隔符时停止,和从cin中读取类�??????
	//注意,getline默认是可以读取空格的


	while (getline(input, temp, pattern))
	{
		res.push_back(temp);
	}
	return res;
}

/*
 *
 * @brief 识别二维码加串口打印
 * @param src 原图
 * @param data 需要发送的消息
 * @param dat 存储在树莓派的信�??
  */
void readcode(Mat& src, String& data, vector<string>& dat, String& data_show)
{
	// Mat warpPerspective_mat(3, 3, CV_32FC1);
	// Mat src, dst;

	// /// ����4��㣬����任����
	// srcTri[0] = Point2f( 0, 0);
	// srcTri[1] = Point2f( src.cols-1, 0 );
	// srcTri[2] = Point2f( 0, src.rows-1);
	// srcTri[3] = Point2f(src.cols-1, src.rows - 1);
 
	// dstTri[0] = Point2f( 0,src.rows*0.13 );
	// dstTri[1] = Point2f( src.cols*0.9,0 );
	// dstTri[2] = Point2f( src.cols*0.2,src.rows*0.7 );
	// dstTri[3] = Point2f( src.cols*0.8,src.rows );
 
	// //����4����ά���֮��ķ���任����?2��x3�У�
	// warpPerspective_mat = getPerspectiveTransform(srcTri, dstTri);
 
	// //Ӧ�÷���任�����Իָ���ԭ�?
	// warpPerspective( src, dst, warpPerspective_mat, src.size() );
	//Mat src_rect=src(Rect(50,205,400,240));
	Read_Code(src, data);
    data_show = data;
	// cout << "data:" << data << endl;
	dat = split(data, '+');

	// cout << dat.size() << endl;
	// cout << "splited:";
	data = "";
	data.push_back('r');
	for (auto i = dat.begin(); i < dat.end(); i++)
	{
		for (const auto& ch : *i)
		{
			data.push_back(ch - 48);
		}
	}
	imshow("src", src);
	waitKey(1);
	// for (const auto& ch : data)
	// {
	// 	printf("%2x ", (unsigned char)ch);
	// }
	// cout << endl;
}

/*
 *
 * @brief 放置物块时的校准
 * @param src 原图
 * @param thre 经过阈值化后的图片（圆环为白色�????
 * @param message 需要发送的消息
  */
void objectplace(Mat& src,Mat& thre,String& message)
{	//imshow("thre", thre);
    dilate(thre, thre, getStructuringElement(0, Size(3, 3)), Point(-1, -1), 16);
    waitKey(2);
    Point2f center;
    int x,y;
    Get_centroid(thre, center);
    // cout << center << endl;
    x = 100-(0.3899*center.x - 43.941);
    y = 100-(0.3864*center.y - 40.413);
    // cout << hex << x << endl;
    // cout << hex << y << endl;
    message.push_back('m');
    message.push_back((unsigned char)abs(y+2));
    message.push_back((unsigned char)abs(x-8));

    // circle(src, center, 2, Scalar(255, 0, 0), 2, 8, 0);
	// imshow("hough circle demo", src);
}
void objectplace_2(Mat& src,Mat& thre,String& message)
{
    dilate(thre, thre, getStructuringElement(0, Size(3, 3)), Point(-1, -1), 16);
	//imshow("thre", thre);
    waitKey(2);
    Point2f center;
    int x,y;
    Get_centroid(thre, center);
    // cout << center << endl;
    x = 100-(0.3899*center.x - 43.941);
    y = 100-(0.3864*center.y - 40.413);
    // cout << hex << x << endl;
    // cout << hex << y << endl;
    message.push_back('m');
    message.push_back((unsigned char)abs(100-x+8));
    message.push_back((unsigned char)abs(y-26));

    circle(src, center, 2, Scalar(255, 0, 0), 2, 8, 0);
	//imshow("hough circle demo", src);
}
void objectplace_H(Mat& src,String& message, VideoCapture video)
{
	vector<Vec3f> circles;
	int x=0,y=0;
	Mat src_rect, src_gray;
	for(int i = 0; i < 1; i++)
	{
		video >> src;
		src_rect=src(Rect(0,0,500,430));
		cvtColor(src_rect, src_gray, COLOR_BGR2GRAY);
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, 90, 100, 98, 70, 0);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//绘制圆心  
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			//绘制圆轮�?  
			circle(src, center, radius, Scalar(155, 50, 255), 3, 8, 0);
			x+=center.x;
			y+=center.y;
		}
	}
	x=x/1.0;
	y=y/1.0;
	Mat shinksrc;
	resize(src_rect, shinksrc, Size(250, 215));
	imshow("shinksrc", shinksrc);
	waitKey(2);
    
    // cout << center << endl;
    x = 100-(0.3899*x - 43.941);
    y = 100-(0.3864*y - 40.413);
    // cout << hex << x << endl;
    // cout << hex << y << endl;
    message.push_back('m');
    message.push_back((unsigned char)abs(y+2));
    message.push_back((unsigned char)abs(x-8));

    //circle(src, center, 2, Scalar(255, 0, 0), 2, 8, 0);
	//imshow("hough circle demo", src);
}
void objectplace_H_2(Mat& src,String& message, VideoCapture video)
{
	vector<Vec3f> circles;
	int x=0,y=0;
	Mat src_rect, src_gray;
	for(int i = 0; i < 5; i++)
	{
		video >> src;
		src_rect=src(Rect(0,0,500,430));
		cvtColor(src_rect, src_gray, COLOR_BGR2GRAY);
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, 90, 100, 98, 70, 0);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//绘制圆心  
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			//绘制圆轮�?  
			circle(src, center, radius, Scalar(155, 50, 255), 3, 8, 0);
			x+=center.x;
			y+=center.y;
		}
	}
	x=x/5.0;
	y=y/5.0;
	Mat shinksrc;
	resize(src_rect, shinksrc, Size(250, 215));
	imshow("shinksrc", shinksrc);
	waitKey(2);
    
    // cout << center << endl;
    x = 100-(0.3899*x - 43.941);
    y = 100-(0.3864*y - 40.413);
    // cout << hex << x << endl;
    // cout << hex << y << endl;
    message.push_back('m');
    message.push_back((unsigned char)abs(100-x+5));
    message.push_back((unsigned char)abs(y+4));
}
/*
 *
 * @brief 抓取转盘区物�??
 * @param img 原图
 * @param data 二维码的内容readcode函数的第三个参数
 * @param fd 串口
  */
void catch_object(Mat& img, vector<string> data, int fd, VideoCapture video)
{
	video >> img;
	static int count = 0;//���ڼ�¼���ǵڼ�����ת�̰�������
	count++;
	string order;//���ڼ�¼˳��1��2��3��
	Point2f location;
	Mat drtimg;
	const int locationx_max = 220;
	const int locationx_min = 80;
	const int locationy_max = 290;
	const int locationy_min = 70;
	// cout << "count" << count << endl;
	if (count == 1)
	{
		order = data[0];
	}
	else if (count == 2)
	{
		order = data[1];
	}
	int i = 0;
	bool dont_cat = 0;
	// cout << "order[1]" << order[0] << endl;
	Color_Detected(img, drtimg, order[0]);
	Get_centroid(drtimg, location);
	if (location.x<locationx_max && location.y<locationy_max && location.x>locationx_min && location.y>locationy_min)
	{
		dont_cat = 1;
	}
	while(i<3)
	{
		video >> img;
		// cout << "order[i]" << order[i] << endl;
		Color_Detected(img, drtimg, order[i]);
		Get_centroid(drtimg, location);
		rectangle(img, Rect(locationx_min, locationy_min, locationx_max- locationx_min, locationy_max-locationy_min), Scalar(0, 0, 255), 2, LINE_8);
		imshow("img", img);
		imshow("drtimg", drtimg);
		waitKey(10);
		if (location.x<locationx_max && location.y<locationy_max && location.x>locationx_min && location.y>locationy_min && !dont_cat)
		{
			//waitKey(4000);
			serialPrintf(fd,"z");
			// cout << "catch!!!" << endl;
			i++;
			waitKey(7000);
			location = Point2f(0, 0);
		}
		else if (!(location.x<locationx_max && location.y<locationy_max && location.x>locationx_min && location.y>locationy_min) && dont_cat)
		{
			dont_cat = 0;
		}
		// cout << "location: " << "x:" << location.x << "  y:" << location.y << endl;
		// cout << "dont_cat: " << dont_cat << endl;
		// cout << "i = " << i <<endl;
	}
}
