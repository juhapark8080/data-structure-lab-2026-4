#include "mapwidget.h"

#include <QPainter>
#include <QFont>
#include <QLinearGradient>
#include <QSizePolicy>
#include <QPaintEvent>
#include <QPen>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(360);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF area = rect().adjusted(18, 18, -18, -18);

    QLinearGradient background(area.topLeft(), area.bottomRight());
    background.setColorAt(0.0, QColor("#eef7fb"));
    background.setColorAt(1.0, QColor("#f7f4eb"));
    painter.fillRect(rect(), background);

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

    if (!selectedFrom.isEmpty() && !selectedTo.isEmpty()
        && airportPositions.contains(selectedFrom) && airportPositions.contains(selectedTo)) {
        painter.setPen(QPen(QColor("#db4c40"), 4));
        painter.drawLine(toPoint(selectedFrom), toPoint(selectedTo));
    }

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

