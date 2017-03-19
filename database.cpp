#include "database.h"

void Database::initialize_from_file()
{
    /// Use file to initialize
}

Database::Database()
{

}

void Database::fromFile(QString Filename)
{
    target_file=Filename ;
    initialize_from_file() ;
}

void Database::addTable(Table *table)
{
    tables.append(table) ;
}

bool Database::dropTable(QString table_name)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==table_name)
        {
            delete tables[i] ;
            tables.removeAt(i) ;
            return true;
        }
    }
    return false ;
}

QString Database::getName() const
{
    return name;
}

void Database::setName(const QString &value)
{
    name = value;
}

bool Database::createRecord(QString tableName, Record *val_record)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==tableName)
        {
            return tables[i]->createRecord(val_record) ;
        }
    }
    return false ;
}

QHash<QString, QString> Database::readRecord(QString tableName, QList<QString> attributes, QString where_attribute, QString equals_value)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==tableName)
        {
            return tables[i]->readRecord(attributes, where_attribute, equals_value) ;
        }
    }
    return QHash<QString, QString>() ;
}

bool Database::updateRecord(QString tableName, QString attribute_to_change,QString new_value, QString where_attribute, QString has_value)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==tableName)
        {
            return tables[i]->updateRecord(attribute_to_change, new_value, where_attribute, has_value) ;
        }
    }
    return false ;
}

bool Database::deleteRecord(QString tableName, QString where_attribute, QString equals_value)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==tableName)
        {
            return tables[i]->deleteRecord(where_attribute, equals_value) ;
        }
    }
    return false ;
}
