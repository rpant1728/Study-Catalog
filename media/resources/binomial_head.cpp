#include <bits/stdc++.h>
using namespace std;

struct node
{
	int key;
	int degree;
	node *leftmost;
	node *sibling;
};

node *create(int key){
	node *temp=new node;
	temp->key=key;
	temp->degree=0;
	temp->sibling=NULL;
	temp->leftmost=NULL;

	return temp;
}

void Union(node **h,node *t){

	





	return;
}

void BH_insert(node *h,int k){

	node *temp=create(k);
	Union(h,temp);

	return;
}

int main()
{
	node *h=NULL;
	int k;

	for (int i = 0; i < 10; ++i)
	{
		cin >> k;
		BH_insert(&h,k)
	}

	return 0;
}