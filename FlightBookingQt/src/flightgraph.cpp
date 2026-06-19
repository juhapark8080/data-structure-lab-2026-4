#include "flightgraph.h"

#include <QSet>
#include <limits>

FlightGraph::FlightGraph()
{
    // 서버나 데이터베이스를 만들지 않는 프로젝트 조건에 맞춰 항공편 데이터를 직접 입력한다.
    // 각 addFlight 호출은 "출발 공항 -> 도착 공항" 방향 간선 하나를 그래프에 추가한다.
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
    // 인접 리스트의 from 위치에 항공편을 추가한다.
    // 이렇게 저장하면 특정 공항에서 출발하는 모든 항공편을 빠르게 조회할 수 있다.
    adjacencyList[from].push_back({from, to, airline, price, durationMinutes});

    // 도착지만 있고 출발 항공편이 없는 공항도 노드 목록에 나타나야 하므로 빈 리스트를 만든다.
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

    // from 공항의 인접 리스트만 확인하면 직항 간선 존재 여부를 알 수 있다.
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

    // 모든 공항까지의 거리를 무한대로 초기화하고, 시작 공항만 0으로 둔다.
    for (const QString &airport : adjacencyList.keys()) {
        distance[airport] = infinity;
    }
    distance[from] = 0;

    while (visited.size() < adjacencyList.size()) {
        QString current;
        int bestDistance = infinity;

        // 아직 방문하지 않은 공항 중 현재까지 비용이 가장 작은 공항을 선택한다.
        for (auto it = distance.constBegin(); it != distance.constEnd(); ++it) {
            if (!visited.contains(it.key()) && it.value() < bestDistance) {
                bestDistance = it.value();
                current = it.key();
            }
        }

        // 더 이상 갈 수 있는 공항이 없거나 목적지에 도착하면 탐색을 종료한다.
        if (current.isEmpty() || current == to) {
            break;
        }

        visited.insert(current);

        // 선택된 공항에서 출발하는 항공편들을 확인하며 더 저렴하거나 빠른 경로로 갱신한다.
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

    // 목적지까지 이어지는 이전 항공편 기록이 없으면 경로를 찾지 못한 것이다.
    if (!previousFlight.contains(to) && from != to) {
        return result;
    }

    // previousFlight를 목적지에서 출발지 방향으로 되짚어 실제 경로를 복원한다.
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
