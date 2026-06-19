# FlightBookingQt

2026-1 데이터구조 최종 프로젝트용 Qt/C++ 항공권 조회 및 예매 GUI 프로그램입니다.

## 핵심 아이디어

- 공항/도시: 그래프의 노드
- 항공편: 그래프의 간선
- 가격/시간: 간선의 가중치
- 최저가/최단시간 경로: Dijkstra 알고리즘으로 탐색

## 빌드 방법

Qt Creator에서 `CMakeLists.txt`를 열고 빌드/실행하면 됩니다.

또는 Qt 개발 환경이 잡혀 있다면 터미널에서 다음처럼 실행할 수 있습니다.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

## 제출 전 할 일

1. 실행 화면 캡처
2. GitHub에 업로드
3. 보고서의 학번, 이름, GitHub 주소 입력
4. `.vs`, `build`, `.qt` 같은 캐시/빌드 폴더는 업로드하지 않기
