
#include "PersonBuddyList.h"
#include "PersonBuddyInfo.h"
#include "PersonLoginInfo.h"
#include "chatdialog.h"

PersonBuddyList::PersonBuddyList(QListWidget *parent) :
    QListWidget(parent)
{
    qDebug(__func__);
    setFocusPolicy(Qt::NoFocus);       // ȥ��itemѡ��ʱ�����߱߿�
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//ˮƽ�������ر�
    initMenu();
    AddGroup(true);
}


void PersonBuddyList::initMenu()
{
    qDebug(__func__);
    //��ʼ����
    blankMenu = new QMenu();
    groupMenu = new QMenu();
    personMenu = new QMenu();
    groupNameEdit=new QLineEdit();
    QAction *addGroup = new QAction(tr("��ӷ���"), this);
    QAction *delGroup = new QAction(tr("ɾ������"), this);
    QAction *rename = new QAction(tr("������"), this);
    //QAction *addBuddy = new QAction(tr("��Ӻ���"),this);
    QAction *delBuddy = new QAction(tr("ɾ������"), this);
    //QAction *groupmessage = new QAction("",this);
    //QAction *personmessage = new QAction("",this);
    //���ã�
    groupNameEdit->setParent(this);  //���ø���
    groupNameEdit->hide(); //���ó�ʼʱ����
    groupNameEdit->setPlaceholderText(tr("δ����"));//���ó�ʼʱ������
    //���֣�
    //blankMenu->addAction(addGroup);
    groupMenu->addAction(delGroup);
    groupMenu->addAction(rename);
  //  groupMenu->addAction(addBuddy);
    personMenu->addAction(delBuddy);
    //��Ϣ�ۣ�
    connect(groupNameEdit,SIGNAL(editingFinished()),this,SLOT(slotRenameEditFshed()));
    //connect(addGroup,SIGNAL(triggered()),this,SLOT(slotAddGroup()));
    connect(delGroup,SIGNAL(triggered()),this,SLOT(slotDelGroup()));
    connect(rename,SIGNAL(triggered()),this,SLOT(slotRename()));
   // connect(addBuddy,SIGNAL(triggered()),this,SLOT(slotAddBuddy()));
    connect(delBuddy,SIGNAL(triggered()),this,SLOT(slotDelBuddy()));
   // connect(,SIGNAL(doubleClicked()),this,SLOT(mouseDoubleClickEvent()));
    //connect(,SIGNAL(doubleClicked()),this,SLOT(mouseDoubleClickEvent()));
}
//������¼�
void PersonBuddyList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event); // ��������û���mousePressEvent��item��select����첻��Ӧ,���ø��࣬��QSS��Ч����ΪQSS���ڸ���QListWidget����������Ӵ��ڣ��������ϲ㴰�ڣ��Ǹ����ڸ������ϵģ��������ڸ����ڲ�����Ϣ
    //��ֹһ���������������item�������������item��հ״�ʱ��ָ����item��currentItem����������item
    if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        if(groupNameEdit->text()!=NULL)
            currentItem->setText(groupNameEdit->text());
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//���λ�õ�Item�������Ҽ��������ȡ
    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//����������������ǵ��������
    {
        if(isHideMap.value(currentItem))                                  //�����ǰ�����أ�����ʾ
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//������Ķ�Ӧ�����������ͺ��ѣ�
                if(subItem!=currentItem)                                 //�������Ļ������д���
                {
                    subItem->setHidden(false);                            //����ȫ����ʾ
                }
            isHideMap.insert(currentItem,false);                          //���ø���Ϊ��ʾ״̬
            currentItem->setIcon(QIcon("img/list_show.bmp"));//���µļ�ͷ
        }
        else                                                             //������ǰ����ʾ��������
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//������Ķ�Ӧ�����������ͺ��ѣ�
                if(subItem!=currentItem)                                 //�������Ļ������д���
                {
                    subItem->setHidden(true);                            //����ȫ������
                }
             isHideMap.insert(currentItem,true);                          //���ø���Ϊ����״̬
             currentItem->setIcon(QIcon("img/list_close.bmp"));//���ϵļ�ͷ
        }
    }
    else if(event->buttons()==Qt::RightButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))
    {
       // QMessageBox::Information(NULL,"ZK","zucaidan",NULL);
    }
}
//�˵��¼���Ϊ����ʾ�˵����������Ҽ���Ӧ��������¼�mousePressEvent������contextMenuEvent
void PersonBuddyList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //���û����¼�
    if(currentItem==NULL)                           //�����������ǿհ״�
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
    if(currentItem==groupMap.value(currentItem))    // ��������������
        groupMenu->exec(QCursor::pos());
    else                                            //�����������Ǻ���
        personMenu->exec(QCursor::pos());
}
//�����
void PersonBuddyList::AddGroup(bool flag)
{
    QListWidgetItem *newItem=new QListWidgetItem(QIcon("img/list_close"),tr("�ҵĺ���"));    //����һ��Item
    newItem->setSizeHint(QSize(this->width(),25));//���ÿ�ȡ��߶�
    this->addItem(newItem);         //�ӵ�QListWidget��
    groupMap.insert(newItem,newItem);//�ӵ�����groupMap�key��value��Ϊ��

     isHideMap.insert(newItem,flag);  //���ø�������״̬


    //groupNameEdit->raise();
    //groupNameEdit->setText(tr("δ����")); //����Ĭ������
    //groupNameEdit->selectAll();        //����ȫѡ
    //groupNameEdit->setGeometry(this->visualItemRect(newItem).left()+15,this->visualItemRect(newItem).top()+1,this->visualItemRect(newItem).width(),this->visualItemRect(newItem).height()-2);//���ֵ�λ��
    //groupNameEdit->show();              //��ʾ
    //groupNameEdit->setFocus();          //��ȡ����
    currentItem = newItem;     // ��ΪҪ��group���������Ե�ǰ��currentItem��Ϊ��group
}
//ɾ����
void PersonBuddyList::slotDelGroup()
{
    foreach(QListWidgetItem* item, groupMap.keys(currentItem))  //������������к��Ѻ��������
    {
        groupMap.remove(item);   //�Ƴ�
        delete item;   //ɾ��
    }
    isHideMap.remove(currentItem); //�Ƴ�
}
//������
void PersonBuddyList::slotRename()
{
    groupNameEdit->raise();
    groupNameEdit->setGeometry(this->visualItemRect(currentItem).left()+15,this->visualItemRect(currentItem).top()+1,this->visualItemRect(currentItem).width(),this->visualItemRect(currentItem).height()-2);//���ֵ�λ��
    groupNameEdit->setText(currentItem->text());   //��ȡ����������
    groupNameEdit->show();                        //��ʾ
    groupNameEdit->selectAll();                   //ȫѡ
    groupNameEdit->setFocus();                        //��ȡ����
}
//��Ӻ���
void PersonBuddyList::AddBuddy(PersonLoginInfo info)
{
    PersonBuddyInfo *buddy=new PersonBuddyInfo();   //����һ���Լ��������Ϣ��
    //buddy->headPath="img/head.bmp";//����ͷ��·��
    buddy->logInfo = info;
    buddy->headPath=info.logInfo.userHeadPath;
    QString name = info.logInfo.userName;
    buddy->name->setText(name == "" ? info.logHostName : name);                  //�����û���
    buddy->sign->setText(info.logInfo.userSign+"("+info.logHostIp+")");



    QList<QListWidgetItem*> tem = groupMap.keys(currentItem);//��ǰ���Ӧ��������鱾��ͺ��ѣ����Ƹ�tem
    //�ؼ�����
    QListWidgetItem *newItem = new QListWidgetItem();       //����һ��newItem
    this->insertItem(row(currentItem)+tem.count(),newItem); //����newItem���뵽����
    this->setItemWidget(newItem, buddy); //��buddy������newItem
    groupMap.insert(newItem,currentItem);   //�ӽ�������keyΪ���ѣ�valueΪ��
    if(isHideMap.value(currentItem))          //������������أ���ӽ�ȥ�ĺ�������Ϊ����
        newItem->setHidden(true);
    else                                      //���򣬸ú�������Ϊ��ʾ
        newItem->setHidden(false);
}
//ɾ������
void PersonBuddyList::slotDelBuddy()
{

    groupMap.remove(currentItem);  //�Ƴ��ú���
    delete currentItem;     //ɾ��
}
//���������
void PersonBuddyList::slotRenameEditFshed()
{
    if(groupNameEdit->text()!=NULL)      //����������༭��Ϊ��
        currentItem->setText(groupNameEdit->text());  //��������
    groupNameEdit->setText("");
    groupNameEdit->hide();  //�����������༭��


}


void PersonBuddyList::slotgroupmessage()
{}
void PersonBuddyList::slotpersonmessage()
{}

void PersonBuddyList::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug(__func__);
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));
    QString qs;
    if(event->buttons()==Qt::LeftButton && currentItem!=groupMap.value(currentItem))
    {
         QWidget *pw = itemWidget(currentItem);
         PersonBuddyInfo *pb=(PersonBuddyInfo*)(pw);
         PersonLoginInfo *pLog = new PersonLoginInfo(pb->logInfo);

          ChatDialog *chat = new ChatDialog(pLog);
          chat->show();
         //QMessageBox::information(NULL,"zk",qs,NULL);//���˫����
        // currentItem->data(1);
        // qDebug()<<pb->sign->text();
     }

}
