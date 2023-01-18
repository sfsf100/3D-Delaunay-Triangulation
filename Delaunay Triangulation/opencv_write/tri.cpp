#include "tria_struct.h"
#include <opencv2/opencv.hpp>

using namespace cv;

double dot_sub(T_Point v1, T_Point v2) {
	return ((v2.x*v2.x) + (v2.y * v2.y)) - ((v1.x*v1.x) + (v1.y * v1.y));
}

double cross(T_Point v1, T_Point v2)// 外積運算，回傳純量（除去方向）
{
	return (v1.x*v2.y - v1.y*v2.x);
}
T_Point Vector_sub(T_Point v1, T_Point v2)
{
	T_Point V_sub;
	V_sub.x = v2.x - v1.x;
	V_sub.y = v2.y - v1.y;
	return V_sub;
}
T_Point Vector_add(T_Point v1, T_Point v2)
{
	T_Point V_add;
	V_add.x = v2.x + v1.x;
	V_add.y = v2.y + v1.y;
	return V_add;
}

bool CircumCircle_new(T_Point xp_p, T_Point p_1, T_Point p_2, T_Point p_3, T_Point &p_c, double &r) {

	//T_Point center;
	double radius;
	T_Point v1 = Vector_sub(p_1, p_2);  // p_2 -p_1
	T_Point v2 = Vector_sub(p_2, p_3); // p_3 -p_2

	double d1 = dot_sub(p_1, p_2);
	double d2 = dot_sub(p_2, p_3);
	double det = cross(v1, v2);  //  pair <p_2,p_1> -(p_3,p_2)
								 //if (det >= EPSILON){//formula
	p_c.x = (d1*v2.y - d2*v1.y) / (2 * det);
	p_c.y = (d2*v1.x - d1*v2.x) / (2 * det);
	T_Point tmp = Vector_sub(p_1, p_c);
	T_Point xp_sub = Vector_sub(p_c, xp_p); // p_3 -p_2
	r = (tmp.x*tmp.x + tmp.y*tmp.y);
	// 平方距離，避免開根號的誤差  sqrt
	double xp_radius = (xp_sub.x*xp_sub.x + xp_sub.y*xp_sub.y);
	//printf("%.2f\n", r);
	return((xp_radius - r) <= EPSILON ? true : false);
	/*}
	return false;*/
}




void Tag_milti_edge(IEDGE *edges, int nedge)
{
	for (int j = 0; j < nedge - 1; j++) {
		for (int k = j + 1; k < nedge; k++) {
			if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)) {
				edges[j].p1 = -1;
				edges[j].p2 = -1;
				edges[k].p1 = -1;
				edges[k].p2 = -1;
			}
			/* Shouldn't need the following, see note above */
			if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)) {
				edges[j].p1 = -1;
				edges[j].p2 = -1;
				edges[k].p1 = -1;
				edges[k].p2 = -1;
			}
		}
	}
}

void Form_new_tri(IEDGE *edges, ITRIANGLE *v, int i, int nedge, int trimax, int *ntri, int *complete)
{
	for (int j = 0; j < nedge; j++) {
		if (edges[j].p1 < 0 || edges[j].p2 < 0)
			continue;

		v[*ntri].p1 = edges[j].p1;
		v[*ntri].p2 = edges[j].p2;
		v[*ntri].p3 = i;
		complete[*ntri] = false;
		*(ntri)++;//三角形數量
	}
}
//


void super_triangle(XYZ *pxyz, int nv) {
	/*  Find the maximum and minimum vertex bounds.
	*/
	float xmin, xmax, ymin, ymax, xmid, ymid;
	float dmax;
	xmin = pxyz[0].x;
	ymin = pxyz[0].y;//已經排序過
	xmax = xmin;
	ymax = ymin;

	for (int a = 1; a < nv; a++) {
		if (pxyz[a].x < xmin) xmin = pxyz[a].x;
		if (pxyz[a].y < ymin) ymin = pxyz[a].y;
		if (pxyz[a].x > xmax) xmax = pxyz[a].x;
		if (pxyz[a].y > ymax) ymax = pxyz[a].y;
	}
	float dx = xmax - xmin;
	float dy = ymax - ymin;
	dmax = (dx > dy) ? dx : dy;
	xmid = (xmax + xmin) / 2.0;//超級三角形中心座標
	ymid = (ymax + ymin) / 2.0;
	/*
	First step : Set up the supertriangle*/

	//vertex list 
	pxyz[nv + 0].x = xmid - 20 * dmax;
	pxyz[nv + 0].y = ymid - dmax;
	
	pxyz[nv + 1].x = xmid;
	pxyz[nv + 1].y = ymid + 20 * dmax;
	
	pxyz[nv + 2].x = xmid + 20 * dmax;
	pxyz[nv + 2].y = ymid - dmax;
	
}



int Triangulate_1125(int nv, XYZ *pxyz, ITRIANGLE *v, int *ntri)
{
	int *complete = NULL;
	IEDGE *edges = NULL;
	int nedge = 0;//編號 第幾個三角形的邊 0~3
	int trimax, emax = 200;
	int status = 0;
	int inside;
	T_Point p_1, p_2, p_3, p_c, xp_p;
	double r;
	bool deter;
	/* Allocate memory for the completeness list, flag for each triangle */
	trimax = 4 * nv;
	if ((complete = (int *)malloc(trimax * sizeof(int))) == NULL) {//完成切割
		status = 1;
		goto skip;
	}

	/* Allocate memory for the edge list */
	if ((edges = (IEDGE*)malloc(emax*(long)sizeof(IEDGE))) == NULL) {
		status = 2;
		goto skip;
	}

	super_triangle(pxyz, nv);

	//triangle list  Note :  The supertriangle is the first triangle in the triangle list.(第一個三角形)
	v[0].p1 = nv; //n vertex 編號
	v[0].p2 = nv + 1;
	v[0].p3 = nv + 2;
	complete[0] = false;
	*ntri = 1;//Formed %d triangles 第幾個三角形
	for (int i = 0; i < nv; i++) {
		//current point
		xp_p.x = pxyz[i].x;
		xp_p.y = pxyz[i].y;
		nedge = 0;

		for (int j = 0; j < (*ntri); j++) {
			//printf("i = %d j = %d (*ntri) =%d %d\n", i, j, (*ntri), complete[j]);// (*ntri) 初始是 1
			if (complete[j])
				continue;
			//p_1, p_2, p_3,p_c,xp_p; 第一個點加入第一個三角形
			p_1.x = pxyz[v[j].p1].x; //第j個三角形 的Node 1
			p_1.y = pxyz[v[j].p1].y;
			p_2.x = pxyz[v[j].p2].x; //第j個三角形 的Node 2
			p_2.y = pxyz[v[j].p2].y;
			p_3.x = pxyz[v[j].p3].x; //第j個三角形 的Node 3
			p_3.y = pxyz[v[j].p3].y;
			//判斷是否在外接圓內
			bool inside = CircumCircle_new(xp_p, p_1, p_2, p_3, p_c, r);//check

			if (p_c.x<xp_p.x && ((xp_p.x - p_c.x)*(xp_p.x - p_c.x))>r)
				complete[j] = true;
			if (inside) {
				//deter = true;
				
				//printf("%d %d %.4f \n", cx, cy, r);
					if (nedge + 3 >= emax) {
						emax += 100;
						if ((edges = (IEDGE*)realloc(edges, emax*(long)sizeof(IEDGE))) == NULL) {
							status = 3;
							goto skip;
						}
					}
					edges[nedge + 0].p1 = v[j].p1;
					edges[nedge + 0].p2 = v[j].p2;
					edges[nedge + 1].p1 = v[j].p2;
					edges[nedge + 1].p2 = v[j].p3;
					edges[nedge + 2].p1 = v[j].p3;
					edges[nedge + 2].p2 = v[j].p1;
					nedge += 3;
					
					
					v[j] = v[(*ntri) - 1];	
					v[j].radius = r;

					complete[j] = complete[(*ntri) - 1];//標記為false
														//printf("j = %d (*ntri) =%d \n", j, (*ntri));
					(*ntri)--;
					j--;
				
			}
		}
		//Tag multiple edges   標記多條邊
		Tag_milti_edge(edges, nedge);

		for (int j = 0; j<nedge; j++) {
			if (edges[j].p1 < 0 || edges[j].p2 < 0)
				continue;
			if ((*ntri) >= trimax) {
				status = 4;
				goto skip;
			}
			//printf("tagged edges %d %d %d\n", j, edges[j].p1, edges[j].p2);
			//printf("*ntri tagged edges %d \n", *ntri);
			v[*ntri].p1 = edges[j].p1;
			v[*ntri].p2 = edges[j].p2;
			v[*ntri].p3 = i;//nv
			complete[*ntri] = false;
			(*ntri)++;
		}
	}
	/*
	Remove triangles with supertriangle vertices*/
	
	for (int i = 0; i < (*ntri); i++) {
		if (v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv) {
			v[i] = v[(*ntri) - 1];
			(*ntri)--;
			i--;
		}
	}

skip:
	free(edges);
	free(complete);
	return(status);
}
