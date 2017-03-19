#include "table.h"

QString Table::getTable_name() const
{
    return table_name;
}

void Table::setTable_name(const QString &value)
{
    table_name = value;
}

Table::Table()
{

}

bool Table::createRecord(Record *val_record)
{
    if(indices.size()==0)
    {
        // traverse the Linked list and insert record at proper place
        if(!first_record)
        {
            first_record->record=val_record ;
            first_record->next=NULL ;
            first_record->previous=NULL ;
            return true ;
        }
        List_node* current=first_record ;
        while(current && val_record->getAttribute(primary_key[0])>current->record->getAttribute(primary_key[0]))
        {
            current=current->next ;
        }
        if(current && current->next && current->previous)
        {
            // inserting somewhere in between list
            List_node* new_node=new List_node ;
            new_node->record=val_record ;
            new_node->next=current ;
            new_node->previous=current->next->previous ;
            new_node->previous->next=new_node ;
            new_node->next->previous=new_node ;
        }
        else if(current && current->next && !current->previous)
        {
            // inserting at start
            List_node* new_node=new List_node ;
            new_node->record=val_record ;
            new_node->next=current ;
            new_node->previous=NULL;
            new_node->next->previous=new_node ;
            first_record=new_node ;
        }
        else if(!current)
        {
            // inserting at the end
            List_node* new_node=new List_node ;
            new_node->record=val_record ;
            new_node->next=NULL ;
            new_node->previous=last_record ;
            new_node->previous->next=new_node ;
            last_record=new_node ;
        }
    }
    else
    {
        // insert the record using the available index. Index will handle the case of primary or secondary index by itself
        // insertion through index should be much more faster than without index
        foreach (Index* index, indices)
        {
            List_node* new_node=new List_node ;
            new_node->record=val_record ;
            index->add_record(new_node) ;
        }
    }
}

QHash<QString, QString> Table::readRecord(QList<QString> attributes, QString where_attribute, QString equals_value)
{
    foreach (Index* index, indices)
    {
        if(index->getTarget_attribute()==where_attribute) // if there is an index available on this attribute use it
        {
            Table::List_node* found_record=index->find_record(equals_value) ;
            if(found_record)
            {
                QHash<QString, QString> attribute_values ;
                for(int i=0; i<attributes.size(); i++)
                {
                    attribute_values[attributes[i]]=found_record->record->getAttribute(attributes[i]) ;
                }
                return attribute_values ;
            }
            else return QHash<QString, QString>() ;
        }
    }

    // traverse the Linked list and read record from proper place
    if(!first_record)
    {
        return QHash<QString, QString>() ; // no record was found. List is empty
    }
    List_node* current=first_record ;
    while(current && current->record->getAttribute(where_attribute)!=equals_value)
    {
        current=current->next ;
    }
    if(current)
    {
        //record has been found
        QHash<QString, QString> attribute_values ;
        for(int i=0; i<attributes.size(); i++)
        {
            attribute_values[attributes[i]]=current->record->getAttribute(attributes[i]) ;
        }
        return attribute_values ;
    }
    else return QHash<QString, QString>() ; // no record was found
}

bool Table::updateRecord(QString attribute_to_change,QString new_value, QString where_attribute, QString has_value)
{
    foreach (Index* index, indices)
    {
        if(index->getTarget_attribute()==where_attribute) // if there is an index available on this attribute use it
        {
            return index->update_record(has_value, attribute_to_change, new_value) ;
        }
    }

    // traverse the Linked list and read record from proper place
    if(!first_record)
    {
        return false ; // no record was found. List is empty
    }
    List_node* current=first_record ;
    while(current && current->record->getAttribute(where_attribute)!=has_value)
    {
        current=current->next ;
    }
    if(current)
    {
        //record has been found
        current->record->setAttribute(attribute_to_change, new_value) ;
        return true ;
    }
    else return false ; // no record was found
}

bool Table::deleteRecord(QString where_attribute, QString equals_value)
{
    foreach (Index* index, indices)
    {
        if(index->getTarget_attribute()==where_attribute) // if there is an index available on this attribute use it
        {
            return index->delete_record(equals_value) ;
        }
    }

    // traverse the Linked list and read record from proper place
    if(!first_record)
    {
        return false; // no record was found. List is empty
    }
    List_node* current=first_record ;
    while(current && current->record->getAttribute(where_attribute)!=equals_value)
    {
        current=current->next ;
    }
    if(current)
    {
        //record has been found
        if(current->next==NULL) // current is the last node
        {
            last_record=current->previous ;
            delete current ;
            return true ;
        }
        else if(current->previous==NULL)
        {
            first_record=current->next ;
            delete current ;
            return true ;
        }
        else
        {
            current->previous->next=current->next ;
            current->next->previous=current->previous ;
            delete current ;
            return true ;
        }
    }
    else return false ; // no record was found
}
