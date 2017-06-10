/******************************/
/*          sum2.c            */
/*    和，二乗和を求める      */
/* 標準入力から実数を読み取り,*/
/* 和と二乗和を逐次出力します */
/* コントロールｚで終了します */
/* 使い方                     */
/* C:\Users\odaka\dl\ch1>sum2 */
/******************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/* 記号定数の定義             */
#define BUFSIZE 256  /*入力バッファサイズ*/

/****************/
/*  main()関数  */
/****************/
int main()
{
 char linebuf[BUFSIZE] ;/*入力バッファ*/
 double data ;/*入力データ*/
 double sum=0.0 ;/*和*/
 double sum2=0.0 ;/*2乗和*/

 while(fgets(linebuf,BUFSIZE,stdin)!=NULL){
  /*行の読み取りが可能な間繰り返す*/
  if(sscanf(linebuf,"%lf",&data)!=0){/*変換できたら*/
    sum+=data ;
    sum2+=data*data ;
    printf("%lf\t%lf\n",sum,sum2) ;
  }
 }

 return 0 ;
}
