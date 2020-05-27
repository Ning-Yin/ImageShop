#include "Formatting.h"
#include "ImageShop.h"

Formatting::Formatting(QImage src, ImageShop *parent)
    : QDialog(parent), source(src) {
    ui.setupUi(this);
    
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &Formatting::onFormat);
    connect(ui.pushButton, &QPushButton::clicked, this, &Formatting::accept);
    connect(this, &Formatting::sendImage, parent, &ImageShop::onReceiveTarget);

    adjustSize();
    setWindowFlags(
        (windowFlags() | Qt::MSWindowsFixedSizeDialogHint)
        & ~Qt::WindowContextHelpButtonHint);
}

void Formatting::onFormat() {
    QImage target;
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        target = source.convertToFormat(QImage::Format_Mono);
        break;
    case 1:
        target = source.convertToFormat(QImage::Format_Grayscale8);
        break;
    case 2:
        target = source.convertToFormat(QImage::Format_RGB32);
        break;
    case 3:
        target = source.convertToFormat(QImage::Format_ARGB32);
        break;
    default:
        break;
    }
    emit sendImage(target);
}