#pragma once
#include "ui_Smoothing.h"
#include <qthread.h>
#include <opencv2/core.hpp>

class ImageShop;

class Smoothing : public QDialog
{
    Q_OBJECT

public:
    Smoothing(QImage src, ImageShop *parent = Q_NULLPTR);
    ~Smoothing();
private:
    double sigma();
    double sigmaC();
    double sigmaS();
public slots:
    void onTuneUi();
    void onSetR();
    void onSetSigma();
    void onSetSigmaC();
    void onSetSigmaS();
    void onSmooth();
signals:
    void sendParameter(int st, int r, double sigma, double sigmaC, double sigmaS);
private:
    class Worker;
private:
    Ui::Smoothing ui;
    QThread thread;
    Worker *worker;
};

class Smoothing::Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QImage src, QObject *parent = Q_NULLPTR);
public slots:
    void onSmooth(int st, int r, double sigma, double sigmaC, double sigmaS);
signals:
    void sendImage(QImage);
private:
    cv::Mat source;
};

