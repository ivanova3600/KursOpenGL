#include "scene.h"
#include <fstream>

float **g_Points; // контрольные точки
float *g_Knots; // веса

// Вычисление значения В-сплайна
float Scene::DeBoor(float x,int i,int k,const float* Knots)
{
    if(k==1){ // для B(i,1)
        if(Knots[i] <= x && x <= Knots[i+1])
            return 1.0f;
        return 0.0f;
    }
    // Если k > 1, то вычисляем значение B(i,k)
    float Den1 = Knots[i+k-1] - Knots[i];
    float Den2 = Knots[i+k] - Knots[i+1];
    float Eq1 = 0, Eq2 = 0;
    if(Den1>0) Eq1 = ((x-Knots[i]) / Den1) * DeBoor(x, i, k-1, Knots);
    if(Den2>0) Eq2 = (Knots[i+k]-x) / Den2 * DeBoor(x, i+1, k-1, Knots);

    return Eq1 + Eq2; // возвращаем их сумму
}

// Получение точки кривой
void Scene::GetPoint(float t,float OutPoint[]) {
    for(int i = 0;i < _g_num_cvs; ++i) { // проходим по всем контрольным точкам
        float Val = DeBoor(t,i,_g_order,g_Knots);
        if(Val > 0.001f){ // умножаем получившееся значение на контрольную точку
            OutPoint[0] += Val * g_Points[i][0];
            OutPoint[1] += Val * g_Points[i][1];
            OutPoint[2] += Val * g_Points[i][2];
        }
    }
}

// чтение контрольных точек и весов из файлов
void Scene::ReadFile(char *str, char *str_, int k)
{
    if(k) {
        for(int i = 0; i < _g_num_cvs; i++){
            delete [] g_Points[i];
        }
        delete [] g_Points;
        delete [] g_Knots;

    }
    std::ifstream in(str);
    in >> _g_num_cvs;
    _g_num_knots = _g_num_cvs + _g_order;
    g_Points = new float*[_g_num_cvs];
    for(int i = 0; i < _g_num_cvs; i++){
        g_Points[i] = new float[3];
        for(int j = 0; j < 3; j++)
            in >> g_Points[i][j];
    }
    in.close();

    std::ifstream in_(str);
    g_Knots = new float[_g_num_knots];
    for(int i = 0; i < _g_num_knots; i++)
        in_ >> g_Knots[i];
    in_.close();

    if(k) this->paintGL();
}

// конструктор
Scene::Scene(QWidget *prt) : QGLWidget(prt),
    _g_degree(3), _g_order(4), _LOD(20), _approx(50), _x_rotate(0), _y_rotate(0), _z_rotate(0),
    _x_tr(0), _y_tr(0), _z_tr(0), _x_sc(0.3), _y_sc(0.3), _z_sc(0.3),
    _x_refl(1), _y_refl(1), _z_refl(1)
{
    QGLFormat frmt; // создать формат по умолчанию
    frmt.setDoubleBuffer(false); // задать простую буферизацию
    setFormat(frmt);
    // обработка событий клавиатуры
    setFocusPolicy(Qt::StrongFocus);
    // установка таймера
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

// инициализация: считываем контрольные точки и веса из файлов
void Scene::initializeGL()
{
    char *str = (char*)"/home/elena/Рабочий стол/kurs/test1.txt";
    char *str_ = (char*)"/home/elena/Рабочий стол/kurs/test1_.txt";
    this->ReadFile(str, str_, 0);

    glEnable(GL_DEPTH_TEST);
}

void Scene::resizeGL(int w, int h)
{
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/h,0.1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// отрисовка
void Scene::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5,30,56, 0,5,0, 0,1,0);

    float points__[_approx][_LOD][3];
    // поворот
    glRotatef(_x_rotate, 1, 0, 0);
    glRotatef(_y_rotate, 0, 1, 0);
    glRotatef(_z_rotate, 0, 0, 1);
    // движение
    glTranslatef(_x_tr, _y_tr, _z_tr);
    // масштаб/сжатие/растяжение
    glScalef(_x_sc, _y_sc, _z_sc);

    glLineWidth(2);

    for(int i = 0; i < _LOD; i++){
        glColor3f(1,0.5,0);
        glBegin(GL_LINE_LOOP);
        // Вычисление точки NURBS-кривой
        float t  = g_Knots[_g_num_knots-1] * i / (float)(_LOD-1);
        if(i==_LOD-1) t -= 0.001f;
        float Outpoint[3] = {0, 0, 0};
        GetPoint(t,Outpoint);
        // Отражение
        Outpoint[0] *= _x_refl;
        Outpoint[1] *= _y_refl;
        Outpoint[2] *= _z_refl;

        glVertex3fv(Outpoint);

        for(int j = 0; j < 3; j++)  points__[0][i][j] = Outpoint[j];

        float inc = 2*M_PI/_approx;
        // построение поверхности
        for(int j = 1; j < _approx; j++){
            float x = Outpoint[0]*cos(inc*j) + Outpoint[2]*sin(inc*j);
            float z = - Outpoint[0]*sin(inc*j) + Outpoint[2]*cos(inc*j);
            glVertex3f(x,Outpoint[1],z);
            points__[j][i][0] = x; points__[j][i][1] = Outpoint[1]; points__[j][i][2] = z;
        }
        glEnd();
    }

    glColor3f(1,1,1);
    // построение поверхности
    for(int j = 0; j < _approx; j++) {
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i < _LOD; i++) glVertex3fv(points__[j][i]);
        glEnd();
    }

    QGLWidget::swapBuffers();
}

// обработка событий клавиатуры
void Scene::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {
        case Qt::Key_Q:
            _x_rotate++; break;
        case Qt::Key_W:
            _x_rotate--; break;
        case Qt::Key_A:
            _y_rotate++; break;
        case Qt::Key_S:
            _y_rotate--; break;
        case Qt::Key_Z:
            _z_rotate++; break;
        case Qt::Key_X:
            _z_rotate--; break;
        case Qt::Key_E:
            _x_tr++; break;
        case Qt::Key_R:
            _x_tr--; break;
        case Qt::Key_D:
            _y_tr++; break;
        case Qt::Key_F:
            _y_tr--; break;
        case Qt::Key_C:
            _z_tr++; break;
        case Qt::Key_V:
            _z_tr--; break;
        case Qt::Key_T:
            _x_sc += 0.01; break;
        case Qt::Key_Y:
            if(_x_sc-0.01 > 0) _x_sc -= 0.01; break;
        case Qt::Key_G:
            _y_sc += 0.01; break;
        case Qt::Key_H:
            if(_y_sc-0.01 > 0) _y_sc -= 0.01; break;
        case Qt::Key_B:
            _z_sc += 0.01; break;
        case Qt::Key_N:
            if(_z_sc-0.01 > 0) _z_sc -= 0.01; break;
        case Qt::Key_O:
            if(_x_refl==1) _x_refl = -1;
            else _x_refl = 1;
            break;
        case Qt::Key_K:
            if(_y_refl==1) _y_refl = -1;
            else _y_refl = 1;
            break;
        case Qt::Key_M:
            if(_z_refl==1) _z_refl = -1;
            else _z_refl = 1;
            break;
        default: break;
    }
    update();
}

// Установка степени аппроксимации
void Scene::SetAccuracy(int value)
{
    _approx = value;
    this->paintGL();
}

// Установка масштаба
void Scene::SetScale(float scl)
{
    _x_sc = _y_sc = _z_sc = scl;
    this->paintGL();
}

Scene::~Scene()
{
    for(int i = 0; i < _g_num_cvs; i++)
        delete [] g_Points[i];
    delete [] g_Points;
    delete [] g_Knots;
}
