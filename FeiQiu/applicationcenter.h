#ifndef APPLICATIONCENTER_H
#define APPLICATIONCENTER_H

#include <QMainWindow>
#include <QProcess>
#include <QUrl>
#include <QDesktopServices>
#include <QGridLayout>
#include  "app.h"
#include <QFileDialog>
#include <QTextEdit>
#include <QDragEnterEvent>
namespace Ui {
class ApplicationCenter;
}

class ApplicationCenter : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ApplicationCenter(QWidget *parent = 0);
    ~ApplicationCenter();

    QProcess* pro;
    QGridLayout* vbox;
    int row;
    int col;
    APP* app;


    
private slots:
    void on_quitButton_clicked();
    void wzq();
    void baidu();
   // void on_wzqButton_clicked();

  void on_addButton_clicked();
  void on_weblineEdit_returnPressed();
  //void text();

  void mouPress(QMouseEvent *event);
private:
    Ui::ApplicationCenter *ui;
    QUrl *url;

 //--------------�Ϸ�-------------
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    bool readFile(const QString &fileName);

    //-----
    QPushButton *button;
     QWidget *btWidget;

    //---����app---
public:
     void saveApp(QString fileName);
     //---��app---
     void openApp();
     bool isDelButVisi;
     void delApp();


//------------ɾ��-----
private slots:

     void delAllApp();//����ť���г���ɾ��ͼ��
     //void emitdelapp();
     void delApp(QObject *app);//ɾ��һ��
signals:

    // void delapp(QObject* app);

private:
     QList<APP*> appList;
public:
     void DeleteOneline(int nNum, QString &strall);
     void deleteOnelineInFile(int nNumLine, QString filename);

};

#endif // APPLICATIONCENTER_H
