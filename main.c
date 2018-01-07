#include <stdio.h>
#include <stdlib.h>

struct node {
  int value;
  struct node * left;
  struct node * right;
  int children;
};

void treeInsert( struct node* this, struct node* new){//this is the line that gives the error

  this->children++;

  if ( new->value < this->value ){//this will insert it into the left one
    if (this->left == NULL){
      this->left = new;
    } else {
      treeInsert( this->left, new);
    }
  } else {//this will insert it into the right one
    if (this->right == NULL){
      this->right = new;
    } else {
      treeInsert( this->right, new);
    }
  }

}

struct node* findValueFromTree( struct node* this, int value){

  if (this->value == value) return this;
  else if (value > this->value){
    if (this->right == NULL) return 0;
    return findValueFromTree(this->right, value);
  }
  else {
    if (this->left == NULL) return 0;
    return findValueFromTree(this->left, value);
  }
}

int main(){
  struct node head = { 10, NULL, NULL, 0 };
  struct node new = { 5, NULL, NULL, 0 };
  struct node bottom = { 2, NULL, NULL, 0 };
  treeInsert(&head, &new);
  treeInsert(&head, &bottom);
  
  struct node finding = *findValueFromTree( &head, 10);
  printf("%d\n", finding.children);
}
