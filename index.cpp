#include "index.h"

QString Index::getTarget_attribute() const
{
    return target_attribute;
}

Index::Index(QString val_type, QString val_target_attribute)
{
    type=val_type ;
    target_attribute=val_target_attribute ;
}

QString Index::getType() const
{
    return type;
}

void Index::add_record(Table::List_node *node)
{

}

Index::~Index()
{
    
}
