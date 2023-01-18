#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <cmath>
#include <direct.h>

using namespace std; //C++ 語法用
using namespace cv;

void convert3Dpoint_new(Point3f p, float d, Point3i &pd)
{
	float f = 4;
	Point center = Point(450 / 2, 375 / 2);
	//Point center = Point(1080 / 2, 1920 / 2);
	float sb = 3.66953*pow(10, -3);// mm轉pixel 像素尺寸								 
	float z = ((d*f) / (p.z*sb));////3d投影公式
	float px, py;
	if (p.x <= center.x&&p.y <= center.y){
		px = center.x - p.x;
		py = center.y - p.y;
	}
	if (p.x >= center.x&&p.y <= center.y){
		px = -(p.x - center.x);
		py = center.y - p.y;
	}
	if (p.x >= center.x&&p.y >= center.y){
		px = -(p.x - center.x);
		py = -(p.y - center.y);
	}
	if (p.x <= center.x&&p.y >= center.y){
		px = center.x - p.x;
		py = -(p.y - center.y);
	}

	float x = round((px *sb*z) / f);//new X Y Z
	float y = round((py *sb*z) / f);
	pd = Point3i(-x, -y, z);//回傳
}
int disparity_to_depth(float d, float dis){
	float f = 12;
	float sb = 3.66953*pow(10, -3);// mm轉pixel 像素尺寸							   
	int z = ((d*f) / (dis*sb));//

	return z;
}

void inverse_Z_to_dis(float z, float baseline, int &disparity){
	float f = 12;//??										
	float sb = 3.66953*pow(10, -3);// mm轉pixel 像素尺寸
	disparity = ((baseline * f) / (z*sb));//
}

void convert3Dpoint_edge_depth(Point3f p, float d, Point3i &pd)
{
	float f = 12;
	Point center = Point(1080 / 2, 1920 / 2);

	//float sb = 4.7*pow(10, -3);// mm轉pixel 像素尺寸	

	//float f = 12;//??
	//Point center = Point(1080 / 2, 1920 / 2);//undefine 540 960    1920 / 2,1080 / 2
	float sb = 3.66953*pow(10, -3);// mm轉pixel 像素尺寸
								   //3d投影公式
	float z = p.z;//
	float px, py;
	if (p.x <= center.x&&p.y <= center.y){
		px = center.x - p.x;
		py = center.y - p.y;
	}
	if (p.x >= center.x&&p.y <= center.y){
		px = -(p.x - center.x);
		py = center.y - p.y;
	}
	if (p.x >= center.x&&p.y >= center.y){
		px = -(p.x - center.x);
		py = -(p.y - center.y);
	}
	if (p.x <= center.x&&p.y >= center.y){
		px = center.x - p.x;
		py = -(p.y - center.y);
	}
	/*int x = round((p.x - center.x)*sb*z / f);
	int y = round((p.y - center.y)*sb*z / f);
	pd = Point3i(x, y, z);//回傳*/
	//new X Y Z
	float x = round((px *sb*z) / f);
	float y = round((py *sb*z) / f);
	pd = Point3i(-x, -y, z);//回傳

}

void inverse_xy(float X, float Y, float Z, Point2i &pd){
	float f = 12;//
	Point center = Point(1080 / 2, 1920 / 2);//undefine 540 960    1920 / 2,1080 / 2
	float sb = 3.66953*pow(10, -3);// mm轉pixel 像素尺寸						   
	int img_x = ((X*f) / (sb*Z)) + center.x;
	int img_y = ((Y*f) / (sb*Z)) + center.y;

	pd = Point2i(img_x, img_y);
}

string int2str(int &i) {
	string s;
	stringstream ss(s);
	ss << i;

	return ss.str();
}
