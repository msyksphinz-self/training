/*********************************************************/
/*                   neuron.c                            */
/*　単体の人工ニューロンの計算                           */
/* 適当な重みとしきい値を有する人工ニューロンを模擬します*/
/* 使い方　                                              */
/*  \Users\odaka\dl\ch4>neuron                           */
/*********************************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/* ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*記号定数の定義*/
#define INPUTNO 2  /*入力数*/ 
#define MAXINPUTNO 100    /*データの最大個数*/ 

/*関数のプロトタイプの宣言*/
double f(double u) ; /*伝達関数*/
void initw(double w[INPUTNO+1]) ;
                         /*重みとしきい値の初期化*/
double forward(double w[INPUTNO+1]
         ,double e[INPUTNO]) ; /*順方向の計算*/
int getdata(double e[][INPUTNO]) ;/*データ読み込み*/ 
         
/*******************/ 
/*    main()関数   */ 
/*******************/ 
int main()
{
 double w[INPUTNO+1] ;/*重みとしきい値*/
 double e[MAXINPUTNO][INPUTNO] ;/*データセット*/
 double o ;/*出力*/
 int i,j ;/*繰り返しの制御*/
 int n_of_e ;/*データの個数*/
 
 /*重みの初期化*/
 initw(w) ;

 /*入力データの読み込み*/
 n_of_e=getdata(e) ;
 printf("データの個数:%d\n",n_of_e) ;
 
 /*計算の本体*/
 for(i=0;i<n_of_e;++i){
  printf("%d ",i) ;
  for(j=0;j<INPUTNO;++j)
   printf("%lf ",e[i][j]) ;
  o=forward(w,e[i]) ;
  printf("%lf\n",o) ;
 }

 return 0 ;
}

/**********************/
/*  getdata()関数     */
/*学習データの読み込み*/
/**********************/
int getdata(double e[][INPUTNO])
{
 int n_of_e=0 ;/*データセットの個数*/
 int j=0 ;/*繰り返しの制御用*/

 /*データの入力*/
 while(scanf("%lf",&e[n_of_e][j])!=EOF){
  ++ j ;
  if(j>=INPUTNO){/*次のデータ*/
   j=0 ;
   ++n_of_e ;
  }
 }
 return n_of_e ;
}

/**********************/
/*  forward()関数     */
/*  順方向の計算      */
/**********************/
double forward(double w[INPUTNO+1],double e[INPUTNO])
{
 int i ;/*繰り返しの制御*/
 double u,o ;/*途中の計算値uと出力o*/

 /*計算の本体*/
 u=0 ;
 for(i=0;i<INPUTNO;++i)
  u+=e[i]*w[i] ;
 u-=w[i] ;/*しきい値の処理*/
 /*出力値の計算*/
 o=f(u) ;
 return o ;
} 

/**********************/
/*    initw()関数     */
/*　　重みの初期化　　*/
/**********************/
void initw(double w[INPUTNO+1])
{

 /*定数を荷重として与える*/
 w[0]=1 ;
 w[1]=1 ;
 w[2]=1.5 ;
// w[2]=0.5 ;

} 


/*******************/
/* f()関数         */
/* 伝達関数        */
/*******************/
double f(double u)
{
 /*ステップ関数の計算*/
 if(u>=0) return 1.0 ;
 else return 0.0 ;
 
 /*シグモイド関数の計算*/
// return 1.0/(1.0+exp(-u)) ;
}



