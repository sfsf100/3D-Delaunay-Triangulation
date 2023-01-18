//
//#include <gl/freeglut.h>
//#include<math.h>
//#include <vector>
//#include <opencv2/opencv.hpp>
//#include <sys/timeb.h>
//#include <cstdlib>
//#include <stdio.h>
//
//#define scalefactor 1
//#define PI 3.14
//
//using namespace std;
//using namespace cv;
//
//int old_rot_x = 0;
//int old_rot_y = 0;
//
//int rot_x = 0;
//int rot_y = 0;
//
//int record_x = 0;
//int record_y = 0;
//
//float x_shift = 0;
//float y_shift = 0;
//float z_shift = 0;
//
//struct RGB_XYZ {
//	float X, Y, Z;
//	float R, G, B;
//};
//vector< RGB_XYZ > store;
//long long getSystemTime() {
//	struct timeb t;
//	ftime(&t);
//	return 1000 * t.time + t.millitm;
//}
//
//void display()
//{
//	glEnable(GL_DEPTH_TEST);
//
//	glClearColor(1, 1, 1, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glPolygonMode(GL_BACK, GL_LINE);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	gluLookAt(0, 0, 500, 0, 0, -2000, 0, 1, 0);
//
//	glTranslatef(x_shift, y_shift, z_shift);
//	glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);
//	glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);
//
//	float scale = scalefactor;
//	int x, y, z;
//	float extr[6];
//	//printf("%d", store.size());
//	for (int i = 0; i <store.size(); i++)//��Ʀ��  
//	{
//		
//		glColor3f(store[i].B/ 255, store[i].G/ 255, store[i].R/ 255); //normalize
//		glBegin(GL_QUADS);
//
//		x = store[i].X * scale;
//		y = -store[i].Y * scale;
//		z = -store[i].Z * scale;
//		//printf("x=%.1f , y=%.1f , z=%.1f\n",x,y,z);
//
//		// Voxel �����ߤ���
//		glVertex3f(x + (-0.5), y + (0.5), z + (0.5));  glVertex3f(x + (-0.5), y + (-0.5), z + (0.5));  glVertex3f(x + (0.5), y + (-0.5), z + (0.5)); glVertex3f(x + (0.5), y + (0.5), z + (0.5));
//
//		glVertex3f(x + (0.5), y + (0.5), z + (-0.5));  glVertex3f(x + (0.5), y + (-0.5), z + (-0.5));  glVertex3f(x + (-0.5), y + (-0.5), z + (-0.5)); glVertex3f(x + (-0.5), y + (0.5), z + (-0.5));
//
//		glVertex3f(x + (0.5), y + (0.5), z + (0.5)); glVertex3f(x + (0.5), y + (-0.5), z + (0.5)); glVertex3f(x + (0.5), y + (-0.5), z + (-0.5)); glVertex3f(x + (0.5), y + (0.5), z + (-0.5));
//
//		glVertex3f(x + (-0.5), y + (0.5), z + (-0.5)); glVertex3f(x + (-0.5), y + (-0.5), z + (-0.5)); glVertex3f(x + (-0.5), y + (-0.5), z + (0.5)); glVertex3f(x + (-0.5), y + (0.5), z + (0.5));
//
//		glVertex3f(x + (-0.5), y + (0.5), z + (-0.5)); glVertex3f(x + (-0.5), y + (0.5), z + (0.5)); glVertex3f(x + (0.5), y + (0.5), z + (0.5)); glVertex3f(x + (0.5), y + (0.5), z + (-0.5));
//
//		glVertex3f(x + (-0.5), y + (-0.5), z + (0.5)); glVertex3f(x + (-0.5), y + (-0.5), z + (-0.5)); glVertex3f(x + (0.5), y + (-0.5), z + (-0.5)); glVertex3f(x + (0.5), y + (-0.5), z + (0.5));
//
//		glEnd();
//	}
//
//	glutSwapBuffers();
//}
//
//void Keyboard(unsigned char key, int x, int y)
//{
//	//printf("�A�ҫ����䪺�X�O%x\t���ɵ��������ƹ��y�ЬO(%d,%d)\n", key, x, y);
//	switch (key)
//	{
//	case 'w':
//		y_shift += 100 * scalefactor;
//		break;
//	case 's':
//		y_shift -= 100 * scalefactor;
//		break;
//	case 'd':
//		x_shift += 100 * scalefactor;
//		break;
//	case 'a':
//		x_shift -= 100 * scalefactor;
//		break;
//	case 'q':
//		z_shift += 100 * scalefactor;
//		break;
//	case 'e':
//		z_shift -= 100 * scalefactor;
//		break;
//	case 'z':
//		break;
//	case 27:
//
//		exit(0);
//		break;
//	}
//	glutPostRedisplay();
//}
//
//void Mouse(int button, int state, int x, int y)
//{
//	if (state) {
//		record_x += x - old_rot_x;
//		record_y += y - old_rot_y;
//
//		rot_x = 0;
//		rot_y = 0;
//	}
//	else {
//		old_rot_x = x;
//		old_rot_y = y;
//	}
//}
//
//void MotionMouse(int x, int y)
//{
//	rot_x = x - old_rot_x;
//	rot_y = y - old_rot_y;
//	glutPostRedisplay();
//}
//
//void WindowSize(int w, int h)
//{
//	printf("�ثe�����j�p��%dX%d\n", w, h);
//	glViewport(0, 0, w, h);            //��������e���ܮɡA�e���]�����
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	float rate = (float)w / (float)h;
//	//glOrtho(-150 * rate, 150 * rate, -200, 200, -1000, 3000);      //�����v
//	gluPerspective(45, rate, 1.0, 74000.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
////Read 3D Voxel data
//void main(int argc, char * argv[])//
//{
//	glutInit(&argc, argv);//��l��
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//single is 2D
//
//	glutInitWindowPosition(900, 900);
//	glutInitWindowSize(800, 800);//size window
//	glutCreateWindow("OpenGL cube");//���D�W��
//	glutReshapeFunc(WindowSize);
//	glutKeyboardFunc(Keyboard);
//	glutMouseFunc(Mouse);
//	glutMotionFunc(MotionMouse);
//	RGB_XYZ  temp;
//	float extr[6];
//	ifstream infile;//�w�qŪ���ɮ׬y�A�۹��{���ӻ��Oin
//	infile.open("D://XYZ_RGB_DATA_11_17.txt");//�}���ɮ�
//	for (int j = 0; j < 392300; j++)//X Y Z R G B
//	{
//		for (int j = 0; j < 6; j++)//X Y Z R G B
//			infile >> extr[j];//Ū���@�ӭȡ]�Ů�B�s��šB����j�}�^�N�g�J��x�}���A��C���_�j��i��
//		temp.X= extr[0];
//		temp.Y = extr[1];
//		temp.Z = extr[2];
//		temp.R = extr[5];
//		temp.G = extr[4];
//		temp.B = extr[3];
//		store.push_back(temp);
//	}
//	infile.close();//Ū���������������ɮ�
//
//	glutDisplayFunc(display);//��J�n��ܪ��ϡAcan put function
//
//	glutMainLoop();//�T�O����B��
//	system("pause");
//}
//
