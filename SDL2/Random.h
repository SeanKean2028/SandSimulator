#pragma once
#include <random>
#include <iostream>
#include <iostream>
using namespace std;
class Random {
public : 
	static Random& getInstance();
	void Reseed();
	void ShowMessage();
	int GetRandomInt(int _min, int _max);
private :
	random_device randomDevice;
	mt19937 generator;
	Random();
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;
};