#include "Figures.h"
#include "Screen.h"
void vect(double t[3], double a[3], double b[3]);



int main() {
	unsigned char color[3];
	color[0] = 255;
	color[1] = 0;
	color[2] = 0;
	/*CImg<unsigned char> img(10, 10, 1, 3);
	img.fill(0);
	
	img.draw_point(1,0, color);
	img.display("My first CImg code");*/
	//sphere a;
	//a.printxc();
    try {
		Screen screen("data.txt"); 
		//screen.print();

		vector<figure*> figures; //вектор фигур
		ifstream fin("figures.txt"); 
		if (!fin.is_open()) {
			cout << "Error! Cannot open\n";
			throw 1;
		}
		string s;
		string word;
		double x, y, z, r;
		int n=0;
		double minn[3];
		double maxx[3];
		double vv[4][3];
		while (!fin.eof())
		{
			getline(fin, s);
			n++;
		}
		fin.seekg(0, ios_base::beg);
		fin.close();
		fin.open("figures.txt");
		figures.resize(n);
		double dmin = 10000000;
		double dmax = 0;
		double* dd;
		double dist;
		double* cam = screen.getcam();
		for(int i=0;i<n;++i) {
			getline(fin, s);
			stringstream st;
			st << s;
			st >> word;
			if (word == "sphere") {
				st >> x;
				st >> y;
				st >> z;
				st >> r;
				figures[i] = new sphere(x,y,z,r);
			}
			if (word == "box") {
				for (int j = 0; j < 3; ++j) { st >> minn[j]; }
				for (int j = 0; j < 3; ++j) { st >> maxx[j]; }
				figures[i] = new box(minn,maxx);
			}
			if (word == "tetra") {
				for (int j = 0; j < 4; ++j) {
					for (int k = 0; k < 3; ++k) {
						st >> vv[j][k];
					}
				}
				figures[i] = new tetra(vv);
			}
			dd = figures[i]->getc();
			if (sqrt(pow(dd[0] - cam[0], 2) + pow(dd[1] - cam[1], 2) + pow(dd[2] - cam[2], 2)) < dmin) {
				dmin = sqrt(pow(dd[0] - cam[0], 2) + pow(dd[1] - cam[1], 2) + pow(dd[2] - cam[2], 2));
			}
			if (sqrt(pow(dd[0] - cam[0], 2) + pow(dd[1] - cam[1], 2) + pow(dd[2] - cam[2], 2)) > dmax) {
				dmax = sqrt(pow(dd[0] - cam[0], 2) + pow(dd[1] - cam[1], 2) + pow(dd[2] - cam[2], 2));
			}
		}
		fin.close();
		//cout << dmax << " " << dmin;
		int color[3];
		for (int i = 0; i < n; ++i) {
			dd = figures[i]->getc();
			dist = sqrt(pow(dd[0] - cam[0], 2) + pow(dd[1] - cam[1], 2) + pow(dd[2] - cam[2], 2));
			for (int k = 0; k < 3; ++k) {
				color[k] = 191 * ((dist - dmin) / (dmax - dmin)) + 64 * ((dist - dmax) / (dmin - dmax));
			}
			cout << color[0] << endl;
			figures[i]->SetColour(color);
		}
		screen.Image(figures).display("");  // здесь все происходит
		
		for (int i = 0; i < n; ++i) { delete figures[i]; }
        return 0;
    }
    catch (int err) { printf("error=%d\n", err); }
	return 0;
}