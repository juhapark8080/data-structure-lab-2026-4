#include "mainwindow.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QStringList>
#include <QTableWidgetItem>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 메인 윈도우 생성 시 UI를 먼저 만들고, 그래프의 공항 데이터를 콤보박스에 넣는다.
    buildUi();
    populateAirports();
    searchDirectFlights();
}

void MainWindow::buildUi()
{
    // QMainWindow 기반의 전체 GUI를 구성한다.
    // 상단은 검색 조건, 왼쪽은 지도, 오른쪽은 항공편 목록과 예매 영역이다.
    setWindowTitle("Graph Flight Booking System");
    resize(1120, 720);

    QWidget *central = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(central);

    QLabel *title = new QLabel("Graph Flight Booking System", this);
    title->setStyleSheet("font-size: 24px; font-weight: 700; color: #17313a;");
    rootLayout->addWidget(title);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    fromComboBox = new QComboBox(this);
    toComboBox = new QComboBox(this);
    QPushButton *searchButton = new QPushButton("Search Direct Flights", this);
    QPushButton *cheapButton = new QPushButton("Cheapest Route", this);
    QPushButton *fastButton = new QPushButton("Fastest Route", this);

    searchLayout->addWidget(new QLabel("From", this));
    searchLayout->addWidget(fromComboBox);
    searchLayout->addWidget(new QLabel("To", this));
    searchLayout->addWidget(toComboBox);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(cheapButton);
    searchLayout->addWidget(fastButton);
    rootLayout->addLayout(searchLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // MapWidget은 그래프의 노드와 간선을 시각적으로 보여주는 영역이다.
    mapWidget = new MapWidget(this);
    mapWidget->setGraph(&graph);
    splitter->addWidget(mapWidget);

    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    flightTable = new QTableWidget(0, 5, this);
    flightTable->setHorizontalHeaderLabels({"Airline", "From", "To", "Price", "Time"});
    flightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    flightTable->setSelectionMode(QAbstractItemView::SingleSelection);
    flightTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rightLayout->addWidget(flightTable, 3);

    // 경로 탐색 결과의 전체 가격/시간과 환승 순서를 텍스트로 보여준다.
    routeSummary = new QTextEdit(this);
    routeSummary->setReadOnly(true);
    routeSummary->setMinimumHeight(130);
    rightLayout->addWidget(routeSummary, 1);

    bookButton = new QPushButton("Book Selected Flight", this);
    bookButton->setMinimumHeight(40);
    rightLayout->addWidget(bookButton);

    statusLabel = new QLabel("Select airports and search flights.", this);
    statusLabel->setStyleSheet("color: #23566b; font-weight: 600;");
    rightLayout->addWidget(statusLabel);

    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 3);
    rootLayout->addWidget(splitter, 1);

    setCentralWidget(central);

    setStyleSheet(
        "QWidget { font-family: Arial; font-size: 14px; }"
        "QPushButton { background: #23566b; color: white; border: 0; padding: 8px 12px; border-radius: 4px; }"
        "QPushButton:hover { background: #2f6f87; }"
        "QComboBox { padding: 6px; }"
        "QTableWidget { gridline-color: #d7e1e5; selection-background-color: #dbeee6; }"
        "QHeaderView::section { background: #e8f0f2; padding: 6px; font-weight: 700; border: 0; }"
    );

    connect(searchButton, &QPushButton::clicked, this, [this]() { searchDirectFlights(); });
    connect(cheapButton, &QPushButton::clicked, this, [this]() { showRoute(true); });
    connect(fastButton, &QPushButton::clicked, this, [this]() { showRoute(false); });
    connect(bookButton, &QPushButton::clicked, this, [this]() { bookSelectedFlight(); });

    // 출발지나 도착지를 바꾸면 지도에서 선택된 공항 표시도 즉시 갱신한다.
    connect(fromComboBox, &QComboBox::currentTextChanged, this, [this]() { mapWidget->setSelectedAirports(fromComboBox->currentText(), toComboBox->currentText()); });
    connect(toComboBox, &QComboBox::currentTextChanged, this, [this]() { mapWidget->setSelectedAirports(fromComboBox->currentText(), toComboBox->currentText()); });
}

void MainWindow::populateAirports()
{
    // 그래프에 저장된 공항 노드 목록을 GUI 선택 박스에 넣는다.
    const QStringList names = graph.airports();
    fromComboBox->addItems(names);
    toComboBox->addItems(names);

    fromComboBox->setCurrentText("Seoul");
    toComboBox->setCurrentText("Jeju");
    mapWidget->setSelectedAirports(fromComboBox->currentText(), toComboBox->currentText());
}

void MainWindow::searchDirectFlights()
{
    const QString from = fromComboBox->currentText();
    const QString to = toComboBox->currentText();

    // 직항 조회는 선택한 출발 공항의 인접 리스트에서 목적지가 같은 간선만 찾는다.
    currentFlights = graph.directFlights(from, to);
    displayFlights(currentFlights);

    if (currentFlights.isEmpty()) {
        routeSummary->setText("No direct flight found. Try Cheapest Route or Fastest Route to search transfer routes.");
        statusLabel->setText("Direct flight not found.");
    } else {
        routeSummary->setText(QString("%1 direct flight(s) found from %2 to %3.")
                                  .arg(currentFlights.size())
                                  .arg(from)
                                  .arg(to));
        statusLabel->setText("Direct flight search complete.");
    }
}

void MainWindow::showRoute(bool cheapest)
{
    const QString from = fromComboBox->currentText();
    const QString to = toComboBox->currentText();

    // 버튼 종류에 따라 가격 기준 또는 시간 기준 Dijkstra 탐색을 실행한다.
    const RouteResult route = cheapest ? graph.cheapestRoute(from, to) : graph.fastestRoute(from, to);
    displayRoute(route, cheapest ? "Cheapest route" : "Fastest route");
}

void MainWindow::displayFlights(const QVector<Flight> &flights)
{
    // 그래프 탐색 결과를 사용자가 비교하기 쉽도록 표 형태로 출력한다.
    flightTable->setRowCount(flights.size());

    for (int row = 0; row < flights.size(); ++row) {
        const Flight &flight = flights[row];
        flightTable->setItem(row, 0, new QTableWidgetItem(flight.airline));
        flightTable->setItem(row, 1, new QTableWidgetItem(flight.from));
        flightTable->setItem(row, 2, new QTableWidgetItem(flight.to));
        flightTable->setItem(row, 3, new QTableWidgetItem(money(flight.price)));
        flightTable->setItem(row, 4, new QTableWidgetItem(minutes(flight.durationMinutes)));
    }
}

void MainWindow::displayRoute(const RouteResult &route, const QString &title)
{
    if (!route.found) {
        currentFlights.clear();
        flightTable->setRowCount(0);
        routeSummary->setText("Route not found.");
        statusLabel->setText("Route search failed.");
        return;
    }

    currentFlights = route.flights;
    displayFlights(currentFlights);

    // 경로에 포함된 항공편을 순서대로 나열하고 전체 가격/시간을 함께 보여준다.
    QStringList lines;
    lines << title;
    for (const Flight &flight : route.flights) {
        lines << QString("%1 -> %2 / %3 / %4 / %5")
                     .arg(flight.from)
                     .arg(flight.to)
                     .arg(flight.airline)
                     .arg(money(flight.price))
                     .arg(minutes(flight.durationMinutes));
    }
    lines << QString("Total price: %1").arg(money(route.totalPrice));
    lines << QString("Total time: %1").arg(minutes(route.totalDuration));

    routeSummary->setText(lines.join("\n"));
    statusLabel->setText(title + " search complete.");
}

void MainWindow::bookSelectedFlight()
{
    const int row = flightTable->currentRow();
    if (row < 0 || row >= currentFlights.size()) {
        QMessageBox::information(this, "Booking", "Please select a flight first.");
        return;
    }

    // 실제 결제 기능은 없으므로 선택한 항공편 정보를 메시지 박스로 보여주며 예매 완료를 표현한다.
    const Flight flight = currentFlights[row];
    QMessageBox::information(this, "Booking Complete",
                             QString("Booking complete!\n\n%1: %2 -> %3\nPrice: %4\nTime: %5")
                                 .arg(flight.airline)
                                 .arg(flight.from)
                                 .arg(flight.to)
                                 .arg(money(flight.price))
                                 .arg(minutes(flight.durationMinutes)));
}

QString MainWindow::money(int value) const
{
    return QString("%1 KRW").arg(value);
}

QString MainWindow::minutes(int value) const
{
    return QString("%1 min").arg(value);
}
