#ifndef UPLOAD_H
#define UPLOAD_H


#include <QPushButton>
#include <QFileDialog>
#include <QMainWindow>

class upload : public QPushButton {
    Q_OBJECT

public:
    upload(QWidget *parent) :  QPushButton(parent) {
         setIconSize(QSize(300,200));
         connect(this, SIGNAL(clicked()), this, SLOT (getpath() )); // if QPushButton clicked...then run clicked() below
    }

    void init();

private slots:
    void getpath();

};


#endif // UPLOAD_H
