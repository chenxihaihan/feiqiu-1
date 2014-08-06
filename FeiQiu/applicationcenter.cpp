#include "applicationcenter.h"
#include "ui_applicationcenter.h"
#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QAction>
#include <QTextStream>
#include <QListIterator>
ApplicationCenter::ApplicationCenter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ApplicationCenter)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Ӧ������"));
    ui->baiduButton->setToolTip(tr("�ٶ�����"));
    ui->wzqButton->setToolTip(tr("��Ϸ-������"));
    ui->addButton->setToolTip(tr("���Ӧ��"));
    QIcon ico(":/img/app1.jpg");
    this->setWindowIcon(ico);

    connect(ui->wzqButton,SIGNAL(clicked()),this,SLOT(wzq()));
    connect(ui->baiduButton,SIGNAL(clicked()),this,SLOT(baidu()));

     pro = new QProcess;
    vbox = new QGridLayout;
    row = 0;
    col = 0;
    ui->appgroupBox->setLayout(vbox);
    vbox->setColumnStretch(6,1);
    vbox->setRowStretch(6,1);


    //--------�Ϸ�-----------
    ui->appgroupBox->setAcceptDrops(false);
    setAcceptDrops(true);


//------------���ļ���ȡ��app------
     openApp();




     //-----���� ɾ��-----
     isDelButVisi = true;
     connect(ui->delAllButton,SIGNAL(clicked()),this,SLOT(delAllApp()));


}
void ApplicationCenter::mouPress(QMouseEvent *event)
{
    QMessageBox::information(NULL,tr("ee"),tr("ee"),NULL);
    ApplicationCenter::mouPress(event);
    if(!event->button() == Qt::RightButton)
    {

       return;
    }
    QMenu *Menu = new QMenu();
    QAction *quitAction = new QAction(tr("ɾ�� (&D)"), this);
    Menu->addAction(quitAction);
    ui->baiduButton->setMenu(Menu);
    Menu->exec();
    // connect(quitAction, SIGNAL(triggered()),this, SLOT(mouPress(QMouseEvent*event)));

}

ApplicationCenter::~ApplicationCenter()
{
    delete ui;
}

void ApplicationCenter::on_quitButton_clicked()
{
    this->close();
}

void ApplicationCenter::wzq()
{
    QProcess *myProcess = new QProcess();
    myProcess->start("wzq/Release/Pente.exe");
}
void ApplicationCenter::baidu()
{
    QDesktopServices::openUrl(QUrl("www.baidu.com"));

}

void ApplicationCenter::on_addButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    app = new APP(fileName);





    if(col == 7)
    {
    row++;
    col = 0;
    }
    //���һ���հ��ʹ������ʾ�������ɸ��ݾ���������Ӧ��
    vbox->addWidget(app->button,row,col,Qt::AlignLeft);//ˮƽ������
    col++;

    connect(app,SIGNAL(appdel(QObject*)),this,SLOT(delApp(QObject *)));
    appList.append(app);

     saveApp(fileName);

     fileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
     app->button->setToolTip(fileName);


}


//-------��app·�������ļ�-----
void ApplicationCenter::saveApp(QString fileName)
{

    QString appFilename=QString("app/app.db");
    QFile appFile(appFilename);
    if(!appFile.open(QFile::WriteOnly|QFile::Text|QFile::Append))//׷��
       {

        QMessageBox::warning(NULL,tr("�����ļ�"),tr("�ļ��޷�����%1��/n%2").arg(appFilename).arg(appFile.errorString()));
        return ;
        }
    QTextStream out(&appFile);
    qDebug()<<fileName;
    out<<fileName;

    out<<"\n";
    appFile.close();

}

void ApplicationCenter::on_weblineEdit_returnPressed()
{
    QString address = ui->weblineEdit->text();
    QDesktopServices::openUrl(QUrl(address));
}

//--------------------�Ϸ�----------------
void ApplicationCenter::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))//hasFormat("text/uri-list")�����Ƿ���������Ϣ��URI
    {
        event->acceptProposedAction();//����ִ���Ϸ���Ϊ��һ�����趨��Ԥ�趯����
    }
}
void ApplicationCenter::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();//QMimeData��urls()ȡ�����Ϸŵ�����QUr
    if (urls.isEmpty())
    {
        return;
    }
    QString fileName = urls.first().toLocalFile();
    app = new APP(fileName);
    qDebug()<<"zzzzzzzzzzzz";
    qDebug()<<fileName;



    if(col == 7)
    {
    row++;
    col = 0;
    }
    //���һ���հ��ʹ������ʾ�������ɸ��ݾ���������Ӧ��
   vbox->addWidget(app->button,row,col,Qt::AlignLeft);//ˮƽ������
    col++;

    connect(app,SIGNAL(appdel(QObject*)),this,SLOT(delApp(QObject *)));
    appList.append(app);

    saveApp(fileName);

    fileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    app->button->setToolTip(fileName);


}

//----------------���ļ���ȡ��app------
void ApplicationCenter::openApp()
{
    QString appFilename = QString("app/app.db");
    qDebug()<<"===";
    qDebug()<<appFilename;
    QFile appFile(appFilename);

    if(appFile.open(QIODevice::ReadOnly))
    {
        QTextStream reader(&appFile);
        QString currentrow;
          while(!reader.atEnd())
          {

            currentrow = reader.readLine();
            qDebug()<<"------*-";
            qDebug()<<currentrow;
            if(currentrow == "")
            {
               break;
            }

            QFileInfo appInfo(currentrow);
            QFileIconProvider appIcoPro;
            QIcon icon = appIcoPro.icon(appInfo);

            button = new QPushButton;
            button->setIcon(icon);//���ð�ťͼ��Ϊ�ļ�ͼ��
            button->setIconSize(QSize(50,50));
            button->setMaximumSize(70,70);
            app = new APP(currentrow);



            connect(app,SIGNAL(appdel(QObject*)),this,SLOT(delApp(QObject *)));

            appList.append(app);

            qDebug()<<"delete";

           // connect(app,SIGNAL(delapp(QObject*)),this,SLOT(delAllApp()));

            if(col == 7)
            {
            row++;
            col = 0;
            }
           // app->button->setToolTip(fileName);
            //���һ���հ��ʹ������ʾ�������ɸ��ݾ���������Ӧ��
            vbox->addWidget(app->button,row,col,Qt::AlignLeft);//ˮƽ������
            col++;

            currentrow = currentrow.right(currentrow.size() - currentrow.lastIndexOf('/') - 1);
            app->button->setToolTip(currentrow);

         }
    }
}


//------------���� ɾ��-----
void ApplicationCenter::delAllApp()
{
    if(isDelButVisi == false)
    {

        QListIterator< APP *>  list(appList);

        while (list.hasNext())
        {
            APP *tem_app=list.next();
            tem_app->delAppBt_hide();

        }
       isDelButVisi=true;
        ui->delAllButton->setText(tr("����"));

    }else
    {
        QListIterator< APP *>  list(appList);

        while (list.hasNext())
        {
            APP *tem_app=list.next();
            tem_app->delAppBt_show();


        }
        isDelButVisi=false;
        ui->delAllButton->setText(tr("���"));
    }

}


void ApplicationCenter::delApp(QObject *app)

{

    int n=0;
    for(QList<APP*>::iterator it = appList.begin();it < appList.end();++it)
    {
        if((*it) == (APP *)app )
        {
          break;
        }
        n++;

    }
    qDebug()<<n;
    deleteOnelineInFile(n,QString("app/app.db"));
    appList.removeOne((APP *)app);
    delete app;


    int appcount=0;
    int rowcount=2;
    QListIterator< APP *>  list(appList);
    qDebug()<<" hahhaha app";

    while (list.hasNext())
    {
        qDebug()<<"kkkkkkkkk";
        APP * temp =list.next();
        int col=appcount%7;
        int row  =appcount/7;

        if(row>rowcount)
        {
            rowcount++;
        }

         vbox->addWidget(temp->button,row,col,Qt::AlignLeft);
         appcount++;

    }

}



//------------------ɾ���ļ�����--------
void ApplicationCenter::DeleteOneline(int nNum, QString &strall)//
{
    int nLine=0;
    int Index=0;
    //�������nLine
    while(Index!=-1)
    {
        Index=strall.indexOf('\n',Index+1);
        nLine++;
    }

    //�����ֱ�Ӵ�λ��0��ʼɾ��\n��һ���ַ�"abc\nme\ndo" \n��index��3Ҫɾ��3+1���ַ�����index+1��
    if(nNum==0)
    {
        int nIndex=strall.indexOf('\n');
        strall.remove(0,nIndex+1);
    }
    else
    {
        int nTemp=nNum;
        int nIndex=0,nIndex2=0;
        while(nTemp--)
        {
            //
            nIndex=strall.indexOf('\n',nIndex+1);//��������nIndex
            if(nIndex!=-1)//˵������Ч��
            {
                nIndex2=strall.indexOf('\n',nIndex+1);
            }
        }
        //ɾ�����в������һ�У���nIndex+1���λ����nIndex2-nIndex���ַ�ȫ��Ĩȥ��
        if(nNum<nLine-1)
        {
            strall.remove(nIndex+1, nIndex2-nIndex);//���ü�һ
        }
        //ɾ���������һ�У���nIndex��ʼlen-nIndex���ַ�ȫĨȥ��
        //���ܴ�nIndex+1����ʼ��
        else if(nNum==nLine-1)
        {
            int len=strall.length();
            strall.remove(nIndex,len-nIndex);
        }
        else
        {

        }

    }
}
void ApplicationCenter::deleteOnelineInFile(int nNumLine, QString filename)
{
    QString strall;
    QFile readfile(filename);
    if(readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        strall=stream.readAll();
    }
    readfile.close();
    DeleteOneline(nNumLine, strall);

    QFile writefile(filename);
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream<<strall;
    }
    writefile.close();
}
