#ifndef RECORD_H
#define RECORD_H
#include <QHash>
#include <QList>
#include <QString>
using namespace std;

class Record
{
    QHash<QString, QString> data ; // [attribute => value]
public:
    Record(QHash<QString, QString> & val_data);
    QString getAttribute(QString attribute_name);
    void setAttribute(QString attribute_name, QString value) ;
};
#endif // RECORD_H
