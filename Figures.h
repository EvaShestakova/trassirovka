#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <omp.h>
#include "CImg.h"
using namespace cimg_library;
using namespace std;

class figure
{
protected:
	double c[3];
	int color[3];
public:
	double* getc() { return c; }
	virtual void print() =0;
	figure() { c[0] = 0; c[1] = 0; c[2] = 0; color[0] = 0; color[1] = 0; color[2] = 0; }
	
	void SetColour(int colour[3]) { for (int i = 0; i < 3; ++i) { color[i] = colour[i]; } }
	
	
	void printxc() { cout << c[2]; }

	friend class Screen;
};

class sphere: public figure{
private:
	double R;
public:
	sphere() { R = 0; }
	sphere(double x, double y, double z, double r) { c[0] = x; c[1] = y; c[2] = z; R = r; }
	void print() {
		cout << c[0] << ' ' << c[1] << ' ' << c[2] << ' ' << R << endl;
	}
	friend class Screen;
};

class box : public figure {
private:
	double min[3];
	double max[3];
public:
	box() { for (int i = 0; i < 3; ++i) { min[i] = 0; max[i] = 0; } }
	box(double minn[3], double maxx[3]);
	void print() {
		cout << c[0] << ' ' << c[1] << ' ' << c[2] << ' ' << min[0] << ' ' << min[1] << ' ' << min[2] << ' ' << endl;
	}
	friend class Screen;
};

class tetra : public figure {
private:
	double v[4][3];
public:
	tetra();
	tetra(double vv[4][3]);
	void print() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 3; ++j) {
				cout << v[i][j] << ' ';
			}
			cout << endl;
		}
		//cout << c[0] << ' ' << c[1] << ' ' << c[2] << ' ' << endl;
	}
	friend class Screen;
};