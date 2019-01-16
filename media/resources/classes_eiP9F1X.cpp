#include <bits/stdc++.h>
using namespace std;
struct node
{
	struct x *next;
	int parent;
	int color;
	int d;	
};
struct x
{
	struct x *next;
	int value;
};
x *getx(int data)
{
	x*temp=new x;
	temp->value=data;
	temp->next=NULL;
	return temp;
}
void enqueue(int Q[],int s,int& head)
{
	if(head==1000)
		return;
	Q[head++]=s;
}
int dequeue(int Q[],int& tail,int &head)
{
	if(head==tail)
	{
		head=tail=0;
		return -1;
	}
	return Q[tail++];
}
int BFS(node G[],int s)
{
	int v;
	G[s].color=2;
	G[s].d=0;
	int Q[1000];
	int head,tail;
	head=tail=0;
	enqueue(Q,s,head);
	int u;
	x *temp;
	while(1)
	{
		u=dequeue(Q,tail,head);
		if(u==-1)
			break;
		temp=G[u].next;

		while(temp!=NULL)
		{
			if(G[temp->value].color==1)
			{
				G[temp->value].color=2;
				
					G[temp->value].d=G[u].d?0:1;
				
				
				G[temp->value].parent=u;
				enqueue(Q,temp->value,head);
			}
			else
			{
				if(G[temp->value].d==G[u].d)
					return 1;
			}
			temp=temp->next;
		}
		G[u].color=3;
	}
	return 0;
}
/*int BFS(node G[],int s)
{
	int iscircular;
	int v;
	G[s].color=2;
	G[s].d=0;
	int Q[1000];
	int head,tail;
	head=tail=0;
	enqueue(Q,s,head);
	int u;
	x *temp;
	while(1)
	{
		u=dequeue(Q,tail,head);
		if(u==-1)
			break;
		temp=G[u].next;
		while(temp!=NULL)
		{
			if(G[temp->value].color==1)
			{
				G[temp->value].color=2;
				G[temp->value].d=G[u].d+1;
				G[temp->value].parent=u;
				enqueue(Q,temp->value,head);
			}
			temp=temp->next;
		}
		G[u].color=3;
	}
}*/
int main()
{
	int n,r;
	int a,b;
	cin>>n>>r;
	node G[n+1];
	for (int i = 1; i <= n; ++i)
	{
		G[i].next=NULL;
		G[i].parent=0;
		G[i].color=1;
		G[i].d=INT_MAX;
	}
	for (int i = 0; i < r; ++i)
	{
		cin>>a>>b;
		x *temp;
		temp=G[a].next;
		if(temp==NULL)
			G[a].next=getx(b);
		else
		{
			while(temp->next!=NULL)
				temp=temp->next;
			temp->next=getx(b);
		}


		temp=G[b].next;
		if(temp==NULL)
			G[b].next=getx(a);
		else
		{
			while(temp->next!=NULL)
				temp=temp->next;
			temp->next=getx(a);
		}
	}
	if(BFS(G,1))
	{
		cout<<"Not Possible"<<endl;
	}
	else
	{
		cout<<"GOOD GUYS: ";
		for (int i = 1; i <=n; ++i)
		{
			if(G[i].d==0)
				cout<<i<<" ";
		}
		cout<<endl<<"BAD GUYS: ";
		for (int i = 1; i <=n; ++i)
		{	
			if(G[i].d==1)
				cout<<i<<" ";
		}
		cout<<endl;
	}
}