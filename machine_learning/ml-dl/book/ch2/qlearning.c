/********************************************/
/*         qlearning.c                      */
/*   強化学習(Q学習)の例題プログラム        */
/*　 迷路の探索を学習します                 */
/*使い方                                    */
/*:\Users\odaka\dl\ch2>qlearning            */
/********************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/* 記号定数の定義             */
#define GENMAX  1000 /*学習の繰り返し回数*/
#define NODENO  15  /*Ｑ値のノード数*/
#define ALPHA 0.1/*学習係数*/
#define GAMMA 0.9/*割引率*/
#define EPSILON 0.3 /*行動選択のランダム性を決定*/
#define SEED 32767 /*乱数のシード*/

/* 関数のプロトタイプの宣言   */
int rand100() ;/*0～100を返す乱数関数*/
int rand01() ;/*0又は1を返す乱数関数*/
double rand1() ;/*0～1の実数を返す乱数関数*/
void printqvalue(int qvalue[NODENO]);/*Q値出力*/
int selecta(int s,int qvalue[NODENO]);/*行動選択*/
int updateq(int s,int qvalue[NODENO]);/*Q値更新*/

/****************/
/*  main()関数  */
/****************/
int main()
{
 int i;
 int s;/*状態*/
 int t;/*時刻*/
 int qvalue[NODENO];/*Q値*/


 srand(SEED);/*乱数の初期化*/

 /*Ｑ値の初期化*/
 for(i=0;i<NODENO;++i)
  qvalue[i]=rand100() ;
 printqvalue(qvalue) ;


 /*学習の本体*/
 for(i=0;i<GENMAX;++i){
  s=0;/*行動の初期状態*/
  for(t=0;t<3;++t){/*最下段まで繰り返す*/
   /*行動選択*/
   s=selecta(s,qvalue) ;
   
   /*Q値の更新*/
   qvalue[s]=updateq(s,qvalue) ;
  }
  /*Q値の出力*/
  printqvalue(qvalue) ;
 }
 return 0;
}

/****************************/
/*       updateq()関数      */
/*　     Q値を更新する      */
/****************************/
int updateq(int s,int qvalue[NODENO])
{
 int qv ;/*更新されるQ値*/
 int qmax ;/*Ｑ値の最大値*/
 
 /*最下段の場合*/
 if(s>6){
  if(s==14)/*報酬の付与*/
   qv=qvalue[s]+ALPHA*(1000-qvalue[s]) ;
  /*報酬を与えるノードを増やす*/
  /*他のノードを追加する場合は*/
  /*下記2行のコメントを外す   */
//  else if(s==11)/*報酬の付与*/
//   qv=qvalue[s]+ALPHA*(500-qvalue[s]) ;
  else/*報酬なし*/
   qv=qvalue[s] ;
 }
 /*最下段以外*/
 else{
  if((qvalue[2*s+1])>(qvalue[2*s+2])) 
   qmax=qvalue[2*s+1];
  else qmax=qvalue[2*s+2];
  qv=qvalue[s]+ALPHA*(GAMMA*qmax-qvalue[s]) ;
 }

 return qv ;
}

/****************************/
/*        selecta()関数     */
/*　     行動を選択する     */
/****************************/
int selecta(int olds,int qvalue[NODENO])
{
 int s ;
 
 /*ε-greedy法による行動選択*/
 if(rand1()<EPSILON){
  /*ランダムに行動*/
  if(rand01()==0) s=2*olds+1 ;
  else s=2*olds+2 ;
 }
 else{
  /*Ｑ値最大値を選択*/
  if((qvalue[2*olds+1])>(qvalue[2*olds+2])) 
   s=2*olds+1;
  else s=2*olds+2;
 }

 return s ;
}



/****************************/
/*    printqvalue()関数     */
/*　   Q値を出力する        */
/****************************/
void printqvalue(int qvalue[NODENO])
{
 int i ;
 
 for (i=1;i<NODENO;++i)
  printf("%d\t",qvalue[i]);

 printf("\n");

}

/****************************/
/*     rand1()関数          */
/*0～1の実数を返す乱数関数  */
/****************************/
double rand1()
{
 /*乱数の計算*/ 
 return (double)rand()/RAND_MAX ;

}

/****************************/
/*     rand01()関数         */
/*　   0又は1を返す乱数関数 */
/****************************/
int rand01()
{
 int rnd ;
 
 /*乱数の最大値を除く*/
 while((rnd=rand())==RAND_MAX) ;
 /*乱数の計算*/ 
 return (int)((double)rnd/RAND_MAX*2) ;

}

/****************************/
/*     rand100()関数        */
/*　   0～100を返す乱数関数 */
/****************************/
int rand100()
{
 int rnd ;
 
 /*乱数の最大値を除く*/
 while((rnd=rand())==RAND_MAX) ;
 /*乱数の計算*/ 
 return (int)((double)rnd/RAND_MAX*101) ;

}

