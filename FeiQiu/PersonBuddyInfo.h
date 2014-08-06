#ifndef PERSONBUDDYINFO_H
#define PERSONBUDDYINFO_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include "PersonLoginInfo.h"

class PersonBuddyInfo : public QWidget
{
    Q_OBJECT
public:
    explicit PersonBuddyInfo(QWidget *parent = 0);
    void initUi();//��ʼ��Ui
    QWidget *head;  //ͷ��
    QLabel *name;  //�û���
    QLabel *sign;  //����ǩ��
    QString headPath;//ͷ��·��

    PersonLoginInfo logInfo;

    bool eventFilter(QObject *obj, QEvent *event);//�¼�������

};





#endif // PERSONBUDDYINFO_H
