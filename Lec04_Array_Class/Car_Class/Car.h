#pragma once
#include <cstdio>
#include <cstring>

class Car {
protected:
    int speed;        // 속도
    char name[40];    // 이름

public:
    int gear;         // 기어

    Car() {}         // 기본 생성자
    ~Car() {}        // 소멸자
    Car(int s, const char* n, int g) // 매개변수가 있는 생성자
        : speed(s), gear(g) { // 멤버 초기화 리스트
        strcpy_s(name, sizeof(name), n); // 생성자 함수 몸체
    }

    // 기어 단수를 변경하는 멤버 함수
    void changeGear(int g = 4) {
        gear = g;
    }

    // 속도를 5씩 증가시키는 멤버 함수
    void speedUp() {
        speed += 5;
    }

    // 자동차의 정보를 화면에 출력함
    void display() {
        printf("[%s] : 기어=%d단 속도=%dkmph\n", name, gear, speed);
    }

    void whereAmI() {
        printf("객체 주소 = %p\n", this);
    }
};
