#include "record.h"
#include<QtDebug>
Record::Record(QHash<QString, QString> &val_data)
{
    data=val_data ;
}

QString Record::getAttribute(QString attribute_name)
{
    if(data.find(attribute_name)==data.end())
    {
        return "INVALID ATTRIBUTE" ;
    }
    else return data[attribute_name] ;
}

void Record::setAttribute(QString attribute_name, QString value)
{
    data[attribute_name]=value ;
}
