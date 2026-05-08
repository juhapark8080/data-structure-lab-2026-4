#include "SportsCar.h"
 
int main() {
 
    // -----------------------------------------------
    // Car 객체 테스트
    // -----------------------------------------------
    printf("=== Car 테스트 ===\n");
 
    Car c1;                                 // 기본 생성자
    Car c2(0, (char*)"Sonata", 1);          // 매개변수 있는 생성자
 
    c2.display();                           // 초기 상태 출력
 
    c2.speedUp();                           // 속도 +5
    c2.display();                           // 속도 5 확인
 
    c2.changeGear();                        // 디폴트 매개변수 (g=4)
    c2.display();                           // 기어 4단 확인
 
    c2.changeGear(2);                       // 기어 2단으로 변경
    c2.display();
 
    c2.whereAmI();                          // this 포인터 주소 출력
 
    // -----------------------------------------------
    // SportsCar 객체 테스트
    // -----------------------------------------------
    printf("\n=== SportsCar 테스트 ===\n");
    
    SportsCar sc(0, (char*)"Ferrari", 1, false);
 
    // 터보 OFF 상태에서 speedUp → 속도 +5
    sc.setTurbo(false);
    sc.speedUp();
    sc.display();                           // 속도 5 확인
 
    // 터보 ON 상태에서 speedUp → 속도 +20
    sc.setTurbo(true);
    sc.speedUp();
    sc.display();                           // 속도 25 확인
 
    sc.changeGear(3);
    sc.display();
 
    sc.whereAmI();                          // this 포인터 주소 출력
 
    return 0;
}