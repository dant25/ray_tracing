#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "scene.h"
#include "Camera.h"

#include <QGraphicsScene>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void criaCena();

    void renderiza();

    Vec3 reflete(Vec3 raio, Vec3 norm);

    void normalizePixel(int i, int j);

    void keyPressEvent(QKeyEvent *keyEvent);

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage img;
    Scene scene;
    Camera *camera;
    QGraphicsScene* graphics;

    Color pixels[800][800];
};

#endif // MAINWINDOW_H
