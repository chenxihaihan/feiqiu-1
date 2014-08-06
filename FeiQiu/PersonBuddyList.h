#ifndef PERSONBUDDYLIST_H
#define PERSONBUDDYLIST_H

#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLineEdit>
#include"PersonLoginInfo.h"

class PersonBuddyList : public QListWidget
{
    Q_OBJECT
public:
    explicit PersonBuddyList(QListWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);//������¼�
    void contextMenuEvent(QContextMenuEvent*);//�˵��¼���Ϊ����ʾ�˵�
    void initMenu();//��ʼ���˵�
    QMenu *blankMenu;//����հ��ϵĲ˵�
    QMenu *groupMenu;//������ϵĲ˵�
    QMenu *personMenu;//������ϵĲ˵�
    QMap<QListWidgetItem*,QListWidgetItem*> groupMap;   // ������ - key:�� value:��
    QMap<QListWidgetItem*,bool> isHideMap;//�����жϸ����Ƿ�������
    QLineEdit *groupNameEdit;//������֣���������ʱ����Ҫ�õ�
    QListWidgetItem *currentItem;//��ǰ����
public:
    void AddBuddy(PersonLoginInfo info);   //��Ӻ���
    void AddGroup(bool);   //�����
public slots:
    void slotgroupmessage();
    void slotpersonmessage();
    void mouseDoubleClickEvent(QMouseEvent *event);

    void slotDelGroup();   //ɾ����

    void slotDelBuddy();   //ɾ������
    void slotRename();     //��������
    void slotRenameEditFshed();//�������

};




#endif // PERSONBUDDYLIST_H
