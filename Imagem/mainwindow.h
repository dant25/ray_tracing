#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include <vector>

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

    Color calcContrib(Ray ray, const Intersection intersect );

    glm::mat4 translate(float x, float y, float z) {
        return glm::translate(glm::mat4(), glm::vec3(x,y,z));
    }

    glm::mat4 scale(float x, float y, float z) {
        return glm::scale(glm::mat4(), glm::vec3(x,y,z));
    }

    glm::mat4 rotate(int x, int y, int z, float ang) {
        return glm::rotate(glm::mat4(), (ang), glm::vec3(x,y,z));
    }

private slots:
    void on_actionSalvar_triggered();

private:
    Ui::MainWindow *ui;
    QImage image;
    Scene scene;
    Camera *camera;
    QGraphicsScene* graphics;

    int tamX;
    int tamY;

    std::vector< std::vector<Color> > pixels;
};

#endif // MAINWINDOW_H
