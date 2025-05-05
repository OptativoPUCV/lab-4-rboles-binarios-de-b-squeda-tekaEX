#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {


    TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;

    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* node = tree->root;
    TreeNode* parent = NULL;

    while (node!=NULL)
    {
        parent = node;
        if (tree->lower_than(key,node->pair->key)){
            node = node->left;
        }
        else if(tree->lower_than(node->pair->key, key)){
            node = node->right;
        }
        else
            return;
    }
    
    TreeNode *newNode = createTreeNode(key,value);
    newNode->parent = parent;

    if (parent == NULL){
        tree->root = newNode;
    }
    else if (tree->lower_than(key, parent->pair->key)){
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL){
        return;
    }

    if(node->left && node->right){
        TreeNode *next = minimum(node->right);
        node->pair->key = next->pair->key;
        node->pair->value = next->pair->value;
        removeNode(tree,next);
    }
    else{
        TreeNode *hijo;
        if(node->left){
            hijo = node->left;
        }
        else{
            hijo = node -> right;
        }
    

        if (node->parent){
            if(node == node->parent->left){
            node->parent->left = hijo;
            }
        }
        else{
            node->parent->right =hijo;
        }

        if(hijo){
            hijo->parent = node->parent;
        }
        free(node);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || key == NULL) return NULL;

    TreeNode* node = tree->root;

    while (node != NULL){
        if (tree->lower_than(key,node->pair->key)){
            node = node->left;
        }
        else if(tree->lower_than(node->pair->key, key)){
            node = node->right;
        }
        else{
            tree->current = node;
            return node->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if(!tree)return NULL;
    TreeNode *node = tree->root;
    TreeNode *ub_node = NULL;
    while (node)
    {
        if(tree->lower_than(node->pair->key,key)){
            node = node ->right;
        }
        else{
            ub_node = node;
            node = node->left;
        }
    }
    if (!ub_node) return NULL;
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (!tree || !tree->root) return NULL;
    TreeNode *node = minimum(tree->root);
    tree->current = node;
    return node ? node->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (!tree || !tree->current) return NULL;
    TreeNode *node = tree->current;

    if (node->right) {
        node = minimum(node->right);
    } else {
        TreeNode *parent = node->parent;
        //iteracion hasta salir de la profundidad actual del nodo
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        node = parent;
    }

    tree->current = node;
    if (!node) return NULL;
    return node->pair;
}