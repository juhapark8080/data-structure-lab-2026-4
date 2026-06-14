#pragma once
#include "AdjMatGraph.h"

#define INF 9999

class WGraph : public AdjMatGraph {
public:
    void insertEdge(int u, int v, int weight) {
        if (weight > INF) weight = INF;
        setEdge(u, v, weight);
    }

    bool hasEdge(int i, int j) {
        return getEdge(i, j) < INF;
    }

    void load(char* filename) {
        FILE* fp;
        fopen_s(&fp, filename, "r");

        if (fp != NULL) {
            int n, val;
            fscanf_s(fp, "%d", &n);

            for (int i = 0; i < n; i++) {
                char str[80];

                fscanf_s(fp, "%s", str, (unsigned)sizeof(str));
                insertVertex(str[0]);

                for (int j = 0; j < n; j++) {
                    fscanf_s(fp, "%d", &val);
                    insertEdge(i, j, val);
                }
            }

            fclose(fp);
        }
    }
};