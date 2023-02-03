// Курсовая работа
// Иванова Елена М8О-301Б-19
// 14 вариант: поверхность вращения, образующая NURBS-кривая 4 порядка
#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>

#include <QtMath>
#include <QTimer>
#include <GL/glut.h>
#include <QtOpenGL>

class Scene : public QGLWidget
{
    Q_OBJECT
private:
    int _g_num_cvs, _g_degree; // количество котрольных точек, степень кривой
    int _g_order, _g_num_knots; // порядок кривой, количество весов
    int _LOD, _approx; // количество точек кривой для отрисовки, степень аппроксимации
    float _x_rotate, _y_rotate, _z_rotate; // углы поворота
    float _x_tr, _y_tr, _z_tr; // движение
    float _x_sc, _y_sc, _z_sc; // растяжение/сжатие
    float _x_refl, _y_refl, _z_refl; // отражение
    float DeBoor(float u,int i,int k,const float* Knots); // получение B-сплайна
    void GetPoint(float t,float OutPoint[]); // получение значения точки кривой

protected:
    void initializeGL(); // инициализация
    void resizeGL(int w, int h); // изменение размара
    void paintGL(); // отрисовка
    void keyPressEvent(QKeyEvent *e); // обработка событий клавиатуры

public:
    Scene(QWidget *prt = 0);
    void ReadFile(char *str, char *str_, int k); // чтение координат контр.точек и весов их файлов
    void SetAccuracy(int value); // установка степени аппроксимации
    void SetScale(float scl); // установка масштаба
    ~Scene();
};

#endif // SCENE_H
