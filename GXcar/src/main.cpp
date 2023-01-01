#include "HeadFile.h"
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string>
#include <unistd.h>

int main()
{
    wiringPiSetup();//初始化串�?
	int fd = serialOpen("/dev/ttyAMA0",115200); //打开串口
    int sz = serialDataAvail(fd); //串口缓冲区数据个�?

    int function=0;//树莓派需要调用的功能序号
    double distance=0, angle=0;//line_detect直线检测函数的返回�?
    
    Mat frame;//摄像头拍到的原始图像
    char recv;//串口数据储存数组

    VideoCapture video(0);
    while(1)
    {
        video >> frame;
        // imshow("1", frame);
        // waitKey(1);
        // cout << "function" << function << endl;
        switch(function)
        {
            case 1://校对x方向
            {
                string message;
                line_detect(frame,distance,angle);
                message.push_back('m');
                message.push_back((unsigned char)abs(50));
                message.push_back((unsigned char)abs(0.3202*distance-9.087));
                if(angle>=0)
                    message.push_back('-');
                else
                    message.push_back('+');
                message.push_back((unsigned char)(1+abs(angle)));
                for (const auto& item : message) 
                {
                    printf("%2x ", (unsigned char)item);
                    // serialPrintf(fd,"%c",item);
                }
                cout << endl;
                function = 0;
                break;
            }
            case 2://校对y方向
            {
                string message;//����32����Ϣ
                line_detect(frame,distance,angle);
                message.push_back('m');
                message.push_back((unsigned char)abs(-distance*0.299+104.75));
                message.push_back((unsigned char)abs(50));
                if(angle>=0)
                    message.push_back('-');
                else
                    message.push_back('+');
                message.push_back((unsigned char)(1+abs(angle+1)));
                for (const auto& item : message) 
                {
                    // printf("%2x ", (unsigned char)item);
                    serialPrintf(fd,"%c",item);
                }
                cout << endl;
                break;
            }
            case 10://识别二维�?
            {
                string message;//发给32的信�?
                readcode(frame, message);
                serialPrintf(fd,message.c_str());
                function = 0;
                break;
            }
            default:break;
        }
   
        while(serialDataAvail(fd) > 0)
        {
            // memset(r_buff, '\0', 128);
            recv = serialGetchar(fd);
            cout << "recv" << recv << endl;
            if(recv=='1')
            {
                function=1;
            }
            else if(recv=='2')
            {
                function=2;
            }
            else if(recv=='r')
            {
                function=10;
            }
            else
                function=0;
        }
        
    }
    return 0;
}