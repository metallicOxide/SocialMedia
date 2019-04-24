#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>


ItemPQ newItemPQ(int a, int b){

  ItemPQ p ;
  p.key = a;
  p.value = b;
  return p;
}


int main(){

  PQ pq = newPQ();
  printf("\naddPQ: {1,100}\n");
  addPQ(pq,newItemPQ(1,100) );
  printf("addPQ: {2,15}\n");
  addPQ(pq,newItemPQ(2,15));
  printf("addPQ: {3,1}\n");
  addPQ(pq,newItemPQ(3,1));
  printf("addPQ: {4,4}\n");  
  addPQ(pq,newItemPQ(4,4));
  printf("addPQ: {5,3}\n");  
  addPQ(pq,newItemPQ(5,3));

  showPQ(pq);
  
  ItemPQ v; 
  for(int i=0; i<5; i++){  
      v = dequeuePQ(pq);
      printf("Dequeued: {%d,%d}\n",v.key,v.value);
      // showPQ(pq);
  }

  printf("addPQ: {2,12}\n");  
  addPQ(pq,newItemPQ(2,12) );
  printf("addPQ: {4,8}\n");  
  addPQ(pq,newItemPQ(4,8));
  printf("addPQ: {3,10}\n");  
  addPQ(pq,newItemPQ(3,10));
  showPQ(pq);
  
  printf("Update {4,8} to {4,11}\n");
  updatePQ(pq, newItemPQ(4,11));
  showPQ(pq);

  printf("addPQ: {3,12}\n"); 
  addPQ(pq,newItemPQ(3,12));
  printf("state after adding {3,12}\n");
  showPQ(pq);

  printf("addPQ: {3,10}\n"); 
  addPQ(pq, newItemPQ(3,10));

  printf("state after adding {3,10}\n");
  showPQ(pq);

  printf("addPQ: {5,11}\n"); 
  addPQ(pq, newItemPQ(5,11));

  printf("state after adding {5,11}\n");
  showPQ(pq);

  printf("addPQ: {7,12}\n"); 
  addPQ(pq, newItemPQ(7,12));

  printf("state after adding {7,12}\n");
  showPQ(pq);

  printf("addPQ: {2,1}\n"); 
  addPQ(pq, newItemPQ(2,1));

  printf("state after adding {2,1}\n");
  showPQ(pq);

  printf("addPQ: {4,2}\n"); 
  addPQ(pq, newItemPQ(4,2));

  printf("state after adding {4,2}\n");
  showPQ(pq);

  for(int i=0; i<6; i++){  
      v = dequeuePQ(pq);
      printf("Dequeued: {%d,%d}\n",v.key,v.value);
  }  
  
  freePQ(pq);
  printf("\n");
  
  return EXIT_SUCCESS;
}
