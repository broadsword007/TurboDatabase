#ifndef RECORD_H
#define RECORD_H
#include <QHash>
#include <QList>
#include <QString>
using namespace std;

class Record
{
    QHash<QString, QString> record_data ; // [attribute => value]
public:
    Record(QHash<QString, QString> val_record_data);
    QString getAttribute(QString attribute_name);
    void setAttribute(QString attribute_name, QString value) ;
    void print() ;
};
#endif // RECORD_H
