#include <bits/stdc++.h>
using namespace std;
#define ll long long int

vector < vector<int> > va;
vector<int> parent;
int ans = 0 , finalans = 0;
vector<ll > data,value;
ll max1 = INT_MAX;


void call1(int a){

	if (finalans==1)
	{
		return;
	}

	if (data[a]==-1)
	{
		value[a] = 0;
		data[a] = data[parent[a]];
	}
	else{

		ll p1 = data[a];
		ll r1 = p1 - data[parent[a]];

		if (r1<0)
		{
			finalans=1;
			return;
		}

		value[a] = r1;
	}

	int n1 = va[a].size();
	
	for (int i = 0; i < n1; ++i)
	{
		call1(va[a][i]);
	}

	return;
}



void call(int a , int h1){

	int n1 = va[a].size();

	if (h1==0)
	{
		ll m1 = max1;
	
		for (int i = 0; i < n1; ++i)
			m1 = min(m1,data[ va[a][i] ]);

		if (m1==max1)
			m1=-1;

		data[a] = m1;
	}

	for (int i = 0; i < n1; ++i)
		call(va[a][i] , (h1+1)%2);
	
	return;
}

int main(){

	int n;
	cin >> n;

	vector<int> v1;

	for (int i = 0; i <= n ; ++i)
	{
		va.push_back(v1);
		parent.push_back(0);
		value.push_back(0);
		data.push_back(0);
	}

	for (int i = 0; i < n-1; ++i)
	{
		int a;
		cin >> a;
		parent[i+2] = a;
		va[a].push_back(i+2);
	}

	for (int i = 0; i < n; ++i)
	{
		ll a1;
		cin >> a1;
		data[i+1] = a1;
	}

	call(1,1);
	call1(1);

	if (finalans==1)
	{
		cout << "-1" << endl;
	}
	else
	{
		ans = 0;
		for (int i = 0; i < n; ++i)
			ans += value[i+1];

		cout << ans << endl;
	}

	return 0;
}