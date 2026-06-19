#include "WGraphPrim.h"
#include "WGraphDijkstra.h"

int main()
{
    int menu;

    printf("1. Prim MST\n");
    printf("2. Dijkstra Shortest Path\n");
    printf("선택: ");
    scanf_s("%d", &menu);

    switch (menu) {
    case 1:
    {
        WGraphPrim g;
        char filename[] = "graph.txt";

        g.load(filename);

        printf("MST By Prim's Algorithm\n");
        g.Prim(0);

        break;
    }

    case 2:
    {
        WGraphDijkstra g;
        char filename[] = "graph_sp.txt";

        g.load(filename);

        printf("Shortest Path By Dijkstra Algorithm\n");
        g.ShortestPath(0);

        break;
    }

    default:
        printf("잘못된 선택입니다.\n");
        break;
    }

    return 0;
}