#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QVector>

struct Flight
{
    QString from;
    QString to;
    QString airline;
    int price;
    int durationMinutes;
};

struct RouteResult
{
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
    QMap<QString, QVector<Flight>> adjacencyList;

    RouteResult dijkstra(const QString &from, const QString &to, bool usePriceWeight) const;
};

#endif