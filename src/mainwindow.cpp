#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <jpeglib.h>
#include "stb_image.h"
#include <transformdialog.h>
#include "mainwindow.h"

static QImage QImageFromImage(const Image* img) {
    if (!img || !img->pixels || img->w <= 0 || img->h <= 0)
        return QImage();

    QImage::Format format =
        (img->channels == 3) ? QImage::Format_RGB888 :
            (img->channels == 4) ? QImage::Format_RGBA8888 :
            QImage::Format_Invalid;

    if (format == QImage::Format_Invalid)
        return QImage();

    QImage result(img->w, img->h, format);
    memcpy(result.bits(), img->pixels, img->w * img->h * img->channels);

    return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    image(nullptr)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    widget->setLayout(layout);

    label = new QLabel(this);
    layout->addWidget(label, 0, Qt::AlignCenter);

    connect(ui->actionAbrir, &QAction::triggered, this, &MainWindow::onExplore);
    connect(ui->actionTranslate, &QAction::triggered, this, &MainWindow::onTranslate);
    connect(ui->actionFechar, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionRotate, &QAction::triggered, this, &MainWindow::onRotate);
    connect(ui->actionMirrorHorizontal, &QAction::triggered, this, &MainWindow::onMirrorHorizontal);
    connect(ui->actionMirrorVertical, &QAction::triggered, this, &MainWindow::onMirrorVertical);
    connect(ui->actionBrightness, &QAction::triggered, this, &MainWindow::onBrightness);
    connect(ui->actionContrast, &QAction::triggered, this, &MainWindow::onContrast);
    connect(ui->actionGrayscale, &QAction::triggered, this, &MainWindow::onGrayscale);
    connect(ui->actionThreshold, &QAction::triggered, this, &MainWindow::onThreshold);
    connect(ui->actionDilation, &QAction::triggered, this, &MainWindow::onDilation);
}

MainWindow::~MainWindow() {
    delete image;
    delete ui;
}

void MainWindow::replaceImage(Image* newImg) {
    if (image != newImg) {
        delete image;
        image = newImg;
    }
    display();
}

void MainWindow::display() {
    label->setPixmap(
        QPixmap::fromImage(QImageFromImage(image))
            .scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
}

void MainWindow::setImage(const char* filename) {
    replaceImage(new Image(filename));
}

void MainWindow::setImage(Image *img) {
    replaceImage(img);
}

void MainWindow::onExplore() {
    QString file_path = QFileDialog::getOpenFileName(
        this,
        "Escolha uma imagem",
        QDir::homePath(),
        "Images (*.jpg *.jpeg *.png)"
        );

    if (file_path.isEmpty())
        return;

    replaceImage(new Image(file_path.toUtf8().constData()));
}

void MainWindow::onTranslate() {
     TransformDialog dlg(
        {
            {"Translate X", "0"},
            {"Translate Y", "0"}
        },
        this
    );

    if (dlg.exec() == QDialog::Accepted) {
        std::vector<QString> values = dlg.getValues();
        int tx = values[0].toInt();
        int ty = values[1].toInt();
        this->replaceImage(image->translate(tx, ty));
    }
}

void MainWindow::onRotate() {
     TransformDialog dlg(
        {
            {"Angle (degrees)", "0"}
        },
        this
    );

    if (dlg.exec() == QDialog::Accepted) {
        std::vector<QString> values = dlg.getValues();
        float angle = values[0].toFloat();
        this->replaceImage(image->rotate(angle));
    }
}

void MainWindow::onMirrorHorizontal() {
    this->replaceImage(image->mirror_h());
}

void MainWindow::onMirrorVertical() {
    this->replaceImage(image->mirror_v());
}

void MainWindow::onBrightness() {
     TransformDialog dlg(
        {
            {"Brightness Adjustment", "0"}
        },
        this
    );

    if (dlg.exec() == QDialog::Accepted) {
        std::vector<QString> values = dlg.getValues();
        int adjustment = values[0].toInt();
        this->replaceImage(image->brightness(adjustment));
    }
}

void MainWindow::onContrast() {
     TransformDialog dlg(
        {
            {"Contrast Factor", "1.0"}
        },
        this
    );

    if (dlg.exec() == QDialog::Accepted) {
        std::vector<QString> values = dlg.getValues();
        float factor = values[0].toFloat();
        this->replaceImage(image->contrast(factor));
    }
}

void MainWindow::onThreshold() {
     TransformDialog dlg(
        {
            {"Threshold Value", "128"}
        },
        this
    );

    if (dlg.exec() == QDialog::Accepted) {
        std::vector<QString> values = dlg.getValues();
        int threshold = values[0].toInt();
        this->replaceImage(image->threshold(threshold));
    }
}

void MainWindow::onDilation() {

}

void MainWindow::onGrayscale() {
    this->replaceImage(image->gray_scale());
}