#include "chatlogging.h"
#include "ui_chatlogging.h"
#include "chatdialog.h"
#include "PersonLoginInfo.h"
chatLogging::chatLogging(PersonLoginInfo *pb,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatLogging)
{
    ui->setupUi(this);
   //QString fileName = QFileDialog::getOpenFileName( this,tr("�������¼"),"./",tr("�ı�(*.txt);;AllFile(*.*)"));
   // QFile f(fileName);
    this->pb = pb;
    QString hostname = pb->logHostName;
    QString fileName=QString("chat/%1.db").arg(hostname);
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QTextStream t(&f);
    ui->textEdit->setText(t.readAll()) ;
    f.close();
     connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clearChat()));
}

chatLogging::~chatLogging()
{
    delete ui;
}


void chatLogging::on_quitButton_clicked()
{
    this->close();
}
//-----------------------ɾ�������¼�������ļ���------------
void chatLogging::clearChat()

{
   QMessageBox msgBox;
   msgBox.setIcon(QMessageBox::Warning);
   msgBox.setWindowTitle(tr("���棡"));
   msgBox.setText(tr("ȷ��Ҫɾ�������¼��"));
   QPushButton *Button1 = msgBox.addButton(tr("ȷ��"),QMessageBox::AcceptRole);
   QPushButton *Button2 = msgBox.addButton(tr("ȡ��"),QMessageBox::RejectRole);
   msgBox.exec();
   if (msgBox.clickedButton() == Button1)
   {
       ui->textEdit->clear();
       QString hostname = pb->logHostName;
       QString filename=QString("chat/%1.db").arg(hostname);
       QFile::remove(filename);//�h���ļ�

   }
   else if (msgBox.clickedButton() == Button2)
   {
       return;
   }
}
