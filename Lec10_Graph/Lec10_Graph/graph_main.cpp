#include "WGraphMST.h"

int main()
{
    WGraphMST g;

    char filename[] = "graph.txt";
    g.load(filename);

    printf("MST By Kruskal's Algorithm\n");
    g.Kruskal();

    return 0;
}