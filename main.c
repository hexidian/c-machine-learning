#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void printArray(int *A, int length){
  for (int i = 0; i < length; i++){
    printf("%d\n", A[i]);
  }
}

void printFloatArray(float *A, int length){
  for (int i = 0; i < length; i++){
    printf("%f\n", A[i]);
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

void treeInsert( struct node* this, struct node* new){

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

struct list {
  int value;
  struct list* next;
  int length;
};

void append(struct list* head, struct list* new){
  head->length++;
  if (head->next == NULL) head->next = new;
  else append(head->next, new);
}

struct list* element( struct list* head, int elementID){
  if (elementID == 0) return head;
  return element( head->next, elementID-1);
}

struct hillClimber {

  int* values;//an array of values
  int length;
  float (*evaluate)(int* testValues, int length);

};

void hillClimb(struct hillClimber* this){
  int* values = this->values;
  int length = this->length;
  int best[3] = {0,0,0}; // in order: value, element, changed by
  int prevValue;
  int newValue;
  int evaluation;
  for (int i = 0; i < length; i++){
    prevValue = values[i];
    for (int changeValue = -1; changeValue < 2; changeValue += 2){
      newValue = prevValue + changeValue;
      values[i] = newValue;
      evaluation = (*(this->evaluate))(values, length);
      if (evaluation > best[0]){
        best[0] = evaluation; best[1] = i; best[2] = changeValue;
      }
    }
    values[i] = prevValue;
  }
  values[best[1]] += best[2];
}

float evaluation(int values[],int length){
  float value = 100;
  for (int i = 0; i < length; i++){
    value -= pow(values[i]-2,2);
  }
  return value;
}

struct tuple {

  float a;
  float b;

};

struct neuron {

  float* omegas;
  float sigma;

  float (*function) (float* omegas, float* input, float sigma);

  //IMPORTANT NOTE: the inputArray will be formatted in different ways depending on the funciton. for example:
    //in a linear funciton it will be formated [x1, y1, x2, y2, x3, y3 ...] and the length will represent the number of coordinate pairs times 2
  void (*backProp) (float* inputArray, int inputLength, struct neuron* self);//the length of the omega array will depend strictly on the function itself, so that does not need to be input

};

void linearErrorFunc(float* inputArray, int inputLength, struct neuron* self){
  //NOTE: in this case we have only 1 input node, so there is only 1 omega

  float omegaError = 0;
  float sigmaError = 0;
  float fx;
  for (int i = 0; i < inputLength; i+=2){
    fx = (self->function)(self->omegas, &inputArray[i], self->sigma);
    omegaError += (inputArray[ i + 1 ] - fx) * inputArray[i];  //omegaError += (y-f(x)) / x
    sigmaError += inputArray[ i + 1 ] - fx;
  }

  //we need to halve these values so that it doesn't rubberband arround the target out of control
  self->sigma += sigmaError/inputLength;
  self->omegas[0] += omegaError/inputLength; //in this case we can make the shortcut of treating it as a single number, because there is only 1 omega in a linear function

}

float linearNeuronFunc (float* omegas, float* input, float sigma) {
  float m = omegas[0];
  float b = sigma;
  float x = input[0];
  return (m*x) + b;
}

float quadraticNeuronFunc (float* omegas, float* input, float sigma) {
  float a = omegas[0];
  float b = omegas[1];
  float c = sigma;
  float x = input[0];
  return a*powf(x,2) + b*x + c;//just ax^2 + bx + c
}

void quadraticErrorFunc(float* inputArray, int inputLength, struct neuron* self) {
  float aError = 0;//the first omega
  float bError = 0;//the second omega
  float cError = 0;//the sigma error
  float fx;
  float error;//just the general error of a signle step
  for (int i = 0; i < inputLength; i+=2) {
    fx = (self->function)(self->omegas, &inputArray[i], self->sigma);
    error = inputArray[i + 1] - fx;
    aError += error * powf(inputArray[i],2);
    bError += error * inputArray[i];
    cError += error;
  }

  self->sigma += cError / (inputLength*2);
  self->omegas[0] += aError / (inputLength*2);
  self->omegas[1] += bError / (inputLength*2);
}

void polynomErrorFunc (float* inputArray, int inputLength, struct neuron* self) {
  float n = inputArray[0];
  float omegaErrors[(int)n];
  float sigmaError = 0;
  float fx;
  float funcInput[2]; funcInput[0] = n;
  for (int i = 0; i < n; i++) {omegaErrors[i] = 0;}//fills the omegaErrors array with 0s
  for (int i = 1; i < inputLength; i+=2){
    funcInput[1] = inputArray[i];
    fx = (self->function)(self->omegas, funcInput, self->sigma);
    for (int j = 0; j < n; j++){
      omegaErrors[j] += (inputArray[i+1] - fx) * powf(inputArray[i],n-j);
    }
    sigmaError += inputArray[i+1] - fx;
  }
  self->sigma += sigmaError / (pow(inputLength,2)*4);
  for (int i = 0; i < n; i++){
    (self->omegas)[i] += omegaErrors[i] / (pow(inputLength,2*n));
  }
}

float polynomNeuronFunc (float* omegas, float* input, float sigma){
  //for the polynomial funciton, the input will be formatted as follows:
  //[n,x] where n is the greatest highest exponent able to be used (also the length of the omegas)
  float n = input[0];//n does have to be a float, in order for the data types to match up
  float x = input[1];
  float final = sigma;
  for (int i = 0; i < n; i++){
    final += powf(x,n-i) * omegas[i];
  }
  return final;
}

int main(){
  /*
  struct node head = { 100, NULL, NULL, 0 };
  struct node nodeArray[99];
  for (int i = 0; i < 99; i++){
    nodeArray[i].value = i; nodeArray[i].left = NULL; nodeArray[i].right = NULL; nodeArray[i].children = 0;
    treeInsert(&head,&nodeArray[i]);
  }
  struct node* newhead = organiseTree(&head);
  printTree(newhead,0);
  */
  /*
  struct list newList = {10, NULL, 1};
  struct list elements[10000];
  for (int i = 0; i < 10000; i++){
    elements[i].next = NULL; elements[i].value = i; elements[i].length = 1;
    append(&newList,&elements[i]);
  }
  struct list* holder;
  clock_t before = clock();
  for (int j = 0; j < 1000; j++){
    holder = &newList;
    for (int i = 0; i < newList.length; i++){
      holder = holder->next;
    }
  }
  float cps = CLOCKS_PER_SEC;
  printf("that took %f seconds\n",(clock()-before)/cps);
  */
  /*
  struct hillClimber tester;
  int values[3] = {1,2,3};
  tester.values = values;
  tester.length = 3;
  tester.evaluate = *evaluation;
  for (int i = 0; i < 100; i++){
    hillClimb(&tester);
  }
  printArray(tester.values,3);
  */
  /*
  struct neuron lin;
  float weights[1] = {0};
  lin.omegas = weights;
  lin.sigma = 0;
  lin.function = &linearNeuronFunc;
  lin.backProp = &linearErrorFunc;
  float input[8] = {1,2,2,3,3,5,4,4};
  for (int i = 0; i < 10000; i++){
    (lin.backProp)(input, 4, &lin);
  }
  printf("omega is now: %f\nsigma is now: %f\n",lin.omegas[0],lin.sigma);
  */
  /*
  struct neuron quad;
  float weights[2] = {0,0};
  quad.omegas = weights;
  quad.sigma = 0;
  quad.function = &quadraticNeuronFunc;
  quad.backProp = &quadraticErrorFunc;
  float input[6] = {0,3,1,6,2,11};
  for (int i = 0; i < 10000; i++){
    (quad.backProp)(input, 6, &quad);
  }
  printf("a is now: %f\nb is now: %f\nc is now: %f\n",quad.omegas[0],quad.omegas[1],quad.sigma);
  */
  /*
  struct neuron poly;
  float weights[4] = {0,0,0,0};
  poly.omegas = weights;
  poly.sigma = 0;
  poly.function = &polynomNeuronFunc;
  poly.backProp = &polynomErrorFunc;
  float input[25] = {4,-10,50,-9,40,-8,30,-5,10,-3,0,-4,0,0,10,1,20,3,25,7,10,9,-4,12,-6};

  for (int i = 0; i < 1000000; i++){
    (poly.backProp)(input, 25, &poly);
  }
  printf("a is now: %f\nb is now: %f\nc is now: %f\nd is now: %f\ne is now: %f\n",poly.omegas[0],poly.omegas[1],poly.omegas[2],poly.omegas[3],poly.sigma);//TODO have this then run python code which will use matplotlib to graph the points and the function

  int fileWriteNumbers = (25)+5;
  float numbersForFileWrite[fileWriteNumbers];
  for (int i = 0; i < fileWriteNumbers-5; i++){
    numbersForFileWrite[i] = input[i];
  }
  for (int i = fileWriteNumbers-5; i < fileWriteNumbers-1; i++){
    numbersForFileWrite[i] = poly.omegas[5-(fileWriteNumbers-i)];
  }
  numbersForFileWrite[fileWriteNumbers-1] = poly.sigma;

  FILE * fp;
  fp = fopen("output.txt","w");
  for (int i = 0; i < fileWriteNumbers; i++){
    fprintf(fp,"%f\n",numbersForFileWrite[i]);
  }
  fclose(fp);
  system("python grapher.py");
  */
}
