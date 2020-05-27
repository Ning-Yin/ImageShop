#include "EdgeDetection.h"
#include "ImageShop.h"
#include <opencv2/imgproc.hpp>
#include <qdebug.h>

using namespace cv;

EdgeDetection::EdgeDetection(QImage src, ImageShop *parent)
    : QDialog(parent) {
    ui.setupUi(this);
    
    switch (src.format())
    {
    case QImage::Format_Grayscale8:
        source = Mat(src.height(), src.width(), CV_8UC1,
            (void *)src.constBits(), src.bytesPerLine());
        break;
    case QImage::Format_RGB32:
        source = Mat(src.height(), src.width(), CV_8UC4,
            (void *)src.constBits(), src.bytesPerLine());
        cvtColor(source, source, COLOR_BGRA2RGB);
        break;
    case QImage::Format_ARGB32:
        source = Mat(src.height(), src.width(), CV_8UC4,
            (void *)src.constBits(), src.bytesPerLine());
        break;
    default:
        break;
    }

    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &EdgeDetection::onTuneUi);
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &EdgeDetection::onDetectEdge);
    connect(ui.comboBoxDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &EdgeDetection::onDetectEdge);
    connect(ui.sliderR, &QSlider::valueChanged, this, &EdgeDetection::onSetR);
    connect(ui.sliderR, &QSlider::valueChanged, this, &EdgeDetection::onDetectEdge);
    connect(ui.sliderThreshold1, &QSlider::valueChanged, this, &EdgeDetection::onSetThreshold1);
    connect(ui.sliderThreshold1, &QSlider::valueChanged, this, &EdgeDetection::onDetectEdge);
    connect(ui.sliderThreshold2, &QSlider::valueChanged, this, &EdgeDetection::onSetThreshold2);
    connect(ui.sliderThreshold2, &QSlider::valueChanged, this, &EdgeDetection::onDetectEdge);
    connect(ui.pushButton, &QPushButton::clicked, this, &EdgeDetection::accept);
    connect(this, &EdgeDetection::sendImage, parent, &ImageShop::onReceiveTarget);
    
    ui.labelDirection->hide();
    ui.comboBoxDirection->hide();
    ui.labelR->hide();
    ui.sliderR->hide();
    ui.valueR->hide();
    adjustSize();
    qDebug() << windowFlags();
    setWindowFlags(
        (windowFlags() | Qt::MSWindowsFixedSizeDialogHint)
        & ~Qt::WindowContextHelpButtonHint);
    qDebug() << windowFlags();
    ui.labelThreshold1->hide();
    ui.sliderThreshold1->hide();
    ui.valueThreshold1->hide();
    ui.labelThreshold2->hide();
    ui.sliderThreshold2->hide();
    ui.valueThreshold2->hide();
}

void EdgeDetection::onTuneUi() {
    switch (ui.comboBox->currentIndex()) {
    case 0:
        // Sobel
        ui.labelDirection->show();
        ui.comboBoxDirection->show();
        ui.labelR->show();
        ui.sliderR->show();
        ui.valueR->show();
        ui.labelThreshold1->hide();
        ui.sliderThreshold1->hide();
        ui.valueThreshold1->hide();
        ui.labelThreshold2->hide();
        ui.sliderThreshold2->hide();
        ui.valueThreshold2->hide();
        break;
    case 1:
        // Scharr
        ui.labelDirection->show();
        ui.comboBoxDirection->show();
        ui.labelR->hide();
        ui.sliderR->hide();
        ui.valueR->hide();
        ui.labelThreshold1->hide();
        ui.sliderThreshold1->hide();
        ui.valueThreshold1->hide();
        ui.labelThreshold2->hide();
        ui.sliderThreshold2->hide();
        ui.valueThreshold2->hide();
        break;
    case 2:
        // Laplacian
    case 3:
        // LoG
        ui.labelDirection->hide();
        ui.comboBoxDirection->hide();
        ui.labelR->show();
        ui.sliderR->show();
        ui.valueR->show();
        ui.labelThreshold1->hide();
        ui.sliderThreshold1->hide();
        ui.valueThreshold1->hide();
        ui.labelThreshold2->hide();
        ui.sliderThreshold2->hide();
        ui.valueThreshold2->hide();
        break;
    case 4:
        // Canny
        ui.labelDirection->hide();
        ui.comboBoxDirection->hide();
        ui.labelR->hide();
        ui.sliderR->hide();
        ui.valueR->hide();
        ui.labelThreshold1->show();
        ui.sliderThreshold1->show();
        ui.valueThreshold1->show();
        ui.labelThreshold2->show();
        ui.sliderThreshold2->show();
        ui.valueThreshold2->show();
        break;
    default:
        break;
    }
}

void EdgeDetection::onSetR() {
    ui.valueR->setText(QString::number(ui.sliderR->value()));
}

void EdgeDetection::onSetThreshold1() {
    ui.valueThreshold1->setText(QString::number(ui.sliderThreshold1->value()).leftJustified(3));
}

void EdgeDetection::onSetThreshold2() {
    ui.valueThreshold2->setText(QString::number(ui.sliderThreshold2->value()).leftJustified(3));
}

void EdgeDetection::onDetectEdge() {
    Mat target;
    int ksize;
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        // Sobel
        ksize = ui.sliderR->value() * 2 - 1;
        if (ui.comboBoxDirection->currentIndex() == 0) {
            Sobel(source, target, -1, 0, 1, ksize);
            convertScaleAbs(target, target);
        }
        else if (ui.comboBoxDirection->currentIndex() == 1) {
            Sobel(source, target, -1, 1, 0, ksize);
            convertScaleAbs(target, target);
        }
        else {
            Sobel(source, target, -1, 1, 1, ksize);
            convertScaleAbs(target, target);
        }
        break;
    case 1:
        // Scharr
        if (ui.comboBoxDirection->currentIndex() == 0) {
            Scharr(source, target, -1, 0, 1);
            convertScaleAbs(target, target);
        }
        else if (ui.comboBoxDirection->currentIndex() == 1) {
            Scharr(source, target, -1, 1, 0);
            convertScaleAbs(target, target);
        }
        else {
            Mat hTarget, vTarget;
            Scharr(source, hTarget, -1, 0, 1);
            Scharr(source, vTarget, -1, 1, 0);
            convertScaleAbs(hTarget, hTarget);
            convertScaleAbs(vTarget, vTarget);
            addWeighted(hTarget, 0.5, vTarget, 0.5, 0, target);
        }
        break;
    case 2:
        // Laplacian
        ksize = ui.sliderR->value() * 2 - 1;
        Laplacian(source, target, -1, ksize);
        convertScaleAbs(target, target);
        break;
    case 3:
        // LoG
        ksize = ui.sliderR->value() * 2 - 1;
        GaussianBlur(source, target, Size(ksize, ksize), 0, 0);
        Laplacian(target, target, -1, ksize);
        convertScaleAbs(target, target);
        break;
    case 4:
        // Canny
        Canny(source, target, ui.sliderThreshold1->value(), ui.sliderThreshold2->value());
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
        qTarget = QImage(pSrc, target.cols, target.rows, target.step, QImage::Format_RGB888)
            .copy();
        qTarget = qTarget.convertToFormat(QImage::Format_RGB32);
    }
    else if (target.type() == CV_8UC4) {
        const uchar *pSrc = (const uchar *)target.data;
        qTarget = QImage(pSrc, target.cols, target.rows, target.step, QImage::Format_ARGB32)
            .copy();
    }
    emit sendImage(qTarget);
}