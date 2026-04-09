#pragma once
#include "Car.h" // 자동차 클래스 헤더파일 포함

// 스포츠카 클래스: 자동차 클래스에 터보 기능 추가
class SportsCar : public Car {
public:
    bool bTurbo; // 터보 장치 ON 여부 (true/false)

    SportsCar() : Car(0, "스포츠카", 1), bTurbo(false) {}

    void setTurbo(bool bTur) {
        bTurbo = bTur;
    }
    void speedUp() {
        if (bTurbo) {
            speed += 20; // 터보가 ON이 되어 있으면 가속이 20씩 빨리 됨
        }
        else {
            Car::speedUp(); // 터보가 꺼져있으면 부모 클래스(Car)의 기존 speedUp(5씩 증가) 실행
        }
    }
};
