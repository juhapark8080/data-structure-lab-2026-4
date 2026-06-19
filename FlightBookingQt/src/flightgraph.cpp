#include "flightgraph.h"

#include <QSet>
#include <limits>

FlightGraph::FlightGraph()
{
    // 고정된 항공편 데이터를 그래프 간선으로 저장한다.
    addFlight("Seoul", "Jeju", "Jeju Air", 78000, 70);
    addFlight("Seoul", "Busan", "Korean Air", 69000, 65);
    addFlight("Seoul", "Tokyo", "Asiana", 210000, 130);
    addFlight("Seoul", "Shanghai", "T'way", 180000, 120);
    addFlight("Busan", "Jeju", "Air Busan", 62000, 55);
    addFlight("Busan", "Osaka", "Air Busan", 160000, 95);
    addFlight("Jeju", "Osaka", "Jeju Air", 145000, 105);
    addFlight("Jeju", "Taipei", "Jin Air", 170000, 130);
    addFlight("Tokyo", "Osaka", "Japan Airlines", 110000, 75);
    addFlight("Tokyo", "Taipei", "ANA", 220000, 170);
    addFlight("Shanghai", "Taipei", "China Eastern", 150000, 115);
    addFlight("Osaka", "Taipei", "Peach", 135000, 160);

    addFlight("Jeju", "Seoul", "Jeju Air", 82000, 70);
    addFlight("Busan", "Seoul", "Korean Air", 72000, 65);
    addFlight("Tokyo", "Seoul", "Asiana", 230000, 135);
    addFlight("Shanghai", "Seoul", "T'way", 195000, 125);
    addFlight("Jeju", "Busan", "Air Busan", 64000, 55);
    addFlight("Osaka", "Busan", "Air Busan", 168000, 100);
    addFlight("Osaka", "Jeju", "Jeju Air", 152000, 110);
    addFlight("Taipei", "Jeju", "Jin Air", 178000, 135);
    addFlight("Osaka", "Tokyo", "Japan Airlines", 118000, 75);
    addFlight("Taipei", "Tokyo", "ANA", 225000, 170);
    addFlight("Taipei", "Shanghai", "China Eastern", 155000, 115);
    addFlight("Taipei", "Osaka", "Peach", 142000, 160);
}

void FlightGraph::addFlight(const QString &from, const QString &to, const QString &airline, int price, int durationMinutes)
{
    adjacencyList[from].push_back({from, to, airline, price, durationMinutes});
    if (!adjacencyList.contains(to)) {
        adjacencyList[to] = QVector<Flight>();
    }
}

QStringList FlightGraph::airports() const
{
    QStringList names = adjacencyList.keys();
    names.sort();
    return names;
}

QVector<Flight> FlightGraph::directFlights(const QString &from, const QString &to) const
{
    QVector<Flight> result;
    for (const Flight &flight : adjacencyList.value(from)) {
        if (flight.to == to) {
            result.push_back(flight);
        }
    }
    return result;
}

QVector<Flight> FlightGraph::flightsFrom(const QString &from) const
{
    return adjacencyList.value(from);
}

RouteResult FlightGraph::cheapestRoute(const QString &from, const QString &to) const
{
    return dijkstra(from, to, true);
}

RouteResult FlightGraph::fastestRoute(const QString &from, const QString &to) const
{
    return dijkstra(from, to, false);
}

RouteResult FlightGraph::dijkstra(const QString &from, const QString &to, bool usePriceWeight) const
{
    const int infinity = std::numeric_limits<int>::max() / 4;
    QMap<QString, int> distance;
    QMap<QString, Flight> previousFlight;
    QSet<QString> visited;

    for (const QString &airport : adjacencyList.keys()) {
        distance[airport] = infinity;
    }
    distance[from] = 0;

    while (visited.size() < adjacencyList.size()) {
        QString current;
        int bestDistance = infinity;

        for (auto it = distance.constBegin(); it != distance.constEnd(); ++it) {
            if (!visited.contains(it.key()) && it.value() < bestDistance) {
                bestDistance = it.value();
                current = it.key();
            }
        }

        if (current.isEmpty() || current == to) {
            break;
        }

        visited.insert(current);

        for (const Flight &flight : adjacencyList.value(current)) {
            const int weight = usePriceWeight ? flight.price : flight.durationMinutes;
            const int candidate = distance[current] + weight;
            if (candidate < distance[flight.to]) {
                distance[flight.to] = candidate;
                previousFlight[flight.to] = flight;
            }
        }
    }

    RouteResult result;
    if (!previousFlight.contains(to) && from != to) {
        return result;
    }

    QString step = to;
    while (step != from) {
        Flight flight = previousFlight.value(step);
        result.flights.prepend(flight);
        result.totalPrice += flight.price;
        result.totalDuration += flight.durationMinutes;
        step = flight.from;
    }

    result.found = !result.flights.isEmpty();
    return result;
}