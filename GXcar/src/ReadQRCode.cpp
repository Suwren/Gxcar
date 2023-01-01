//
// Created by wsc on 2022/11/17.
//
#include "HeadFile.h"
/*
 * @brief 读取二维码信息
 * @param src 输入图片
 * @param data 二维码信息
 * */
void Read_Code(Mat &src,String &data)
{
    if(!src.empty()){//判断传入图片是否为空
        QRCodeDetector QRcode;
        data=QRcode.detectAndDecode(src);
    }
}
