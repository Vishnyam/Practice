#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class drinks{
public:
	static int total;
	drinks (){}
	bool empty() const {return product == NULL;}
	friend ostream& operator<< (ostream& s, const drinks& v) {
		double* vbuf = v.product;
		for(int i = 0; i < v.size; ++i)
			s << i + 1 <<" - " << vbuf[i] << "  VikaCoins" << endl;
		return s;
	}
	virtual void cost () = 0;
	void sell (int k) const{
		total += product[k - 1];
	}
protected:
	int size;
	double *product;
};

int drinks :: total = 0;

class alcoholic_drinks : public drinks{
public:
	alcoholic_drinks(int i){
		size = i;
		product = new double[i];
		cost();
	}
	
	void cost (){
		srand(time(NULL));
		for (int j = 0; j < size; ++j)
			product[j] = 100 + rand() % 901;
	}
	
	alcoholic_drinks (const alcoholic_drinks& v){
		if (v.empty()){
			product = NULL;
			return;
		}
		product = new double[size];
		for (int i = 0; i < size; i++)
			product[i] = v.product[i];
	}
	
	alcoholic_drinks& operator= (const alcoholic_drinks& v){
		if (&v == this)
			return *this;
		delete [] product;
		if (v.empty()) 
			return *this;
		product = new double[size];
		for (int i = 0; i < size; i++)
            product[i] = v.product[i];
		return *this;
	}
	~alcoholic_drinks(){
		delete [] product;
	}
};

class alcohol_free_drinks : public drinks{
public:
	alcohol_free_drinks(int i){
		size = i;
		product = new double[i];
		cost();
	}
	
	void cost (){
		srand(time(NULL));
		for (int j = 0; j < size; ++j)
			product[j] = 10 + rand() % 91;
	}
	alcohol_free_drinks (const alcohol_free_drinks& v){
		if (v.empty()){
			product = NULL;
			return;
		}
		product = new double[size];
		for (int i = 0; i < size; i++)
			product[i] = v.product[i];
	}
	
	alcohol_free_drinks& operator= (const alcohol_free_drinks& v){
		if (&v == this)
			return *this;
		delete [] product;
		if (v.empty()) 
			return *this;
		product = new double[size];
		for (int i = 0; i < size; i++)
            product[i] = v.product[i];
		return *this;
	}
	~alcohol_free_drinks(){
		delete [] product;
	}
};


int main(){
	alcoholic_drinks a(10);
	alcohol_free_drinks b(10);
	cout << "Hello! This is a supermarket where you can get a drink.\nThere are 10 alcoholic drinks and 10 alcohol free drinks.\nChoose what you want and I will tell you a total sum of VikaCoins that you need to pay. \nThe list of alcoholic products: \n";
	cout << a;
	while (true){
		int k;
		cout << "Do you want anything of it? Yes - enter a number of product; No - enter 0: ";
		cin >> k;
		if (k < 0 || k > 10)
			cout << "We haven't got this product" << endl;
		if (k == 0)
			break;
		if (k > 0 && k < 10)
			a.sell(k);
	}
	cout << "The list of non-alcohol products: \n";
	cout << b;
	while (true){
		int k;
		cout << "Do you want anything of it? Yes - enter a number of product; No - enter 0: ";
		cin >> k;
		if (k < 0 || k > 10)
			cout << "We haven't got this product" << endl;
		if (k == 0)
			break;
		if (k > 0 && k < 10)
			b.sell(k);
	}

	cout << "Total sum: "<< a.total << " VikaCoins" << endl;

	return 0;
}
