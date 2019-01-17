#include <bits/stdc++.h>
using namespace std;

struct node
{
	int key;
	int height;
	node *left,*right;	
};

node *create(int k,int h){

	node *temp=new node;

	temp->key=k;
	temp->height=h;
	temp->left=NULL;
	temp->right=NULL;

	return temp;
}

void inorder(node *root){

	if (root==NULL)
	{
		return;		
	}

	inorder(root->left);

	cout << root->key << " ";

	inorder(root->right);

	return;
}
void pnorder(node *root){

	if (root==NULL)
	{
		return;		
	}

		cout << root->key << " ";


	pnorder(root->left);


	pnorder(root->right);

	return;
}

int Height(node *root){

	if (root==NULL)
		return 0;

	return root->height;
}

void rotate_left(node * &root){

	node *rl=root->left;
	node *rr=root->right;

	root->right=rr->left;

	rr->left=root;
	root->height=max( Height(root->left) , Height(root->right) )+1;

	root = rr;
	root->height=max( Height(root->left) , Height(root->right) )+1;

	return;
}

void rotate_right(node * &root){

	node *rl=root->left;
	node *rr=root->right;

	root->left=rl->right;

	rl->right=root;
	root->height=max( Height(root->left) , Height(root->right) )+1;

	root = rl;
	root->height=max( Height(root->left) , Height(root->right) )+1;

	return;
}

void insert(node * &root,int k){

	if (root==NULL)
	{
		root=create(k,1);
		return;
	}

	if (root->key > k)
	{

		if (root->left==NULL)
		{
			node *t=create(k,1);
			root->left=t;
		}

		else
			insert(root->left,k);
		
		if (Height(root->left)-Height(root->right)==2)
		{
			if ( root->left->right !=NULL && root->left->key < k )
				rotate_left(root->left);
			
			rotate_right(root);
		}
	}

	else
	{
		if (root->right==NULL)
		{
			node *t=create(k,1);
			root->right=t;
		}
		else
			insert(root->right,k);
		
		if (Height(root->right)-Height(root->left)==2)
		{
			if ( root->right->left !=NULL && root->right->key > k )
				rotate_right(root->right);
			
			rotate_left(root);
		}
	}

	root->height=max( Height(root->left) , Height(root->right) )+1;


	return;
}

node *BST_SEARCH(node *root,int k){

	if (root==NULL || root->key == k)
	{
		return root;
	}

	if (root->key >= k)
	{
		return BST_SEARCH(root->left,k);
	}
	else{
		return BST_SEARCH(root->right,k);
	}
}

node *minn(node *n){

	while(n->left!=NULL)
		n=n->left;
	
	return n;
}

node * deleted(node * &root,int k){

	if (root == NULL)
	return root;

	if ( k < root->key )
		root->left=deleted(root->left, k);

	else if( k > root->key )
		root->right=deleted(root->right, k);

	else
	{
		if( (root->left == NULL) || (root->right == NULL) )
		{
			node *temp;

			if (root->left==NULL)
			{
				temp=root->right;
			}
			else{
				temp=root->left;
			}

			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
			*root = *temp; // Copy the contents of
							// the non-empty child
			delete temp;

		}

		else
		{
			node *temp = minn(root->right);

			root->key = temp->key;

			root->right = deleted(root->right, temp->key);
		}
	}

	if (root == NULL)
	return root;

		root->height = 1 + max(Height(root->left),Height(root->right));


	if (Height(root->left)-Height(root->right)>1)
	{
		if ( Height(root->left->left)-Height(root->left->right) < 0 )
			rotate_left(root->left);
		
		if (root->left!=NULL)
		{		
			rotate_right(root);

		}
	}

	if (Height(root->right)-Height(root->left) > 1)
	{
		if ( Height(root->right->left)-Height(root->right->right) > 0 )
			rotate_right(root->right);
		
		if (root->right!=NULL)
		{		
			rotate_left(root);

		}	
	}

	root->height = 1 + max(Height(root->left),Height(root->right));

	return root;	
}

int main()
{
	int N,n;
	cin >> N;

	node *root=NULL;

	for (int i = 0; i < N; ++i)
	{
		cin >> n;
		insert(root,n);
	}


	while(1){
		cin >> n;

		pnorder(root);
	cout << endl;
	root=deleted(root,n);
		pnorder(root);


	}

	return 0;
}

/*

15
1 
2 
3 
4 
5 
6 
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 
21 
22 
23 
24 
25 
26 
27 
28 
29 
30 
31 
32
*/
// // C program to delete a node from AVL Tree
// #include<stdio.h>
// #include<stdlib.h>

// // An AVL tree node
// struct Node
// {
// 	int key;
// 	struct Node *left;
// 	struct Node *right;
// 	int height;
// };

// // A utility function to get maximum of two integers
// int max(int a, int b);

// // A utility function to get height of the tree
// int height(struct Node *N)
// {
// 	if (N == NULL)
// 		return 0;
// 	return N->height;
// }

// // A utility function to get maximum of two integers
// int max(int a, int b)
// {
// 	return (a > b)? a : b;
// }

// /* Helper function that allocates a new node with the given key and
// 	NULL left and right pointers. 
// struct Node* newNode(int key)
// {
// 	struct Node* node = (struct Node*)
// 						malloc(sizeof(struct Node));
// 	node->key = key;
// 	node->left = NULL;
// 	node->right = NULL;
// 	node->height = 1; // new node is initially added at leaf
// 	return(node);
// }

// // A utility function to right rotate subtree rooted with y
// // See the diagram given above.
// struct Node *rightRotate(struct Node *y)
// {
// 	struct Node *x = y->left;
// 	struct Node *T2 = x->right;

// 	// Perform rotation
// 	x->right = y;
// 	y->left = T2;

// 	// Update heights
// 	y->height = max(height(y->left), height(y->right))+1;
// 	x->height = max(height(x->left), height(x->right))+1;

// 	// Return new root
// 	return x;
// }

// // A utility function to left rotate subtree rooted with x
// // See the diagram given above.
// struct Node *leftRotate(struct Node *x)
// {
// 	struct Node *y = x->right;
// 	struct Node *T2 = y->left;

// 	// Perform rotation
// 	y->left = x;
// 	x->right = T2;

// 	// Update heights
// 	x->height = max(height(x->left), height(x->right))+1;
// 	y->height = max(height(y->left), height(y->right))+1;

// 	// Return new root
// 	return y;
// }

// // Get Balance factor of node N
// int getBalance(struct Node *N)
// {
// 	if (N == NULL)
// 		return 0;
// 	return height(N->left) - height(N->right);
// }

// struct Node* insert(struct Node* node, int key)
// {
// 	/* 1. Perform the normal BST rotation */
// 	if (node == NULL)
// 		return(newNode(key));

// 	if (key < node->key)
// 		node->left = insert(node->left, key);
// 	else if (key > node->key)
// 		node->right = insert(node->right, key);
// 	else // Equal keys not allowed
// 		return node;

// 	/* 2. Update height of this ancestor node */
// 	node->height = 1 + max(height(node->left),
// 						height(node->right));

// 	/* 3. Get the balance factor of this ancestor
// 		node to check whether this node became
// 		unbalanced */
// 	int balance = getBalance(node);

// 	// If this node becomes unbalanced, then there are 4 cases

// 	// Left Left Case
// 	if (balance > 1 && key < node->left->key)
// 		return rightRotate(node);

// 	// Right Right Case
// 	if (balance < -1 && key > node->right->key)
// 		return leftRotate(node);

// 	// Left Right Case
// 	if (balance > 1 && key > node->left->key)
// 	{
// 		node->left = leftRotate(node->left);
// 		return rightRotate(node);
// 	}

// 	// Right Left Case
// 	if (balance < -1 && key < node->right->key)
// 	{
// 		node->right = rightRotate(node->right);
// 		return leftRotate(node);
// 	}

// 	/* return the (unchanged) node pointer */
// 	return node;
// }

// /* Given a non-empty binary search tree, return the
// node with minimum key value found in that tree.
// Note that the entire tree does not need to be
// searched. */
// struct Node * minValueNode(struct Node* node)
// {
// 	struct Node* current = node;

// 	/* loop down to find the leftmost leaf */
// 	while (current->left != NULL)
// 		current = current->left;

// 	return current;
// }

// // Recursive function to delete a node with given key
// // from subtree with given root. It returns root of
// // the modified subtree.
// struct Node* deleteNode(struct Node* root, int key)
// {
// 	// STEP 1: PERFORM STANDARD BST DELETE

// 	if (root == NULL)
// 		return root;

// 	// If the key to be deleted is smaller than the
// 	// root's key, then it lies in left subtree
// 	if ( key < root->key )
// 		root->left = deleteNode(root->left, key);

// 	// If the key to be deleted is greater than the
// 	// root's key, then it lies in right subtree
// 	else if( key > root->key )
// 		root->right = deleteNode(root->right, key);

// 	// if key is same as root's key, then This is
// 	// the node to be deleted
// 	else
// 	{
// 		// node with only one child or no child
// 		if( (root->left == NULL) || (root->right == NULL) )
// 		{
// 			struct Node *temp = root->left ? root->left :
// 											root->right;

// 			// No child case
// 			if (temp == NULL)
// 			{
// 				temp = root;
// 				root = NULL;
// 			}
// 			else // One child case
// 			*root = *temp; // Copy the contents of
// 							// the non-empty child
// 			free(temp);
// 		}
// 		else
// 		{
// 			// node with two children: Get the inorder
// 			// successor (smallest in the right subtree)
// 			struct Node* temp = minValueNode(root->right);

// 			// Copy the inorder successor's data to this node
// 			root->key = temp->key;

// 			// Delete the inorder successor
// 			root->right = deleteNode(root->right, temp->key);
// 		}
// 	}

// 	// If the tree had only one node then return
// 	if (root == NULL)
// 	return root;

// 	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
// 	root->height = 1 + max(height(root->left),
// 						height(root->right));

// 	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
// 	// check whether this node became unbalanced)
// 	int balance = getBalance(root);

// 	// If this node becomes unbalanced, then there are 4 cases

// 	// Left Left Case
// 	if (balance > 1 && getBalance(root->left) >= 0)
// 		return rightRotate(root);

// 	// Left Right Case
// 	if (balance > 1 && getBalance(root->left) < 0)
// 	{
// 		root->left = leftRotate(root->left);
// 		return rightRotate(root);
// 	}

// 	// Right Right Case
// 	if (balance < -1 && getBalance(root->right) <= 0)
// 		return leftRotate(root);

// 	// Right Left Case
// 	if (balance < -1 && getBalance(root->right) > 0)
// 	{
// 		root->right = rightRotate(root->right);
// 		return leftRotate(root);
// 	}

// 	return root;
// }

// // A utility function to print preorder traversal of
// // the tree.
// // The function also prints height of every node
// void preOrder(struct Node *root)
// {
// 	if(root != NULL)
// 	{
// 		printf("%d ", root->key);
// 		preOrder(root->left);
// 		preOrder(root->right);
// 	}
// }

// /* Drier program to test above function*/
// int main()
// {
// struct Node *root = NULL;

// /* Constructing tree given in the above figure */
// 	root = insert(root, 9);
// 	root = insert(root, 5);
// 	root = insert(root, 10);
// 	root = insert(root, 0);
// 	root = insert(root, 6);
// 	root = insert(root, 11);
// 	root = insert(root, -1);
// 	root = insert(root, 1);
// 	root = insert(root, 2);

// 	/* The constructed AVL Tree would be
// 			9
// 		/ \
// 		1 10
// 		/ \	 \
// 	0 5	 11
// 	/ / \
// 	-1 2 6
// 	*/

// 	printf("Preorder traversal of the constructed AVL "
// 		"tree is \n");
// 	preOrder(root);

// 	root = deleteNode(root, 10);

// 	/* The AVL Tree after deletion of 10
// 			1
// 		/ \
// 		0 9
// 		/	 / \
// 	-1 5	 11
// 		/ \
// 		2 6
// 	*/

// 	printf("\nPreorder traversal after deletion of 10 \n");
// 	preOrder(root);

// 	return 0;
// }


// */