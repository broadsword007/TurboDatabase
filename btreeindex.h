#ifndef BTREEINDEX_H
#define BTREEINDEX_H
#include <QString>
#include "index.h"
#include <iostream>
class BTreeIndex : public Index
{
    struct TreeNode
    {
        QList<Table::List_node*> records ;
        QList<QString> attribute_values ;
        TreeNode* parent ;
        QList<TreeNode*> children ;
    };
    int index_to_access ; // sometime we are returning a node(e.g. in search) but we have to specify the target key number as well
    int order;
    TreeNode * root ;
    TreeNode* search(TreeNode * node, QString attribute_value);
    void insert(TreeNode* node,Table::List_node* record_node) ;
    void delete_node(QString attribute);

    void split(TreeNode* node) ;
    void bubble_up(TreeNode* parent, TreeNode* node) ;

    int get_position_of_key_in_node(TreeNode* node, QString key) ;
    int get_position_of_parent_key(TreeNode* parent_node, TreeNode* child_node);
    QString get_max_val_of_tree(TreeNode* node) ;
    QString get_min_val_of_tree(TreeNode* node) ;

    Table::List_node* remove_max_record_from_tree(TreeNode* node);
    Table::List_node* remove_min_record_from_tree(TreeNode* node);
public:
    BTreeIndex(int val_order, QString val_type, QString val_target_attribute);
    void construct_index(Table::List_node* first_node) ;
    void add_record(Table::List_node* record_node) ;
    Table::List_node* find_record(QString attribute_value);
    bool update_record(QString current_value,QString attribute_to_change, QString new_value);
    bool delete_record(QString attribute_value);
};

#endif // BTREEINDEX_H
