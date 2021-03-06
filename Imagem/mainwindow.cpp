#include <QMessageBox>
#include <iostream>
#include <omp.h>
#include <sys/time.h>

#include <iomanip>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tamX = 2000;
    tamY = 2000;

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
    tempo = (tf - ti) / 1000000;
    std::cout << "Tempo gasto em segundos para desenhar: " << tempo << std::endl;

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
    Material material_vermelho( Color(0.2, 0.0, 0.0), Color(1.0, 0, 0), Color(0.2, 0.2, 0.2), 200, 1.0, 1.0, 0.5 );      ///VERMELHA

    Material material_azul( Color(0.0, 0.0, 0.2), Color(0.0, 0, 1.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 1.0 );      ///AZUL

    Material material_verde( Color(0.0, 0.2, 0.0), Color(0.0, 0.9, 0.0), Color(1.0, 1.0, 1.0), 50, 1.0, 1.0, 0.1 );      ///VERDE

    Material material_branco( Color(0.2, 0.2, 0.2), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 200, 1.0, 1.0, 0.05 );      ///BRANCA

    Material material_troper( Color(0.4, 0.4, 0.4), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 10, 1.0, 1.0, 0.5 );      ///BRANCA

    Material material_amarelo( Color(0.2, 0.2, 0.0), Color(1.0, 1.0, 0.0), Color(0.2, 0.2, 0.2), 200, 1.0, 1.0, 0.6 );      ///AMARELO

    Material material_preto( Color(0.01, 0.01, 0.01), Color(0.05, 0.05, 0.05), Color(0.9, 0.9, 0.9), 10, 1.0, 1.0, 0.5 );      ///PRETO

    Material material_escuro( Color(0.01, 0.01, 0.01), Color(0.1, 0.1, 0.1), Color(1.0, 1.0, 1.0), 4, 1.0, 1.0, 0.8 );      ///ESCURO

    Material material_gold( Color(0.09, 0.07, 0.02), Color(0.8549, 0.6470, 0.1254), Color(1.0, 1.0, 1.0), 4, 1.0, 1.0, 1.0 );      ///OURO

    Material material_prata( Color(0.07, 0.07, 0.07), Color(0.6470, 0.6470, 0.6470), Color(1.0, 1.0, 1.0), 4, 1.0, 1.0, 1.0 );      ///PRATA

    Material material_lamina( Color(0.7, 0.0, 0.0), Color(1.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 1, 1.0, 1.0, 1.0 );      ///VERMELHO SABRE

    //=====================================================================================================

    glm::mat4 transform_chao = translate(0,0,0) * scale(100, 100, 100);
    glm::mat4 transform_fundo = translate(0,100,-100) * scale(101, 101, 101) * rotate(1,0,0, 90);
    glm::mat4 transform_Ldireita = translate(-100,100,0) * scale(101, 101, 101) * rotate(0,0,1, -90);

    glm::mat4 transform_cubo = translate(-5,6,0) * scale(4, 6, 4);

    glm::mat4 transform_yoda = translate(15,0,0) * scale(18, 18, 18) * rotate(0,1,0, -45);

    glm::mat4 transform_troper1 = translate(-15,0,2) * scale(4, 4, 4) * rotate(0,1,0, 35);
    glm::mat4 transform_troper2 = translate(15,0,2) * scale(4, 4, 4) * rotate(0,1,0, -35);

    glm::mat4 transform_sala = translate(-8,-9,-16) * scale(4, 4, 4);

    glm::mat4 transform_r2d2 = translate(0,0,0) * scale(12, 12, 12);

    glm::mat4 transform_sabre = translate(-4, -15, 10) * scale(9, 6, 9) * rotate(0,0,1, -5) * rotate(1,0,0, 90);

    glm::mat4 transform_mascara = translate(2, -3.2, 8.5) * scale(10, 10, 10);// * rotate(0,1,0, 35);

    //=====================================================================================================

    //scene.addObject( "../resources/plane.obj", material_branco, transform_chao);            ///CHÃO
    //scene.addObject( "../resources/plane.obj", material_branco, transform_fundo);           ///FUNDO
    //scene.addObject( "../resources/plane.obj", material_branco, transform_Ldireita);        ///LATERAL DIREITA

    //scene.addObject( "../resources/yoda2.obj", material_verde, transform_yoda);

    //scene.addObject( "../resources/storm_troper.obj", material_troper, transform_troper1);
    //scene.addObject( "../resources/storm_troper.obj", material_troper, transform_troper2);

    //scene.addObject( "../resources/cubo.obj", material_vermelho, transform_cubo);
    scene.addObject( "../resources/mascara2.obj", material_preto, transform_mascara);

    //scene.addObject( "../resources/r2d2.obj", material_gold, transform_r2d2);
    scene.addObject( "../resources/sala.obj", material_escuro, transform_sala);
    scene.addObject( "../resources/plane.obj", material_escuro, transform_fundo);           ///FUNDO

    scene.addObject( "../resources/lamina.obj", material_lamina, transform_sabre);
    scene.addObject( "../resources/suporte.obj", material_prata, transform_sabre);

    //=====================================================================================================

    //scene.addLight(Point{20, 40, 0}, 0.0, 0.6, 0.0, 0.001);     ///LUZ VERDE
    scene.addLight(Point{-15, 60, -15}, 1.0, 0.0, 0.0, 0.001);     ///LUZ VERMELHA
    scene.addLight(Point{15, 60, -15}, 1.0, 0.0, 0.0, 0.001);     ///LUZ VERMELHA
    scene.addLight(Point{15, 30, 25}, 1.0, 1.0, 1.0, 0.0001);     ///LUZ BRANCA NORMAL

    //=====================================================================================================

    camera = new Camera(tamX, tamY);
    camera->setPos( Point{0, 0, 20} );
    camera->lookAt( Point{0, 0, 0} );
}

Vec3 MainWindow::reflete(Vec3 raio, Vec3 norm)
{
    raio.x *= -1.0;
    raio.y *= -1.0;
    raio.z *= -1.0;
    Vec3 reflect = raio - ( norm * (2.0 * Dot(raio,norm) ) );
    reflect.normalize();
    return reflect;
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
            ray.IDfonte = -1;

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
                //Ray ray2( intersect.point, reflete( ray.raio(), intersect.normal ) );
                Ray ray2( intersect.point, reflete( ray.raio(), intersect.vertexes_norm ) );
                ray2.IDfonte = intersect.objIndex;

                if( scene.Intersect( ray2, intersect2))
                {
                    if( intersect.objIndex != intersect2.objIndex )
                    {
                        cor.setColor(0,0,0);

                        auxCor = calcContrib(ray2, intersect2);
                        cor.r += auxCor.r * intersect2.material.getSpecular().r * intersect2.material.Ks * intersect.material.Ks;
                        cor.g += auxCor.g * intersect2.material.getSpecular().g * intersect2.material.Ks * intersect.material.Ks;
                        cor.b += auxCor.b * intersect2.material.getSpecular().b * intersect2.material.Ks * intersect.material.Ks;


                        pixels[i][j].r += cor.r;
                        pixels[i][j].g += cor.g;
                        pixels[i][j].b += cor.b;
                    }
                }


                normalizePixel(i, j);
                img.setPixel(i,j, qRgb(pixels[i][j].r * 255, pixels[i][j].g * 255,  pixels[i][j].b * 255));
            }
            else
            {
                img.setPixel(i,j, qRgb(40,40,90));
            }
        }
    }

    image.swap(img);
    image.save("../quadro.png");
    image.save("quadro_backup.png");
}


Color MainWindow::calcContrib( Ray ray, const Intersection intersect )
{
    Color cor;
    float att_sombra = 1.0;

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
                att_sombra *= 0.7;
        }
        ///FIM CALCULO DE SOMBRAS

        float distanceToLight = (scene.lights[l].position - intersect.point).length();
        float attenuation = 1.0 / (1.0 + scene.lights[l].attenuation * pow(distanceToLight, 2.0));

        Vec3 L = scene.lights[l].position - intersect.point;
        L.normalize();

        //double fator_dif = Dot(intersect.normal, L);
        double fator_dif = Dot(intersect.vertexes_norm, L);
        if( fator_dif <= 0.0001  )
            fator_dif = 0.0;

        ///COEFICIENTE DIFUSO
        cor.r += attenuation * scene.lights[l].color.r * fabs(fator_dif)*intersect.material.diffuse.r*intersect.material.Kd;
        cor.g += attenuation * scene.lights[l].color.g * fabs(fator_dif)*intersect.material.diffuse.g*intersect.material.Kd;
        cor.b += attenuation * scene.lights[l].color.b * fabs(fator_dif)*intersect.material.diffuse.b*intersect.material.Kd;

        //Vec3 reflexao = reflete(L,intersect.normal);
        Vec3 reflexao = reflete(L,intersect.vertexes_norm);
        double fator_esp = Dot( ray.raio(), reflexao );
        if( fator_esp <= 0.0001  )
            fator_esp = 0.0;

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

    cor.r *= att_sombra;
    cor.g *= att_sombra;
    cor.b *= att_sombra;

    return cor;
}


void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    const float moveSpeed = 3.0; //units per second

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
            return;
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

    //std::cout << "Cam pos: " << camera->pos.x << "\t" << camera->pos.y << "\t" << camera->pos.z << std::endl;
    //std::cout << "Cam look at: " << camera->getLookAt().x << "\t" << camera->getLookAt().y << "\t" << camera->getLookAt().z << std::endl;
}

void MainWindow::on_actionSalvar_triggered()
{
    image.save("../Save.png");
}
