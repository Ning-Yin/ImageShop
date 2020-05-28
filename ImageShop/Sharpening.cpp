#include "Sharpening.h"
#include "ImageShop.h"
#include <opencv2/imgproc.hpp>

using namespace cv;

Sharpening::Sharpening(QImage src, ImageShop *parent)
    : QDialog(parent) {
    ui.setupUi(this);

    switch (src.format())
    {
    case QImage::Format_Grayscale8:
        source = Mat(src.height(), src.width(), CV_8UC1, (void *)src.constBits(), src.bytesPerLine());
        break;
    case QImage::Format_RGB32:
        source = Mat(src.height(), src.width(), CV_8UC4, (void *)src.constBits(), src.bytesPerLine());
        cvtColor(source, source, COLOR_BGRA2RGB); // big endian
        // cvtColor(source, source, COLOR_RGBA2RGB); // little endian
        break;
    case QImage::Format_ARGB32:
        source = Mat(src.height(), src.width(), CV_8UC4, (void *)src.constBits(), src.bytesPerLine());
        break;
    default:
        break;
    }

    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sharpening::onTuneUi);
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Sharpening::onSharpen);
    connect(ui.sliderR, &QSlider::valueChanged, this, &Sharpening::onSetR);
    connect(ui.sliderR, &QSlider::valueChanged, this, &Sharpening::onSharpen);
    connect(ui.pushButton, &QPushButton::clicked, this, &Sharpening::accept);
    connect(this, &Sharpening::sendImage, parent, &ImageShop::onReceiveTarget);

    ui.labelR->hide();
    ui.sliderR->hide();
    ui.valueR->hide();
    adjustSize();
    setWindowFlags((windowFlags() | Qt::MSWindowsFixedSizeDialogHint) & ~Qt::WindowContextHelpButtonHint);
}

void Sharpening::onTuneUi() {
    switch (ui.comboBox->currentIndex())
    {
    case 0:
    case 1:
    case 2:
        ui.labelR->show();
        ui.sliderR->show();
        ui.valueR->show();
        break;
    case 3:
    case 4:
        ui.labelR->hide();
        ui.sliderR->hide();
        ui.valueR->hide();
        break;
    default:
        break;
    }
}

void Sharpening::onSetR() {
    ui.valueR->setText(QString::number(ui.sliderR->value()));
}

void Sharpening::onSharpen() {
    Mat target;
    Mat kernel;
    int ksize = ui.sliderR->value() * 2 - 1;
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        // mean
        blur(source, target, Size(ksize, ksize));
        addWeighted(source, 2, target, -1, 0, target);
        break;
    case 1:
        // median
        medianBlur(source, target, ksize);
        addWeighted(source, 2, target, -1, 0, target);
        break;
    case 2:
        // gaussian
        GaussianBlur(source, target, Size(ksize, ksize), 0);
        addWeighted(source, 2, target, -1, 0, target);
        break;
    case 3:
        // Laplacian 4
        kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
        filter2D(source, target, -1, kernel);
        convertScaleAbs(target, target);
        break;
    case 4:
        // Laplacian 8
        kernel = (Mat_<char>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
        filter2D(source, target, -1, kernel);
        convertScaleAbs(target, target);
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
