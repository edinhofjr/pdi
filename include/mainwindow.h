#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <image.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QLabel *label;
    Image *image;

    void setImage(Image*);
    void setImage(const char*);
    void display();
    void replaceImage(Image*);

private slots:
    void onExplore();
    void onTranslate();
    void onRotate();
    void onMirrorHorizontal();
    void onMirrorVertical();
    void onBrightness();
    void onContrast();
    void onGrayscale();
    void onThreshold();
    void onDilation();
};

#endif // MAINWINDOW_H
