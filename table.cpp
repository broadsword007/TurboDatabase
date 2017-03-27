#include "table.h"
#include "index.h"
#include <QtDebug>
#include <QFile>
QString Table::getTable_name() const
{
    return table_name;
}

void Table::setTable_name(const QString &value)
{
    table_name = value;
}

void Table::print_table()
{
    qDebug()<<"Table name : "<<table_name<<endl ;
    foreach(QString attr_name, attribute_type_hash)
    {
        qDebug()<<attr_name<<"  " ;
    }
    qDebug()<<endl ;
    List_node* current_node=first_record ;
    while(current_node)
    {
        current_node->record->print() ;
        current_node=current_node->next ;
    }
}

Table::List_node *Table::get_first_record_pointer()
{
    return first_record ;
}
Table::List_node *Table::get_last_record_pointer()
{
    return last_record ;
}

void Table::insertAtStart(List_node *val_record)
{
    List_node* new_node=val_record ;
    new_node->next=first_record ;
    new_node->previous=NULL;
    if(new_node->next)new_node->next->previous=new_node ;
    else last_record=new_node ;
    first_record=new_node ;

}

void Table::insertAtEnd(List_node *val_record)
{
    // inserting at the end
    List_node* new_node=val_record ;
    new_node->next=NULL ;
    new_node->previous=last_record ;
    if(new_node->previous)new_node->previous->next=new_node ;
    last_record=new_node ;
}

void Table::insertAfter(Table::List_node *val_record, Table::List_node *current)
{
    // inserting after the current node
    List_node* new_node=val_record ;
    new_node->previous=current ;
    new_node->next=current->next ;
    if(new_node->next)new_node->next->previous=new_node ;
    else
    {
        last_record=new_node ;
    }
    current->next=new_node ;
}

void Table::insertBefore(List_node *val_record, Table::List_node *current)
{
    // inserting somewhere in between list
    List_node* new_node=val_record ;
    new_node->next=current ;
    new_node->previous=current->previous ;
    if(new_node->previous)new_node->previous->next=new_node ;
    else
    {
        first_record=new_node ;
    }
    current->previous=new_node ;
}

void Table::deleteNode(Table::List_node *current)
{
    if(current)
    {
        //record has been found
        if(current->next==NULL) // current is the last node
        {
            last_record= (current->previous ? current->previous : NULL) ;
            if(last_record)last_record->next=NULL ;
            else first_record=NULL ;
            delete current ;
        }
        else if(current->previous==NULL) // current is the first node
        {
            first_record=(current->next ? current->next : NULL);
            if(first_record) first_record->previous=NULL ;
            else last_record=NULL ;
            delete current ;
        }
        else
        {           //current is a node in-between
            current->previous->next=current->next ;
            current->next->previous=current->previous ;
            delete current ;
        }
    }
}

void Table::replaceNode(Table::List_node *to_replace, Table::List_node *with)
{
    if(to_replace->previous) to_replace->previous->next=with ;
    else first_record=with ;
    if(to_replace->next) to_replace->next->previous=with ;
    else last_record=with ;
    delete to_replace ;
}

void Table::serialized_to_file(QFile * filehandle)
{
    QString to_return="***table\n" ;
    to_return+=table_name+"\n" ;
    to_return+=QString::number(times_used)+"\n" ;
    to_return+="***Attributes\n" ;
    foreach (QString attr_name, attribute_type_hash.keys())
    {
        to_return+=attr_name+"\n" ;
        to_return+=attribute_type_hash[attr_name]+"\n" ;
    }
    to_return+="###Attributes\n" ;
    to_return+="***data\n" ;
    List_node* current_node=first_record ;
    while(current_node)
    {
        foreach (QString attr_name, attribute_type_hash.keys()) {
            to_return+=current_node->record->getAttribute(attr_name)+"\n" ;
        }
        current_node=current_node->next ;
    }
    to_return+="###data\n" ;
    filehandle->write(to_return.toLocal8Bit()) ;
}

Table::Table(QString val_table_name , QHash<QString,QString> val_attribute_type_hash)
{
    table_name=val_table_name ;
    first_record=NULL ;
    last_record=NULL ;
    times_used=0;
    attribute_type_hash=val_attribute_type_hash ;
}

void Table::addIndex(Index *val_index)
{
    indices.append(val_index);
}

bool Table::createRecord(Record *val_record)
{
    if(indices.size()==0)
    {
        // traverse the Linked list and insert record at proper place
        if(!first_record)
        {
            first_record=new List_node ;
            first_record->record=val_record ;
            first_record->next=NULL ;
            first_record->previous=NULL ;
            last_record=first_record ;
            return true ;
        }
        List_node* current=first_record ;
        qDebug()<<"newer : "<<val_record->getAttribute(attribute_type_hash.keys().last());
        qDebug()<<"previous : "<< current->record->getAttribute(attribute_type_hash.keys().last());
        while(current && val_record->getAttribute(attribute_type_hash.keys().last())>current->record->getAttribute(attribute_type_hash.keys().last()))
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
            return true ;
        }
        else if(current && !current->previous)
        {
            // inserting at start
            List_node* new_node=new List_node ;
            new_node->record=val_record ;
            new_node->next=current ;
            new_node->previous=NULL;
            new_node->next->previous=new_node ;
            first_record=new_node ;
            return true ;
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
            return true ;
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
    if(indices.size()==0)
    {
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
        return true ;
    }
    else
    {
        foreach (Index* index, indices)
        {
            if(index->getTarget_attribute()==where_attribute) // if there is an index available on this attribute use it
            {
                index->update_record(has_value, attribute_to_change, new_value) ;
            }
        }
        return true ;
    }
}

bool Table::deleteRecord(QString where_attribute, QString equals_value)
{
    if(indices.size()==0)
    {

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
                last_record= (current->previous ? current->previous : NULL) ;
                if(last_record)last_record->next=NULL ;
                else first_record=NULL ;
                delete current ;
                return true ;
            }
            else if(current->previous==NULL) // current is the first node
            {
                first_record=(current->next ? current->next : NULL);
                if(first_record) first_record->previous=NULL ;
                else last_record=NULL ;
                delete current ;
                return true ;
            }
            else
            {           //current is a node in-between
                current->previous->next=current->next ;
                current->next->previous=current->previous ;
                delete current ;
                return true ;
            }
        }
        return true ;
    }
    else
    {
        foreach (Index* index, indices)
        {
            if(index->getTarget_attribute()==where_attribute) // if there is an index available on this attribute use it
            {
                return index->delete_record(equals_value) ;
            }
        }
        return true ;
    }
}
