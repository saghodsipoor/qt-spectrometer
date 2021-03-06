#include "TestWindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QRandomGenerator>
#include <QPushButton>

#include "Screen.h"


TestWindow::TestWindow(QWidget *parent) : QMainWindow(parent)
{
    buildWindow();
    appendTimer();
}

void TestWindow::sendRandomSamples()
{
    QVector<qreal> ys;
    m_samplesSize = QRandomGenerator::global()->bounded(300, 700);
    for (auto i = 0; i < m_samplesSize; ++i)
        ys << QRandomGenerator::global()->bounded(0, 100);
    emit receivedNewSamples(ys);
}

void TestWindow::appendTimer()
{
    m_timer->setInterval(UPDATE_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &TestWindow::sendRandomSamples);
    connect(this, &TestWindow::receivedNewSamples, m_spectrometer, &Spectrometer::updateScreen);

    connect(m_startButton, &QPushButton::pressed, this, [this](){
        m_timer->start();
    });
    connect(m_stopButton, &QPushButton::pressed, this, [this](){
        m_timer->stop(); }
    );
    connect(m_updateAxisButton, &QPushButton::pressed, this, &TestWindow::updateSpectrometerAxis);

    m_timer->start();
}

void TestWindow::buildWindow()
{
    setWindowTitle("Spectrometer Test Window");
    setFixedSize(WIDTH, HEIGHT);

    m_spectrometer = new Spectrometer {this};
    m_timer  = new QTimer{this};
    m_startButton = new QPushButton {"Start", this};
    m_stopButton = new QPushButton {"Stop", this};
    m_updateAxisButton = new QPushButton {"Update Axis", this};

    m_startButton->move(width() - 150, 20);
    m_stopButton->move(width() - 150, 60);
    m_updateAxisButton->move(width() - 150, 140);

    m_xsLabel = new QLineEdit {this};
    m_xsValues = new QLineEdit {this};
    m_ysLabel = new QLineEdit {this};
    m_ysValues = new QLineEdit {this};

    m_xsLabel->setPlaceholderText("Xs Label");
    m_xsValues->setPlaceholderText("Xs Values");
    m_ysLabel->setPlaceholderText("Ys Label");
    m_ysValues->setPlaceholderText("Ys Values");

    m_xsLabel->move(width() - 370, 100);
    m_xsValues->move(width() - 260, 100);
    m_ysLabel->move(width() - 370, 140);
    m_ysValues->move(width() - 260, 140);
}

void TestWindow::updateSpectrometerAxis()
{
    QVector<qreal> newXValues, newYValues;

    for (const auto& valString : m_xsValues->text().split(" "))
        newXValues << valString.toDouble();
    for (const auto& valString : m_ysValues->text().split(" "))
        newYValues << valString.toDouble();

    m_spectrometer->updateXAxis(m_xsLabel->text(), newXValues);
    m_spectrometer->updateYAxis(m_ysLabel->text(), newYValues);
}
