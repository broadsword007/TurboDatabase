#include "btreeindex.h"
#include <QString>
#include <QDebug>

BTreeIndex::TreeNode* BTreeIndex::search(BTreeIndex::TreeNode *node, QString attribute_value)
{
    for(int i=0; i<node->attribute_values.size(); i++)
    {
        int comparison_result=QString::compare(attribute_value,node->attribute_values[i],  Qt::CaseInsensitive);
        if(comparison_result==0)
        {
            index_to_access=i ;
            return node ; // found key. Returning a pointer to the node with key # stored in index_to_access
        }
        if(node->children.size()==0 && i==node->attribute_values.size()-1)
        {
            index_to_access=-1 ;
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
            if(node->children.size()==0) //is leaf node
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
                return ;
            }
            else return ;
        }
    }
}

int BTreeIndex::get_position_of_key_in_node(BTreeIndex::TreeNode *node, QString key)
{
    if(!node) return -1 ;
    for(int i=0; i<node->attribute_values.size(); i++)
    {
        if(node->attribute_values[i]==key)
        {
            return i ;
        }
    }
    return -1 ;
}

int BTreeIndex::get_position_of_parent_key(BTreeIndex::TreeNode *parent_node, BTreeIndex::TreeNode *child_node)
{
    if(parent_node->attribute_values.size()==0) {qDebug()<< "Invalid node!" ; return -1;}
    else if(child_node->attribute_values[0]< parent_node->attribute_values[0])
    {
        qDebug()<<"Invalid child" ;
        return -1 ;
    }
    if(child_node->attribute_values[0] > parent_node->attribute_values.last())
    {
        return parent_node->attribute_values.size()-1 ; // child node is the r right child of the last key in parent
    }
    for(int i=0; i<parent_node->attribute_values.size(); i++)
    {
        if(child_node->attribute_values[0] < parent_node->attribute_values[i])
        {
            return i ; // child node is the left child of i-th key in parent
        }
    }
}

QString BTreeIndex::get_max_val_of_tree(BTreeIndex::TreeNode *node)
{
    if(node->children.size()==0)
    {
        return node->attribute_values.last() ;
    }
    else
    {
        return get_max_val_of_tree(node->children.last());
    }
}

QString BTreeIndex::get_min_val_of_tree(BTreeIndex::TreeNode *node)
{
    if(node->children.size()==0)
    {
        return node->attribute_values.first() ;
    }
    else
    {
        return get_max_val_of_tree(node->children.first());
    }
}

Table::List_node * BTreeIndex::remove_max_record_from_tree(BTreeIndex::TreeNode *node)
{
    if(node->children.size()==0)
    {
        Table::List_node* to_return=node->records.last() ;
        if(this->type=="primary")
        {
            if(to_return->previous) to_return->previous->next=to_return->next ;
            if(to_return->next)to_return->next->previous=to_return->previous ;
        }
        node->attribute_values.removeLast() ;
        node->records.removeLast() ;
        return to_return ;
    }
    else
    {
        return remove_max_record_from_tree(node->children.last());
    }
}
Table::List_node * BTreeIndex::remove_min_record_from_tree(BTreeIndex::TreeNode *node)
{
    if(node->children.size()==0)
    {
        Table::List_node* to_return=node->records.first() ;
        if(this->type=="primary")
        {
            if(to_return->previous) to_return->previous->next=to_return->next ;
            if(to_return->next)to_return->next->previous=to_return->previous ;
        }
        node->attribute_values.removeFirst() ;
        node->records.removeFirst() ;
        return to_return ;
    }
    else
    {
        return remove_min_record_from_tree(node->children.first());
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
        root->attribute_values.append(record_node->record->getAttribute(target_attribute));
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
    else return search(root, attribute_value)->records[index_to_access] ;
}

bool BTreeIndex::update_record(QString current_value,QString attribute_to_change, QString new_value)
{
    TreeNode * searched_node=search(root, current_value) ;
    Table::List_node* destination_keynode= ( searched_node ? searched_node->records[index_to_access] : NULL) ;
    if(destination_keynode)
    {
        destination_keynode->record->setAttribute(target_attribute, new_value) ;
        return true ;
    }
    else return false ;
}

bool BTreeIndex::delete_record(QString attribute_value)
{
    TreeNode * searched_node=search(root, attribute_value) ;
    int searched_index=index_to_access ;
    if(!searched_node) return false ; // if node is not found return false
    
    if(searched_node->children.size()==0 && searched_node->attribute_values.size()>(order/2)-1) // leaf && no overflow
    {
        // simply remove key from the node case-I
        searched_node->attribute_values.removeAt(searched_index) ;
        Table::List_node * node_to_delete=searched_node->records[searched_index] ;
        if(this->type=="primary")
        {
            if(this->type=="primary")
            {
                if(node_to_delete->previous) node_to_delete->previous->next=node_to_delete->next ;
                if(node_to_delete->next)node_to_delete->next->previous=node_to_delete->previous ;
            }
            delete node_to_delete;
        }
        searched_node->records.removeAt(searched_index) ;
    }
    else if(searched_node->children.size()>0 && searched_node->attribute_values.size()>(order/2)-1) // non-leaf & no overflow
    {
        // case II

        int position_of_key=get_position_of_key_in_node(searched_node, attribute_value) ;
        TreeNode* left_subtree= (position_of_key==-1 ? NULL : searched_node->children[position_of_key]) ;
        TreeNode* right_subtree= (position_of_key==-1 ? NULL : searched_node->children[position_of_key+1]) ;

        if(left_subtree->attribute_values.size()>(order/2)-1) // case II-a
        {
            // replace max value of whole left_subtree  with searched key and remove key
            Table::List_node* max_record= remove_max_record_from_tree(left_subtree) ;
            Table::List_node* record_to_delete= searched_node->records[searched_index] ; ;
            if(this->type=="primary")
            {
                if(record_to_delete->previous) record_to_delete->previous->next=max_record ;
                if(record_to_delete->next) record_to_delete->next->previous=max_record ;
                delete record_to_delete ;
            }
            searched_node->records[searched_index]=max_record ;
            searched_node->attribute_values[searched_index]=max_record->record->getAttribute(target_attribute) ;
        }
        else if(right_subtree->attribute_values.size()>(order/2)-1) // case II-b
        {
            // replace max value of whole right_subtree node with searched key and remove key
            Table::List_node* min_record= remove_min_record_from_tree(right_subtree) ;
            Table::List_node* record_to_delete= searched_node->records[searched_index] ; ;
            if(this->type=="primary")
            {
                if(record_to_delete->previous) record_to_delete->previous->next=min_record ;
                if(record_to_delete->next) record_to_delete->next->previous=min_record ;
                delete record_to_delete ;
            }
            searched_node->records[searched_index]=min_record ;
            searched_node->attribute_values[searched_index]=min_record->record->getAttribute(target_attribute) ;
        }
        else // case II-c
        {
            // merge left_subtree node or right_subtree node with searched node & move key from searched node to this merged node and remove it
            TreeNode * new_node=new TreeNode ;
            for(int i=0; i<left_subtree->attribute_values.size(); i++) // copy the contents of the left child
            {
                new_node->attribute_values.append(left_subtree->attribute_values[i]) ;
                new_node->records.append(left_subtree->records[i]) ;
                new_node->children.append(left_subtree->children[i]) ;
            }
            new_node->children.append(left_subtree->children[left_subtree->attribute_values.size()]) ; // copy last children

            if(left_subtree->children.size()>0 && right_subtree->children.size()>0) // merging the rightmost subtree of left and leftmost subtree of right
            {
                TreeNode * rightmost_branch_left_subtree=left_subtree->children.last() ;
                TreeNode * leftmost_branch_right_subtree=right_subtree->children.first() ;

                for(int i=0; i<leftmost_branch_right_subtree->attribute_values.size(); i++)
                {
                    rightmost_branch_left_subtree->attribute_values.append(leftmost_branch_right_subtree->attribute_values[i]) ;
                    rightmost_branch_left_subtree->records.append(leftmost_branch_right_subtree->records[i]) ;
                    rightmost_branch_left_subtree->children.append(leftmost_branch_right_subtree->children[i]) ;
                }
                rightmost_branch_left_subtree->children.append(leftmost_branch_right_subtree->children[leftmost_branch_right_subtree->attribute_values.size()]) ; // copy last children
            }
            new_node->attribute_values.append(right_subtree->attribute_values[0]) ;
            new_node->records.append(right_subtree->records[0]) ;

            for(int i=1; i<right_subtree->attribute_values.size(); i++) // copy the contents of the right child : 0th item has already been copied : it was exception
            {
                new_node->attribute_values.append(right_subtree->attribute_values[i]) ;
                new_node->records.append(right_subtree->records[i]) ;
                new_node->children.append(right_subtree->children[i]) ;
            }

            searched_node->parent->attribute_values.removeAt(position_of_key) ;
            if(type=="primary")
            {
                delete searched_node->parent->records[position_of_key] ;
            }
            searched_node->parent->records.removeAt(position_of_key) ;
            delete searched_node->parent->children[position_of_key] ;
            searched_node->parent->children.removeAt(position_of_key) ;

            searched_node->parent->children[position_of_key]=new_node ;
        }
    }
    else
    {
        // case III
        // node containing key is root but key can't be removed from this node simply because removing will leave us with
        // less than (order/2)-1 keys
        int parent_key_position= get_position_of_parent_key(searched_node->parent, searched_node) ;
        int searched_key_position= get_position_of_key_in_node(searched_node, attribute_value) ;
        TreeNode * left_sibling ;
        TreeNode * right_sibling ;
        if(parent_key_position== searched_node->parent->attribute_values.size()-1)
        {
            right_sibling=NULL ;
            left_sibling=searched_node->parent->children[parent_key_position] ;
        }
        else if(parent_key_position==0)
        {
            left_sibling=NULL ;
            right_sibling=searched_node->parent->children[parent_key_position+1] ;
        }
        else
        {
            right_sibling=searched_node->parent->children[parent_key_position-1] ;
            left_sibling=searched_node->parent->children[parent_key_position+1] ;
        }

        if(left_sibling && left_sibling->attribute_values.size()>(order/2)-1)
        {
            // remove this key and rotate right

            searched_node->attribute_values[searched_key_position]=searched_node->parent->attribute_values[parent_key_position] ;
            searched_node->records[searched_key_position]=searched_node->parent->records[parent_key_position] ;

            Table::List_node* rightmost_value_left_subtree= remove_max_record_from_tree(left_sibling) ;

            searched_node->parent->attribute_values[parent_key_position]=rightmost_value_left_subtree->record->getAttribute(target_attribute) ;
            searched_node->parent->records[parent_key_position]=rightmost_value_left_subtree;
        }
        else if(right_sibling && right_sibling->attribute_values.size()>(order/2)-1)
        {
            // remove key and rotate left
            searched_node->attribute_values[searched_key_position]=searched_node->parent->attribute_values[parent_key_position] ;
            searched_node->records[searched_key_position]=searched_node->parent->records[parent_key_position] ;

            Table::List_node* leftmost_value_right_subtree= remove_min_record_from_tree(right_sibling) ;

            searched_node->parent->attribute_values[parent_key_position]=leftmost_value_right_subtree->record->getAttribute(target_attribute) ;
            searched_node->parent->records[parent_key_position]=leftmost_value_right_subtree;
        }
        else
        {
            // merge searched node with any of its siblings along with their common parent
            // this will create node with 2((order/2)-1)+1 keys. Now remove the searched key and we get order-2 keys.
            TreeNode* new_node;
            bool merged_with_left_sibling=0 ,merged_with_right_sibling=0 ;
            if(left_sibling)
            {
                merged_with_left_sibling=true ;
                new_node=new TreeNode ;

                // merge with left sibling

                for(int i=0; i< left_sibling->attribute_values.size(); i++)
                {
                    new_node->attribute_values.append(left_sibling->attribute_values[i]) ;
                    new_node->records.append(left_sibling->records[i]) ;
                }
                new_node->attribute_values.append(searched_node->parent->attribute_values[parent_key_position]) ;
                new_node->records.append(searched_node->parent->records[parent_key_position]) ;
                for(int i=0; i< searched_node->attribute_values.size(); i++)
                {
                    if(i==searched_key_position)
                    {
                        new_node->attribute_values.append(searched_node->attribute_values[i]) ;
                        new_node->records.append(searched_node->records[i]) ;
                    }
                }
            }
            else
            {
                merged_with_right_sibling=true ;
                new_node=new TreeNode ;

                //merge with right sibling

                for(int i=0; i< right_sibling->attribute_values.size(); i++)
                {
                    new_node->attribute_values.append(right_sibling->attribute_values[i]) ;
                    new_node->records.append(right_sibling->records[i]) ;
                }
                new_node->attribute_values.append(searched_node->parent->attribute_values[parent_key_position]) ;
                new_node->records.append(searched_node->parent->records[parent_key_position]) ;
                for(int i=0; i< searched_node->attribute_values.size(); i++)
                {
                    if(i!=searched_key_position)
                    {
                        new_node->attribute_values.append(searched_node->attribute_values[i]) ;
                        new_node->records.append(searched_node->records[i]) ;
                    }
                    else
                    {
                        if(type=="primary")
                        {
                            delete searched_node->records[i] ;
                        }
                    }
                }
            }
            int parent_key_merged= (merged_with_left_sibling ? parent_key_position-1 : parent_key_position);
            searched_node->parent->attribute_values.removeAt(parent_key_merged) ;
            searched_node->parent->records.removeAt(parent_key_merged) ;
            delete searched_node->parent->children[parent_key_merged] ;
            searched_node->parent->children[parent_key_merged]=new_node ;
        }
    }

}
