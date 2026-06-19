#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "flightgraph.h"

#include <QMap>
#include <QPointF>
#include <QString>
#include <QWidget>

class MapWidget : public QWidget
{
public:
    explicit MapWidget(QWidget *parent = nullptr);

    void setGraph(const FlightGraph *graph);
    void setSelectedAirports(const QString &from, const QString &to);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const FlightGraph *flightGraph = nullptr;
    QString selectedFrom;
    QString selectedTo;
    QMap<QString, QPointF> airportPositions;
};

#endif