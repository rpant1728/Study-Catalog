#include<bits/stdc++.h>
using namespace std;
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);

#define MAX_CHAR		26
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

int main()
{
    fast_io
    ll n, t;
    cin>>n>>t;
    vll a(n);
    vll s(n);
    set<ll> set;
    ll sum = 0;
    forall(i,0,n){
    	set.insert(i);
    	cin>>a[i];
    	sum += a[i];
    }

    ll m = n-1;
    ll ans = 0;
    ll h = n-1;
    while(set.size()){
		ans += (m+1)*(t/sum);
		t = t%sum;
		if(t==0)break;
		vll v;
		for(auto it=set.begin();it!=set.end();it++){
			if(a[*it]>t){
				v.pb(*it);
				sum -= a[*it];
				m--;
			}else{
				t -= a[*it];
				ans++;
			}
		}
		forall(i,0,v.size()){
			set.erase(v[i]);
		}
		// cout<<sum<<endl;
    }
    cout<<ans<<endl;


}