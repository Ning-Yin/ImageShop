#pragma once
#include "ui_FeatureExtraction.h"
#include <opencv2/core.hpp>
#include <qthread.h>

class ImageShop;

class FeatureExtraction : public QDialog
{
    Q_OBJECT

public:
    FeatureExtraction(QImage src, ImageShop *parent = Q_NULLPTR);
    ~FeatureExtraction();
private:
    double k();
public slots:
    void onTuneUi();
    void onExtractFeature();
    void onSetBr();
    void onSetKr();
    void onSetK();
    void onSetRt();
    void onSetNf();
signals:
    void sendParameter(int ft, int br, int kr, double k, int rt, int nf);
private:
    class Worker;
private:
    Ui::FeatureExtraction ui;
    QThread thread;
    Worker *worker;
};

class FeatureExtraction::Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QImage src, QObject *parent = Q_NULLPTR);
public slots:
    void onExtractFeature(int ft, int br, int kr, double k, int rt, int nf);
signals:
    void sendImage(QImage);
private:
    cv::Mat source, source_gray;
};
