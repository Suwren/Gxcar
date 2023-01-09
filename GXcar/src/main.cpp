#include "HeadFile.h"
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string>
#include <unistd.h>

int main()
{
    wiringPiSetup();//初始化串
	int fd = serialOpen("/dev/ttyAMA0",115200); //打开串口
    while(fd==-1)
    {
        fd = serialOpen("/dev/ttyAMA0",115200);
    }
    int sz = serialDataAvail(fd); //串口缓冲区数据个�???
    Mat mainimg(320, 480, CV_8UC3, Scalar(255,255,255));
    int function=0;//树莓派需要调用的功能序号
    double distance=0, angle=0;//line_detect直线检测函数的返回
    vector<string> codedata;//二维码的数据
    codedata={"123","321"};
    String data_show;
    char recv;//串口数据储存数组

    VideoCapture video(0);

    // namedWindow("mainWin", 0);
    // setWindowProperty("mainWin", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
    // imshow("mainWin", mainimg);
    // rectangle(mainimg, Rect(0, 0, 640, 480), Scalar(0, 0, 255), -1, LINE_8); 
    
    while(!video.isOpened())
    {
        VideoCapture video(0);
        cout << "VideoCapture error" << endl;
    }
    while(1)
    {
        Mat frame;//摄像头拍到的原始图像
        video >> frame;
        // putText(mainimg, data_show, Point(20,180), FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255));
        // imshow("mainWin", mainimg);
        serialPrintf(fd,"t");
        waitKey(1);
        // serialPrintf(fd,"1");
        // cout << "1" <<endl;
        switch(function)
        {
            case 1://第一次校对
            {
                string message;
                waitKey(500);
                line_detect(frame,distance,angle);
                if(abs(angle)>8)
                {
                    video >> frame;
                    line_detect(frame,distance,angle);
                }
                message.push_back('m');
                message.push_back((unsigned char)abs(50));
                message.push_back((unsigned char)abs(0.3202*distance-9.087+10));
                if(angle>=0)
                    message.push_back('-');
                else
                    message.push_back('+');
                message.push_back((unsigned char)(1+abs(angle)));
                for (const auto& item : message) 
                {
                    // printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                // cout << endl;                
                function = 0;
                serialFlush (fd);
                break;
            }
            case 2://第二次校对
            {
                string message;
                waitKey(500);
                line_detect(frame,distance,angle);
                if(abs(angle)>8)
                {
                    video >> frame;
                    line_detect(frame,distance,angle);
                }
                message.push_back('m');
                message.push_back((unsigned char)abs(-distance*0.299+84.75));
                message.push_back((unsigned char)abs(50));
                if((angle+3)>=0)
                    message.push_back('-');
                else
                    message.push_back('+');
                message.push_back((unsigned char)(1+abs(angle+3)));
                for (const auto& item : message) 
                {
                    // printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                //cout << endl;
                function = 0;
                serialFlush (fd);
                break;
            }
            case 3://第三次校对
            {
                string message;
                waitKey(500);
                line_detect(frame,distance,angle);
                if(abs(angle)>8)
                {
                    video >> frame;
                    line_detect(frame,distance,angle);
                }
                message.push_back('m');
                message.push_back((unsigned char)abs(50));
                message.push_back((unsigned char)abs(100-0.3202*distance-4.087));
                if((angle)>=0)
                    message.push_back('-');
                else
                    message.push_back('+');
                message.push_back((unsigned char)(1+abs(angle)));
                for (const auto& item : message) 
                {
                    // printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
               //cout << endl;
                function = 0;
                serialFlush (fd);
                break;
            }
            case 72://识别二维�???
            {
                string message;//发给32的信�???
                readcode(frame, message,codedata,data_show);
                if(message.size()>1)
                    for (const auto& item : message) 
                    {
                        // printf("%2x ", (unsigned char)item);
                        serialPrintf(fd,"%c",item);
                    }
                // Mat fra;
                // resize(frame, fra, Size(240, 160));
                // imshow("fra", fra);
                // waitKey(20);
                // cout << endl;
               
                function = 0;
                serialFlush (fd);
                break;
            }
            case 122://从转盘抓取物体
            {
                catch_object(frame , codedata, fd, video);
                function = 0;
                serialFlush (fd);
                break;
            }
            case 5://识别红色
            {
                Mat thre;
                String mes;
                // imshow("1", frame);
                // Color_Detected_circle(frame, thre, 'R');
                objectplace_H(frame,mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    // printf("%2x ", (unsigned char)item);
                   serialPrintf(fd,"%c",item);
                }
                function = 0;
                serialFlush (fd);
                break;
            }
            case 6://识别绿色
            {
                Mat thre;
                String mes;
                // imshow("1", frame);
                // Color_Detected_circle(frame, thre, 'G');
                // imshow("2", thre);
                // waitKey(2);
                objectplace_H(frame,mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    // printf("%2x ", (unsigned char)item);
                   serialPrintf(fd,"%c",item);
                }
                function = 0;
                serialFlush (fd);
                break;
            }
            case 7://识别蓝色圆环
            {
                Mat thre;
                String mes;
                // imshow("1", frame);
                // Color_Detected_circle(frame, thre, 'B');
                // imshow("thre", thre);
                // waitKey(2);
                objectplace_H(frame,mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    // printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                function = 0;
                serialFlush (fd);
                break;
            }
            case 97://红
            {
                Mat thre;
                String mes;
                // frame = frame(Rect(0,100,640,380));
                // imshow("1", frame);

                // Color_Detected_circle(frame, thre, 'R');
                objectplace_H_2(frame, mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                function = 0;
                serialFlush (fd);
                break;
            }
            case 98://绿
            {
                Mat thre;
                String mes;
                // imshow("1", frame);
                // frame = frame(Rect(0,100,640,380));
                // Color_Detected_circle(frame, thre, 'G');
                objectplace_H_2(frame, mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                function = 0;
                serialFlush (fd);
                break;
            }
            case 99://蓝
            {
                Mat thre;
                String mes;
                // imshow("1", frame);
                // frame = frame(Rect(0,100,640,380));
                // Color_Detected_circle(frame, thre, 'B');
                objectplace_H_2(frame, mes, video);
                mes.push_back('+');
                mes.push_back((unsigned char)(1+abs(0)));
                for (const auto& item : mes) 
                {
                    printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                //cout << endl;
                function = 0;
                serialFlush (fd);
                break;
            }
            default:break;
        }
   
        while(serialDataAvail(fd) > 0)
        {
            // memset(r_buff, '\0', 128);
            recv = serialGetchar(fd);
            
            cout << "recv" << recv << endl;
            //第一次校�??1 第二次校�??2
            if(recv=='1')
            {
                function=1;
            }
            else if(recv=='2')
            {
                function=2;
            }
            else if(recv=='3')
            {
                function=3;
            }
            //读取二维码r
            else if(recv=='r')
            {
                function=72;
            }
            //粗加工区 �?5 �?6 �?7
            else if(recv=='5')
            {
                function=5;
            }  
            else if(recv=='6')
            {
                function=6;
            }        
            else if(recv=='7')
            {
                function=7;
            }              
            else if(recv=='z')    
            {
                function=122;
            }  
            else if(recv=='a')    
            {
                function=97;
            }  
            else if(recv=='b')    
            {
                function=98;
            }  
            else if(recv=='c')    
            {
                function=99;
            }  
            else
                function=0;
        }

    }
    return 0;
}
