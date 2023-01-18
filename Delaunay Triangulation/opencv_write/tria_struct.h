#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>

#define EPSILON 1e-4
typedef struct {
	int p1, p2, p3; //triangle three vertex
	double radius;
} ITRIANGLE;
typedef struct {
	int p1, p2; // edge start point and end point
} IEDGE;
typedef struct {
	int X, Y, Z=0;//3d data store volume
	double x, y;	
} XYZ;
struct T_Point {
	double x, y;
};