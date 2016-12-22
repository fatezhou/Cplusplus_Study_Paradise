#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QAxObject"
#include "QFileDialog"
#include "QMessageBox"

#include <QtGui>
#include <QAxWidget>
#include <qaxselect.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void writeExl();
    void readExl();

    void readExlA();
    void writeExlA();
    void exlMode();
    void formatExcel();
    void cellColor(QAxObject *cell,bool bold,int size,bool italic,int line,QString color);
    void cellBackAndSideColor(QAxObject *cell,QString backColor,QString sideColor);
    void mergeCell(QAxObject *work_sheet,int s_row,int s_col,int e_row,int e_col);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
