#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QVector>

struct Flight
{
    // 항공편 하나가 그래프의 간선(edge)에 해당한다.
    // from/to는 연결되는 두 공항 노드이고, price/durationMinutes는 가중치이다.
    QString from;
    QString to;
    QString airline;
    int price;
    int durationMinutes;
};

struct RouteResult
{
    // 최저가/최단시간 탐색 결과를 GUI에 넘기기 위한 구조체이다.
    QVector<Flight> flights;
    int totalPrice = 0;
    int totalDuration = 0;
    bool found = false;
};

class FlightGraph
{
public:
    FlightGraph();

    void addFlight(const QString &from, const QString &to, const QString &airline, int price, int durationMinutes);
    QStringList airports() const;
    QVector<Flight> directFlights(const QString &from, const QString &to) const;
    QVector<Flight> flightsFrom(const QString &from) const;
    RouteResult cheapestRoute(const QString &from, const QString &to) const;
    RouteResult fastestRoute(const QString &from, const QString &to) const;

private:
    // 인접 리스트 방식의 그래프 저장 구조.
    // key: 출발 공항 노드, value: 해당 공항에서 출발하는 항공편 간선 목록.
    QMap<QString, QVector<Flight>> adjacencyList;

    // usePriceWeight가 true이면 가격을, false이면 시간을 가중치로 사용한다.
    RouteResult dijkstra(const QString &from, const QString &to, bool usePriceWeight) const;
};

#endif
