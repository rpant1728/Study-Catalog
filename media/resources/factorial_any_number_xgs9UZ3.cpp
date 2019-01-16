#include <bits/stdc++.h>

using namespace std;

void multiply(int s[],int &size,int i1){

	int curr=0;

	for (int i = 0; i < size; ++i)
	{
		curr+=s[i]*i1;
		s[i]=curr%10;
		curr=curr/10;		
	}

	while(curr!=0)
	{
		s[size]=curr%10;

		curr=curr/10;

		size++;
	}

	return;
}

void extraLongFactorials(int n) {

	int s[1000000];

	s[0]=1;

	int size=1;

	for (int i = 2; i <= n; ++i)
		multiply(s,size,i);	

	for (int i =size-1; i >=0 ; --i)
	{
		cout << s[i];
	}

	cout << endl;

	cout << size << endl;
	
	return;
}

int main() {
    int n;
    cin >> n;
    extraLongFactorials(n);
    return 0;
}
