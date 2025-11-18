#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <jpeglib.h>

struct Pixel {
    uint8_t r,g,b;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *menu = this->menuBar()->addMenu("Arquivo");

    QAction *openAction = new QAction("Abrir", this);
    QAction *saveAction = new QAction("Salvar", this);
    QAction *aboutAction = new QAction("Sobre", this);
    QAction *exitAction = new QAction("Sair", this);

    menu->addAction(openAction);
    menu->addAction(saveAction);
    menu->addAction(aboutAction);
    menu->addAction(exitAction);

    QWidget *widget = new QWidget(this);
    this->setCentralWidget(widget);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    widget->setLayout(layout);
    this->label = new QLabel(this);
    QPixmap pix(":/images/images/no-image.png");
    QPixmap scaled = pix.scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(scaled);
    layout->addWidget(label, 0, Qt::AlignCenter);

    connect(openAction, &QAction::triggered, this, &MainWindow::onExplore);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onExplore() {
    this->filePath = QFileDialog::getOpenFileName(
        this,
        "Escolha uma imagem",
        QDir::homePath(),
        "Images (*.jpg *.jpeg)"
        );

    qDebug() << this->filePath;
}
