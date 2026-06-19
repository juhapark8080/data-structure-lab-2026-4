#include "mapwidget.h"

#include <QFont>
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QSizePolicy>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(360);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 실제 지도 이미지 대신, 각 공항 노드를 화면 비율 좌표로 배치한다.
    // 값은 0~1 사이의 상대 좌표라 창 크기가 바뀌어도 비슷한 위치를 유지한다.
    airportPositions["Seoul"] = QPointF(0.28, 0.22);
    airportPositions["Busan"] = QPointF(0.34, 0.62);
    airportPositions["Jeju"] = QPointF(0.23, 0.82);
    airportPositions["Tokyo"] = QPointF(0.76, 0.28);
    airportPositions["Osaka"] = QPointF(0.64, 0.56);
    airportPositions["Shanghai"] = QPointF(0.20, 0.48);
    airportPositions["Taipei"] = QPointF(0.52, 0.84);
}

void MapWidget::setGraph(const FlightGraph *graph)
{
    flightGraph = graph;
    update();
}

void MapWidget::setSelectedAirports(const QString &from, const QString &to)
{
    selectedFrom = from;
    selectedTo = to;
    update();
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // paintEvent는 Qt가 화면을 다시 그릴 때 자동으로 호출된다.
    // 여기서 배경, 항공편 간선, 공항 노드를 순서대로 직접 그린다.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF area = rect().adjusted(18, 18, -18, -18);

    QLinearGradient background(area.topLeft(), area.bottomRight());
    background.setColorAt(0.0, QColor("#eef7fb"));
    background.setColorAt(1.0, QColor("#f7f4eb"));
    painter.fillRect(rect(), background);

    // 배경 격자는 지도 느낌을 주기 위한 시각적 요소이다.
    painter.setPen(QPen(QColor("#c7d6dc"), 1));
    for (int i = 1; i < 4; ++i) {
        const qreal x = area.left() + area.width() * i / 4.0;
        painter.drawLine(QPointF(x, area.top()), QPointF(x, area.bottom()));
        const qreal y = area.top() + area.height() * i / 4.0;
        painter.drawLine(QPointF(area.left(), y), QPointF(area.right(), y));
    }

    auto toPoint = [&](const QString &airport) {
        const QPointF normalized = airportPositions.value(airport);
        return QPointF(area.left() + normalized.x() * area.width(),
                       area.top() + normalized.y() * area.height());
    };

    // 그래프에 저장된 모든 항공편 간선을 회색 선으로 표시한다.
    if (flightGraph != nullptr) {
        painter.setPen(QPen(QColor("#9ab4bd"), 2));
        for (const QString &airport : flightGraph->airports()) {
            for (const Flight &flight : flightGraph->flightsFrom(airport)) {
                if (airportPositions.contains(flight.from) && airportPositions.contains(flight.to)) {
                    painter.drawLine(toPoint(flight.from), toPoint(flight.to));
                }
            }
        }
    }

    // 사용자가 선택한 출발지와 도착지는 빨간 선으로 강조해 현재 검색 조건을 보여준다.
    if (!selectedFrom.isEmpty() && !selectedTo.isEmpty()
        && airportPositions.contains(selectedFrom) && airportPositions.contains(selectedTo)) {
        painter.setPen(QPen(QColor("#db4c40"), 4));
        painter.drawLine(toPoint(selectedFrom), toPoint(selectedTo));
    }

    // 각 공항 노드는 원으로 표시한다. 출발지는 초록색, 도착지는 주황색으로 구분한다.
    for (auto it = airportPositions.constBegin(); it != airportPositions.constEnd(); ++it) {
        const QString airport = it.key();
        const QPointF point = toPoint(airport);
        QColor fill("#ffffff");
        QColor border("#23566b");

        if (airport == selectedFrom) {
            fill = QColor("#2f7d59");
            border = QColor("#17402d");
        } else if (airport == selectedTo) {
            fill = QColor("#d65f37");
            border = QColor("#7c321e");
        }

        painter.setBrush(fill);
        painter.setPen(QPen(border, 2));
        painter.drawEllipse(point, 11, 11);

        painter.setPen(QColor("#17313a"));
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(QRectF(point.x() - 42, point.y() + 13, 84, 22), Qt::AlignCenter, airport);
    }
}
