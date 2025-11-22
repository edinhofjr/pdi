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
    
    setImage(":/images/images/no-image.png");
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