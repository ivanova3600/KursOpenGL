#include "mainwidget.h"
#include <fstream>
#include <QMessageBox>

// Установка текста в QTextEdit
void MainWidget::SetText(char *str, int i)
{
    QFile file(str);
    if(file.open(QFile::ReadOnly | QFile::Text)){
        if(i) text_1->setPlainText(file.readAll());
        else text_2->setPlainText(file.readAll());
    }
    file.close();
}

// Конструктор
MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    scene = new Scene;
    button = new QPushButton("Ok");
    button_2 = new QPushButton("Сохранить");
    button_3 = new QPushButton("Управление");
    lineEdits = new QLineEdit;
    lineEdits_2 = new QLineEdit;
    lineEdits_3 = new QLineEdit;
    QLabel *label = new QLabel("Контрольные точки");
    QLabel *label_2 = new QLabel("Веса");
    QLabel *label_3 = new QLabel("Имя файла");
    QLabel *label_4 = new QLabel("Масштаб");
    QLabel *label_5 = new QLabel("Аппроксимация");
    // открываем файлы и помещаем текст в QTextEdit
    lineEdits->setText("/home/elena/Рабочий стол/kurs/test1.txt");
    lineEdits_2->setText("/home/elena/Рабочий стол/kurs/test1_.txt");

    text_1 = new QTextEdit;
    text_1->setReadOnly(true);
    SetText((char*)"/home/elena/Рабочий стол/kurs/test1.txt", 1);

    text_2 = new QTextEdit;
    text_2->setReadOnly(true);
    SetText((char*)"/home/elena/Рабочий стол/kurs/test1_.txt", 0);

    // Настройка слайдеров
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(100);
    slider->setSingleStep(1);
    slider->setValue(50);

    slider_2 = new QSlider(Qt::Horizontal);
    slider_2->setMinimum(0);
    slider_2->setMaximum(100);
    slider_2->setSingleStep(1);
    slider_2->setValue(30);

    QGridLayout *grid = new QGridLayout(this); // слой, на который все будет добавляться
    grid->addWidget(scene, 0, 0, 5, 5);

    grid->addWidget(label, 0, 5, 1, 3);
    grid->addWidget(lineEdits, 1, 5, 1, 3);
    grid->addWidget(text_1, 2, 5, 1, 3);

    grid->addWidget(label_2, 0, 8, 1, 3);
    grid->addWidget(lineEdits_2, 1, 8, 1, 3);
    grid->addWidget(text_2, 2, 8, 1, 3);

    grid->addWidget(button, 3, 5);
    grid->addWidget(label_5, 3, 6);
    grid->addWidget(slider, 3, 7, 1, 2);

    grid->addWidget(label_4, 3, 9);
    grid->addWidget(slider_2, 3, 10, 1, 2);
    grid->addWidget(label_3, 4, 5);
    grid->addWidget(lineEdits_3, 4, 6, 1, 3);
    grid->addWidget(button_2, 4, 9);
    grid->addWidget(button_3, 4, 10);

    // связь кнопок и слайдеров со слотами
    QObject::connect(button, &QPushButton::clicked, this, &MainWidget::on_ok_clicked);
    QObject::connect(button_2, &QPushButton::clicked, this, &MainWidget::save_image);
    QObject::connect(button_3, &QPushButton::clicked, this, &MainWidget::control);
    QObject::connect(slider, &QSlider::valueChanged, this, &MainWidget::slider_changed);
    QObject::connect(slider_2, &QSlider::valueChanged, this, &MainWidget::slider_changed_2);
}

// если нажать на кнопку "Ок"
void MainWidget::on_ok_clicked()
{
    QString str = lineEdits->text();
    QByteArray ba = str.toLocal8Bit();
    char *c_str = ba.data();

    QString str_ = lineEdits_2->text();
    QByteArray ba_ = str_.toLocal8Bit();
    char *c_str_ = ba_.data();

    std::ifstream in(c_str);
    text_1->clear(); text_2->clear();
    SetText(c_str,1); SetText(c_str_, 0);

    scene->ReadFile(c_str, c_str_, 1);
}

// если изменить слайдер "Аппроксимация"
void MainWidget::slider_changed(int value)
{
    scene->SetAccuracy(value);
}

// если изменить слайдер "Масштаб"
void MainWidget::slider_changed_2(int value)
{
    scene->SetScale(static_cast<float>(value/100.f));
}

// Сохранение изображение
void MainWidget::save_image()
{
    QString str = lineEdits_3->text();
    QPixmap::grabWindow(scene->winId()).save(str);
}

// Информация по аффинным преобразованиям
void MainWidget::control()
{
    QMessageBox::information(this, "Управление", "Вращение: по X - Q и W, по Y - A и S, по Z - Z и X\nДвижение: по X - E и R, по Y - D и F, по Z - C и V\nРастяжение/сжатие: по X - T и Y, по Y - G и H, по Z - B и N\nОтражение: по X - O, по Y - K, по Z - M");
}
