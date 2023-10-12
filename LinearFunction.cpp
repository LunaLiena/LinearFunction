#include<iostream>
#include<string>
#include<cmath>
#include<sstream>
using namespace std;
//превращение любого типа данных в строку
template<typename T>
string toString(T val) {
	char buf[30]{};
	sprintf_s(buf, "%.9lf", val);
	string res = buf;
	return res;
}

template<typename T>//превращение из строки в любой тип данных
T fromString(const string& str) {
	istringstream iss(str);
	T res;
	iss >> res;
	return res;
}

class LeastSquaresMethod {//В данном классе хранятся значения x и y
private:
	int size;
	double* ListX;
	double* ListY;
public:
	virtual double getA() { return 0; }
	virtual double getB() { return 0; }
	virtual double getC() { return 0; }
	virtual double getAprox() { return 0; }
	virtual double getAproxA() { return 0; }
	virtual double getAproxB() { return 0; }
	virtual double getAproxC() { return 0; }
	virtual void get_result() {}
	LeastSquaresMethod() {
		cout << "Введите, сколько переменных x в вашей функции: ";
		cin >> size;
		ListX = new double[size];
		ListY = new double[size];

		cout << "Введите значения ваших x-ов в массив:\n";
		for (int i = 0; i < size; i++) {
			cout << "[" << "x" << "]" << "[" << i + 1 << "] = ";
			cin >> ListX[i];
		}
		cout << endl;
		cout << "Введите значения ваших y-ов в массив:\n";
		for (int i = 0; i < size; i++) {
			cout << "[" << "y" << "]" << "[" << i + 1 << "] = ";
			cin >> ListY[i];
		}
		cout << endl << endl;
	}
	double* getListX() { return ListX; }
	double* getListY() { return ListY; }
	int SizeList() { return size; }
	~LeastSquaresMethod() {
		delete[]ListX;
		delete[]ListY;
	}
};
//класс, который производит сумму по x,y и x и y вместе 
class Sum :public LeastSquaresMethod {
public:
	Sum() {
		for (int i = 0; i < SizeList(); i++) {
			this->Sumx += getListX()[i];
			this->Sumsqrx += pow(getListX()[i], 2);

			this->Sumpowx4 += pow(getListX()[i], 4);
			this->Sumpowx3 += pow(getListX()[i], 3);
			this->Sumpow_x2_y += pow(getListX()[i], 2) * getListY()[i];
			this->Sumy += getListY()[i];
			this->Sumxy += getListX()[i] * getListY()[i];
		}
	}
	double SumX() { return Sumx; }
	double SumY() { return Sumy; }
	double SumXY() { return Sumxy; }

	double SumSqrX() { return Sumsqrx; }
	double SumPow4X() { return Sumpowx4; }
	double SumPow3X() { return Sumpowx3; }
	double SumPow2XY() { return Sumpow_x2_y; }
private:
	double Sumx;
	double Sumy;
	double Sumxy;
	double Sumsqrx;
	double Sumpowx4;
	double Sumpowx3;
	double Sumpow_x2_y;
};
/*--------------------------------------------------------------------------------*/
//Линейная функция
class LinearFn :public Sum {
public:
	double getA()override { return A; }
	double getB()override { return B; }
	void get_result()override { cout << res << endl; }
	double getAprox()override { return Aprox; }
	double getAproxA()override { return AproxA; }
	double getAproxB() { return AproxB; }
private:
	double Aprox = (SumSqrX() * SizeList()) - (SumX() * SumX());
	double AproxA = (SumXY() * SizeList()) - (SumY() * SumX());
	double AproxB = (SumSqrX() * SumY()) - (SumX() * SumXY());
	double A = AproxA / Aprox;
	double B = AproxB / Aprox;
	string res = toString(A) + "*" + "x" + "+" + toString(B);
};
//Степенная функция
class PowerFn :public Sum {
public:
	double getA()override { return (AproxAlpha / Aprox); }
	double getB()override { return Beta; }
	double getAprox()override { return Aprox; }
	double getAproxA()override { return AproxAlpha; }
	double getAproxB()override { return (SumSqrX() * SumY()) - (SumX() * SumXY()); }
	void get_result()override { cout << pow(e, getB()) << "*" << " x" << "^" << getA() << endl; }
private:
	double Aprox = (SumSqrX() * SizeList()) - (SumX() * SumX());
	double Beta = getAproxB() / getAprox();
	double e = 2.71;
	double AproxAlpha = (SumXY() * SizeList()) - (SumX() * SumY());
};
//-----------------------------------------------------------------//
class ExponentialFn :public Sum {//показательная функция
public:
	double getAprox()override { return Aprox; }
	double getAproxA()override { return AproxAlpha; }
	double getAproxB()override { return AproxBeta; }
	double getA()override { return Alpha; }
	double getB()override { return Beta; }
	void get_result()override { cout << "y = " << getB() << "^" << getA() << "x" << endl; }
private:
	double Aprox = (SumSqrX() * SizeList()) - (SumX() * SumX());
	double AproxAlpha = (SumXY() * SizeList() - SumX() * SumY());
	double AproxBeta = (SumSqrX() * SumY() - SumX() * SumXY());
	double Alpha = AproxAlpha / Aprox;
	double Beta = pow(AproxBeta, 2.70);
};

class QuadraticFn :public Sum {//квадратичная функция
public:
	double getAproxA()override { return AproxA; }
	double getAproxB()override { return AproxB; }
	double getAproxC()override { return AproxC; }
	double getAprox()override { return Aprox; }
	double getA()override { return A; }
	double getB()override { return B; }
	double getC()override { return C; }

	void get_result()override {	cout << "y = " << getA() << " x^2 + " << getB() << " x + " << getC() << endl; }
private:
	double Aprox = SumPow4X()*SumSqrX()*SizeList()+SumPow3X()*SumX()*SumSqrX()+SumSqrX()*SumPow3X()*SumX() - SumPow3X()*SumPow3X() *SizeList() - SumPow4X()*SumX()*SumX()- pow(SumSqrX(),3);
	double AproxA = SumPow2XY()*SumSqrX()*SizeList() + SumPow3X()*SumX()*SumY() + SumSqrX()*SumXY()*SumX() - SumPow3X()*SumXY()*SizeList() - SumPow2XY()*pow(SumX(),2) - pow(SumSqrX(),2)*SumY();
	double AproxB = SumPow4X()*SumXY()*SizeList() + SumPow2XY()*SumX()*SumSqrX() + SumSqrX()*SumPow3X()*SumY() - SumPow2XY()*SumPow3X()*SizeList() - SumPow4X()*SumX()*SumY() - SumSqrX()*SumXY()*SumSqrX();
	double AproxC = SumPow4X() * SumSqrX() * SumY() + SumPow3X() * SumXY() * SumSqrX() + SumPow2XY() * SumPow3X() * SumX() - SumPow3X() * SumPow3X() * SumY() - SumPow4X() * SumXY() * SumX() - SumPow2XY() * pow(SumSqrX(), 2);
	double A = AproxA/Aprox;
	double B = AproxB/Aprox;
	double C = AproxC/Aprox;
};

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Linear function" << endl << endl;
	LinearFn obj;
	cout << "Aprox(triangle)(Linear func) = " << obj.getAprox() << endl;
	cout << "AproxA(triangle)(Linear func) = " << obj.getAproxA() << endl;
	cout << "AproxB(triangle)(Linear func) = " << obj.getAproxB() << endl;
	cout << "A(triangle)(Linear func) = " << obj.getA() << endl;
	cout << "B(triangle)(Linear func) = " << obj.getB() << endl;
	cout << "Linear function (result) = "; obj.get_result();
	cout << endl << endl;

	cout << "Power function" << endl << endl;
	PowerFn obj1;
	cout << "Aprox(triangle)(Power func) = " << obj1.getAprox() << endl;
	cout << "AproxA (triangle)(Power func) = " << obj1.getAproxA() << endl;
	cout << "AproxB (triangle)(Power func) = " << obj1.getAproxB() << endl;
	cout << "A (triangle)(Power func) = " << obj1.getA() << endl;
	cout << "B (triangle)(Power func) = " << obj1.getB() << endl;
	cout << "Power function (result) = "; obj1.get_result();
	cout << endl << endl;
	
	cout << "Exponential function" << endl << endl;
	ExponentialFn obj2;
	cout << "Aprox(triangle)(Power func) = " << obj2.getAprox() << endl;
	cout << "AproxA (triangle)(Power func) = " << obj2.getAproxA() << endl;
	cout << "AproxB (triangle)(Power func) = " << obj2.getAproxB() << endl;
	cout << "A (triangle)(Power func) = " << obj2.getA() << endl;
	cout << "B (triangle)(Power func) = " << obj2.getB() << endl;
	cout << "Exponential function (result) = "; obj2.get_result(); cout << endl;

	
	cout << endl << endl;
	cout << "Quadratic function" << endl << endl;
	QuadraticFn obj3;
	cout << "Aprox(triangle)(Quadratic func) = " << obj3.getAprox() << endl;
	cout << "AproxA (triangle)(Quadratic func) = " << obj3.getAproxA() << endl;
	cout << "AproxB (triangle)(Quadratic func) = " << obj3.getAproxB() << endl;
	cout << "AproxC (triangle)(Quadratic func) = " << obj3.getAproxC() << endl;
	cout << "A (triangle)(Quadratic func) = " << obj3.getA() << endl;
	cout << "B (triangle)(Quadratic func) = " << obj3.getB() << endl;
	cout << "C (triangle)(Quadratic func) = " << obj3.getC() << endl;
	cout << "Quadratic function (result) = "; obj3.get_result(); cout << endl;
	

	return 0;
}