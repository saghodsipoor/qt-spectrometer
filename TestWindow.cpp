#include "TestWindow.h"

//test
#include <QLineEdit>
#include <QPushButton>
#include <QRandomGenerator>
//tset

#include <QPushButton>

#include "Screen.h"
#include "Xaxis.h"

TestWindow::TestWindow(QWidget *parent) : QMainWindow(parent)
{
    buildWindow();
    appendTimer();
}

void TestWindow::sendRandomSamples()
{
    QVector<qreal> ys;
    for (auto i = 0; i < m_samplesSize; ++i)
        ys << QRandomGenerator::global()->bounded(0, 100);
    emit receivedNewSamples(ys);
}

void TestWindow::appendTimer()
{
    m_timer->setInterval(UPDATE_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &TestWindow::sendRandomSamples);
    connect(this, &TestWindow::receivedNewSamples, m_spectrometer, &Spectrometer::updateSpectrometer);

    connect(m_startButton, &QPushButton::pressed, this, [this](){
        m_timer->start();
    });
    connect(m_stopButton, &QPushButton::pressed, this, [this](){
        m_timer->stop(); }
    );

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

    m_startButton->move(width() - 150, 20);
    m_stopButton->move(width() - 150, 60);

    auto xAxis = new Xaxis {this};
    xAxis->setFixedWidth(m_spectrometer->width());
    xAxis->move(0, m_spectrometer->height());

}

