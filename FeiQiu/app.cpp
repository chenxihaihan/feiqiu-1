
//----------appĿ�����ڵ����·��ʱ���ܹ��½���ť�����ҵ����ťʵ�ֹ���---------

#include "app.h"
#include <QMessageBox>
#include <QDebug>
#define MSPAINT "C:\\Windows\\System32\\MSpaint"

APP::APP(QObject *parent) ://rundll32.exeDllHost.exe
    QObject(parent)
{
}
APP::APP(QString fileName)
{
    FileName = fileName;//������ļ���
    pro = new QProcess;
    QFileInfo fileinfo(fileName);

     qDebug()<<fileName;
     qDebug()<<"-------";

     if(fileName == "")
     {
          return;
     }
    QFileIconProvider iconProvider;//��ȡ�ļ�ͼ�꣬����
    QIcon icon = iconProvider.icon(fileinfo);//�õ��ļ�����ͼ��
    button = new QPushButton;
    button->setIcon(icon);//���ð�ťͼ��Ϊ�ļ�ͼ��
    button->setIconSize(QSize(50,50));
    button->setMaximumSize(90,90);
    flags = "app";//��ʼ��flags

    connect(button,SIGNAL(clicked()),this,SLOT(buttonPresssed()));

    //------ɾ��APPС��ť-----
    QIcon ic("img/del.jpg");
    delAppBt=new QPushButton;
    delAppBt->setIcon(ic);
    delAppBt->setIconSize(QSize(20,20));
    delAppBt->setMaximumSize(20,20);
    delAppBt->move(40,0);
    delAppBt->setFlat(true);
    //delAppBt->installEventFilter(this);

    btWidget = new QWidget;
    btWidget->setParent(button);
    btWidget->move(10, 10);
    btWidget->setGeometry(0, 0, 601, 321);

    delAppBt->setParent(btWidget);
    delAppBt->setVisible(false);

    connect(delAppBt,SIGNAL(clicked()),this,SLOT(emitdelapp()));


}

void APP::buttonPresssed()
{

    qDebug() << FileName;
    QFileInfo file_info(FileName);
    QString qs_file_suffix  = file_info.suffix();

    qDebug() << qs_file_suffix;

    qs_file_suffix = "." + qs_file_suffix;

    qDebug() << qs_file_suffix;

    QSettings *settings=new QSettings("HKEY_CLASSES_ROOT\\" +qs_file_suffix,QSettings::NativeFormat);
    QString qs_tem;
    //QDir tem_dir;
    QString qs_sxb;

    bool tem_flag = false;
    if(qs_file_suffix != ".exe")
    {

        if(qs_file_suffix == ".jpg" || qs_file_suffix == ".png" || qs_file_suffix == ".gif" ||qs_file_suffix == ".bmp" || qs_file_suffix == ".ico")
        {
              FileName = change_name(FileName);
              pro->startDetached(MSPAINT, QStringList(FileName));
        }
        else
        {
            foreach(QString str,settings->childKeys())
            {
                 QString value=settings->value(str).toString();
                 qs_tem = value.toLocal8Bit().data();
                 qDebug() << qs_tem;
               // break;

                 if(tem_flag == true)
                 {
                     break;
                 }

                 QSettings *settings1=new QSettings("HKEY_CLASSES_ROOT\\" + qs_tem+"\\shell\\open\\command",QSettings::NativeFormat);
                 foreach(QString str,settings1->childKeys())
                 {
                     QStringList value=settings1->value(str).toString().split("\"");

                     qDebug() << value.count();

                     for( int i = 0; i < value.count(); i++)
                     {
                         qDebug() << value[i];
                     }

                     if(value[0] == "")
                     {
                         qs_tem = value[1];
                     }
                     else
                     {
                         qs_tem = value[0];
                     }

                     if(qs_tem.contains("%Systemroot%"))
                     {
                         qs_tem.replace("%Systemroot%", "C:\\Windows");
                     }

                     value = qs_tem.split(".exe");
                     qs_sxb = value[0] + ".exe";

                     bool ok = pro->startDetached( qs_sxb, QStringList(FileName));

                     qDebug()  << ok;
                     if(ok)
                     {
                          tem_flag = true;
                          break;
                     }
                 }
            }
        }
    }
    else
    {
         pro->startDetached( FileName, QStringList());
    }

   /* if(flags == "app")
    {
        pro->startDetached(FileName,QStringList());//��ǰ����ر�ʱ�����õ��ⲿ�����ܹ��ճ�����
                                                  //��һ���������ⲿexe·������exe·���к��пո�ʱ������Ҫ�õ��ڶ�������������ⲿӦ�ó���ֻ��Ҫexe��һ���Ϳ��Դ�ʱ��
                                                //�ڶ�������д��QStringList()�Ϳ�����
    }
    else if(flags =="web")
    {
        QDesktopServices::openUrl(QUrl(webAddress));//ֱ�Ӵ��ⲿ��ҳ
    }*/
}
QString APP::change_name(QString tem_file_name)
{
    tem_file_name.replace("/", "\\\\");
    return tem_file_name;
}
void APP::emitdelapp()
{
    emit appdel(this);
}
 void APP::delAppBt_show()
 {

    delAppBt->setVisible(true);
 }
 void APP::delAppBt_hide()
 {
    delAppBt->setVisible(false);
 }
