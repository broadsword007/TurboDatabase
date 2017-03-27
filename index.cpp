#include "index.h"

QString Index::getTarget_attribute() const
{
    return target_attribute;
}

Index::Index(QString val_type, QString val_target_attribute, Table* val_targetTable)
{
    type=val_type ;
    target_attribute=val_target_attribute ;
    targetTable=val_targetTable ;
}

QString Index::getType() const
{
    return type;
}
Index::~Index()
{
    
}
