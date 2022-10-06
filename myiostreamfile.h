#ifndef MYIOSTREAMFILE_H
#define MYIOSTREAMFILE_H

#include <QDialog>
#include <QString>

class Myiostreamfile : public QDialog
{
    Q_OBJECT
public:    
    explicit Myiostreamfile(QWidget *parent = nullptr);
    void saveToCSV(QString text);
    QString readFileTxt(int flagIndex);
    QList<QList<QString>> loadFile(QString strSplit);

signals:

};

#endif // MYIOSTREAMFILE_H
