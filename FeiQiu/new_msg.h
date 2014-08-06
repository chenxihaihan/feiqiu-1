#ifndef NEW_MSG_H
#define NEW_MSG_H
#include "title_tips.h"
#include <QWidget>

class new_msg : public QWidget
{
    Q_OBJECT
public:
    explicit new_msg(QWidget *parent = 0);
    ~new_msg();
    void showAsQQ();//�����½���ʾ
        int timeCount;
        double tran;
public slots:
        void setMsg(QString title,QString content);
private slots:
        void myMove();
        void myStay();
        void myClose();

private:
        title_tips *titleW;

        QLabel *content;

        QPoint normalPoint;//��ȫ��ʾʱλ��
        QRect deskRect;

        QTimer *timerShow;//ƽ����ʾ�Ķ�ʱ��
        QTimer *timerStay;//ͣ��ʱ��Ķ�ʱ�� 10s
        QTimer *timerClose;//�رյ����Ķ�ʱ��

protected:
        void paintEvent(QPaintEvent *);
    
};

#endif // NEW_MSG_H
