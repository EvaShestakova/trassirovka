#pragma once
#include "Figures.h"
class Screen
{
private:
	double cam[3];
	double normal[3];//��������� ������ ����!
	double up[3];//��������� ������ ����!
	double left[3];
	double a0; //screen
	double limit; //a0+a1
	double alpha;
	double wp, hp, w, h; //������ ������ � �������� � �������
	double light[3];
	double p; //������ �������
	double leftup[3]; //���������� ������ �������� ���� ������
	double ***PC; //������ ��������� ������� �������� wp*hp*3
public:
	Screen();
	Screen(const string data);
	~Screen();
	void SetZero();
	void print();
	double* getcam() { return cam; }
	CImg<unsigned char> Image(vector<figure*> figures);//��� ��� �������� ������� ������� ���������� ��������
};

