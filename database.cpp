#include "database.h"
#include <QFile>
#include <QDebug>
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

void Database::save_to_file(QString filename)
{
     QFile file("save");
     if (!file.open(QIODevice::ReadWrite))
     {
         qDebug()<<"Unable to open the file!"<<endl ;
         return;
     }
     QString to_be_written="***Database\n" ;
     to_be_written+="***tables\n" ;
     for(int i=0; i<tables.size(); i++)
     {
         to_be_written+=tables[i]->getTable_name()+"\n" ;
     }
     to_be_written+="###tables\n" ;
     file.write(to_be_written.toLocal8Bit()) ;
     for(int i=0; i<tables.size(); i++)
     {
         tables[i]->serialized_to_file(&file) ;
     }
     to_be_written="###Database\n" ;
     file.write(to_be_written.toLocal8Bit()) ;
     qDebug()<<"Written to file"<<endl ;
     return  ;
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

void Database::add_index(QString tablename, Index *index)
{
    for(int i=0; i<tables.size(); i++)
    {
        if(tables[i]->getTable_name()==tablename)
        {
            return tables[i]->addIndex(index) ;
        }
    }
}

void Database::print_table(QString table_name)
{
    foreach (Table* table, tables) {
        if(table->getTable_name()==table_name) {table->print_table() ; return;}
    }
}
