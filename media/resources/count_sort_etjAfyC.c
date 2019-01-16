#include <bits/stdc++.h>
using namespace std;

int main()
{
	int N;
	cin >> N;

	int A[N],B[N];
	int C[N]= {0} ;

	for (int i = 0; i < N; ++i)
	{
		cin >> A[i];
		C[A[i]]++;
	}

	for (int i = 0; i < N; ++i)
	{
		C[i]=C[i]+C[i-1];
	}


	for (int i = N-1;  i>=0; --i)
	{
		B[C[A[i]]-1]=A[i];
		C[A[i]]--;
	}

	for (int i = 0; i < N; ++i)
	{
		printf(" %d \n",B[i] );
	}

	return 0;
}