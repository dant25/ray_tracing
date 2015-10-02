#include <QMessageBox>
#include <iostream>
#include <omp.h>
#include <sys/time.h>

#include "math/Quaternions.h"

#include "Sphere.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image = QImage(800,800, QImage::Format_RGB32);
    img = QImage(800,800, QImage::Format_RGB32);


    criaCena();


    double ti,tf,tempo;
    ti = tf = tempo = 0;
    timeval tempo_inicio,tempo_fim;
    gettimeofday(&tempo_inicio,NULL);

    renderiza();

    gettimeofday(&tempo_fim,NULL);
    tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
    ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
    tempo = (tf - ti) / 1000;
    std::cout << "Tempo gasto em milissegundos para desenhar: " << tempo << std::endl;


    graphics = new QGraphicsScene(this);

    graphics->addPixmap(QPixmap::fromImage(image));

    ui->graphicsView->setScene(graphics);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::criaCena()
{
    Material material( Color(0.2, 0.0, 0.0), Color(1.0, 0, 0), Color(1.0, 1.0, 1.0), 10 );      ///VERMELHA
    scene.addSphere(Point{-14, 0, 0}, 10, material);

    Material material2( Color(0.0, 0.0, 0.2), Color(0.0, 0, 1.0), Color(1.0, 1.0, 1.0), 10 );      ///AZUL
    scene.addSphere(Point{14, 0, 0}, 10, material2);

    Material material3( Color(0.0, 0.2, 0.0), Color(0.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 10 );      ///VERDE
    scene.addSphere(Point{0, -14, 0}, 10, material3);

    Material material4( Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 10 );      ///BRANCA
    scene.addSphere(Point{0, 14, 0}, 10, material4);

    scene.addLight(Point{0, 20, 40}, 1.0, 1.0, 1.0, 0.0001);

    camera = new Camera(800, 800);
    camera->setPos( Point{0, 0, 30} );
    camera->lookAt( Point{0, 0, 0} );
}

Vec3 MainWindow::reflete(Vec3 raio, Vec3 norm)
{
    return ( norm * (2 * Dot(raio,norm) ) ) - raio ;
}

void MainWindow::normalizePixel(int i, int j)
{
    if( pixels[i][j].r > 1.0 )
        pixels[i][j].r = 1.0;

    if( pixels[i][j].g > 1.0 )
        pixels[i][j].g = 1.0;

    if( pixels[i][j].b > 1.0 )
        pixels[i][j].b = 1.0;

}

void MainWindow::renderiza()
{
    QImage img = QImage(800,800, QImage::Format_RGB32);

    #pragma omp parallel for
    for(int i = 0; i < camera->imgHeight; i++)
    {
        for(int j = 0; j < camera->imgWidth; j++)
        {
            Ray ray = camera->createRay(i, j);

            Intersection intersect;
            if(scene.Intersect(ray, intersect))
            {
                float distanceToLight = (scene.lights[0].position - intersect.point).length();
                float attenuation = 1.0 / (1.0 + scene.lights[0].attenuation * pow(distanceToLight, 2));


                ///COEFICIENTE AMBIENTE
                pixels[i][j].r = scene.lights[0].color.r * intersect.material.ambient.r;
                pixels[i][j].g = scene.lights[0].color.g * intersect.material.ambient.g;
                pixels[i][j].b = scene.lights[0].color.b * intersect.material.ambient.b;

                Vec3 L = scene.lights[0].position - intersect.point;
                L.normalize();

                double fator_dif = Dot(intersect.normal, L);
                if( fator_dif < 0  )
                    fator_dif=0;

                ///COEFICIENTE DIFUSO
                pixels[i][j].r += attenuation * scene.lights[0].color.r * fabs(fator_dif)*intersect.material.diffuse.r;
                pixels[i][j].g += attenuation * scene.lights[0].color.g * fabs(fator_dif)*intersect.material.diffuse.g;
                pixels[i][j].b += attenuation * scene.lights[0].color.b * fabs(fator_dif)*intersect.material.diffuse.b;

                Vec3 reflexao = reflete(L,intersect.normal);
                double fator_esp = Dot( ray.raio(), reflexao );
                if( fator_esp < 0  )
                    fator_esp=0;

                ///COEFICIENTE ESPECULAR
                pixels[i][j].r += attenuation * scene.lights[0].color.r * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.r;
                pixels[i][j].g += attenuation * scene.lights[0].color.g * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.g;
                pixels[i][j].b += attenuation * scene.lights[0].color.b * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.b;


                ///REFLEXÃO DO RAIO
                Intersection intersect2;
                Ray ray2( intersect.point, reflete( ray.raio(), intersect.normal ) );
                ray2.IDfonte = intersect.objIndex;

                if( scene.Intersect( ray2, intersect2) )
                {
                    L = scene.lights[0].position - intersect2.point;
                    L.normalize();
                    fator_dif = Dot(intersect2.normal, L);

                    if( fator_dif < 0  )
                        fator_dif = 0;

                    ///COEFICIENTE DIFUSO NA REFLEXÃO
                    pixels[i][j].r += attenuation * scene.lights[0].color.r * fabs(fator_dif) * intersect2.material.diffuse.r;
                    pixels[i][j].g += attenuation * scene.lights[0].color.g * fabs(fator_dif) * intersect2.material.diffuse.g;
                    pixels[i][j].b += attenuation * scene.lights[0].color.b * fabs(fator_dif) * intersect2.material.diffuse.b;
                }


                normalizePixel(i, j);
                img.setPixel(i,j, qRgb(pixels[i][j].r * 255, pixels[i][j].g * 255,  pixels[i][j].b * 255));
            }
            else
            {
                img.setPixel(i,j, qRgb(40,40,40));
            }
        }
    }

    image.swap(img);
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    const float moveSpeed = 1.0; //units per second

    const float ang = 10.0;

    switch(keyEvent->key())
    {
        case Qt::Key_S:
        {
            camera->setPos( camera->getPos() + camera->kc*moveSpeed );
            break;
        }
        case Qt::Key_W:
        {
            camera->setPos( camera->getPos() - camera->kc*moveSpeed );
            break;
        }
        case Qt::Key_A:
        {
            camera->setPos( camera->getPos() - camera->ic*moveSpeed );
            break;
        }
        case Qt::Key_D:
        {
            camera->setPos( camera->getPos() + camera->ic*moveSpeed );
            break;
        }
        case Qt::Key_Z:
        {
            camera->setPos( camera->getPos() - camera->jc*moveSpeed );
            break;
        }
        case Qt::Key_X:
        {
            camera->setPos( camera->getPos() + camera->jc*moveSpeed );
            break;
        }


        case Qt::Key_Escape:
        {
            QApplication::quit();
            break;
        }


        case 56:        ///NUMPAD 8
        {
            Point3D newVIEW, newUP;

            newUP = rotateQuat( Point3D(camera->jc.x, camera->jc.y, camera->jc.z) , Point3D(camera->ic.x, camera->ic.y, camera->ic.z), ang);
            newUP = Point3D::normalize(newUP);
            newVIEW = rotateQuat( Point3D(camera->kc.x, camera->kc.y, camera->kc.z) , Point3D(camera->ic.x, camera->ic.y, camera->ic.z), ang);
            newVIEW = Point3D::normalize(newVIEW);

            camera->jc = Vec3(newUP.x, newUP.y, newUP.z);
            camera->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);

            break;
        }
        case 50:        ///NUMPAD 2
        {
            Point3D newVIEW, newUP;

            newUP = rotateQuat( Point3D(camera->jc.x, camera->jc.y, camera->jc.z) , Point3D(camera->ic.x, camera->ic.y, camera->ic.z), -ang);
            newUP = Point3D::normalize(newUP);
            newVIEW = rotateQuat( Point3D(camera->kc.x, camera->kc.y, camera->kc.z) , Point3D(camera->ic.x, camera->ic.y, camera->ic.z), -ang);
            newVIEW = Point3D::normalize(newVIEW);

            camera->jc = Vec3(newUP.x, newUP.y, newUP.z);
            camera->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);

            break;
        }
        case 54:        ///NUMPAD 6
        {
            Point3D newVIEW, newRIGHT;

            newVIEW = rotateQuat( Point3D(camera->kc.x, camera->kc.y, camera->kc.z) , Point3D(camera->jc.x, camera->jc.y, camera->jc.z), ang);
            newVIEW = Point3D::normalize(newVIEW);
            newRIGHT = rotateQuat( Point3D(camera->ic.x, camera->ic.y, camera->ic.z) , Point3D(camera->jc.x, camera->jc.y, camera->jc.z), ang);
            newRIGHT = Point3D::normalize(newRIGHT);

            camera->ic = Vec3(newRIGHT.x, newRIGHT.y, newRIGHT.z);
            camera->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);

            break;
        }
        case 52:        ///NUMPAD 4
        {
            Point3D newVIEW, newRIGHT;

            newVIEW = rotateQuat( Point3D(camera->kc.x, camera->kc.y, camera->kc.z) , Point3D(camera->jc.x, camera->jc.y, camera->jc.z), -ang);
            newVIEW = Point3D::normalize(newVIEW);
            newRIGHT = rotateQuat( Point3D(camera->ic.x, camera->ic.y, camera->ic.z) , Point3D(camera->jc.x, camera->jc.y, camera->jc.z), -ang);
            newRIGHT = Point3D::normalize(newRIGHT);

            camera->ic = Vec3(newRIGHT.x, newRIGHT.y, newRIGHT.z);
            camera->kc = Vec3(newVIEW.x, newVIEW.y, newVIEW.z);

            break;
        }

    }

    renderiza();

    graphics->addPixmap(QPixmap::fromImage(image));

    //ui->graphicsView->update();
}
