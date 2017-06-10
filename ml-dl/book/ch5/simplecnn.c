/*********************************************************/
/*               simplecnn.c                             */
/* 畳み込みニューラルネットの基本構造デモンストレーション*/
/* CNNの基本構造(骨組のみ）を示します                    */
/* 使い方　                                              */
/*  \Users\odaka\dl\ch5>simplecnn < data1.txt            */
/*********************************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/* ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*記号定数の定義*/
#define INPUTSIZE 11  /*入力数*/ 
#define FILTERSIZE 3 /*フィルタの大きさ*/
#define FILTERNO 2 /*フィルタの個数*/
#define POOLSIZE 3 /*プーリングサイズ*/
#define POOLOUTSIZE 3/*プーリングの出力サイズ*/
#define MAXINPUTNO 100    /*学習データの最大個数*/ 
#define SEED 65535    /*乱数のシード*/ 
#define LIMIT 0.001    /*誤差の上限値*/ 
#define BIGNUM 100    /*誤差の初期値*/ 
#define HIDDENNO 3  /*中間層のセル数*/ 
#define ALPHA  10  /*学習係数*/ 

/*関数のプロトタイプの宣言*/
void conv(double filter[FILTERSIZE][FILTERSIZE]
   ,double e[][INPUTSIZE]
   ,double convout[][INPUTSIZE]) ; /*畳み込みの計算*/
double calcconv(double filter[][FILTERSIZE]
               ,double e[][INPUTSIZE],int i,int j) ;
                               /*  フィルタの適用  */
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) ; 
             /*プーリングの計算*/           
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j) ;/* 最大値プーリング */
int getdata(double e[][INPUTSIZE][INPUTSIZE],int r[]) ;/*データ読み込み*/ 
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[]
             ,int n_of_e) ;/*データ表示*/ 
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE]) ;
                /*フィルタの初期化*/
double drnd(void) ;/* 乱数の生成     */
double f(double u) ; /*伝達関数（シグモイド関数）*/
void initwh(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ;
                         /*中間層の重みの初期化*/
void initwo(double wo[HIDDENNO+1]) ;/*出力層の重みの初期化*/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
         ,double wo[HIDDENNO+1],double hi[]
         ,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]) ; /*順方向の計算*/
void olearn(double wo[HIDDENNO+1],double hi[]
         ,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ; /*出力層の重みの調整*/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
         ,double wo[HIDDENNO+1],double hi[]
         ,double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o) ; /*中間層の重みの調整*/
double f(double u) ; /*伝達関数（シグモイド関数）*/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
          ,double wo[HIDDENNO+1]) ; /*結果の出力*/
          
/*******************/ 
/*    main()関数   */ 
/*******************/ 
int main()
{
 double filter[FILTERNO][FILTERSIZE][FILTERSIZE] ;
                            /*フィルタ*/
 double e[MAXINPUTNO][INPUTSIZE][INPUTSIZE] ;/*入力データ*/
 int t[MAXINPUTNO] ;/*教師データ*/
 double convout[INPUTSIZE][INPUTSIZE]={0} ;/*畳み込み出力*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;/*出力データ*/
 int i,j,m,n ;/*繰り返しの制御*/
 int n_of_e ;/*学習データの個数*/
 double err=BIGNUM ;/*誤差の評価*/
 int count=0 ;/*繰り返し回数のカウンタ*/
 double ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;/*全結合層への入力データ*/
 double o ;/*最終出力*/
 double hi[HIDDENNO+1] ;/*中間層の出力*/
 double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1] ;/*中間層の重み*/
 double wo[HIDDENNO+1] ;/*出力層の重み*/
 
 /*乱数の初期化*/
 srand(SEED) ;
 
 /*フィルターの初期化*/
 initfilter(filter) ;
 
 /*全結合層の重みの初期化*/
 initwh(wh) ;/*中間層の重みの初期化*/
 initwo(wo) ;/*出力層の重みの初期化*/
 
 /*入力データの読み込み*/
 n_of_e=getdata(e,t) ;
 showdata(e,t,n_of_e) ;
 
  /*学習*/
 while(err>LIMIT){
  err=0.0 ;
  for(i=0;i<n_of_e;++i){/*学習データ毎の繰り返し*/
   for(j=0;j<FILTERNO;++j){/*フィルタ毎の繰り返し*/
    /*畳み込みの計算*/
    conv(filter[j],e[i],convout) ;
    /*プーリングの計算*/
    pool(convout,poolout) ;
    /*プーリング出力を全結合層の入力へコピー*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;/*教師データ*/
   }
   /*順方向の計算*/
   o=forward(wh,wo,hi,ef) ;
   /*出力層の重みの調整*/
   olearn(wo,hi,ef,o) ;
   /*中間層の重みの調整*/
   hlearn(wh,wo,hi,ef,o) ;
   /*誤差の積算*/
   err+=(o-t[i])*(o-t[i]) ;
  }
  ++count ;
  /*誤差の出力*/
  fprintf(stderr,"%d\t%lf\n",count,err) ;
 }/*学習終了*/
 
 printf("\n***Results***\n") ;
 /*結合荷重の出力*/
 printf("Weights\n") ;
 print(wh,wo) ; 

 /*教師データに対する出力*/
 printf("Network output\n") ;
 printf("#\tteacher\toutput\n") ;
 for(i=0;i<n_of_e;++i){
  printf("%d\t%d\t",i,t[i]) ;
  for(j=0;j<FILTERNO;++j){/*フィルタ毎の繰り返し*/
    /*畳み込みの計算*/
    conv(filter[j],e[i],convout) ;
    /*プーリングの計算*/
    pool(convout,poolout) ;
    /*プーリング出力を全結合層の入力へコピー*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      ef[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    ef[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]=t[i] ;/*教師データ*/
   }
   o=forward(wh,wo,hi,ef) ;
  printf("%lf\n",o) ;
 }

 return 0 ;
}

/**********************/
/*   print()関数      */
/*   結果の出力       */
/**********************/
void print(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
          ,double wo[HIDDENNO+1])
{
 int i,j ;/*繰り返しの制御*/

 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   printf("%lf ",wh[i][j]) ;
 printf("\n") ;
 for(i=0;i<HIDDENNO+1;++i)
  printf("%lf ",wo[i]) ;
 printf("\n") ;
} 

/**********************/
/*  hlearn()関数      */
/*  中間層の重み学習  */
/**********************/
void hlearn(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
    ,double wo[HIDDENNO+1]
    ,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o)
{
 int i,j ;/*繰り返しの制御*/
 double dj ;/*中間層の重み計算に利用*/

 for(j=0;j<HIDDENNO;++j){/*中間層の各セルjを対象*/
  dj=hi[j]*(1-hi[j])*wo[j]*(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;
  for(i=0;i<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++i)/*i番目の重みを処理*/
   wh[j][i]+=ALPHA*e[i]*dj ;
  wh[j][i]+=ALPHA*(-1.0)*dj ;/*しきい値の学習*/
 }
}

/**********************/
/*  olearn()関数      */
/*  出力層の重み学習  */
/**********************/
void olearn(double wo[HIDDENNO+1]
    ,double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1],double o)
{
 int i ;/*繰り返しの制御*/
 double d ;/*重み計算に利用*/

 d=(e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO]-o)*o*(1-o) ;/*誤差の計算*/
 for(i=0;i<HIDDENNO;++i){
  wo[i]+=ALPHA*hi[i]*d ;/*重みの学習*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;/*しきい値の学習*/
 
} 

/**********************/
/*  forward()関数     */
/*  順方向の計算      */
/**********************/
double forward(double wh[HIDDENNO][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;/*繰り返しの制御*/
 double u ;/*重み付き和の計算*/
 double o ;/*出力の計算*/

 /*hiの計算*/
 for(i=0;i<HIDDENNO;++i){
  u=0 ;/*重み付き和を求める*/
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO;++j)
   u+=e[j]*wh[i][j] ; 
  u-=wh[i][j] ;/*しきい値の処理*/
  hi[i]=f(u) ;
 }
 /*出力oの計算*/
 o=0 ;
 for(i=0;i<HIDDENNO;++i)
  o+=hi[i]*wo[i] ;
 o-=wo[i] ;/*しきい値の処理*/
 
 return f(o) ;
} 

/**********************/
/*    initwo()関数    */
/*中間層の重みの初期化*/
/**********************/
void initwh(double wh[][POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1])
{
 int i,j ;/*繰り返しの制御*/

 /*乱数による重みの決定*/ 
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<POOLOUTSIZE*POOLOUTSIZE*FILTERNO+1;++j)
   wh[i][j]=drnd() ;
} 

/**********************/
/*    initwo()関数    */
/*出力層の重みの初期化*/
/**********************/
void initwo(double wo[])
{
 int i ;/*繰り返しの制御*/

 /*乱数による重みの決定*/
 for(i=0;i<HIDDENNO+1;++i)
   wo[i]=drnd() ;
} 

/**********************/
/*  initfilter()関数  */
/* 　フィルタの初期化 */
/**********************/
void initfilter(double filter[FILTERNO][FILTERSIZE][FILTERSIZE])
{
 int i,j,k ;/*繰り返しの制御*/
 
 for(i=0;i<FILTERNO;++i)
  for(j=0;j<FILTERSIZE;++j)
   for(k=0;k<FILTERSIZE;++k)
    filter[i][j][k]=drnd() ;
} 

/*******************/
/* drnd()関数      */
/* 乱数の生成      */
/*******************/
double drnd(void)
{
 double rndno ;/*生成した乱数*/

 while((rndno=(double)rand()/RAND_MAX)==1.0) ;
 rndno=rndno*2-1 ;/*-1から1の間の乱数を生成*/
 return rndno; 
}

/**********************/
/*  pool()関数        */
/* プーリングの計算   */
/**********************/
void pool(double convout[][INPUTSIZE]
         ,double poolout[][POOLOUTSIZE]) 
{
 int i,j ;/*繰り返しの制御*/

 for(i=0;i<POOLOUTSIZE;++i)
  for(j=0;j<POOLOUTSIZE;++j)
   poolout[i][j]=maxpooling(convout,i,j) ;
}
 
/**********************/
/* maxpooling()関数   */
/* 最大値プーリング   */
/**********************/
double maxpooling(double convout[][INPUTSIZE]
                 ,int i,int j)
{
 int m,n ;/*繰り返しの制御用*/
 double max ;/*最大値*/
 int halfpool=POOLSIZE/2 ;/*プーリングのサイズの1/2*/
 
 max
 =convout[i*POOLOUTSIZE+1+halfpool][j*POOLOUTSIZE+1+halfpool];
 for(m=POOLOUTSIZE*i+1;m<=POOLOUTSIZE*i+1+(POOLSIZE-halfpool);++m)
  for(n=POOLOUTSIZE*j+1;n<=POOLOUTSIZE*j+1+(POOLSIZE-halfpool);++n)
   if(max<convout[m][n]) max=convout[m][n] ;

 return max ;
}
               
/**********************/
/* showdata()関数     */
/*入力データの表示　　*/
/**********************/
void showdata(double e[][INPUTSIZE][INPUTSIZE],int t[],int n_of_e)
{
 int i=0,j=0,k=0 ;/*繰り返しの制御用*/

 /*データの表示*/
 for(i=0;i<n_of_e;++i){
  printf("N=%d category=%d\n",i,t[i]) ;
  for(j=0;j<INPUTSIZE;++j){
   for(k=0;k<INPUTSIZE;++k)
    printf("%.3lf ",e[i][j][k]) ;
   printf("\n") ;
  }
  printf("\n") ;
 }
}
             
/**********************/
/*  getdata()関数     */
/*入力データの読み込み*/
/**********************/
int getdata(double e[][INPUTSIZE][INPUTSIZE],int t[])
{
 int i=0,j=0,k=0 ;/*繰り返しの制御用*/

 /*データの入力*/
 while(scanf("%d",&t[i])!=EOF){/*教師データの読み込み*/
  /*画像データの読み込み*/
  while(scanf("%lf",&e[i][j][k])!=EOF){
   ++ k ;
   if(k>=INPUTSIZE){/*次のデータ*/
    k=0 ;
    ++j ;
    if(j>=INPUTSIZE) break ;/*入力終了*/
   }
  }
  j=0; k=0 ;
  ++i ; 
 }
 return i ;
}

/**********************/
/*  conv()関数        */
/*  畳み込みの計算    */
/**********************/
void conv(double filter[][FILTERSIZE]
         ,double e[][INPUTSIZE],double convout[][INPUTSIZE])
{
 int i=0,j=0 ;/*繰り返しの制御用*/
 int startpoint=FILTERSIZE/2 ;/*畳み込み範囲の下限*/

 for(i=startpoint;i<INPUTSIZE-startpoint;++i)
  for(j=startpoint;j<INPUTSIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv()関数    */
/*  フィルタの適用    */
/**********************/
double calcconv(double filter[][FILTERSIZE]
             ,double e[][INPUTSIZE],int i,int j)
{
 int m,n ;/*繰り返しの制御用*/
 double sum=0 ;/*和の値*/
 
 for(m=0;m<FILTERSIZE;++m)
  for(n=0;n<FILTERSIZE;++n)
   sum+=e[i-FILTERSIZE/2+m][j-FILTERSIZE/2+n]*filter[m][n];
   
 return sum ;
}

/*******************/
/* f()関数         */
/* 伝達関数        */
/*(シグモイド関数) */
/*******************/
double f(double u)
{
 return 1.0/(1.0+exp(-u)) ;
}

