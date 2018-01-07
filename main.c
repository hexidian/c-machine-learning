#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printArray(int *A, int length){
  for (int i = 0; i < length; i++){
    printf("%d\n", A[i]);
  }
}

void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
	int i,j,k;

	// i - to mark the index of left aubarray (L)
	// j - to mark the index of right sub-raay (R)
	// k - to mark the index of merged subarray (A)
	i = 0; j = 0; k =0;

	while(i<leftCount && j< rightCount) {
		if(L[i]  < R[j])
    {
      A[k++] = L[i++];
    } else {
      A[k++] = R[j++];
    }
	}
	while(i < leftCount)  {A[k++] = L[i++];}
	while(j < rightCount) {A[k++] = R[j++];}
}

// Recursive function to sort an array of integers.
void MergeSort(int *A,int n) {
	int mid,i, *L, *R;
	if(n < 2){return;} // base condition. If the array has less than two element, do nothing.

	mid = n/2;  // find the mid index.

	// create left and right subarrays
	// mid elements (from index 0 till mid-1) should be part of left sub-array
	// and (n-mid) elements (from mid to n-1) will be part of right sub-array
	L = (int*)malloc(mid*sizeof(int));
	R = (int*)malloc((n- mid)*sizeof(int));

	for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
	for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

	MergeSort(L,mid);  // sorting the left subarray
	MergeSort(R,n-mid);  // sorting the right subarray
	Merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.

  free(L);//we no longer need to reserve the memory for L and R
  free(R);
}

struct node {
  int value;
  struct node * left;
  struct node * right;
  int children;
};

void printTree(struct node* head, int depth){
  for (int i = 0; i <depth; i++){
    printf("  ");
  }
  printf("%d\n", head->value);
  if (head->left != NULL) printTree(head->left, depth + 1);
  if (head->right != NULL) printTree(head->right, depth +1);
}

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

void getAllTreeValues( struct node* this, int* values, int *upToIndex){

  values[*upToIndex] = this->value;
  (*upToIndex)++;
  if (this->left != NULL) getAllTreeValues(this->left, values, upToIndex);
  if (this->right != NULL) getAllTreeValues(this->right, values, upToIndex);

}

void getSortedTreeValues( struct node* this, struct node* values[], int* upToIndex){
  if (this->left != NULL) getSortedTreeValues(this->left, values, upToIndex);
  values[(*upToIndex)] = this;
  (*upToIndex)++;
  if (this->right != NULL) getSortedTreeValues(this->right, values, upToIndex);
}

void fillTree( struct node* nodes[], int headIndex, int min, int max){
  if (min < headIndex && max > headIndex){//the normal situtation
    nodes[headIndex]->left = nodes[ headIndex - ((headIndex - min) / 2 + 1) ];
    nodes[headIndex]->right = nodes[ headIndex + ((max - headIndex) / 2 + 1) ];
    fillTree(nodes, headIndex - ((headIndex - min) / 2 + 1), min, headIndex-1);
    fillTree(nodes, headIndex + ((max - headIndex) / 2 + 1), headIndex+1, max);
  } else if (min < headIndex) {//if we have hit the right bound
    nodes[headIndex]->left = nodes[ headIndex - ((headIndex - min) / 2 + 1) ];
    fillTree(nodes, headIndex - ((headIndex - min) / 2 + 1), min, headIndex-1);
    nodes[headIndex]->right = NULL;
  } else if (max > headIndex) {//if we have hit the left bound
    nodes[headIndex]->right = nodes[ headIndex + ((max - headIndex) / 2 + 1) ];
    fillTree(nodes, headIndex + ((max - headIndex) / 2 + 1), headIndex+1, max);
    nodes[headIndex]->left = NULL;//erase any old pointers
  } else {
    nodes[headIndex]->left = NULL;
    nodes[headIndex]->right = NULL;
  }
}

struct node* organiseTree( struct node* head){
  int size = head->children +1;
  struct node* nodes[size];
  int* index = (int *) malloc(2);//the number of bytes in an int
  *index = 0;
  getSortedTreeValues(head, nodes, index);
  fillTree( nodes, (size-1)/2, 0, size - 1);
  return (nodes[(size-1)/2]);
}

int main(){
  struct node head = { 100, NULL, NULL, 0 };
  struct node nodeArray[99];
  for (int i = 0; i < 99; i++){
    nodeArray[i].value = i; nodeArray[i].left = NULL; nodeArray[i].right = NULL; nodeArray[i].children = 0;
    treeInsert(&head,&nodeArray[i]);
  }
  struct node* newhead = organiseTree(&head);
  printTree(newhead,0);
}
