#ifndef TABLE_H
#define TABLE_H
#include <QHash>
#include <QString>
#include "record.h"
//#include "index.h"
#include <QFile>
class Index;
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
    int times_used ;
    QString table_name ;
    QList<QString> primary_key ;
    QList<Index*> indices ;
    List_node* first_record ;
    List_node* last_record ;
    QHash<QString, QString> attribute_type_hash ; // [attribute => attribute_type]
public:
    Table(QString val_table_name, QHash<QString , QString> val_attribute_type_hash);
    void addIndex(Index* val_index) ;
    bool createRecord(Record* val_record) ;
    QHash<QString, QString> readRecord(QList<QString> attributes, QString where_attribute, QString equals_value) ;
    bool updateRecord(QString attribute_to_change,QString new_value, QString where_attribute, QString has_value) ;
    bool deleteRecord(QString where_attribute, QString equals_value) ;
    QString getTable_name() const;
    void setTable_name(const QString &value);
    void print_table() ;
    List_node* get_first_record_pointer() ;
    List_node* get_last_record_pointer() ;

    void insertAtStart(List_node* val_record) ;
    void insertAtEnd(List_node* val_record) ;
    void insertAfter(List_node* val_record, List_node* current) ;
    void insertBefore(List_node* val_record, List_node* current) ;
    void deleteNode(List_node* current) ;
    void replaceNode(List_node* to_replace, List_node* with) ;

    void serialized_to_file(QFile* filehandle) ;
};

#endif // TABLE_H
