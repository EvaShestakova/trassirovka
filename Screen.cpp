#include "Screen.h"
bool inside(double A1[3], double A2[3], double A3[3], double B[3]);
double mod(double a[3]);
void vect(double t[3], double a[3], double b[3]);
double scal(double a[3], double b[3]);

Screen::Screen()
{
    SetZero();
}

Screen::Screen(const string data)
{
    ifstream fin(data);
    if (!fin.is_open()) {
        cout << "Error! Cannot open\n";
        throw 1;
    }
    string s;
    fin >> s;
    for (int i = 0; i < 3; ++i) { fin >> cam[i]; }
    fin >> s;
    for (int i = 0; i < 3; ++i) { fin >> normal[i]; }
    double temp = 0;
    temp = mod(normal);
    for (int i = 0; i < 3; ++i) { normal[i] = normal[i] / temp; }
    fin >> s;
    for (int i = 0; i < 3; ++i) { fin >> up[i]; }
    temp = mod(up);
    for (int i = 0; i < 3; ++i) { up[i] = up[i] / temp; }
    fin >> s;
    fin >> a0;
    fin >> s;
    fin >> limit;
    fin >> s;
    fin >> alpha;
    fin >> s;
    fin >> wp;
    fin >> s;
    fin >> hp;
    fin >> s;
    h = 2 * a0 * tan((alpha / 2) * M_PI / 180);
    p = h / hp;
    w = p * wp;
    left[0] = up[1] * normal[2] - up[2] * normal[1];
    left[1] = up[2] * normal[0] - up[0] * normal[2];
    left[2] = up[0] * normal[1] - up[1] * normal[0];
    leftup[0] = cam[0]+a0 * normal[0] + h / 2 * up[0] + w / 2 * left[0];
    leftup[1] = cam[1]+a0 * normal[1] + h / 2 * up[1] + w / 2 * left[1];
    leftup[2] = cam[2]+a0 * normal[2] + h / 2 * up[2] + w / 2 * left[2];
    PC = new double** [hp];
    for (int i = 0; i < hp; i++) {
        PC[i] = new double* [wp];
        for (int j = 0; j < wp; j++) {
            PC[i][j] = new double[3];
        }
    }
    for (int i = 0; i < 3; ++i) { 
        PC[0][0][i] = leftup[i] - left[i] * p  - up[i] * p ;
    }
    for (int m = 1; m < hp; ++m) {
        for (int i = 0; i < 3; ++i) {
                PC[m][0][i] = PC[m - 1][0][i] - up[i] * p ;
            }
    }
    for (int m = 0; m < hp; ++m) {
        for (int n = 1; n < wp; ++n) {
            for (int i = 0; i < 3; ++i) {
                PC[m][n][i] = PC[m][n-1][i]-left[i]*p;
                //if ((m == 479) && (n == 639)) { cout << PC[m][n][i]; }
            }
        }
    }
    for (int i = 0; i < 3; ++i) { fin >> light[i]; }
    fin.close();
}

Screen::~Screen()
{
    for (int i = 0; i < hp; i++) {
        for (int j = 0; j < wp; j++) {
            delete[] PC[i][j];
        }
        delete[] PC[i];
    }
    delete[] PC;
    
}

void Screen::SetZero()
{
    for (int i = 0; i < 3; ++i) { cam[i] = 0; }
    for (int i = 0; i < 3; ++i) { normal[i] = 0; }
    for (int i = 0; i < 3; ++i) { up[i] = 0; }
    for (int i = 0; i < 3; ++i) { left[i] = 0; }
    a0 = 0;
    limit = 0;
    alpha = 0;
    wp = 0;
    hp = 0;
    h = 0;
    p = 0;
    w = 0;
    leftup[0] = 0;
    leftup[1] = 0;
    leftup[2] = 0;
    PC = nullptr;
    for (int i = 0; i < 3; ++i) { light[i] = 0; }
}

void Screen::print()
{
    for (int i = 0; i < 3; ++i) { cout << cam[i]<<' '; }
    cout << '\n';
    for (int i = 0; i < 3; ++i) { cout << normal[i] << ' '; }
    cout << '\n';
    for (int i = 0; i < 3; ++i) { cout << up[i] << ' '; }
    cout << '\n';
    for (int i = 0; i < 3; ++i) { cout << left[i] << ' '; }
    cout << '\n';
    cout << a0 << '\n';
    cout << limit << '\n';
    cout << alpha << '\n';
    cout << wp << '\n';
    cout << hp << '\n';
    cout << w << '\n';
    cout << h << '\n';
    for (int i = 0; i < 3; ++i) { cout << light[i] << ' '; }
    cout << '\n';
    cout << p << '\n';
    for (int i = 0; i < 3; ++i) { cout << left[i] << ' '; }
    cout << '\n';
    for (int i = 0; i < 3; ++i) { cout << leftup[i] << ' '; }
    cout << '\n';
   for (int m = 0; m < 5; ++m) {
        for (int n = 0; n < 5; ++n) {
            for (int i = 0; i < 3; ++i) {
                cout << PC[m][n][i];
            }
            cout << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
    cout << '\n';
    for (int m = hp-5; m < hp; ++m) {
        for (int n = wp-5; n < wp; ++n) {
            for (int i = 0; i < 3; ++i) {
                cout << PC[m][n][i];
            }
            cout << ' ';
        }
        cout << '\n';
    }
   /* for (int m = 0; m < hp; ++m) {
            for (int i = 0; i < 3; ++i) {
                cout << PC[m][0][i];
        }
        cout << '\n';
    }*/
}

CImg<unsigned char> Screen::Image(vector<figure*> figures)
{
    CImg<unsigned char> img(wp,hp, 1, 3); 
    img.fill(0);//фон
    double a[3];
    double b[3];
    double c[3]; 
    double n[3];//нормаль к поверхности
    double l[3];//вектор к источнику света
    double t=0;//параметр на прямой 
    double D = 0;//D/2 дискриминант пополам или коэффициент в уравнении плоскости
    double r[3];//точка на фигуре
    unsigned char tempcolor[3];
    unsigned char black[3];
    double temp=0;
    int* color;
    black[0] = 0;
    black[1] = 0;
    black[2] = 0;
    b[0] = cam[0];
    b[1] = cam[1];
    b[2] = cam[2];
#pragma omp parallel for
   for (int i = 0; i < hp; ++i) {
        for (int j = 0; j < wp; ++j) {
            for (int m = 0; m < figures.size(); ++m) {
                    double x0 = (*(figures[m])).c[0];
                    double y0 = (*(figures[m])).c[1];
                    double z0 = (*(figures[m])).c[2];
                    for (int k = 0; k < 3; ++k) {
                        a[k] = PC[i][j][k];
                        c[k] = a[k] - b[k];
                    }
                    temp = mod(c);
                    for (int k = 0; k < 3; ++k) {
                        c[k] = c[k] / temp;
                    }
                    if (sphere* sph = dynamic_cast<sphere*>(figures[m])) {
                        color = sph->color;
                        D = pow((c[0] * b[0] + c[1] * b[1] + c[2] * b[2] - c[0] * x0 - c[1] * y0 - c[2] * z0), 2) - (pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2)) * (pow(b[0], 2) + pow(b[1], 2) + pow(b[2], 2) + pow(x0, 2) + pow(y0, 2) + pow(z0, 2) - 2 * (b[0] * x0 + b[1] * y0 + b[2] * z0) - pow(sph->R, 2));
                        if (D < 0) {
                           // img.draw_point(j, i, black);
                        }
                        else {
                            temp = (-(c[0] * b[0] + c[1] * b[1] + c[2] * b[2] - c[0] * x0 - c[1] * y0 - c[2] * z0) - sqrt(D)) / (pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2));
                            if ((temp < t || t == 0)&&temp<=limit) {
                                t = temp;
                                for (int k = 0; k < 3; ++k) {
                                    r[k] = c[k] * t + b[k];
                                }
                                temp = sqrt(pow(r[0] - x0, 2) + pow(r[1] - y0, 2) + pow(r[2] - z0, 2));
                                n[0] = (r[0] - x0) / temp;
                                n[1] = (r[1] - y0) / temp;
                                n[2] = (r[2] - z0) / temp;
                                for (int k = 0; k < 3; ++k) {
                                    l[k] = light[k] - r[k];
                                }
                                temp = mod(l);//sqrt(pow(l[0], 2) + pow(l[1], 2) + pow(l[2], 2));
                                for (int k = 0; k < 3; ++k) {
                                    l[k] = l[k] / temp;
                                }
                                temp = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
                                if (temp < 0) { temp = 0; }
                                for (int k = 0; k < 3; ++k) {
                                    tempcolor[k] = color[k] * temp;
                                }
                                img.draw_point(j, i, tempcolor);
                            }
                        }
                    }
                    if (tetra* te = dynamic_cast<tetra*>(figures[m])) {
                        color = te->color;
                        double A = 0;
                        double B = 0;
                        double C = 0;
                        double A1[3];
                        double A2[3];
                        double A3[3];
                        int hh = 1;
                        int hhh = 2;
                        int ost = 3;
                        //if (i == 0 && j == 0) { te->print(); }
                        for (int h = 0; h < 4; h++) {
                            A1[0] = te->v[h][0];
                            A1[1] = te->v[h][1];
                            A1[2] = te->v[h][2];
                            A2[0] = te->v[hh][0];
                            A2[1] = te->v[hh][1];
                            A2[2] = te->v[hh][2];
                            A3[0] = te->v[hhh][0];
                            A3[1] = te->v[hhh][1];
                            A3[2] = te->v[hhh][2];
                            A = (A2[1] - A1[1]) * (A3[2] - A1[2]) - (A3[1] - A1[1]) * (A2[2] - A1[2]);
                            B = (A3[0] - A1[0]) * (A2[2] - A1[2]) - (A2[0] - A1[0]) * (A3[2] - A1[2]);
                            C = (A2[0] - A1[0]) * (A3[1] - A1[1]) - (A3[0] - A1[0]) * (A2[1] - A1[1]);
                            D = -A1[0] * A - A1[1] * B - A1[2] * C;
                            if (A * c[0] + B * c[1] + C * c[2] != 0) {
                                temp = (-(A * b[0] + B * b[1] + C * b[2] + D)) / (A * c[0] + B * c[1] + C * c[2]);
                                if ((temp < t || t == 0) && temp <= limit) {
                                    //if (i == 137 && j == 346) { if (hh == 3) { cout << temp; } }
                                   // if (i == 137 && j == 346) { if (hh == 3) { for (int k = 0; k < 3; ++k) { cout <<c[k] << ' '; } } }
                                    //cout << A << B << C << D<<endl;
                                    for (int k = 0; k < 3; ++k) { r[k] = c[k] * temp + b[k]; /*cout << r[k] << ' ';*/ }
                                   // cout << endl;
                                    //if (i == 137 && j == 346) { if (hh == 3) { for (int k = 0; k < 3; ++k) { cout << r[k] << ' '; } } } //- 0.347319 1.31209 5.34732
                                    if (inside(A1, A2, A3, r)) {
                                        //if (i == 137 && j == 346) { cout << "egsfgdfgh"; }
                                       // cout << "egsfgdfgh";
                                        t = temp;
                                        if ((A * (te->v[ost][0] - r[0]) + B * (te->v[ost][1] - r[1]) + C * (te->v[ost][2] - r[2])) < 0) {
                                            n[0] = A;
                                            n[1] = B;
                                            n[2] = C;
                                        }
                                        else {
                                            n[0] = -A;
                                            n[1] = -B;
                                            n[2] = -C;
                                        }
                                        temp= mod(n);
                                        for (int i = 0; i < 3; ++i) { n[i] = n[i] / temp; }
                                        for (int k = 0; k < 3; ++k) { l[k] = light[k] - r[k]; }
                                        temp = mod(l);
                                        for (int k = 0; k < 3; ++k) { l[k] = l[k] / temp; }
                                        temp = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
                                        if (temp < 0) { temp = 0; }
                                        for (int k = 0; k < 3; ++k) { tempcolor[k] = color[k] * temp; }
                                        img.draw_point(j, i, tempcolor);
                                        
                                    }
                                }
                            }
                            hh++;
                            if (hh == 4) { hh = 0; }
                            hhh++;
                            if (hhh == 4) { hhh = 0; }
                            ost++;
                            if (ost == 4) { ost = 0; }
                        }
                    }
                    
                    if (box* bo = dynamic_cast<box*>(figures[m])) {
                        color = bo->color;
                        double A = 0;
                        double B = 0;
                        double C = 0;
                        double A1[3];
                        double A2[3];
                        double A3[3];
                        double A4[3];
                        int hh = 1;
                        int hhh = 2;
                        int ost = 3;
                        double v[3];
                        double e[3][3];
                        int h = 2;
                        for (int k = 0; k < 3; ++k) { v[k] = bo->max[k] - bo->min[k];  for (int j = 0; j < 3; ++j) { e[k][j] = 0; } }
                        for (int k = 0; k < 3; ++k) { 
                            e[k][k] = 1;
                            if (scal(e[k], v) < 0) { e[k][k] = -1; }
                        }
                        double V[3][4][3]; //0-номер грани 1-номер точки 2-номер координаты xy xz yz
                        for (int k = 0; k < 3; ++k) {
                            V[0][0][k] = bo->min[k];
                            V[1][0][k] = bo->min[k];
                            V[2][0][k] = bo->min[k];
                            V[0][1][k] = bo->min[k] + e[0][k]*scal(e[0],v);
                            V[0][2][k] = V[0][1][k] + e[1][k] * scal(e[1], v);
                            V[0][3][k] = V[0][2][k] - e[0][k] * scal(e[0], v);
                            V[1][1][k] = bo->min[k] + e[0][k] * scal(e[0], v);
                            V[1][2][k] = V[1][1][k] + e[2][k] * scal(e[2], v);
                            V[1][3][k] = V[1][2][k] - e[0][k] * scal(e[0], v);
                            V[2][1][k] = bo->min[k] + e[1][k] * scal(e[1], v);
                            V[2][2][k] = V[2][1][k] + e[2][k] * scal(e[2], v);
                            V[2][3][k] = V[2][2][k] - e[1][k] * scal(e[1], v);
                        }
                        for (int m = 0; m < 3; ++m) {
                            A1[0] = V[m][0][0];
                            A1[1] = V[m][0][1];
                            A1[2] = V[m][0][2];
                            A2[0] = V[m][1][0];
                            A2[1] = V[m][1][1];
                            A2[2] = V[m][1][2];
                            A3[0] = V[m][2][0];
                            A3[1] = V[m][2][1];
                            A3[2] = V[m][2][2];
                            A4[0] = V[m][3][0];
                            A4[1] = V[m][3][1];
                            A4[2] = V[m][3][2];
                            A = (A2[1] - A1[1]) * (A3[2] - A1[2]) - (A3[1] - A1[1]) * (A2[2] - A1[2]);
                            B = (A3[0] - A1[0]) * (A2[2] - A1[2]) - (A2[0] - A1[0]) * (A3[2] - A1[2]);
                            C = (A2[0] - A1[0]) * (A3[1] - A1[1]) - (A3[0] - A1[0]) * (A2[1] - A1[1]);
                            D = -A1[0] * A - A1[1] * B - A1[2] * C;
                            if (A * c[0] + B * c[1] + C * c[2] != 0) {
                                temp = (-(A * b[0] + B * b[1] + C * b[2] + D)) / (A * c[0] + B * c[1] + C * c[2]);
                                if ((temp < t || t == 0) && temp <= limit) {
                                    for (int k = 0; k < 3; ++k) { r[k] = c[k] * temp + b[k]; }
                                    if (inside(A1, A2, A3, r) || inside(A1, A4, A3, r)) {
                                        t = temp;
                                        for (int k = 0; k < 3; ++k) { n[k] = -e[h][k]; }
                                        for (int k = 0; k < 3; ++k) { l[k] = light[k] - r[k]; }
                                        temp = mod(l);
                                        for (int k = 0; k < 3; ++k) { l[k] = l[k] / temp; }
                                        temp = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
                                        if (temp < 0) { temp = 0; }
                                        for (int k = 0; k < 3; ++k) { tempcolor[k] = color[k] * temp; }
                                        img.draw_point(j, i, tempcolor);
                                    }
                                }
                            }
                            A1[0] = V[m][0][0] + e[h][0] * scal(e[h], v);
                            A1[1] = V[m][0][1] + e[h][1] * scal(e[h], v);
                            A1[2] = V[m][0][2] + e[h][2] * scal(e[h], v);
                            A2[0] = V[m][1][0] + e[h][0] * scal(e[h], v);
                            A2[1] = V[m][1][1] + e[h][1] * scal(e[h], v);
                            A2[2] = V[m][1][2] + e[h][2] * scal(e[h], v);
                            A3[0] = V[m][2][0] + e[h][0] * scal(e[h], v);
                            A3[1] = V[m][2][1] + e[h][1] * scal(e[h], v);
                            A3[2] = V[m][2][2] + e[h][2] * scal(e[h], v);
                            A4[0] = V[m][3][0] + e[h][0] * scal(e[h], v);
                            A4[1] = V[m][3][1] + e[h][1] * scal(e[h], v);
                            A4[2] = V[m][3][2] + e[h][2] * scal(e[h], v);
                            A = (A2[1] - A1[1]) * (A3[2] - A1[2]) - (A3[1] - A1[1]) * (A2[2] - A1[2]);
                            B = (A3[0] - A1[0]) * (A2[2] - A1[2]) - (A2[0] - A1[0]) * (A3[2] - A1[2]);
                            C = (A2[0] - A1[0]) * (A3[1] - A1[1]) - (A3[0] - A1[0]) * (A2[1] - A1[1]);
                            D = -A1[0] * A - A1[1] * B - A1[2] * C;
                            if (A * c[0] + B * c[1] + C * c[2] != 0) {
                                temp = (-(A * b[0] + B * b[1] + C * b[2] + D)) / (A * c[0] + B * c[1] + C * c[2]);
                                if ((temp < t || t == 0) && temp <= limit) {
                                    for (int k = 0; k < 3; ++k) { r[k] = c[k] * temp + b[k]; }
                                    if (inside(A1, A2, A3, r) || inside(A1, A4, A3, r)) {
                                        t = temp;
                                        for (int k = 0; k < 3; ++k) { n[k] = -e[h][k]; }
                                        for (int k = 0; k < 3; ++k) { l[k] = light[k] - r[k]; }
                                        temp = mod(l);
                                        for (int k = 0; k < 3; ++k) { l[k] = l[k] / temp; }
                                        temp = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
                                        if (temp < 0) { temp = 0; }
                                        for (int k = 0; k < 3; ++k) { tempcolor[k] = color[k] * temp; }
                                        img.draw_point(j, i, tempcolor);
                                    }
                                }
                            }
                            h--;

                        }

                    }
            }
            t = 0;
        }
    }
    return img; 
}





bool inside(double A1[3], double A2[3], double A3[3], double B[3]) {
    double a[3];
    double b[3];
    for (int i = 0; i < 3; ++i) { a[i] = A2[i] - A1[i]; }
    for (int i = 0; i < 3; ++i) { b[i] = B[i] - A1[i]; }
    double t[3];
    double c[3];
    vect(t, a, b);
   // cout << t[0] << t[1] << t[2];
    if (t[0] != 0 || t[1] != 0 || t[2] != 0) {
        double temp;
        temp = mod(t);
        for (int i = 0; i < 3; ++i) { t[i] = t[i] / temp; }
        //cout << t[0] << t[1] << t[2];
        for (int i = 0; i < 3; ++i) { a[i] = A3[i] - A2[i]; }
        for (int i = 0; i < 3; ++i) { b[i] = B[i] - A2[i]; }
        vect(c, a, b);
        if (c[0] != 0 || c[1] != 0 || c[2] != 0) {
            temp = mod(c);
            for (int i = 0; i < 3; ++i) { c[i] = c[i] / temp; }
            for (int i = 0; i < 3; ++i) { if (fabs(t[i] -c[i])>=0.000001) { return false; } }
            for (int i = 0; i < 3; ++i) { a[i] = A1[i] - A3[i]; }
            for (int i = 0; i < 3; ++i) { b[i] = B[i] - A3[i]; }
            vect(c, a, b);
            if (c[0] != 0 || c[1] != 0 || c[2] != 0) {
                temp = mod(c);
                for (int i = 0; i < 3; ++i) { c[i] = c[i] / temp; }
                for (int i = 0; i < 3; ++i) { if (fabs(t[i] - c[i]) >= 0.000001) { return false; } }
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

double mod(double a[3]) {
    return sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
}

void vect(double t[3], double a[3], double b[3]) {
    t[0] = a[1] * b[2] - a[2] * b[1];
    t[1] = a[2] * b[0] - a[0] * b[2];
    t[2] = a[0] * b[1] - a[1] * b[0];
}

double scal(double a[3], double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}