#include "PersonBuddyInfo.h"
#include <QPainter>

PersonBuddyInfo::PersonBuddyInfo(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}
void PersonBuddyInfo::initUi()
{
    //��ʼ��
    head=new QWidget(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    //����ͷ���С
    head->setFixedSize(32,32);
    //���ø���ǩ������Ϊ��ɫ
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
    //����
    head->move(5,5);
    name->move(45,8);
    sign->move(45,25);
    //װ���¼�������
    head->installEventFilter(this);
    headPath="img/head";
}
//�¼�����������Ҫ��Ϊ����ͼƬ�ܹ�ȫ�������head����
bool PersonBuddyInfo::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == head)
    {
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(head);
            painter.drawPixmap(head->rect(), QPixmap(headPath));
        }
    }
    return QWidget::eventFilter(obj, event);
}


