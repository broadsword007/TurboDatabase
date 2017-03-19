#ifndef TABLE_H
#define TABLE_H
#include <QHash>
#include <QString>
#include "record.h"
#include "index.h"
using namespace std;

class Table
{
public:
    struct List_node
    {
        Record* record ;
        List_node* previous ;
        List_node* next ;
    };
private:
    struct meta_data
    {
        QHash<QString, QString> attribute_datatype ;
        int times_used ;
    };
    QString table_name ;
    QList<QString> primary_key ;
    QList<Index*> indices ;
    List_node* first_record ;
    List_node* last_record ;
public:
    Table();
    bool createRecord(Record* val_record) ;
    QHash<QString, QString> readRecord(QList<QString> attributes, QString where_attribute, QString equals_value) ;
    bool updateRecord(QString attribute_to_change,QString new_value, QString where_attribute, QString has_value) ;
    bool deleteRecord(QString where_attribute, QString equals_value) ;
    QString getTable_name() const;
    void setTable_name(const QString &value);
};

#endif // TABLE_H
