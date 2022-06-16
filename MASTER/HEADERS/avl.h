#pragma once

#include "../HEADERS/patient.h"

typedef struct TreeNode                 //Komvos dentrou,periexei asthenh
{  
    struct TreeNode *left; 
    struct TreeNode *right; 
    int height; 
    Patient *treepatient;
}TreeNode;

typedef struct Tree                     //Dentro, exei riza
{
    TreeNode* root;
}Tree;

int height(TreeNode*);                  //upsos dentrou
int max(int,int);                       //bohthitikh sunarthsh
int getBalance(TreeNode*);              //sunarthsh gia rotations

void printTree(TreeNode*);              //ektupwsh
void deleteTree(TreeNode*);             //diagrafh
void insertTree(Tree*,Patient*);        //eisagwgh komvou sto dentro

Tree* initTree();                       //arxikopoihsh

TreeNode* newNode(Patient*);            //neos komvos
TreeNode* insertNode(TreeNode**,TreeNode*); //eisagwgh me katallhla rotations(auth kaloume sto insertTree)


TreeNode *rightRotate(TreeNode*);       //rotate
TreeNode *leftRotate(TreeNode*);        //rotate

int searchTree(TreeNode*);              //posoi komvoi einai to dentro
int searchTreeExit(TreeNode*,char*);    //posoi komvoi tou dentrou exoun - gia exit
