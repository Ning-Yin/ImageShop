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
private:
    double k();
public slots:
    void onTuneUi();
    void onSetBr();
    void onSetKr();
    void onSetK();
    void onSetRt();
    void onExtractFeature();
signals:
    void sendImage(QImage);
private:
    class Worker;
private:
    Ui::FeatureExtraction ui;
    cv::Mat source, source_gray;
};

class FeatureExtraction::Worker : public QThread
{
    Q_OBJECT
};
