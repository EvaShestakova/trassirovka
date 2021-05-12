#pragma once
#include "Figures.h"
class Screen
{
private:
	double cam[3];
	double normal[3];//единичная должна быть!
	double up[3];//единичная должна быть!
	double left[3];
	double a0; //screen
	double limit; //a0+a1
	double alpha;
	double wp, hp, w, h; //ширина высота в пикселях и обычная
	double light[3];
	double p; //размер пикселя
	double leftup[3]; //координата левого верхнего угла экрана
	double ***PC; //массив координат центров пикселей wp*hp*3
public:
	Screen();
	Screen(const string data);
	~Screen();
	void SetZero();
	void print();
	double* getcam() { return cam; }
	CImg<unsigned char> Image(vector<figure*> figures);//вот она основная функция которая возвращает картинку
};

