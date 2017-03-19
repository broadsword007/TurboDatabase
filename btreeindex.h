#ifndef BTREEINDEX_H
#define BTREEINDEX_H
#include <QString>
#include "index.h"
class BTreeIndex : public Index
{
    struct TreeNode
    {
        QList<Table::List_node*> records ;
        QList<QString> attribute_values ;
        TreeNode* parent ;
        QList<TreeNode*> children ;
    };
    int order;
    TreeNode * root ;
    Table::List_node* search(TreeNode * node, QString attribute_value);
    void insert(TreeNode* node,Table::List_node* record_node) ;
    void delete_node(QString attribute);

    void split(TreeNode* node) ;
    void bubble_up(TreeNode* parent, TreeNode* node) ;
public:
    BTreeIndex(int val_order, QString val_type, QString val_target_attribute);
    void construct_index(Table::List_node* first_node) ;
    void add_record(Table::List_node* record_node) ;
    Table::List_node* find_record(QString attribute_value);
    bool update_record(QString current_value,QString attribute_to_change, QString new_value);
    bool delete_record(QString attribute_value);
};

#endif // BTREEINDEX_H
