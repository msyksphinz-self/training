/*************************************/
/*              direct.c             */
/*全探索でナップサック問題を解く     */
/*使い方                             */
/*\Users\odaka\dl\ch3>direct<data.txt*/
/*************************************/


/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>


/*   記号定数の定義                 */
#define MAXVALUE 100 /*重さと価値の最大値*/
#define N 30 /*荷物の個数*/
#define WEIGHTLIMIT (N*MAXVALUE/10)
             /*重量制限*/
#define SEED 32768 /*乱数のシード*/

/*   関数のプロトタイプの宣言      */
void initparcel(int parcel [N][2]);/*荷物の初期化*/
void prints(int solution);/*解候補の出力*/
int solve(int parcel [N][2]) ;/*探索の本体*/
int pow2n(int n);/*2のべき乗*/
int calcval(int parcel[N][2],int i);/*評価値の計算*/

/****************/
/*  main()関数  */
/****************/
int main()
{
 int parcel[N][2] ;/*荷物*/
 int solution=0xfff ;/*解*/

 /*荷物の初期化*/
 initparcel(parcel) ;
 /*探索の本体*/
 solution=solve(parcel) ;
 /*解の出力*/
 prints(solution) ;
 return 0 ;
}

/****************************/
/*       solve()関数  　    */
/*　     探索の本体　　     */
/****************************/
int solve(int parcel [N][2])
{
 int i ;/*繰返しの制御*/
 int limit ;/*探索の上限*/
 int maxvalue=0 ;/*評価値の最大値*/
 int value ;/*評価値*/
 int solution ;/*解候補*/

 /*探索範囲の設定*/
 limit=pow2n(N) ;
 /*解の探索*/
 for(i=0;i<limit;++i){
  /*評価値の計算*/
  value=calcval(parcel,i) ;
  /*最大値の更新*/
  if(value>maxvalue){
   maxvalue=value ;
   solution=i ;
   printf("*** maxvalue %d\n",maxvalue) ;
  }
 }
 return solution ;
}

/****************************/
/*       calcval()関数  　  */
/*　     評価値の計算　　   */
/****************************/
int calcval(int parcel[N][2],int i)
{
 int pos ;/*遺伝子座の指定*/
 int value=0 ;/*評価値*/
 int weight=0 ;/*重量*/

 /*各遺伝子座を調べて重量と評価値を計算*/
 for(pos=0;pos<N;++pos){
  weight+=parcel[pos][0]*((i>>pos)&0x1) ;
  value+=parcel[pos][1]*((i>>pos)&0x1) ;
 }
 /*致死遺伝子の処理*/
 if(weight>=WEIGHTLIMIT) value=0 ;
 return value;
}

/****************************/
/*       pow2n()関数    　  */
/*　     2のべき乗   　　   */
/****************************/
int pow2n(int n)
{
 int pow=1 ;
 for(;n>0;--n)
  pow*=2 ;
 return pow ;
}

/****************************/
/*       prints()関数   　  */
/*　    解候補の出力   　   */
/****************************/
void prints(int solution)
{
 int i ;
 for(i=0;i<N;++i)
  printf("%1d ",(solution>>i)&0x1) ;
 printf("\n") ;
}

/****************************/
/*      initparcel()関数    */
/*　    荷物の初期化   　   */
/****************************/
void initparcel(int parcel [N][2])
{
 int i=0 ;
 while((i<N) && 
  (scanf("%d %d",&parcel[i][0],&parcel[i][1])!=EOF)){
  ++i ;
 }
}
