#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFile>

#include "scene.h"

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    Scene *scene;
    QPushButton *button, *button_2, *button_3; // кнопки
    QLineEdit *lineEdits, *lineEdits_2, *lineEdits_3; // поля ввода
    QTextEdit *text_1, *text_2; // поля с файлами
    QSlider *slider, *slider_2; // слайдеры
    void SetText(char *str, int i); // установка текста в QTextEdit

public:
    MainWidget(QWidget *parent = nullptr);

public slots:
   void on_ok_clicked();
   void slider_changed(int value);
   void save_image();
   void control();
   void slider_changed_2(int value);
};

#endif // MAINWIDGET_H
