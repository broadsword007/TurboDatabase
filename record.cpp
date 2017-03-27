#include "record.h"
#include<QtDebug>
Record::Record(QHash<QString, QString> val_record_data)
{
    record_data=val_record_data ;
}

QString Record::getAttribute(QString attribute_name)
{
    QHash<QString,QString> test=record_data ;
    if(record_data.keys().contains(attribute_name))
    {
        return record_data[attribute_name] ;
    }
    else return "INVALID ATTRIBUTE" ;
}

void Record::setAttribute(QString attribute_name, QString value)
{
    record_data[attribute_name]=value ;
}

void Record::print()
{
    foreach (QString value, record_data.values()) {
        qDebug()<<value<<"  " ;
    }
    qDebug()<<endl;
}
