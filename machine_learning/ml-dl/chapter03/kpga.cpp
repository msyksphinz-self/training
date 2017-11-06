#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXVALUE  (100)                   // Max value of weight and worth
#define N         (30)                    // number of weights
#define WEIGHTLIMIT (N * MAXVALUE / 4)    // limit of weights
#define POOLSIZE    (30)                  // Pool size
#define LASTG       (50)                  // Last generation
#define MRATE       (0.01)                // Mutation Rate
#define SEED        (32767)               // Seed of random
#define YES         (1)
#define NO          (0)

void initparcel();
int evalfit (int gene[]);
void mating (int pool[POOLSIZE][N],
			 int ngpool[POOLSIZE * 2][N]);
void mutation (int ngpool[POOLSIZE * 2][N]);
void printp (int pool[POOLSIZE][N]);
void initpool (int pool[POOLSIZE][N]);
int rndn (int l);
int notval(int v);
int selectp (int roulette[POOLSIZE], int totalfitness);
void crossing (int m[], int p[], int c1[], int c2[]);
void selecting (int ngpool[POOLSIZE * 2][N],
				int pool[POOLSIZE][N]);

int parcel[N][2];


int main (int argc, char *argv[])
{
  int pool[POOLSIZE][N];
  int ngpool[POOLSIZE * 2][N];

  srand (SEED);
  initparcel ();
  initpool(pool);

  for (int generation = 0; generation < LASTG; generation++) {
	printf("Generation %d\n", generation);
	mating (pool, ngpool);
	mutation (ngpool);
	selecting (ngpool, pool);
	printp(pool);
  }

  return 0;
  
}


void initparcel ()
{
  int i = 0;
  while ((i < N) &&
		 (scanf("%d %d", &parcel[i][0], &parcel[i][1]) != EOF)) {
	i++;
  }
}


void selecting (int ngpool[POOLSIZE*2][N],
				int pool[POOLSIZE][N])
{
  int totalfitness = 0;
  int roulette[POOLSIZE * 2];
  int ball;
  int acc = 0;

  int i, j, c;
  
  for (i = 0; i < POOLSIZE; i++) {
	totalfitness = 0;
	for (c = 0; c < POOLSIZE * 2; c++) {
	  roulette[c] = evalfit(ngpool[c]);
	  totalfitness += roulette[c];
	}

	ball = rndn(totalfitness);
	acc = 0;
	for (c = 0; c < POOLSIZE * 2; c++) {
	  acc += roulette[c];
	  if (acc > ball) break;
	}

	for (j = 0; j < N; j++) {
	  pool[i][j] = ngpool[c][j];
	}
  }
}


int selectp (int roulette[POOLSIZE], int totalfitness)
{
  int acc = 0;
  int ball = rndn(totalfitness);
  int i;
  for (i = 0; i < POOLSIZE; i++) {
	acc += roulette[i];
	if (acc > ball) break;
  }
  return i;
}


void mating (int pool[POOLSIZE][N],
			 int ngpool[POOLSIZE * 2][N])
{
  int roulette[POOLSIZE];
  int totalfitness = 0;
  for (int i = 0; i < POOLSIZE; i++) {
	roulette[i] = evalfit (pool[i]);
	totalfitness += roulette[i];
  }

  int mama, papa;
  
  for (int i = 0; i < POOLSIZE; i++) {
	do {
	  mama = selectp (roulette, totalfitness);
	  papa = selectp (roulette, totalfitness);
	} while (mama == papa);

	crossing (pool[mama], pool[papa],
			  ngpool[i*2], ngpool[i*2+1]);
  }
}


void crossing (int m[], int p[], int c1[], int c2[])
{
  int cp = rndn(N);

  int j;
  for (j = 0;j < cp; j++) {
	c1[j] = m[j];
	c2[j] = p[j];
  }
  for (; j < N; j++) {
	c2[j] = m[j];
	c1[j] = p[j];
  }
}


int evalfit (int g[])
{
  int weight = 0;
  int value = 0;
  for (int pos = 0; pos < N; pos++) {
	weight += parcel[pos][0] * g[pos];
	value  += parcel[pos][1] * g[pos];
  }
  if (weight >= WEIGHTLIMIT) value = 0;
  return value;
}


/***********************/
/*   printp()関数      */
/*   結果出力          */
/***********************/
void printp(int pool[POOLSIZE][N])
{
 int i,j ;/* 繰り返しの制御変数 */
 int fitness ;/* 適応度 */
 double totalfitness=0 ;/* 適応度の合計値 */
 int elite,bestfit=0 ;/*エリート遺伝子の処理用変数*/

 for(i=0;i<POOLSIZE;++i){
  for(j=0;j<N;++j)
   printf("%1d",pool[i][j]) ;
  fitness=evalfit(pool[i]) ;
  printf("\t%d\n",fitness) ;
  if(fitness>bestfit){/*エリート解*/
   bestfit=fitness ;
   elite=i ;
  }
  totalfitness+=fitness ;
 }
 /*エリート解の適応度を出力*/
 printf("%d\t%d \t",elite,bestfit) ;
 /*平均の適応度を出力*/
 printf("%lf\n",totalfitness/POOLSIZE) ;
}


/***********************/
/*   initpool()関数    */
/*   初期集団の生成    */
/***********************/
void initpool(int pool[POOLSIZE][N])
{
 int i,j ;/* 繰り返しの制御変数 */

 for(i=0;i<POOLSIZE;++i)
  for(j=0;j<N;++j)
   pool[i][j]=rndn(2) ;
}


/************************/
/*     rndn()関数       */
/*    n未満の乱数の生成 */
/************************/
int rndn(int l)
{
 int rndno ;/*生成した乱数*/

 while((rndno=((double)rand()/RAND_MAX)*l)==l) ;

 return rndno;
}

/***********************/
/*   mutation()関数    */
/*   突然変異          */
/***********************/
void mutation(int ngpool[POOLSIZE*2][N])
{
 int i,j ;/* 繰り返しの制御変数 */

 for(i=0;i<POOLSIZE*2;++i)
  for(j=0;j<N;++j)
   if((double)rndn(100)/100.0<=MRATE)
    /*反転の突然変異*/
    ngpool[i][j]=notval(ngpool[i][j]) ;

}

/************************/
/*   notval()関数       */
/*   真理値の反転       */
/************************/
int notval(int v)
{
 if(v==YES) return NO ;
 else return YES ;
}


	
		  
