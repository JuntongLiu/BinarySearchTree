/* 
 * 
 *
 * This file is a test implementation of a binary search tree and the corresponding tree 
 * manipulation functions, that include:
 * 
 *  1.) binary tree node structure.
 *  2.) function to insert node into the tree.
 *  3.) function to find a given node's closest node.
 *  4.) function to print all node's value in decent order.
 *  5.) function to compare and return a closest node to a given node.
 *  6.) function to save all nodes of the tree into a list.
 *  7.) function to find a given node.
 *  8.) function to convert, sort and print a list.
 *  9.) function to delete a given node.
 *  10.) function to find the leave node which has a key closest to a given key.
 *  11.) function to find a node's height in the tree.
 *
 *
 *  Note, this is a test implementation. The code is not tested. To use it is at your own risk. 
 *  The codes have neither been tested or optimized yet.
 *
 *
 *
 *
 *                                            Juntong Liu 
 *                                                                    2006.10.16
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * Structure for a node in the binary search tree.
 *  
 */
typedef struct _BSTNode
{
   int iValue;
   struct _BSTNode *pNext;
   struct _BSTNode *pPrev;
} BSTNode;


/* unsigned int iValue_diff; */   /* used for search for closest */

struct node_list           /* for printing the tree's values */
{
  BSTNode *head;
  BSTNode *tail;
  int num_nodes;
}node_list = {NULL, NULL, 0};


/* 
 * declare the root of a binary space partition tree.
 *                            
 *
 *                             r          r --- root node
 *                           /  \ 
 *                          p    n        p --- left child node
 *                         /\    /\
 *                        p  n  p  n      n --- right child node
 *                       .............
 */
BSTNode *pRoot = NULL;


/* function prototypes. We pass a pointer of a root node to functions in case that we need to run several tree at the same time */
void insert_node(BSTNode **pRoot, BSTNode *pNode);
int btree_delete_node(BSTNode *pRoot, int key);
BSTNode *find_closest_node(BSTNode *, unsigned int *, int);
BSTNode *btree_find_node_height(BSTNode *pRoot, BSTNode *node, unsigned int *height );
BSTNode *btree_find_node(BSTNode *pNode, int key, unsigned int *height);
void print_nodes(BSTNode *pRoot);
static void compare_nodes(BSTNode *node, BSTNode **closest, unsigned int *diff, unsigned int iValue);
static void save_to_list(BSTNode *pRoot);
static void print_list(void);
static inline int is_left(BSTNode *base, BSTNode *new);
//static void btree_find_closest_leave(BSTNode *start_node, BSTNode **closest_node, int *diff, int key);
//static BSTNode *find_closer_node(BSTNode *start_node, BSTNode *node, unsigned int *height, unsigned int *flag);

/* 
 * Inserts a node into the BST tree.
 *
 * pRoot - The root of the BST Tree.
 * pNode - The new node to be inserted into the tree.
 * 
 */
void insert_node(BSTNode **pRoot, BSTNode *pNode)
{
  // if this sub tree is empty, hook it as sub-root 
  if(*pRoot == NULL)
  {
    *pRoot = pNode;
    return;
  }
  else
  {       
    // sub-tree is not empty, hook it onto the tree according it's value 
    if((*pRoot)->iValue > pNode->iValue)
      insert_node(&(*pRoot)->pPrev, pNode);
    else
        insert_node(&(*pRoot)->pNext, pNode);
  }
}



/*
 * Find the closest node in the BST tree.
 *
 * pRoot       - The root of the BST tree.
 * puiDistance - Pointer to an unsigned int where the absolute 
 *                    distance to the closest node will be written.
 * iValue      - The function returns the node in the tree with the 
 *               closest absolute distance to (iValue).
 *
 * return      - The node with the value closest to the provided value.
 */
BSTNode *find_closest_node(BSTNode *pRoot, unsigned int *puiDistance, int iValue)
{
 
  BSTNode *target;
  unsigned int height = 0;

  unsigned int diff_left, diff_right;
  BSTNode *closest_left, *closest_right, *closest_node;
  
  closest_left = closest_right = NULL;

  if(pRoot == NULL)
    return NULL;
  
  diff_left = diff_right = abs(pRoot->iValue - iValue);

  if(pRoot->pPrev)                                                 // walk the left sub-tree and find the closest node on this subtree 
    compare_nodes(pRoot->pPrev, &closest_left, &diff_left, iValue);

  if(pRoot->pNext)                                                 // walk the right subtree and find the cloaset node on this subtree
  {
    compare_nodes(pRoot->pNext, &closest_right, &diff_right, iValue);
  }
  
  printf("diff_right = %d, diff_left=%d\n", diff_right, diff_left);
  
  if(closest_left && closest_right)                                // if both branch are successful, 
  {
    if(abs(iValue - closest_left->iValue) < abs(iValue - closest_right->iValue))   // compare the two and find the smallest
      closest_node = closest_left;
    else
      closest_node = closest_right;
  }
  else if(closest_left)                // if only has one branch
  {
    closest_node = closest_left;       // closest must be on it
  }
  else if(closest_right)
  {
    closest_node = closest_right;
  }
  else if(pRoot)                      //  if only has a root node
  {
    closest_node = pRoot;
  }
  else
    closest_node = NULL;
  
  /*start_node = pRoot;*/
  //if(closest_node == pRoot)
  //  height = 0;

  if(closest_node != NULL)
  {
#if 0        /* this cost little more */
    for(;;){
      target = find_closer_node(start_node, closest_node, &height, &flag); 
      if(flag == 1){
	*puiDistance = height;
	return target;
      }
      start_node = target;
    }
#endif 
    target = btree_find_node_height(pRoot, closest_node, &height);
    if(target == NULL)
          printf(" btree_find_node_height() error!\n");
  }
  else
  {
    closest_node  = NULL;
    height = 0;
  };

  *puiDistance = height;
  
  printf("The height is: %d \n", height);
  
  return closest_node;

}



/*
 * Print the value of all nodes in the tree in descending order.
 *
 * pRoot - The root node.
 */
void print_nodes(BSTNode *pRoot)
{
  save_to_list(pRoot);
  print_list();  

}



/* 
 *  Function: save_to_list
 *  Purpose:  save all the nodes' value into a list to prepare to print it out.
 *  Input:    pRoot  --- tree root
 *            number of nodes on tree.
 *
 */
static void save_to_list(BSTNode *pRoot)
{
  BSTNode *tmp;

  if(pRoot)
  {
      //printf("Root: value=%d\n", pRoot->iValue);
    // allocate a new block 
    tmp = (BSTNode *)malloc(sizeof(BSTNode));
    if(!tmp)
    {
      perror("Out of memory!");
      exit(1);
    }
    memset(tmp, 0, sizeof(BSTNode));    // clean the memory block
    tmp->pNext = NULL;                  
    tmp->pPrev = NULL;
    tmp->iValue = pRoot->iValue;        // set the i value
    
    // add to list 
    if(node_list.tail)                  // list is empty
    {
      node_list.tail->pNext = tmp;      // ... no
      tmp->pPrev = node_list.tail;
      node_list.tail = tmp;
    }
    else
    {                                   // ...yes, list is empty
      node_list.head = tmp;             // this is the first one on the list
      node_list.tail = tmp;
    }
    node_list.num_nodes++;
  } 
  //printf("One node is on the list now ...\n");
  // do the same thing to the whole tree 
  if(pRoot->pPrev)
  {
    //  printf("Lift: value = %d\n", pRoot->iValue);
    save_to_list(pRoot->pPrev);
  }
  
  if(pRoot->pNext)
  {
      //printf("Right: value = %d\n", pRoot->iValue);
    save_to_list(pRoot->pNext);
  }
}
    
 
 
/* 
 * Function:  print_list
 * Purpose:   print all the node's value of a tree in decent order.
 */
static void print_list(void)
{
  BSTNode *tmp;
  int i, j, hold;
  int ivalue[node_list.num_nodes];
  tmp = node_list.head;

  // prepare for sorting 
  for(i=0; i<node_list.num_nodes; i++)
  {
    ivalue[i] = tmp->iValue;
    tmp = tmp->pNext;                       
  }
  
  // sort the value 
  for(i=0; i<(node_list.num_nodes - 1); i++)
  {
    for(j=(i+1); j<node_list.num_nodes; j++)
    {
      if(ivalue[i] < ivalue[j])
      {
	hold = ivalue[i];
	ivalue[i] = ivalue[j];
	ivalue[j] = hold;
      }
    }
  }

  // print the all the nodes' value to the console in decent order 
  for(i=0; i<node_list.num_nodes; i++)
    printf("Node %d; value=%d\n", i, ivalue[i]);
  
  // Free the memory used by the list 
  tmp = node_list.head;
  while(tmp)
  {
    node_list.head = tmp->pNext;
    free(tmp);
    tmp = node_list.head;
    //printf("0000000 Free the list 000000\n");
  }
  
  // reset the list header
  node_list.head = NULL;
  node_list.tail = NULL;
  node_list.num_nodes = 0;
}
    


/*
 * Function name:   is_left()
 * Purpose: compare the key of a new node with a base node to decide
 *          the branch. 
 * Input:     
 *            base_node   --  
 *            new_node
 * Return: 
 *            1  --- left
 *            0  --- right
 */
static inline int is_left(BSTNode *base, BSTNode *new)
{
  if(base->iValue > new->iValue)
    return 1;
  else
    return 0;
}



/* 
 * Function:  compare_nodes(unsigned int, BSTNode*, int *)
 * Purpose:   Walk a sub-tree and try to find the closest node to a given node.
 * Input:     node  ---- the start node of a subtree
 *            diff  ---- the value difference
 *            iValue --- the given node's value which we need to find a node with value clost to this value
 * Output:    diff  ---- the node and it's closest node's(on this subtree) value difference
 *            clossest .. the closest node on this subtree 
 *
 */
static void compare_nodes(BSTNode *node, BSTNode **closest, unsigned int *diff,
		   unsigned int iValue)
{
  int this_diff;
  this_diff = abs(node->iValue - iValue);

  if(this_diff < (*diff) && this_diff != 0)     // if the previous diff is great than this one and it is not the node itself
  {                                             // update the output and continue
    *diff = this_diff;
    *closest = node;
  };
  
  if(node->pPrev)
    compare_nodes(node->pPrev, closest, diff, iValue);

  if(node->pNext)
    compare_nodes(node->pNext, closest, diff, iValue);
  
}



/* 
 * Function:  static find_closer_node(unsigned int, BSTNode*, int *)
 * 
 * Purpose:   find a node which is one step closer to a node
 *
 */#if 0
static BSTNode *find_closer_node(BSTNode *start_node, BSTNode *node, unsigned int *height, 
		   unsigned int *flag)
{ 

  if(!start_node)
  {
    (*flag) = 1;
    (*height) = 0; 
    return NULL;
  };
  
  if(start_node->iValue > node->iValue)
  {
    if(start_node->pPrev)
    {
      (*height)++;
      return start_node->pPrev;
    }
    else
      goto not_found;
  }
  else if(start_node->iValue < node->iValue)
  {
    if(start_node->pNext)
    {
      (*height)++;
      return start_node->pNext;
    }
    else
      goto not_found;
  }
  else if(start_node->iValue == node->iValue)
  {
    *flag = 1;
    return start_node;
  };

 not_found:
      *flag = 1;
      return NULL;

}
#endif 
/* 
 * Function:  btree_find_node_height(BSTNode *pRoot, BSTNode *node, unsigned int *height)
 * 
 * Input:    start_node --- root node here
 *           iValue  --- look for the node has this key.
 * Output:   height  --- the node's height.
 *           and a pointer to the node.
 *  
 * Purpose:   find a given node in the tree and it's height.
 *
 */
BSTNode *btree_find_node_height(BSTNode *pRoot, BSTNode *node, unsigned int *height)
{ 
  BSTNode *start_node;
  *height = 0;
  start_node = pRoot;
  
  if(!start_node){
    return NULL;
  }

  for(;;)
  {
    if(start_node->iValue > node->iValue)              // which branch the node is on
    {
      if((start_node = start_node->pPrev))             // node's value smaller, so on left branch
      {
	(*height)++;
	continue;
      }
      else
	break;
    }
    else if(start_node->iValue < node->iValue)         // on right branch
    {                                                  // ...yes
      if((start_node = start_node->pNext))
      {
	(*height)++;
	continue;
      }
      else
	break;
    }
    else if(start_node->iValue == node->iValue)       // the node value equal?
      return start_node;                              // ...yes, we find it
    
  }
    
  return NULL;                  /* not found */
    
}


/* 
 * Function:  btree_find_node(BSTNode *pRoot, int key)
 * 
 * Input:    key  --- look for the node has this key.
 *           
 * Output:    pointer to the node found, and its parent node(*pNode - parent node)
 *            pNode --- root node here
 *            height ---- the heigh of the node
 *.
 * Purpose:   find a given node in the tree. When testing, the block is not allocated on the heap,
 *            so, there is no need to use free to free the block. And we need keep track of two level node, 
 *            parent and child. So the deletion can be done by setting the parent's corresponding pointer to NULL.
 *            To minimize the change, the *pNode is used for pass the parent node to the calling function.
 *
 */
BSTNode *btree_find_node(BSTNode *pNode, int key, unsigned int *height)
{ 
  BSTNode *start_node;     //, *parentNode;

  start_node = pNode = pRoot;
  
  if(!start_node){
    *height = 0;
    return NULL;
  }

  for(;;)
  {    
    if(start_node->iValue > key)               // if the given value is less than the start node value   
    {
      if(start_node->pPrev != NULL)            // ...check previous(left) branch.     ((start_node = start_node->pPrev))  
      {
          pNode = start_node;                  // record parent node
          start_node = start_node->pPrev;      // reset start_node, so  we can continue
	  (*height)++;                         // increase the height
	  continue;                            // continue to the next level
      }
      else                                     // we reached a leave node
      {
    	  //return start_node;
	      break;                           // Can not find the node
      }
    }
    else if(start_node->iValue < key)          // if the given value is great than the start node value
    {
      if(start_node->pNext != NULL)            // ...check the right branch. (start_node = start_node->pNext))
      {
          pNode = start_node;
          start_node = start_node->pNext;
	  (*height)++;
	  continue;
      }
      else                                    // we reached a leave node
      {
    	  //return start_node;
    	  break;                              // can not find the node
      }
    }
    else if(start_node->iValue == key)        // we find the node
    {
      //pRoot = parentNode;                   // set this one as a parent  
      return start_node;
    }
  }
    
  return NULL;                                // can not found the node 
    
}


/*
 * Function: btree_find_closest_leave()
 * 
 * Purpose: find the leave node which has a key closest to a given key.
 * Input:   start_node --- root->left and root->right, this function need to 
 *                         be called two times to do the job, one for root's
 *                         left sub-tree, one time for root's right subtree.
 *          closest_node --- point to the closest leave node which has the key
 *                         closest to the given key.
 *          diff         --- the diff between the closest node and the key
 *          key          --- the given key
 */
#if 0
static void btree_find_closest_leave(BSTNode *start_node, BSTNode **closest_node,
				  int *diff, int key)
{
  int this_diff;
  this_diff = abs(start_node->iValue - key);

  if(start_node->pNext == NULL && start_node->pPrev == NULL)
    if(this_diff < *diff)
    {
      *diff = this_diff;
      *closest_node = start_node;
    }

  if(start_node->pNext)
    btree_find_closest_leave(start_node->pNext, closest_node, diff, key);

  if(start_node->pPrev)
    btree_find_closest_leave(start_node->pPrev, closest_node, diff, key);

}
#endif 

/* 
 * function:  btree_delete_node()
 * purpose:   delete a given node from the tree
 *
 * return:    0  --- OK,
 *           -1  --- not fund
 *           -2  --- error.
 *
 * Note: when do a deletion, it should always choose a leave node
 * from the to-be-deleted-node's child branch which has the value
 * closest to the deleted node. 
 */
int btree_delete_node(BSTNode *root, int key)
{
  BSTNode *start_node, *node, *closest_child, *parentNode; //*closest_node;
  unsigned int height;
 // int this_key;
 // this_key = key;

  if(!(start_node = pRoot))       // if tree is empty, return -1
    return -1;

  // first, find the node to be deleted
  node = btree_find_node(start_node, key, &height);                       // node is the one to be deleted, start_node is its parent node
  printf("DBG:::Delete node has find the node, value=%d\n", node->iValue);
  if(!node)
    return -1;

  // then, the node may have children, so we need to take care of them
  for(;;)
  {
      // find the closest child node to the node 
      if(node)
      {
	//closest_node = find_closest_node(node, &height, node->iValue);
        // find the closest children   
          if(node->pNext != NULL && node->pPrev != NULL)                                           // node has two children
          {
            if(abs(node->iValue - node->pPrev->iValue) > abs(node->iValue - node->pNext->iValue))  // check to see which is closer
            {
                closest_child = node->pNext;                             // ...right
                node->iValue = node->pNext->iValue;                      // shift the value to up
            }
            else
            {
                closest_child = node->pPrev;                             // ...left
                node->iValue = node->pPrev->iValue;                      // shift the value up
            }
          }
          else if (node->pPrev != NULL)                                                           // node has only a lift child
          {
                closest_child = node->pPrev;
                node->iValue = node->pPrev->iValue;                      // shift the value up
                //printf("DBG.:: Here 111...\n");
          }
          else if (node->pNext != NULL)                                                           // node has only a right child
          {
              closest_child = node->pNext;
              node->iValue = node->pNext->iValue;                        // shift the value up
          }
          else                                                                                // this is a leave node
          {           
              // before delete the leave node, we need to see which branch it is hooked to its parent.
              if(parentNode->pNext != NULL)
              {
                 if( parentNode->pNext->iValue == node->iValue)                  // on right branch?
                 {
                     printf("DBG::2 HERE 2222  ParentNode->pPrev->iValue=%d\n",parentNode->pNext->iValue );
                     parentNode ->pNext = NULL;                                    // ...yes, free the node
                     // free(start_node->pNext);
                     break;
                 }
              }
              else if (parentNode ->pPrev != NULL)
              {
                 if (parentNode ->pPrev->iValue == node->iValue)              // on left branch?
                 {
                     parentNode ->pPrev = NULL;
                     // free(start_node->pPrev);
                     break;
                 }
              }
              printf("DBG:::Delete node:: node value=%d has been deleted\n", parentNode->iValue);
              //break;
          }
      
          printf("DBG::: Delete node has find a closest node, value=%d\n", closest_child->iValue);
      
        
          if(closest_child != NULL )                   //&& closest_node != node)
          {
	    //node->iValue = closest_child->iValue;    // put the closest node's value into the deleted one 
              parentNode = node;                       // record the parents node
              node = closest_child;
            //continue;
          }

      //if(closest_child->pPrev == NULL && closest_child->pNext == NULL)
 //     {    // is this a leave node?
//	free(closest_child);  // yes, we can delete it.
  //      printf("Delete node:: one leave node has been freed\n");
//	break;
  //    }
  //    else{
	// btree_delete_node(root, closest_node->iValue);
 //         node->iValue = closest_child->iValue;
//	/*start_node =*/ node = closest_child;
//	//this_key = closest_child->iValue;
 //     }
      
    }
  }
    return 0;              
}

// Do a quick test
int main()
{

  BSTNode node1, node2, node3, node4, node5, node6, node7, node8, root, *closenode;
  unsigned int level = 0;
  int deleteNodeReturn;
  
  node7.iValue = 45;
  node7.pPrev = NULL;
  node7.pNext = NULL;
  

  node1.iValue = 250; //100;
  node1.pPrev = NULL;
  node1.pNext = NULL;

  node2.iValue = 100; //200;
  node2.pPrev = NULL;
  node2.pNext = NULL;

  node3.iValue = 300;
  node3.pPrev = NULL;
  node3.pNext = NULL;

  node4.iValue = 30;
  node4.pPrev = NULL;
  node4.pNext = NULL;

  node5.iValue = 20;
  node5.pPrev = NULL;
  node5.pNext = NULL;

  node6.iValue = 200;
  node6.pPrev = NULL;
  node6.pNext = NULL;

  node8.iValue = 260;
  node8.pPrev = NULL;
  node8.pNext = NULL;
  
  root.iValue = 40;   //19; //150;
  root.pPrev = NULL;
  root.pNext = NULL;

  insert_node(&pRoot, &root);  /* 40, 19 */
  insert_node(&pRoot, &node1); /* 250 */
  insert_node(&pRoot, &node3); /* 300 */
  insert_node(&pRoot, &node4); /* 30 */
  insert_node(&pRoot, &node2); /* 100 */
  insert_node(&pRoot, &node5); /* 20 */
  insert_node(&pRoot, &node6); /* 200 */
  insert_node(&pRoot, &node7); // 45
  insert_node(&pRoot, &node8); // 260


  closenode = find_closest_node(pRoot, &level, 250); //310);
  if(closenode)
    printf("The closest node is: %d, level= %d\n", closenode->iValue, level);

  
  print_nodes(pRoot);

  deleteNodeReturn = btree_delete_node(pRoot, 30);
  if(deleteNodeReturn == 0)
      printf("one node is Deleted\n");
  else if(deleteNodeReturn == -1)
      printf("Node can not be found\n");
  else if (deleteNodeReturn == -2)
      printf("Error!\n");
 
  print_nodes(pRoot);

  return 0;

}
