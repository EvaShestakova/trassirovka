#include "Figures.h"

box::box(double minn[3], double maxx[3]) {
	for (int i = 0; i < 3; ++i) {
		min[i] = minn[i];
		max[i] = maxx[i];
		c[i] = (min[i] + max[i]) / 2;
	}
}

tetra::tetra() {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			v[i][j] = 0;
		}
	}
}
tetra::tetra(double vv[4][3]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			v[i][j] = vv[i][j];
		}
	}
	for (int i = 0; i < 3; ++i) {
		c[i] = (vv[0][i] + vv[1][i] + vv[2][i] + vv[3][i]) / 4;
	}
}