//var 6
#include <iostream>
#include <cmath>
using namespace std;

class Vector{
	double* body;
public:
	Vector(const int i){
		body = new double[3];
	}
	bool empty() const {return body == NULL;}
	
	Vector (const Vector& v){
		if (v.empty()){
			body = NULL;
			return;
		}
		body = new double[3];
		for (int i = 0; i < 3; i++)
			body[i] = v.body[i];
	}
	
	~Vector(){
		delete [] body;
	}
	
	Vector& operator= (const Vector& v){
		if (&v == this)
			return *this;
		delete [] body;
		if (v.empty()) 
			return *this;
		body = new double[3];
		for (int i = 0; i < 3; i++)
            body[i] = v.body[i];
		return *this;
	}
	
	Vector operator+ (const Vector& v) const{
		if( this -> empty())
			return v;
		if(v.empty())
			return *this;
		Vector resv(3);
		for (int i = 0; i < 3; ++i)
			resv.body[i] = v.body[i] + body[i];
		return resv;
	}
	
	Vector operator- (const Vector& v) const{
		if( this -> empty())
			return v;
		if(v.empty())
			return *this;
		Vector resv(3);
		for (int i = 0; i < 3; ++i)
			resv.body[i] = body[i] - v.body[i];
		return resv;
	}
	
	double length() const{
		if(this->empty())
			return 0;
		double len = 0;
		for (int i = 0; i < 3; i++)
			len += body[i] * body[i];
		return sqrt(len);
	}
	
	double scalar(const Vector& v){
		if (v.empty() || this->empty())
			return 0;
		double res = 0;
		for (int i = 0; i < 3; i++)
			res += body[i] * v.body[i];
		return res;
	}
	
	double cos(const Vector& v){
		if (v.empty() || this->empty())
			return 0;
		if (v.length() == 0 || this->length() == 0)
			return 0;
		return (this->scalar(v))/(v.length() * (this->length()));
	}
	
	friend ostream& operator<< (ostream& s, const Vector& v) {
		double* vbuf = v.body;
		for(int i = 0; i < 3; ++i)
			s << vbuf[i] << ' ';
		return s;
	}
	
	double& operator [] (int i) {
		return body[i];
	}
};

int main(){
	Vector v1(3), v2(3);
	cout << "Hello! This is the program that implements operations on three dimensional vectors." << endl;
	cout << "Please, enter coordinates." << endl;
	cout << "Vector 1: ";
	for (int i = 0; i < 3; ++i)
		cin >> v1[i];
	cout << "Vector 2: ";
	for (int i = 0; i < 3; ++i)
		cin >> v2[i];
	while (true){
		int k, cont;
		cout << "Select the operation you want to perform:" << endl;
		cout << "1 - sum; 2 - diff; 3 - scalar; 4 - length; 5 - cos" << endl;
		cin >> k;
		if (k < 1 || k > 5){
			cout << "Wrong select" << endl;
			break;
		}
		if (k == 1)
			cout << "Sum: " << v1 + v2 << endl;
		if (k == 2)
			cout << "Diff: " << v1 - v2 << endl;
		if (k == 3)
			cout << "Scalar: " << v1.scalar(v2) << endl;
		if (k == 4){
			cout << "Len 1: "<< v1.length() << endl;
			cout << "Len 2: "<< v2.length() << endl;
		}
		if (k == 5)
			cout << "Cos: " << v1.cos(v2) << endl;
		cout << "Do you want to see another operation? 1 - yes, 0 - no" << endl;
		cin >> cont;
		if (cont != 1)
			break;
	}
	return 0;

}
