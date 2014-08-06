#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<QMessageBox>
#include <QFile>
#include <QFileDialog>


//----------------���������¼------------


bool ChatDialog::saveFile(QString &fileName,PersonLoginInfo *pb)//�����ļ�
{
    if(ui->ShowMessage->document()->isEmpty())
    {
      return true;
    }
    this->pb = pb;
    QString hostname = pb->logHostName;

    QString filename=QString("chat/%1.db").arg(hostname);
    QFile file(filename);
    if(!file.open(QFile::WriteOnly|QFile::Text|QFile::Append))//׷��
       {

        QMessageBox::warning(NULL,tr("�����ļ�"),tr("�ļ��޷�����%1��/n%2").arg(fileName).arg(file.errorString()));
        return false;
        }
    QTextStream out(&file);
    out<<ui->ShowMessage->toPlainText();
    out<<"\n";
    file.flush();//ˢ��
    file.close();
    return true;
}
 //-------------------�������¼-----------------
void ChatDialog::openChatSave()
{
    connect(ui->openChatButton,SIGNAL(clicked()),this,SLOT(openChat()));

}
void ChatDialog::openChat()

{
    saveFile(fileName,pb);
    Chatlog = new chatLogging(pb);
    Chatlog->show();
}
