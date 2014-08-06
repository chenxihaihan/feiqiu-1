#ifndef TITLE_TIPS_H
#define TITLE_TIPS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
class title_tips : public QWidget
{
    Q_OBJECT
public:
    explicit title_tips(QWidget *parent = 0);
    
signals:
    void myClose();

public:
    void setTitleText(QString title);
private:
    QLabel *titleText;//��������
    QPushButton *close_button;//����رհ�ť

protected:
    void paintEvent(QPaintEvent *);
};

#endif // TITLE_TIPS_H
