#ifndef INDEX_H
#define INDEX_H
#include <QString>
#include "record.h"
#include "table.h"
class Index
{
protected:
    QString type ;
    QString target_attribute;
public:

    Index(QString val_type, QString val_target_attribute);
    virtual void construct_index(Table::List_node* first_node)=0 ;
    QString getType() const;
    virtual void add_record(Table::List_node* node)=0 ;
    virtual Table::List_node* find_record(QString attribute_value)=0;
    virtual bool update_record(QString current_value,QString attribute_to_change, QString new_value)=0 ;
    virtual bool delete_record(QString attribute_value)=0;
    ~Index() ;
    QString getTarget_attribute() const;
};

#endif // INDEX_H
