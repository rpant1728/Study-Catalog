#include <stdlib.h>
#include <stdio.h>

struct node
{
	int num;
	int s;
	int f;
	int p;
};

void merge(struct node data[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
   	struct node L[n1+1], R[n2+1];
 
    for (i = 0; i <= n1; i++)
        L[i] = data[l + i];
   

    for (j = 0; j <= n2; j++)
        R[j]= data[m + 1+ j] ;

    i = 0; 
    j = 0; 
    k = l;

    while (i < n1 && j < n2)
    {
        if (L[i].f <= R[j].f)
        {
            data[k] = L[i];
            i++;
        }
        else
        {
            data[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1)
    {
        data[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        data[k] = R[j];
        j++;
        k++;
    }
}
 
void mergeSort(struct node data[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
 
        mergeSort(data, l, m);
        mergeSort(data, m+1, r);
        merge(data, l, m, r);
    }
}

int binarySearch(struct node data[] , int l, int r, int x){
	
	int mid = (r + l)/2;
	 
	if (data[mid].f == x)  
	    return mid;

	if (l==mid)
	{
		return l;
	}

   if (r >= l)
   { 
        if (data[mid].f > x) 
            return binarySearch(data, l, mid-1, x);
 
        return binarySearch(data, mid+1, r, x);
   }
}

int compitable(struct node data[] , int n,int i){

	int p=binarySearch(data,0,i-1,data[i].s);

	return p;
}

int solve_for_j(struct node data[],int n,int solution[],int i,int flag[]){


	if (solution[i] > 0)
		return solution[i];

	else if (i < 0)
		return 0;

	int com=compitable(data,n,i);

	int a,b;

	a=solve_for_j(data,n,solution,i-1,flag);
	b=solve_for_j(data,n,solution,com,flag)+data[i-1].p;

	if (b >= a)
	{
		flag[i]=1;

		return b;
	}

	else
		return a;
}

int main(){

	int n,a,b,c;
	scanf("%d",&n);

	struct node data[n];

	for (int i = 0; i < n; ++i){

		data[i].num=i+1;
		scanf("%d %d %d",&a,&b,&c);

		data[i].s=a;
		data[i].f=b;
		data[i].p=c;
	}

	mergeSort(data,0,n-1);

	int solution[n],flag[n];

	for (int i = 0; i < n; ++i)
	{
		solution[i]=-100;
		flag[i]=0;
	}

	int ans=solve_for_j(data,n,solution,n-1,flag);

	printf("%d\n", ans );

	return 0;
}