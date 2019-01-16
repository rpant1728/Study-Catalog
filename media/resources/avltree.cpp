#include <bits/stdcpp.h>
using namespace std;

struct node
{
	int key;
	int height;
	node *left,*right;	
};

node *create(int k,int h){

	node *temp=new node;

	temp->key=int k;
	temp->height=h;
	temp->left=NULL;
	temp->right=NULL;

	return temp;
}

int Height(node *root){

	if (root==NULL)
	{
		return 0;
	}

	return root->height;
}

void insert(node * &root,int k){

	if (root==NULL)
	{
		root=create(k,1);
		return;
	}

	if (root->key > k)
	{
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

int main()
{
	int N;
	cin >> N;

	node *root=NULL

	for (int i = 0; i < N; ++i)
	{
		cin >> n;
		insert(root,n);
	}





	return 0;
}