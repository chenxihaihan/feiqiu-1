#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QApplication>

#include <QtGui>

#include <QTextCodec>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    
private:
    Ui::MyWidget *ui;

protected:

    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void savePixmap();
private:

    int x1,y1;

    QPixmap pixmap;

    QPoint lastPoint;	//��ͼ�������

    QPoint endPoint;	//��ͼ�յ�����

    bool isDrawing;
};

#endif // MYWIDGET_H
