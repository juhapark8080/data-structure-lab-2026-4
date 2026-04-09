#include <iostream>
#include "Car.h"
#include "SportsCar.h"

int main() {
    // 1. 일반 자동차 테스트
    std::cout << "--- 일반 자동차 테스트 ---\n";
    Car myCar(50, "기본자동차", 3);
    myCar.display();       // 초기 상태 출력
    myCar.speedUp();       // 속도 5 증가
    myCar.display();       // 증가 후 출력
    myCar.whereAmI();      // 객체 주소 출력

    std::cout << "\n--- 스포츠카 테스트 ---\n";

    // 2. 스포츠카 테스트
    SportsCar mySportsCar;  // 생성자에서 speed=0, name="스포츠카", gear=1 초기화
    mySportsCar.changeGear(5);   // 기어 변경
    mySportsCar.setTurbo(true);  // 터보 모드 ON

    std::cout << "터보 작동 전 속도 테스트 진행\n";
    mySportsCar.speedUp();        // 터보 ON이므로 speed += 20
    mySportsCar.display();        // 현재 상태 출력
    std::cout << "터보 상태: " << (mySportsCar.bTurbo ? "ON" : "OFF") << "\n";

    return 0;
}