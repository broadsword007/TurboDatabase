#ifndef DATABASE_H
#define DATABASE_H
#include <QList>
#include "table.h"
#include <QString>
class Database
{
    QString name ;
    QString target_file ;
    QList<Table*> tables;
    void initialize_from_file();
public:
    Database();
    void fromFile(QString Filename) ;
    void addTable(Table* table) ;
    bool dropTable(QString table_name) ;
    QString getName() const;
    void setName(const QString &value);

    bool createRecord(QString tableName, Record* val_record) ;
    QHash<QString, QString> readRecord(QString tableName, QList<QString> attributes, QString where_attribute, QString equals_value) ;
    bool updateRecord(QString tableName,QString attribute_to_change,QString new_value, QString where_attribute, QString has_value) ;
    bool deleteRecord(QString tableName, QString where_attribute, QString equals_value) ;
};

#endif // DATABASE_H
