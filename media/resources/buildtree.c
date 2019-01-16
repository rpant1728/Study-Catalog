#include <stdio.h>
#include <stdlib.h>
#define SIZE 7
typedef struct _node
{
	int data;
	struct _node *left;
	struct _node *right;
}node;

node *createnode(int d)
{
	node *temp = (node *)malloc(sizeof(node));
	temp->data = d;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

int search(int a[] , int start, int end, int d)
{
	int i;
	for(i = start; i<= end; i++)
	{
		if(a[i] == d)
			return i;
	}
}

node * buildtree(int in[], int pre[], int start, int end) 
{
	if(start > end)
		return NULL;
	static int i = 0;

	node *temp = createnode(pre[i++]);

	if(start == end)
		return temp;

	int root = search(in, start, end, temp->data );

	temp->left = buildtree(in, pre, start, root -1);
	temp->right = buildtree(in, pre, root+1, end);
	return temp;
}

node * buildtree1(int post[], int pre[], int start, int end)
{
	if(start > end)
		return NULL;
	static int i = 0;
	node *temp = createnode(pre[i++]);

	if(start == end)
		return temp;
	int root = search(post, start, end, pre[i] );

	temp->left = buildtree1(post, pre, start, root );
	temp->right = buildtree1(post, pre, root+1, end-1);
	return temp;


}
node * buildtree2(int post[], int in[], int start, int end)
{
	if(start > end)
		return NULL;
	int d = end;
	static int i = SIZE-1;
	node *temp = createnode(post[i--]);

	if(start == end)
		return temp;
	int root = search(in, start, end, temp->data );

	temp->right = buildtree2(post, in, root+1, end);
	temp->left = buildtree2(post, in, start, root-1 );
	
	return temp;


}

int maxlevel(node *root)
{
	if(root == NULL)
		return 0;
	
	{
		int a = maxlevel(root->left);
		//printf("%d\n",a );
		int b = maxlevel(root->right);
		//printf("%d\n",b );
		if(a<b)
			return 1 + a;
		else
			return 1 +  b;
		//return (c);
	}

}

void printTree(node *root)
{
	//	printf("yy\n");

	if(root == NULL)
		return;
	printTree(root->left);
	
	
	printTree(root->right);
	printf("%d ",root->data );

		
}
int findleave(node *root)
{
	if(root == NULL)
		return 0;
	if(root->left == NULL && root->right == NULL )
		return 1;
	return  findleave(root->left) + findleave(root->right);

}

int main()
{
	int a[] = {4,2,5,8,11,1,6,3,9,7,10,0,12};
	int b[] = {0,1,2,4,5,8,11,3,6,7,9,10,12};
	//int c[] = {3,4,1,5,6,2,0};

	node * root = buildtree(a, b, 0, 12);
	//node *root1 = createnode(3);
	//root1->left = createnode(3);
	//root1->right = createnode(3);
	printTree(root);
	printf("\n%d\n",findleave(root) );

}