/****************************************/
/*              kpdatagen.c             */
/*  ナップサック問題のデータ生成機      */
/* 荷物の重さと価値を乱数で生成します   */
/*使い方                                */
/*\Users\odaka\dl\ch3>kpdatagen>data.txt*/
/****************************************/


/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/*   記号定数の定義                 */
#define MAXVALUE 100 /*重さと価値の最大値*/
#define N 30 /*荷物の個数*/
#define SEED 32768 /*乱数のシード*/

/*   関数のプロトタイプの宣言      */
int randdata() ;
    /*MAXVALUE以下の整数を返す乱数関数*/

/****************/
/*  main()関数  */
/****************/
int main()
{
 int i ;

 srand(SEED) ;
 for(i=0;i<N;++i)
  printf("%ld %ld\n",randdata(),randdata()) ;
 return 0 ;
}

/**********************************/
/*     randdata()関数             */
/*MAXVALUE以下の整数を返す乱数関数*/
/**********************************/
int randdata()
{
 int rnd ;
 
 /*乱数の最大値を除く*/
 while((rnd=rand())==RAND_MAX) ;
 /*乱数の計算*/ 
 return (int)((double)rnd/RAND_MAX*MAXVALUE+1) ;

}
