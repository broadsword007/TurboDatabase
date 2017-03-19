#include "btreeindex.h"
#include <QString>
#include <QDebug>

Table::List_node *BTreeIndex::search(BTreeIndex::TreeNode *node, QString attribute_value)
{
    for(int i=0; i<node->attribute_values.size(); i++)
    {
        int comparison_result=QString::compare(attribute_value,node->attribute_values[i],  Qt::CaseInsensitive);
        if(comparison_result==0)
        {
            return node->records[i] ; // found key. Returning a pointer to the record
        }
        if(node->children.size()==0 && i==node->attribute_values.size()-1)
        {
            return NULL ; // no such key exists
        }
        else if(comparison_result<0) // if key is less than current value
        {
            return search(node->children[i], attribute_value) ; //start searching from child branch before current value
        }
        else if(comparison_result>0 && i==node->attribute_values.size()-1) // if key is greater than all children
        {
            return search(node->children[i+1], attribute_value) ; //start searching from rightmost branch
        }
    }
}

void BTreeIndex::insert(TreeNode *node, Table::List_node *record_node)
{
    for(int i=0; i<node->attribute_values.size(); i++)
    {
        int comparison_result=QString::compare(record_node->record->getAttribute(target_attribute),node->attribute_values[i],  Qt::CaseInsensitive);
        if(comparison_result<0) // if key is less than current value
        {
            if(node->children.size()==0) //is root node
            {

                Table::List_node* after=node->records[i] ; // after is the node which has to come after the node to be inserted
                if(type=="primary")  // if the index is primary we change the actual order of data as well
                {
                    record_node->next=after ;
                    record_node->previous=after->previous ;
                    after->previous->next=record_node ;
                    after->previous=record_node ;
                }
                node->records.insert(i, record_node) ;
                node->attribute_values.insert(i, record_node->record->getAttribute(target_attribute));
                if(node->records.size()==order)
                {
                    split(node) ; //split
                }
            }
            else insert(node->children[i], record_node) ;
        }
        else if(comparison_result>0 && i==node->attribute_values.size()-1) // if key is greater than all children
        {
            if(node->children.size()==0) //is root node
            {
                Table::List_node* before=node->records[i] ; // before is the node which has to come before the node to be inserted
                if(type=="primary")  // if the index is primary we change the actual order of data as well
                {
                    record_node->previous=before ;
                    record_node->next=before->next ;
                    before->next->previous=record_node ;
                    before->next=record_node ;
                }
                node->records.insert(i, record_node) ;
                node->attribute_values.insert(i, record_node->record->getAttribute(target_attribute));
                if(node->records.size()==order)
                {
                    split(node) ; //split
                }
            }
            else insert(node->children[i+1], record_node) ;
        }
    }
}

void BTreeIndex::split(BTreeIndex::TreeNode *node)
{
    if(node->records.size()<3)
    {
        qDebug()<<"A really odd order of B tree. You are such a pathetic person! I hate you!"<<endl ;
    }
    int breakpoint=ceil(node->records.size()/2) ;
    TreeNode* new_node=new TreeNode;
    new_node->records.append(node->records[breakpoint]) ;
    new_node->attribute_values.append(node->records[breakpoint]->record->getAttribute(target_attribute)) ;
    TreeNode* left_child=new TreeNode ;
    for(int i=0; i<breakpoint; i++)
    {
        left_child->records.append(node->records[i]);
        left_child->children.append(node->children[i]) ;
        left_child->attribute_values.append(node->attribute_values[i]) ;
        left_child->parent=new_node ;
    }
    TreeNode* right_child=new TreeNode ;
    for(int i=breakpoint+1; i<node->records.size(); i++)
    {
        right_child->records.append(node->records[i]);
        right_child->children.append(node->children[i]) ;
        right_child->attribute_values.append(node->attribute_values[i]) ;
        right_child->parent=new_node ;
    }
    new_node->children.append(left_child) ;
    new_node->children.append(right_child) ;
    new_node->parent=node->parent ;
    delete node ;
    if(node->parent==NULL) // is a root node
    {
        root=node ;
    }
    else bubble_up(new_node->parent, new_node) ;
}

void BTreeIndex::bubble_up(BTreeIndex::TreeNode *parent, BTreeIndex::TreeNode *node)
{
    for(int i=0; i<parent->attribute_values.size(); i++)
    {
        int comparison_result=QString::compare(node->attribute_values[0],parent->attribute_values[i],  Qt::CaseInsensitive);
        if(comparison_result< 0) // found where to insert the guest node
        {
            parent->attribute_values.insert(i, node->attribute_values[0]) ;
            parent->records.insert(i, node->records[0]) ;
            parent->children[i]=node->children[1] ;
            parent->children.insert(i-1, node->children[0]) ;
            if(parent->records.size()==order)
            {
                split(parent) ;
            }
            else return ;
        }
    }
}

BTreeIndex::BTreeIndex(int val_order,QString val_type, QString val_target_attribute):Index (val_type, val_target_attribute)
{
    order=val_order ;
}

void BTreeIndex::construct_index(Table::List_node *first_node)
{

}

void BTreeIndex::add_record(Table::List_node *record_node)
{
    if(root==NULL) // tree is empty
    {
        root=new TreeNode ;
        root->records.append(record_node) ; //simply insert as root
        root->parent=NULL ;
    }
    else insert(root, record_node) ;
}

Table::List_node *BTreeIndex::find_record(QString attribute_value)
{
    if(root==NULL)
    {
        qDebug()<<"BTree has not yet been initialized!"<<endl ;
        return NULL ;
    }
    else return search(root, attribute_value) ;
}

bool BTreeIndex::update_record(QString current_value,QString attribute_to_change, QString new_value)
{
    Table::List_node* destination_node= search(root, current_value) ;
    if(destination_node)
    {
        destination_node->record->setAttribute(target_attribute, new_value) ;
        return true ;
    }
    else return false ;
}

bool BTreeIndex::delete_record(QString attribute_value)
{

}
