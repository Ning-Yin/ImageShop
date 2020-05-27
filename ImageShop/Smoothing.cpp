#include "Smoothing.h"
#include "ImageShop.h"
#include "Help.h"
#include <opencv2/imgproc.hpp>

using namespace cv;

Smoothing::Smoothing(QImage src, ImageShop *parent)
    : QDialog(parent) {
    ui.setupUi(this);

    switch (src.format())
    {
    case QImage::Format_Grayscale8:
        source = Mat(src.height(), src.width(), CV_8UC1, (void *)src.constBits(), src.bytesPerLine());
        break;
    case QImage::Format_RGB32:
        source = Mat(src.height(), src.width(), CV_8UC4, (void *)src.constBits(), src.bytesPerLine());
        cvtColor(source, source, COLOR_BGRA2RGB);
        break;
    case QImage::Format_ARGB32:
        source = Mat(src.height(), src.width(), CV_8UC4, (void *)src.constBits(), src.bytesPerLine());
        break;
    default:
        break;
    }
    
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Smoothing::onTuneUi);
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Smoothing::onSmooth);
    connect(ui.sliderR, &QSlider::valueChanged, this, &Smoothing::onSetR);
    connect(ui.sliderR, &QSlider::valueChanged, this, &Smoothing::onSmooth);
    connect(ui.sliderSigma, &QSlider::valueChanged, this, &Smoothing::onSetSigma);
    connect(ui.sliderSigma, &QSlider::valueChanged, this, &Smoothing::onSmooth);
    connect(ui.sliderSigmaC, &QSlider::valueChanged, this, &Smoothing::onSetSigmaC);
    connect(ui.sliderSigmaC, &QSlider::valueChanged, this, &Smoothing::onSmooth);
    connect(ui.sliderSigmaS, &QSlider::valueChanged, this, &Smoothing::onSetSigmaS);
    connect(ui.sliderSigmaS, &QSlider::valueChanged, this, &Smoothing::onSmooth);
    connect(ui.pushButton, &QPushButton::clicked, this, &Smoothing::accept);
    connect(ui.pushButtonHelp, &QPushButton::clicked, this, &Smoothing::onHelp);
    connect(this, &Smoothing::sendImage, parent, &ImageShop::onReceiveTarget);

    ui.labelR->hide();
    ui.sliderR->hide();
    ui.valueR->hide();
    ui.labelSigma->hide();
    ui.sliderSigma->hide();
    ui.valueSigma->hide();
    adjustSize();
    setWindowFlags(
        (windowFlags()
            | Qt::MSWindowsFixedSizeDialogHint)
        & ~Qt::WindowContextHelpButtonHint);
    ui.labelSigmaC->hide();
    ui.sliderSigmaC->hide();
    ui.valueSigmaC->hide();
    ui.labelSigmaS->hide();
    ui.sliderSigmaS->hide();
    ui.valueSigmaS->hide();
}

double Smoothing::sigma() {
    int slider = ui.sliderSigma->value();
    double ret;
    // 1~100     =>   0.1~10.0
    // 101~200   =>   10.2~30.0
    if (slider <= 100) {
        ret = slider / 10.0;
    }
    else {
        ret = slider / 5.0 - 10.0;
    }
    return ret;
}

double Smoothing::sigmaC() {
    int slider = ui.sliderSigmaC->value();
    double ret;
    // 1~100     =>   0.1~10.0
    // 101~300   =>   10.2~50.0
    // 301~500   =>   50.5~150.0
    if (slider <= 100) {
        ret = slider / 10.0;
    }
    else if (slider <= 300) {
        ret = slider / 5.0 - 10.0;
    }
    else {
        ret = slider / 2.0 - 100.0;
    }
    return ret;
}

double Smoothing::sigmaS() {
    int slider = ui.sliderSigmaS->value();
    double ret;
    // 1~100     =>   0.1~10.0
    // 101~200   =>   10.2~30.0
    if (slider <= 100) {
        ret = slider / 10.0;
    }
    else {
        ret = slider / 5.0 - 10.0;
    }
    return ret;
}

void Smoothing::onTuneUi() {
    switch (ui.comboBox->currentIndex())
    {
    case 0:
    case 1:
    case 2:
    case 3:
        ui.labelR->show();
        ui.sliderR->show();
        ui.valueR->show();
        ui.labelSigma->hide();
        ui.sliderSigma->hide();
        ui.valueSigma->hide();
        ui.labelSigmaC->hide();
        ui.sliderSigmaC->hide();
        ui.valueSigmaC->hide();
        ui.labelSigmaS->hide();
        ui.sliderSigmaS->hide();
        ui.valueSigmaS->hide();
        break;
    case 4:
        ui.labelR->hide();
        ui.sliderR->hide();
        ui.valueR->hide();
        ui.labelSigma->show();
        ui.sliderSigma->show();
        ui.valueSigma->show();
        ui.labelSigmaC->hide();
        ui.sliderSigmaC->hide();
        ui.valueSigmaC->hide();
        ui.labelSigmaS->hide();
        ui.sliderSigmaS->hide();
        ui.valueSigmaS->hide();
        break;
    case 5:
        ui.labelR->hide();
        ui.sliderR->hide();
        ui.valueR->hide();
        ui.labelSigma->hide();
        ui.sliderSigma->hide();
        ui.valueSigma->hide();
        ui.labelSigmaC->show();
        ui.sliderSigmaC->show();
        ui.valueSigmaC->show();
        ui.labelSigmaS->show();
        ui.sliderSigmaS->show();
        ui.valueSigmaS->show();
        break;
    default:
        break;
    }
}

void Smoothing::onSetR() {
    ui.valueR->setText(QString::number(ui.sliderR->value()).leftJustified(4, ' '));
}

void Smoothing::onSetSigma() {
    ui.valueSigma->setText(QString::number(sigma()).leftJustified(4, ' '));
}

void Smoothing::onSetSigmaC() {
    ui.valueSigmaC->setText(QString::number(sigmaC()).leftJustified(4, ' '));
}

void Smoothing::onSetSigmaS() {
    ui.valueSigmaS->setText(QString::number(sigmaS()).leftJustified(4, ' '));
}

void Smoothing::onSmooth() {
    Mat target;
    int ksize;
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        // mean
        ksize = ui.sliderR->value() * 2 - 1;
        blur(source, target, Size(ksize, ksize));
        break;
    case 1:
        // median
        ksize = ui.sliderR->value() * 2 - 1;
        medianBlur(source, target, ksize);
        break;
    case 2:
        // maximum
        ksize = ui.sliderR->value() * 2 - 1;
        dilate(source, target, getStructuringElement(MORPH_RECT, Size(ksize, ksize)));
        break;
    case 3:
        // minimum
        ksize = ui.sliderR->value() * 2 - 1;
        erode(source, target, getStructuringElement(MORPH_RECT, Size(ksize, ksize)));
        break;
    case 4:
        // gaussian
        GaussianBlur(source, target, Size(0, 0), sigma());
        break;
    case 5:
        // bilateral
        bilateralFilter(source, target, 0, sigmaC(), sigmaS());
        break;
    default:
        break;
    }
    QImage qTarget;
    if (target.type() == CV_8UC1) {
        qTarget = QImage(target.cols, target.rows, QImage::Format_Grayscale8);
        uchar *pSrc = target.data;
        for (int row = 0; row < target.rows; row++) {
            uchar *pDest = qTarget.scanLine(row);
            memcpy(pDest, pSrc, target.cols);
            pSrc += target.step;
        }
    }
    else if (target.type() == CV_8UC3) {
        const uchar *pSrc = (const uchar *)target.data;
        qTarget = QImage(pSrc, target.cols, target.rows, target.step, QImage::Format_RGB888).copy();
        qTarget = qTarget.convertToFormat(QImage::Format_RGB32);
    }
    else if (target.type() == CV_8UC4) {
        const uchar *pSrc = (const uchar *)target.data;
        qTarget = QImage(pSrc, target.cols, target.rows, target.step, QImage::Format_ARGB32).copy();
    }
    emit sendImage(qTarget);
}

void Smoothing::onHelp() {
    SmoothingHelp *help = new SmoothingHelp(this);
    help->exec();
}
