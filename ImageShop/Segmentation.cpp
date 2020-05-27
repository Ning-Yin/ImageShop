#include "Segmentation.h"
#include "ImageShop.h"
#include <opencv2/imgproc.hpp>

using namespace cv;

Segmentation::Segmentation(QImage src, ImageShop *parent)
    : QDialog(parent) {
    ui.setupUi(this);

    source = Mat(src.height(), src.width(), CV_8UC1, (void *)src.constBits(), src.bytesPerLine());
    connect(ui.sliderThreshold, &QSlider::valueChanged, this, &Segmentation::onSegment);
    connect(ui.sliderThreshold, &QSlider::valueChanged, this, &Segmentation::onSetThreshold);
    connect(ui.pushButtonOtsu, &QPushButton::clicked, this, &Segmentation::onOtsu);
    connect(ui.pushButtonTriangle, &QPushButton::clicked, this, &Segmentation::onTriangle);
    connect(ui.pushButton, &QPushButton::clicked, this, &Segmentation::accept);
    connect(this, &Segmentation::sendImage, parent, &ImageShop::onReceiveTarget);

    adjustSize();
    setWindowFlags(
        (windowFlags()
            | Qt::MSWindowsFixedSizeDialogHint)
        & ~Qt::WindowContextHelpButtonHint);
}

void Segmentation::onSetThreshold() {
    ui.valueThreshold->setText(QString::number(ui.sliderThreshold->value()).leftJustified(3));
}

void Segmentation::onSegment() {
    Mat target;
    int t = ui.sliderThreshold->value();
    threshold(source, target, t, 255, THRESH_BINARY);
    QImage qTarget;
    qTarget = QImage(target.cols, target.rows, QImage::Format_Grayscale8);
    uchar *pSrc = target.data;
    for (int row = 0; row < target.rows; row++) {
        uchar *pDest = qTarget.scanLine(row);
        memcpy(pDest, pSrc, target.cols);
        pSrc += target.step;
    }
    emit sendImage(qTarget);
}

void Segmentation::onOtsu() {
    Mat target;
    int t = threshold(source, target, 0, 255, THRESH_OTSU);
    ui.sliderThreshold->setValue(t);
    //QImage qTarget;
    //qTarget = QImage(target.cols, target.rows, QImage::Format_Grayscale8);
    //uchar *pSrc = target.data;
    //for (int row = 0; row < target.rows; row++) {
    //    uchar *pDest = qTarget.scanLine(row);
    //    memcpy(pDest, pSrc, target.cols);
    //    pSrc += target.step;
    //}
    //emit sendImage(qTarget);
}

void Segmentation::onTriangle() {
    Mat target;
    int t = threshold(source, target, 0, 255, THRESH_TRIANGLE);
    ui.sliderThreshold->setValue(t);
    //QImage qTarget;
    //qTarget = QImage(target.cols, target.rows, QImage::Format_Grayscale8);
    //uchar *pSrc = target.data;
    //for (int row = 0; row < target.rows; row++) {
    //    uchar *pDest = qTarget.scanLine(row);
    //    memcpy(pDest, pSrc, target.cols);
    //    pSrc += target.step;
    //}
    //emit sendImage(qTarget);
}