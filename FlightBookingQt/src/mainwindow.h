#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flightgraph.h"
#include "mapwidget.h"

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    FlightGraph graph;
    MapWidget *mapWidget = nullptr;
    QComboBox *fromComboBox = nullptr;
    QComboBox *toComboBox = nullptr;
    QTableWidget *flightTable = nullptr;
    QTextEdit *routeSummary = nullptr;
    QPushButton *bookButton = nullptr;
    QLabel *statusLabel = nullptr;
    QVector<Flight> currentFlights;

    void buildUi();
    void populateAirports();
    void searchDirectFlights();
    void showRoute(bool cheapest);
    void displayFlights(const QVector<Flight> &flights);
    void displayRoute(const RouteResult &route, const QString &title);
    void bookSelectedFlight();
    QString money(int value) const;
    QString minutes(int value) const;
};

#endif
