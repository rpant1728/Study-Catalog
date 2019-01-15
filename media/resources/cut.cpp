#include<bits/stdc++.h>
using namespace std;
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);

#define pb              push_back
#define forall(i,a,b)   for(ll i=a;i<b;i++)
#define all(a)          a.begin(), a.end()
#define PI              3.1415926535897932384626433832795l
#define mem(n,i)        memset(n,i,sizeof (n));
#define deci(n)         cout<<fixed<<setprecision(n);
#define mod				1000000007
 
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
vector<ll> m(100001, -1);
ll func(ll n){
	if(m[n]!=-1)return m[n];
	ll ans = 1;
	for(int i=n;i>=1;i--){
		if(i%2==0)
			ans = (ans*(i/2))%mod;
		else 
			ans = (ans*i)%mod;
	}
	m[n] = ans;
	return ans;
}

int main()
{
    fast_io;
    ll x;
    cin>>x;
    
    while(x--){
	    ll n;
	    cin>>n;
	    vector<ll> v(n);
	    mll map;
	    forall(i,0,n){
	    	cin>>v[i];
	    	map[v[i]]++;
	    }
	    ll c = 0;
	    ll ans = 1;
	 	for(auto it=map.rbegin();it!=map.rend();it++){
	 		ll t = it->second;
	 		// cout<<t<<endl;
	 		if(c%2==0){
	 			if(t%2==0){
	 				ans = (ans*func(t))%mod;
	 			}else{
	 				ans = (ans*func(t-1))%mod;
	 				ans = (ans*t)%mod;
	 			}
	 		}else{
	 			if(t%2==0){
	 				ans = (ans*func(t))%mod;
	 				ans = (ans*2)%mod;
	 			}else{
	 				ans = (ans*func(t-1))%mod;
	 				ans = (ans*t)%mod;
	 			}
	 		}
	 		c+=t;
	 	}   
	 	cout<<ans<<endl;
	}

}