#include <QMessageBox>
#include <iostream>
#include <omp.h>
#include <sys/time.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tamX = 800;
    tamY = 800;

    pixels.resize(tamX);
    for (int i = 0; i < tamX; i++)
        pixels[i].resize(tamY);


    image = QImage(tamX,tamY, QImage::Format_RGB32);
    img = QImage(tamX,tamY, QImage::Format_RGB32);


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
/*
    Material material( Color(0.2, 0.0, 0.0), Color(1.0, 0, 0), Color(1.0, 1.0, 1.0), 10 );      ///VERMELHA
    scene.addSphere(Point{-14, 0, 0}, 10, material);

    Material material2( Color(0.0, 0.0, 0.2), Color(0.0, 0, 1.0), Color(1.0, 1.0, 1.0), 10 );      ///AZUL
    scene.addSphere(Point{14, 0, 0}, 10, material2);

    Material material3( Color(0.0, 0.2, 0.0), Color(0.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 10 );      ///VERDE
    scene.addSphere(Point{0, -14, 0}, 10, material3);

    Material material4( Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 10 );      ///BRANCA
    scene.addSphere(Point{0, 14, 0}, 10, material4);
*/

    Material material5( Color(0.2, 0.2, 0.0), Color(1.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 10 );      ///AMARELO


    scene.addTorus( "../resources/torus.obj", material5);

/*
    ///=======
    /// CHAO
    scene.addTriangle( Point{-4, -3, -3}, Point{4, -3, -3},Point{4, -3, -7}, material );
    scene.addTriangle( Point{-4, -3, -3}, Point{4, -3, -7},Point{-4, -3, -7}, material );
    /// TRAS
    scene.addTriangle( Point{-4, -3, -7}, Point{4, -3, -7},Point{4, 5, -7}, material2 );
    scene.addTriangle( Point{-4, -3, -7}, Point{4, 5, -7},Point{-4, 5, -7}, material2 );
    /// ESQ
    scene.addTriangle( Point{-4, -3, -3}, Point{-4, -3, -7},Point{-4, 5, -7}, material3 );
    scene.addTriangle( Point{-4, -3, -3}, Point{-4, 5, -7},Point{-4, 5, -3}, material3 );
    /// DIR
    scene.addTriangle( Point{4, -3, -3}, Point{4, 5, -3},Point{4, 5, -7}, material4 );
    scene.addTriangle( Point{4, -3, -3}, Point{4, 5, -7},Point{4, -3, -7}, material4 );
    /// TETO
    scene.addTriangle( Point{-4, 5, -3}, Point{-4, 5, -7},Point{4, 5, -7}, material5 );
    scene.addTriangle( Point{-4, 5, -3}, Point{4, 5, -7},Point{4, 5, -3}, material5 );
    ///=======
*/

    scene.addLight(Point{0, 4, -5}, 1.0, 1.0, 1.0, 0.0001);
    scene.addLight(Point{0, 50, 70}, 1.0, 1.0, 1.0, 0.0001);


    camera = new Camera(tamX, tamY);
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
    QImage img = QImage(tamX,tamY, QImage::Format_RGB32);

    #pragma omp parallel for
    for(int i = 0; i < camera->imgHeight; i++)
    {
        for(int j = 0; j < camera->imgWidth; j++)
        {
            Ray ray = camera->createRay(i, j);

            Intersection intersect;
            if(scene.Intersect(ray, intersect))
            {
                /*float distanceToLight = (scene.lights[0].position - intersect.point).length();
                float attenuation = 1.0 / (1.0 + scene.lights[0].attenuation * pow(distanceToLight, 2));


                ///COEFICIENTE AMBIENTE
                pixels[i][j].r = scene.lights[0].color.r * intersect.material.ambient.r;
                pixels[i][j].g = scene.lights[0].color.g * intersect.material.ambient.g;
                pixels[i][j].b = scene.lights[0].color.b * intersect.material.ambient.b;

                Vec3 L = scene.lights[0].position - intersect.point;
                L.normalize();

                double fator_dif = Dot(intersect.normal, L);
                if( fator_dif < 0  )
                    fator_dif = 0;

                ///COEFICIENTE DIFUSO
                pixels[i][j].r += attenuation * scene.lights[0].color.r * fabs(fator_dif)*intersect.material.diffuse.r;
                pixels[i][j].g += attenuation * scene.lights[0].color.g * fabs(fator_dif)*intersect.material.diffuse.g;
                pixels[i][j].b += attenuation * scene.lights[0].color.b * fabs(fator_dif)*intersect.material.diffuse.b;

                Vec3 reflexao = reflete(L,intersect.normal);
                double fator_esp = Dot( ray.raio(), reflexao );
                if( fator_esp < 0  )
                    fator_esp = 0;

                ///COEFICIENTE ESPECULAR
                pixels[i][j].r += attenuation * scene.lights[0].color.r * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.r;
                pixels[i][j].g += attenuation * scene.lights[0].color.g * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.g;
                pixels[i][j].b += attenuation * scene.lights[0].color.b * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.b;

                normalizePixel(i, j);*/

                Color cor, auxCor;

                for(int luz=0; luz < scene.lights.size(); luz++)
                {
                    auxCor = calcContrib(ray, intersect, luz);
                    cor.r += auxCor.r;
                    cor.g += auxCor.g;
                    cor.b += auxCor.b;
                }

                pixels[i][j].r = cor.r;
                pixels[i][j].g = cor.g;
                pixels[i][j].b = cor.b;

                normalizePixel(i, j);

                ///REFLEXÃO DO RAIO
                Intersection intersect2;
                Ray ray2( intersect.point, reflete( ray.raio(), intersect.normal ) );
                ray2.IDfonte = intersect.objIndex;

                if( scene.Intersect( ray2, intersect2) )
                {
                    /*L = scene.lights[0].position - intersect2.point;
                    L.normalize();
                    fator_dif = Dot(intersect2.normal, L);

                    if( fator_dif < 0  )
                        fator_dif = 0;

                    reflexao = reflete(L, intersect2.normal);
                    fator_esp = Dot( ray.raio(), reflexao );

                    if( fator_esp < 0  )
                        fator_esp = 0;

                    ///COEFICIENTE DIFUSO NA REFLEXÃO
                    float a = attenuation * scene.lights[0].color.r * fabs(fator_dif) * intersect2.material.diffuse.r;
                    float b = attenuation * scene.lights[0].color.g * fabs(fator_dif) * intersect2.material.diffuse.g;
                    float c = attenuation * scene.lights[0].color.b * fabs(fator_dif) * intersect2.material.diffuse.b;

                    ///COEFICIENTE ESPECULAR NA REFLEXÃO
                    a += attenuation * scene.lights[0].color.r * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.r;
                    b += attenuation * scene.lights[0].color.g * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.g;
                    c += attenuation * scene.lights[0].color.b * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.b;


                    pixels[i][j].r = 3.0*(pixels[i][j].r /4.0) + (a/4.0);
                    pixels[i][j].g = 3.0*(pixels[i][j].g /4.0) + (b/4.0);
                    pixels[i][j].b = 3.0*(pixels[i][j].b /4.0) + (c/4.0);*/

                    cor.setColor(0,0,0);
                    for(int luz=0; luz < scene.lights.size(); luz++)
                    {
                        auxCor = calcContrib(ray2, intersect2, luz);
                        cor.r += auxCor.r;
                        cor.g += auxCor.g;
                        cor.b += auxCor.b;
                    }

                    ///USA APENAS 1/4 DA REFLEXÃO NA COR FINAL
                    pixels[i][j].r = 3.0*(pixels[i][j].r /4.0) + (cor.r/4.0);
                    pixels[i][j].g = 3.0*(pixels[i][j].g /4.0) + (cor.g/4.0);
                    pixels[i][j].b = 3.0*(pixels[i][j].b /4.0) + (cor.b/4.0);
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


Color MainWindow::calcContrib( Ray ray, const Intersection intersect, int l )
{
    Color cor;

    float distanceToLight = (scene.lights[l].position - intersect.point).length();
    float attenuation = 1.0 / (1.0 + scene.lights[l].attenuation * pow(distanceToLight, 2));


    ///COEFICIENTE AMBIENTE
    cor.r = scene.lights[l].color.r * intersect.material.ambient.r;
    cor.g = scene.lights[l].color.g * intersect.material.ambient.g;
    cor.b = scene.lights[l].color.b * intersect.material.ambient.b;

    Vec3 L = scene.lights[l].position - intersect.point;
    L.normalize();

    double fator_dif = Dot(intersect.normal, L);
    if( fator_dif < 0  )
        fator_dif = 0;

    ///COEFICIENTE DIFUSO
    cor.r += attenuation * scene.lights[l].color.r * fabs(fator_dif)*intersect.material.diffuse.r;
    cor.g += attenuation * scene.lights[l].color.g * fabs(fator_dif)*intersect.material.diffuse.g;
    cor.b += attenuation * scene.lights[l].color.b * fabs(fator_dif)*intersect.material.diffuse.b;

    Vec3 reflexao = reflete(L,intersect.normal);
    double fator_esp = Dot( ray.raio(), reflexao );
    if( fator_esp < 0  )
        fator_esp = 0;

    ///COEFICIENTE ESPECULAR
    cor.r += attenuation * scene.lights[l].color.r * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.r;
    cor.g += attenuation * scene.lights[l].color.g * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.g;
    cor.b += attenuation * scene.lights[l].color.b * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.b;

    if(cor.r > 1.0)
        cor.r = 1.0;
    if(cor.g > 1.0)
        cor.g = 1.0;
    if(cor.b > 1.0)
        cor.b = 1.0;

    return cor;
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
            camera->Rotation_i(-ang);
            break;
        }
        case 50:        ///NUMPAD 2
        {
            camera->Rotation_i(ang);
            break;
        }
        case 54:        ///NUMPAD 6
        {
            camera->Rotation_j(ang);
            break;
        }
        case 52:        ///NUMPAD 4
        {
            camera->Rotation_j(-ang);
            break;
        }

    }

    renderiza();

    graphics->addPixmap(QPixmap::fromImage(image));

    //ui->graphicsView->update();
}
