
//#include <fstream>
//#include "tria_struct.h"
//int Triangulate(int nv, XYZ *pxyz, ITRIANGLE *v, int *ntri);
//int Triangulate_1125(int nv, XYZ *pxyz, ITRIANGLE *v, int *ntri);
//int XYZCompare(const void *, const void *);
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <time.h>
//#include <cmath>
//#include <direct.h>
//
//using namespace std; //C++ 語法用
//using namespace cv;
//
//
//void inverse(Mat edge, Mat &result)
//{
//	result = Mat::zeros(edge.rows, edge.cols, CV_8U);
//	for (int i = 0; i < edge.rows; ++i) {
//		for (int j = 0; j < edge.cols; ++j) {
//			if (edge.at<uchar>(i, j) == 255)
//				result.at<uchar>(i, j) = 0;
//			else
//				result.at<uchar>(i, j) = 255;
//		}
//	}
//}
//
//
//
//void main()
//{
//	int i;
//	int ntri = 0;
//	double x, y, z;
//	ITRIANGLE *v;
//	XYZ *p = NULL;
//	int nv = 0;
//	FILE *fptr;
//	Mat source = imread("D://im2.png");
//	Mat cdst, gray, dst, blur;
//	cvtColor(source, gray, COLOR_BGR2GRAY);
//	GaussianBlur(gray, blur, Size(5, 5), 1.5);
//	Canny(blur, dst, 40, 100, 3);
//	int counter = 0;
//	int r = rand() % (250 + 1) + 10;
//	srand(time(NULL));
//	for (int i = 1; i < dst.rows - 1; i++) {
//		for (int j = 1; j < dst.cols - 1; j++) {
//			if (dst.at<uchar>(i, j) == 255) {
//				r = rand() % (255 + 1) + 10;
//				if (r > 105) {
//					counter++;
//					p = (XYZ*)realloc(p, (nv + 1) * sizeof(XYZ));
//					p[nv].x = j;
//					p[nv].y = i;
//
//					nv++;
//				}	
//				else
//					dst.at<uchar>(i, j) = 0;
//			}
//		}
//	}
//	printf("counter= %d\n", counter);
//	imshow("edge", dst);
//
//
//	//if ((fptr = fopen("D:\\abc.txt", "r")) == NULL) {
//	//	//fprintf(stderr, "Failed to open file \"%s\"\n", argv[1]);
//	//	/*exit(-1);*/
//	//	printf("no\n");
//	//}
//	/*while (fscanf(fptr, "%lf %lf %lf", &x, &y, &z) == 3) {
//	p = (XYZ*)realloc(p, (nv + 1) * sizeof(XYZ));
//	p[nv].x = x;
//	p[nv].y = y;
//	p[nv].z = z;
//	nv++;
//	}
//	fprintf(stderr, "Read %d points\n", nv);*/
//	//if (nv < 3)
//	//	exit(-1);
//	////fclose(fptr);
//	//printf("nv %d\n", nv);
//
//	p = (XYZ*)realloc(p, (nv + 3) * sizeof(XYZ));
//
//	v = (ITRIANGLE*)malloc(3 * nv * sizeof(ITRIANGLE));
//	qsort(p, nv, sizeof(XYZ), XYZCompare);////
//
//	for (int i = 1; i < nv - 1; i++) {
//		printf("i = %d %.2f %.2f \n", i, p[i].x, p[i].y);
//	}
//	////Triangulate(nv, p, v, &ntri);
//	Triangulate_1125(nv, p, v, &ntri);//
//	//fprintf(stderr, "Formed %d triangles\n", ntri);
//	////輸出存取
//	//FILE* Write = fopen("triangle_.txt", "w");
//
//	Mat result_img;
//	/*imwrite("canny.png", dst);*/
//	inverse(dst, result_img);
//	imshow("edge_before", result_img);
//	cvtColor(result_img, result_img, COLOR_GRAY2BGR);
//	///* Write triangles in geom format */
//	for (i = 0; i<ntri; i++) {
//		//fprintf(Write, "%g %g %g %g %g %g\n", p[v[i].p1].x, p[v[i].p1].y, p[v[i].p2].x, p[v[i].p2].y, p[v[i].p3].x, p[v[i].p3].y);
//		//printf("f3 %g %g %g %g %g %g %g %g %g 1 1 1\n",
//		//p[v[i].p1].x, p[v[i].p1].y, p[v[i].p1].z,
//		//p[v[i].p2].x, p[v[i].p2].y, p[v[i].p2].z,p[v[i].p3].x, p[v[i].p3].y, p[v[i].p3].z);/**/
//		
//		line(result_img, Point(p[v[i].p1].x, p[v[i].p1].y), Point(p[v[i].p2].x, p[v[i].p2].y), cv::Scalar(0, 0, 0), 1, 8, 0);
//
//		line(result_img, cv::Point(p[v[i].p1].x, p[v[i].p1].y), cv::Point(p[v[i].p3].x, p[v[i].p3].y), cv::Scalar(0, 0, 0), 1, 8, 0);
//		line(result_img, cv::Point(p[v[i].p2].x, p[v[i].p2].y), cv::Point(p[v[i].p3].x, p[v[i].p3].y), cv::Scalar(0, 0, 0), 1, 8, 0);
//		printf("f3 %g %g  %g %g  %g %g  1 1 1\n",
//			p[v[i].p1].x, p[v[i].p1].y,
//			p[v[i].p2].x, p[v[i].p2].y,
//			p[v[i].p3].x, p[v[i].p3].y);/**/
//	}//
//
//
//	imshow("edge", result_img);
//	waitKey();
//	//fclose(Write);
//	/*free(v);
//	free(p);*/
//	//exit(0);
//	system("pause");
//	//return 0;
//}
////
//int XYZCompare(const void *v1, const void *v2)
//{
//	XYZ *p1, *p2;
//	p1 = (XYZ*)v1; //
//	p2 = (XYZ*)v2;
//	/*if (p1->x < p2->x)
//	return(-1);
//	else if (p1->x > p2->x)
//	return(1);
//	else
//	return(0);*/
//
//	int ret = (p1->x) - p2->x;
//	if (ret>0) return 1;
//	if (ret<0) return -1;
//}