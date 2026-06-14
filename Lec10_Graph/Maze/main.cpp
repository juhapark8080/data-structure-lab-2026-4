#include <stdio.h>
#include <stack>
#include <deque>
#include "Location2d.h"
using namespace std;

const int MAZE_SIZE = 6;

char map[MAZE_SIZE][MAZE_SIZE] = {
    {'1', '1', '1', '1', '1', '1'},
    {'e', '0', '1', '0', '0', '1'},
    {'1', '0', '0', '0', '1', '1'},
    {'1', '0', '1', '0', '1', '1'},
    {'1', '0', '1', '0', '0', 'x'},
    {'1', '1', '1', '1', '1', '1'},
};

bool isValidLoc(int r, int c)
{
    if (r < 0 || c < 0 || r >= MAZE_SIZE || c >= MAZE_SIZE) return false;
    else return map[r][c] == '0' || map[r][c] == 'x';
}


int main()
{
    int select;

    printf("미로 탐색 방법 선택\n");
    printf("1. Stack을 이용한 DFS\n");
    printf("2. Deque를 Stack처럼 이용한 DFS\n");
    printf("3. Deque를 Queue처럼 이용한 BFS\n");
    printf("선택: ");
    scanf_s("%d", &select);

    Location2D entry(1, 0);

    switch (select) {
    case 1:
    {
        stack<Location2D> locStack;
        locStack.push(entry);

        while (locStack.empty() == false) {
            Location2D here = locStack.top();
            locStack.pop();

            int r = here.row;
            int c = here.col;

            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("미로 탐색 성공\n");
                return 0;
            }
            else {
                map[r][c] = '.';

                if (isValidLoc(r - 1, c)) locStack.push(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locStack.push(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locStack.push(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locStack.push(Location2D(r, c + 1));
            }
        }

        printf("미로 탐색 실패\n");
        break;
    }

    case 2:
    {
        deque<Location2D> locDeque;
        locDeque.push_front(entry);

        while (locDeque.empty() == false) {
            Location2D here = locDeque.front();
            locDeque.pop_front();

            int r = here.row;
            int c = here.col;

            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("미로 탐색 성공\n");
                return 0;
            }
            else {
                map[r][c] = '.';

                if (isValidLoc(r - 1, c)) locDeque.push_front(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locDeque.push_front(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locDeque.push_front(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locDeque.push_front(Location2D(r, c + 1));
            }
        }

        printf("미로 탐색 실패\n");
        break;
    }

    case 3:
    {
        deque<Location2D> locDeque;
        locDeque.push_back(entry);

        while (locDeque.empty() == false) {
            Location2D here = locDeque.front();
            locDeque.pop_front();

            int r = here.row;
            int c = here.col;

            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("미로 탐색 성공\n");
                return 0;
            }
            else {
                map[r][c] = '.';

                if (isValidLoc(r - 1, c)) locDeque.push_back(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locDeque.push_back(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locDeque.push_back(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locDeque.push_back(Location2D(r, c + 1));
            }
        }

        printf("미로 탐색 실패\n");
        break;
    }

    default:
        printf("잘못된 선택입니다.\n");
        break;
    }

    return 0;
}