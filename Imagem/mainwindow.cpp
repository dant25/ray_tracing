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

    Material material( Color(0.2, 0.0, 0.0), Color(1.0, 0, 0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///VERMELHA
    scene.addSphere(Point{-14, 0, 0}, 10, material);

    Material material2( Color(0.0, 0.0, 0.2), Color(0.0, 0, 1.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///AZUL
    scene.addSphere(Point{14, 0, 0}, 10, material2);

    Material material3( Color(0.0, 0.2, 0.0), Color(0.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///VERDE
    scene.addSphere(Point{0, -14, 0}, 10, material3);

    Material material4( Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///BRANCA
    scene.addSphere(Point{0, 14, 0}, 10, material4);


    Material material5( Color(0.2, 0.2, 0.0), Color(1.0, 1.0, 0.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///AMARELO

    glm::mat4 transform = translate(0,0,5) * scale(1.5, 1.5, 1.5) * rotate(1,1,1, 45);

    //scene.addObject( "../resources/cone.obj", material5, transform);
    //scene.addObject( "../resources/torus.obj", material5, transform);
    scene.addObject( "../resources/cubo.obj", material5, transform);


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

    //scene.addLight(Point{0, 4, -5}, 1.0, 1.0, 1.0, 0.0001);
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
                Color cor, auxCor;
                cor.setColor(0,0,0);

                auxCor = calcContrib(ray, intersect);
                cor.r += auxCor.r;
                cor.g += auxCor.g;
                cor.b += auxCor.b;


                pixels[i][j].r = cor.r;
                pixels[i][j].g = cor.g;
                pixels[i][j].b = cor.b;

                ///REFLEXÃO DO RAIO
                Intersection intersect2;
                Ray ray2( intersect.point, reflete( ray.raio(), intersect.normal ) );
                ray2.IDfonte = intersect.objIndex;

                if( scene.Intersect( ray2, intersect2) )
                {
                    cor.setColor(0,0,0);

                    auxCor = calcContrib(ray2, intersect2);
                    cor.r += auxCor.r * intersect.material.getSpecular().r * intersect.material.Ks;
                    cor.g += auxCor.g * intersect.material.getSpecular().g * intersect.material.Ks;
                    cor.b += auxCor.b * intersect.material.getSpecular().b * intersect.material.Ks;


                    pixels[i][j].r += cor.r;
                    pixels[i][j].g += cor.g;
                    pixels[i][j].b += cor.b;
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


Color MainWindow::calcContrib( Ray ray, const Intersection intersect )
{
    Color cor;

    ///COEFICIENTE AMBIENTE
    cor.r = intersect.material.ambient.r * intersect.material.Ka;
    cor.g = intersect.material.ambient.g * intersect.material.Ka;
    cor.b = intersect.material.ambient.b * intersect.material.Ka;

    for(int l=0; l < scene.lights.size(); l++)
    {
        ///CALCULO DE SOMBRAS
        Vec3 raio_luz = scene.lights[l].position - intersect.point;
        float dist_luz = raio_luz.length();
        raio_luz.normalize();

        Intersection luz_intersect;
        Ray r(intersect.point + (intersect.normal*0.1), raio_luz );

        if( scene.Intersect( r, luz_intersect) )
        {
            if( luz_intersect.dist < dist_luz )
                continue;
        }
        ///FIM CALCULO DE SOMBRAS

        float distanceToLight = (scene.lights[l].position - intersect.point).length();
        float attenuation = 1.0 / (1.0 + scene.lights[l].attenuation * pow(distanceToLight, 2));

        Vec3 L = scene.lights[l].position - intersect.point;
        L.normalize();

        double fator_dif = Dot(intersect.normal, L);
        if( fator_dif < 0  )
            fator_dif = 0;

        ///COEFICIENTE DIFUSO
        cor.r += attenuation * scene.lights[l].color.r * fabs(fator_dif)*intersect.material.diffuse.r*intersect.material.Kd;
        cor.g += attenuation * scene.lights[l].color.g * fabs(fator_dif)*intersect.material.diffuse.g*intersect.material.Kd;
        cor.b += attenuation * scene.lights[l].color.b * fabs(fator_dif)*intersect.material.diffuse.b*intersect.material.Kd;

        Vec3 reflexao = reflete(L,intersect.normal);
        double fator_esp = Dot( ray.raio(), reflexao );
        if( fator_esp < 0  )
            fator_esp = 0;

        ///COEFICIENTE ESPECULAR
        cor.r += attenuation * scene.lights[l].color.r * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.r*intersect.material.Ks;
        cor.g += attenuation * scene.lights[l].color.g * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.g*intersect.material.Ks;
        cor.b += attenuation * scene.lights[l].color.b * fabs(pow(fator_esp, intersect.material.k))*intersect.material.specular.b*intersect.material.Ks;
    }

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
        case 49:        ///NUMPAD 1
        {
            camera->Rotation_k(ang);
            break;
        }
        case 51:        ///NUMPAD 3
        {
            camera->Rotation_k(-ang);
            break;
        }

    }

    renderiza();

    graphics->addPixmap(QPixmap::fromImage(image));

    //rstd::cout << "Cam pos: " << camera->pos.x << "\t" << camera->pos.y << "\t" << camera->pos.z << std::endl;
    //std::cout << "Cam look at: " << camera->getLookAt().x << "\t" << camera->getLookAt().y << "\t" << camera->getLookAt().z << std::endl;
}
