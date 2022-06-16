#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include "../HEADERS/avl.h"
#include "../HEADERS/date.h"

  
int height(TreeNode *treenode) 
{ 
    if (treenode == NULL) 
        return 0; 
    return treenode->height; 
} 
  
int max(int a, int b) 
{ 
    return (a > b)? a : b; 
} 

int getBalance(TreeNode *treenode) 
{ 
    if (treenode == NULL) 
        return 0; 
    return height(treenode->left) - height(treenode->right); 
}

void printTree(TreeNode* root)
{
    if(root ==NULL)
    {
        return;
    }
    else
    {
        printTree(root->left);
        printf("\t\t%s \n",root->treepatient->entryDate);
        printTree(root->right);     
    }
}

void deleteTree(TreeNode* treenode)
{
    if(treenode == NULL)
    {
        return;
    }
    else
    {
        if(treenode->left !=NULL)
            deleteTree(treenode->left);
        if(treenode->right !=NULL)
            deleteTree(treenode->right);
        treenode->left = NULL;
        treenode->right = NULL;
        treenode->treepatient = NULL;
        free(treenode);
    }
}

void insertTree(Tree* tree, Patient* patient)
{
    TreeNode* treenode = newNode(patient);
    insertNode(&(tree->root),treenode);
}

Tree* initTree ()
{
    Tree* newtree = malloc(sizeof(Tree));
    newtree->root = NULL;
    return(newtree);
}

TreeNode* newNode(Patient* patient) 
{ 
    TreeNode* node = malloc(sizeof(TreeNode)); 

    node->treepatient = patient;
    node->left   = NULL; 
    node->right  = NULL; 
    node->height = 1;  //leaf 
    return(node); 
} 


TreeNode* insertNode(TreeNode** root, TreeNode* treenode)
{
    if(*root == NULL)               //an h riza einai kenh,bazoume ton komvo edw
    {
        *root = treenode;
        (*root)->right = NULL;
        (*root)->left = NULL;
    }
    else                              //an h eiza den einai kenh
    {
        //elexgos twn dates gia na kathorisoume se poio upodentro tha mpei
        //aristera oi mikroteres h ises
        //deksia oi megaluteres
        int compare = compare_dates((*root)->treepatient->entryDate,treenode->treepatient->entryDate);
        if( compare == 1)
        {
            insertNode(&((*root)->left),treenode);
        }
        else if ( compare ==-1)
        {
            insertNode(&((*root)->right),treenode);
        }
        else
        {
            insertNode(&((*root)->left),treenode);
        }
    }
        //kanoume allagh tou height kai tsekaroume to balance gia 1,0,-1
    treenode->height = 1 + max(height(treenode->left), height(treenode->right)); 
    int balance = getBalance(treenode);

    //Rotates
        // Left Left    
        if (balance > 1 && (compare_dates((*root)->left->treepatient->entryDate,treenode->treepatient->entryDate)) == 1 ) 
            return rightRotate(treenode); 
    
        // Right Right
        if (balance < -1 &&  (compare_dates((*root)->right->treepatient->entryDate,treenode->treepatient->entryDate)) == -1) 
            return leftRotate(treenode); 
    
        // Left Right
        if (balance > 1 && (compare_dates((*root)->left->treepatient->entryDate,treenode->treepatient->entryDate)) == -1 ) 
        { 
            treenode->left =  leftRotate(treenode->left); 
            return rightRotate(treenode); 
        } 
    
        // Right Left 
        if (balance < -1 && (compare_dates((*root)->right->treepatient->entryDate,treenode->treepatient->entryDate)) == 1 ) 
        { 
            treenode->right = rightRotate(treenode->right); 
            return leftRotate(treenode); 
        } 
    return treenode;
}

TreeNode *rightRotate(TreeNode *new_node) 
{ 
    TreeNode *node1 = new_node->left; 
    TreeNode *node2 = node1->right; 
  
    // Perform rotation 
    node1->right = new_node; 
    new_node->left = node2; 
  
    // Update heights 
    new_node->height = max(height(new_node->left), height(new_node->right))+1; 
    node1->height = max(height(node1->left), height(node1->right))+1; 
    free(node2);
    // Return new root 
    return node1; 
} 
   
TreeNode *leftRotate(TreeNode *new_node) 
{ 
    TreeNode *node1 = new_node->right; 
    TreeNode *node2 = node1->left; 
  
    node1->left = new_node; 
    new_node->right = node2; 
  
    new_node->height = max(height(new_node->left), height(new_node->right))+1; 
    node1->height = max(height(node1->left), height(node1->right))+1; 
    free(node2);

    return node1; 
} 
  
int searchTree(TreeNode* treenode)
{
    int c=0;
    if(treenode == NULL)
        return 0;
    else
    {   
        c+=searchTree(treenode->right);
        c+=searchTree(treenode->left);
        c++;
    }
    return c;
}

int searchTreeExit(TreeNode* treenode,char* exit)
{
    int entries=0;
    if(treenode == NULL)
        return 0;
    else
    {
        searchTreeExit(treenode->left,exit);
        searchTreeExit(treenode->right,exit);
        if(strcmp(treenode->treepatient->exitDate,exit)==0)
            entries++;
    }
    return entries;
}