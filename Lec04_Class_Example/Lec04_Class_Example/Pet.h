#pragma once
#include <iostream>
#include <string>
using namespace std;
class Pet {
protected:
	string name;
	int age;
	string type;
public:
	//생성자
	Pet (string n, int a, string t)
		:name(n), age(a), type(t) { }

	//Get name
	string getName() {
		return name;
	}

	//Get Age
	int getAge() {
		return age;
	}
	//Get Type
	string getType() {
		return type;
	}
};

class Puppy : public Pet {
private:
	string breed; // 1. 멤버 변수 선언 추가

public: // 2. 접근 제어자를 public으로 변경
	// 생성자
	Puppy(string n, int a, string t, string b)
		: Pet(n, a, t), breed(b) {
	}

	// get breed
	string getBreed() {
		return breed;
	}
};
