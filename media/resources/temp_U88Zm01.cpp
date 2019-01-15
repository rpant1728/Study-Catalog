#include<bits/stdc++.h>
using namespace std;
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);

#define pb    		    push_back
#define forall(i,a,b)   for(ll i=a;i<b;i++)
#define forrev(i,a,b)	for(ll i=b-1;i>=a;i--)
#define all(a)          a.begin(), a.end()
#define PI 	            3.1415926535897932384626433832795l
#define mem(n,i)        memset(n,i,sizeof (n));
#define deci(n)         cout<<fixed<<setprecision(n);
#define countBits(a)    __builtin_popcount (a)
#define mod 			1000000007
 
typedef unsigned long long ull;
typedef long double ld;
typedef long long ll;
typedef vector<string> vs;
typedef vector<ll> vll;
typedef map<string, string> mss;
typedef map <ll,string> mlls;
typedef map <ll,ll> mll;
typedef unordered_map <ll,ll> umll;
typedef pair<ll, ll> pii;
typedef pair<ll, ll> pll;


int main() 
{
	fast_io;
	ll n;
	cin>>n;
	vll p(n+1,0);
	forall(i,1,n){
		cin>>p[i+1];
	}
	vll s(n+1);
	vll a(n+1, 0);
	forall(i,1,n+1){
		cin>>s[i];
	}
	vll el;
	vll ol;
	ol.pb(1);
	vector<vector<ll>> adj(n+1);
	forall(i,2,n+1){
		adj[p[i]].pb(i);
		if(s[i]<0){
			el.pb(i);
		}
	}
	// forall(i,1,n+1){
	// 	cout<<i<<"-";
	// 	forall(j,0,adj[i].size()){
	// 		cout<<adj[i][j]<<" ";
	// 	}
	// 	cout<<endl;
	// }
	a[1] = s[1];
	forall(i,0,el.size()){
		// cout<<el[i]<<" ";
		a[el[i]] = INT_MAX;
		if(adj[el[i]].size()==0){
			a[el[i]] = 0;
			continue;
		}
		forall(j,0,adj[el[i]].size()){
			a[el[i]] = min(s[adj[el[i]][j]] - s[p[el[i]]], a[el[i]]);
		}
		s[el[i]] = a[el[i]] + s[p[el[i]]];
		forall(j,0,adj[el[i]].size()){
			a[adj[el[i]][j]] = s[adj[el[i]][j]] - s[el[i]];
		}
	}
	ll ans = 0;
	forall(i,1,n+1){
		if(a[i]<0){
			cout<<-1<<endl;
			return 0;
		}
		// cout<<i<<"-"<<a[i]<<endl;
		ans+=a[i];
	}
	cout<<ans<<endl;

	
}