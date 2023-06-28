#include<bits/stdc++.h>
#pragma GCC optimize(2,"inline")
#define ff(x,a,b) for(int x=a;x<=b;++x)
#define fff(x,a,b) for(int x=a;x>=b;--x)
using namespace std;
typedef long long ll;
const int MAXN=3005,MAXM=20005;
unsigned int f[MAXN][MAXM];
unsigned int p1=131,p2=13331;//哈希基质数 
unsigned int hasha;
int ma,na,m,n;
char mygetchar()//get input int
{
	char ch=getchar();
	while((ch<'a'||ch>'z')) ch=getchar();
	return ch;
}
void Solve1()
{
	ff(i,1,n) ff(j,1,m) f[i][j]=mygetchar()-'a'+1+p1*f[i][j-1]+p2*f[i-1][j]-p1*p2*f[i-1][j-1];
	unsigned int p11=1,p22=1;
	ff(i,1,ma) p11=p11*p1;
	ff(j,1,na) p22=p22*p2;
	ff(i,na,n) ff(j,ma,m) 
	{
		unsigned int hashb=f[i][j]-p11*f[i][j-ma]-p22*f[i-na][j]+p11*p22*f[i-na][j-ma];
//		printf("hashb(%d,%d)=%lld\n",i,j,hashb);
		if(hashb==hasha) printf("%d %d\n",i-na,j-ma);
	}
}
int main()
{
	scanf("%d%d",&na,&ma);
	ff(i,1,na) ff(j,1,ma) f[i][j]=mygetchar()-'a'+1;
	scanf("%d%d",&n,&m);
	ff(i,1,na) ff(j,1,ma) f[i][j]+=p1*f[i][j-1]+p2*f[i-1][j]-p1*p2*f[i-1][j-1];//计算一维Hash
	hasha=f[na][ma];
//	printf("hasha=%lld\n",hasha);
	Solve1();
}