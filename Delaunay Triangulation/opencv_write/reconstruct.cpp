#include <opencv2/opencv.hpp>
#include<gl/GLUT.H>
#include <fstream>
#include <direct.h>
#include<cmath>
#include <time.h>
#include "tria_struct.h"

#define scalefactor 1

using namespace std; 
using namespace cv;

int compare(const void *a, const void *b);
void convert3Dpoint_edge_depth(Point3f p, float d, Point3i &pd);
int XYZCompare(const void *, const void *);
int Triangulate_1125(int nv, XYZ *pxyz, ITRIANGLE *v, int *ntri);
void convert3Dpoint_new(Point3f p, float d, Point3i &pd);

int old_rot_x = 0, old_rot_y = 0;
int rot_x = 0, rot_y = 0;
int record_x = 0, record_y = 0;

float x_shift = 0, y_shift = 0, z_shift = 0;
struct Tri_Node {
	unsigned char R, G, B;
	short T1_n1, T1_n2;	
	short T2_n1, T2_n2;
	short T3_n1, T3_n2;
	int T1_n3, T2_n3, T3_n3;
};
vector<Tri_Node> tri_node;

void Display_triangle()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1.0);//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 0, 0, 0, -2000, 0, 1, 0);/**/

	glTranslatef(x_shift, y_shift, z_shift);
	glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);
	glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);

	float scale = scalefactor;
	int x, y, z;
	int x1, y1, z1;
	int x2, y2, z2;
	int scale_v = 4;
	//srand(time(NULL));
	for (int n = 0; n < tri_node.size(); n++)
	{	
		float r = tri_node[n].R / 255.0;
		float g = tri_node[n].G / 255.0;
		float b = tri_node[n].B / 255.0;
		/*printf("%d %d %d\n", tri_node[n].R, tri_node[n].G, tri_node[n].B);
		printf("%.4f %.4f %.4f\n", r,g,b);*/

		x = tri_node[n].T1_n1;
		y = -tri_node[n].T1_n2;
		z = -tri_node[n].T1_n3;

		x1 = tri_node[n].T2_n1;
		y1 = -tri_node[n].T2_n2;
		z1 = -tri_node[n].T2_n3;

		x2 = tri_node[n].T3_n1;
		y2 = -tri_node[n].T3_n2;
		z2 = -tri_node[n].T3_n3;

		// mode change
		//glBegin(GL_LINE_LOOP);//
		glBegin(GL_TRIANGLES);//
		glColor3f(b, g, r);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x, y, z);
		glEnd();
		//printf("%d %d %d %d %d %d\n", m_x[0], m_y[0], m_x[1], m_y[1], m_x[2], m_y[2]);	
		//Mat imgR = imread("D://img1R.png", IMREAD_COLOR);	
	}

	glutSwapBuffers();
}


void Keyboard(unsigned char key, int x, int y)
{
	//printf("你所按按鍵的碼是%x\t此時視窗內的滑鼠座標是(%d,%d)\n", key, x, y);
	switch (key) {
	case 'w':	y_shift += 100 * scalefactor;
		break;
	case 's':	y_shift -= 100 * scalefactor;	
		break;
	case 'd':	x_shift += 100 * scalefactor;
		break;
	case 'a':	x_shift -= 100 * scalefactor;	
		break;
	case 'q':	z_shift += 100 * scalefactor;	
		break;
	case 'e':	z_shift -= 100 * scalefactor;	
		break;
	case 'z':
		break;
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (state) {
		record_x += x - old_rot_x;
		record_y += y - old_rot_y;

		rot_x = 0;
		rot_y = 0;
	}
	else {
		old_rot_x = x;
		old_rot_y = y;
	}
}

void MotionMouse(int x, int y)
{
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
}
void WindowSize(int w, int h){
	printf("目前視窗大小為%dX%d\n", w, h);
	glViewport(0, 0, w, h);            //當視窗長寬改變時，畫面也跟著變
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float rate = (float)w / (float)h;
	//glOrtho(-150 * rate, 150 * rate, -200, 200, -1000, 3000);      //正交投影
	gluPerspective(45, rate, 1.0, 10000.0); //105000
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void inverse(Mat edge, Mat &result){
	result = Mat::zeros(edge.rows, edge.cols, CV_8U);
	for (int i = 0; i < edge.rows; ++i) {
		for (int j = 0; j < edge.cols; ++j) {
			if (edge.at<uchar>(i, j) == 255)
				result.at<uchar>(i, j) = 0;
			else
				result.at<uchar>(i, j) = 255;
		}
	}
}

void tri_mesh()
{
	int ntri = 0,nv = 0;
	double x, y, z;
	ITRIANGLE *v;
	XYZ *p = NULL;
	Mat source = imread("D://im2.png",1);//im2.png
	Mat disp = imread("D://result.png",-1);

	Mat cdst, gray, dst, blur;
	cvtColor(source, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, blur, Size(5, 5), 1.5);
	Canny(blur, dst, 30, 40, 3);
	int counter = 0;
	int note_ex=0;

	Point3i pd;
	for (int i = 1; i < dst.rows - 21; i++) {
		for (int j = 1; j < dst.cols - 1; j++) {/**/

			if (dst.at<uchar>(i, j) == 255&& disp.at<ushort>(i, j)>0 ) {// 
				note_ex = abs(j - note_ex);// 
				if (note_ex>10) {
					note_ex = j;
					
					counter++;
					p = (XYZ*)realloc(p, (nv + 1) * sizeof(XYZ));
					convert3Dpoint_new(Point3f(j, i, disp.at<ushort>(i,j)), 60, pd);
					/*printf("%d %d %d %d\n", note_ex, note_ey,j, i);*///
					p[nv].x = j;
					p[nv].y = i;
					p[nv].X = pd.x;
					p[nv].Y = pd.y;
					p[nv].Z = pd.z;
					nv++;
				}				
			}		
		}
	}
	//printf("counter= %d\n", counter);
	//imshow("edge", dst);

	p = (XYZ*)realloc(p, (nv + 3) * sizeof(XYZ));
	v = (ITRIANGLE*)malloc(3 * nv * sizeof(ITRIANGLE));
	qsort(p, nv, sizeof(XYZ), XYZCompare);////

	Triangulate_1125(nv, p, v, &ntri);//
	int cx, cy;	  printf("%d \n", ntri);
	Tri_Node temp;
	Mat result_img;

	inverse(dst, result_img); imwrite("Canny_inverse.png", result_img);//// //
	//imshow("edge_before", result_img);//
	cvtColor(result_img, result_img, COLOR_GRAY2BGR);
	srand(time(NULL));/**/
	int v_p1, v_p2, v_p3;
	int xx, yy, zz;   /* Write triangles in geom format  */
	for (int i = 0; i<ntri; i++) {
		v_p1= v[i].p1, v_p2= v[i].p2, v_p3= v[i].p3;
		if (p[v[i].p1].Z != 0 && p[v[i].p2].Z != 0 &&p[v[i].p3].Z != 0) {//
			temp.T1_n1 = p[v_p1].X; temp.T1_n2 = p[v_p1].Y; temp.T1_n3 = p[v_p1].Z;
			temp.T2_n1 = p[v_p2].X; temp.T2_n2 = p[v_p2].Y; temp.T2_n3 = p[v_p2].Z;
			temp.T3_n1 = p[v_p3].X; temp.T3_n2 = p[v_p3].Y; temp.T3_n3 = p[v_p3].Z;//(+ p[v[i].p2].x + p[v[i].p3].x) / 3
			xx = p[v_p1].x , yy= p[v_p1].y;//(+ p[v[i].p2].y+ p[v[i].p3].y)/3
			//printf("%d %.2f \n", i, v[i].radius);
			int rr = v[i].radius; 
			
			if (abs(temp.T3_n3 - temp.T2_n3) > 400 || abs(temp.T3_n3 - temp.T1_n3) > 400|| abs(temp.T2_n3 - temp.T1_n3)>400)//距離分佈將前景與背景區隔開 test by you
				continue;
			
				unsigned char R = source.at<Vec3b>(yy, xx)[0];
				unsigned char G = source.at<Vec3b>(yy, xx)[1];
				unsigned char B = source.at<Vec3b>(yy, xx)[2];
			
				int Rc = rand() % (255 + 1) + 10;
				int Bc = rand() % (255 + 1) + 10;
				int Gc = rand() % (255 + 1) + 10;
				
				line(result_img, Point(p[v_p1].x, p[v_p1].y), Point(p[v_p2].x, p[v_p2].y), Scalar(Bc, Gc, Rc), 1, 8, 0);
				line(result_img, Point(p[v_p1].x, p[v_p1].y), Point(p[v_p3].x, p[v_p3].y), Scalar(Bc, Gc, Rc), 1, 8, 0);
				line(result_img, Point(p[v_p2].x, p[v_p2].y), Point(p[v_p3].x, p[v_p3].y), Scalar(Bc, Gc, Rc), 1, 8, 0);
				temp.R = R;
				temp.G = G; 
				temp.B = B; 

				tri_node.push_back(temp);//
		}
	}

	imwrite("tri_depth_1212_.png", disp);
	waitKey();
	free(v);
	free(p);

}
int compare(const void *a, const void *b);

int XYZCompare(const void *v1, const void *v2)
{
	XYZ *p1, *p2;
	p1 = (XYZ*)v1; //
	p2 = (XYZ*)v2;
	int ret = (p1->x) - p2->x;
	if (ret>0) return 1;
	if (ret<0) return -1;
}
int compare(const void *a, const void *b)//這函式是 qsort 所需的比較函式
{
	int c = *(int *)a;
	int d = *(int *)b;
	if (c < d) { return -1; }               //傳回 -1 代表 a < b
	else if (c == d) { return 0; }      //傳回   0 代表 a = b
	else return 1;                          //傳回  1 代表 a>b
}

int main(int argc, char * argv[]){
	glutInit(&argc, argv);//初始化
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//single is 2D
	
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 700);//size window
	glutCreateWindow("OpenGL cube");//標題名稱
	glutReshapeFunc(WindowSize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MotionMouse);
	double time1 = static_cast<double>(getTickCount());
	tri_mesh();   printf("finish\n");
	time1 = ((double)getTickCount() - time1) / getTickFrequency(); //計時器结束
	printf("CPU所運算的時間：%f s\n", time1);//
	glutDisplayFunc(Display_triangle);//放入要顯示的圖，can put function
	
	glutMainLoop();//確保持續運行
	system("pause");//
	return 0;
}