#include <bits/stdc++.h>
using namespace std;

node* bstsearch(node * &root,int k){
	if (root==null || root -> key== k)
	{
		return root
	}

	if (root->key > k)
	{
		return bstsearch(root->left,k);
	}
	else 
	{
		return bstsearch(root->right,k);
	}

}

int bstmin(node * &root){

	node *temp=root;

	if (root==null)
	{
		return -1;
	}

	while(temp->left!=null)

}

int suce(){

	if (root->right!=null)
	{
		return root->right->key;
	}

	node *y=root->parent;

	while(y!=null	&& y->right == root){

		root=y;
		y=y->parent;

	}
	return y;
}

void bstdelete(node *&root , int k){

	node *temp = bstsearch(root,k);

	if (temp -> right == null || temp-> left == null )
	{
		y=temp;
	}
	else
		y=suce(temp);


	if (y->left != null)
	{
		x= y->left;
	}
	else{
		x=y->right;
	}



	if (x!=null)
	{
		x->parent = y->parent;

		if (y->parent == null)
		{
			root=x			;
		}

		else if (y->parent ->left = y)
		{
			y->parent ->left=x;
		}
		else{
			y->parent ->right=x;
		}

		if (y->key != temp->key)
		{
			y->key = temp->key;			
		}

		free(temp);

	}

}

int main(){







	return 0;
}