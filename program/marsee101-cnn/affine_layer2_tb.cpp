// affine_layer2_tb.cpp
// 2018/05/03 by marsee
// HLS Streaming
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ap_int.h>
#include <hls_stream.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <ap_axi_sdata.h>
#include <hls_video.h>

#include "layer_general.h"
#include "affine_layer2.h"
#include "relu_affine1_output.h"
#include "af2_weight.h"
#include "af2_bias.h"

int affine_layer2 (hls::stream<ap_fixed_axis<IN_W,IN_I,1,1> >& ins,
				   hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> >& outs);
int affine_layer2_2 (hls::stream<ap_fixed_axis<IN_W,IN_I,1,1> >& ins,
					 hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> >& outs);

int affine_layer2_soft (hls::stream<float_axis<1,1> >& ins,
						hls::stream<float_axis<1,1> >& outs);

int main()
{
  using namespace std;

  hls::stream<ap_fixed_axis<IN_W,IN_I,1,1> > ins;
  hls::stream<ap_fixed_axis<IN_W,IN_I,1,1> > ins2;
  hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> > outs;
  hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> > outs2;
  hls::stream<float_axis<1,1> > ins_soft;
  hls::stream<float_axis<1,1> > outs_soft;

  mdata_type dot;
  fmdata_type fdot;

  ap_fixed_axis<IN_W,IN_I,1,1> pix;
  float_axis<1,1> fpix;
  ap_fixed_axis<OUT_W,OUT_I,1,1> pdata;
  ap_fixed_axis<OUT_W,OUT_I,1,1> pdata2;
  float_axis<1,1> fpdata;

  // ins に入力データを用意する
  for(int i=0; i<5; i++){    //　dummy data
    pix.user = 0;
    pix.data[0]= (affine_type)i;
    ins << pix;
    ins2 << pix;
    fpix.user = 0;
    fpix.data[0] = (float)i;
    ins_soft << fpix;
  }

  // 1 画面分のデータを ins、ins_soft に入力する
  for(int i=0; i < NUMBER_OF_MIDDLE_LAYER; i++){
    pix.data[0] = relu_affine1_out[i];
    fpix.data[0] = relu_affine1_fout[i];

    if (i == 0){    // 最初のデータの時に TUSER を 1 にする
      pix.user = 1;
      fpix.user = 1;
    } else {
      pix.user = 0;
      fpix.user = 0;
    }

    if (i == NUMBER_OF_MIDDLE_LAYER-1){ // 行の最後でTLASTをアサートする
      pix.last = 1;
      fpix.last = 1;
    } else {
      pix.last = 0;
      fpix.last = 0;
    }

    ins << pix;
    ins2 << pix;
    ins_soft << fpix;
  }

  affine_layer2(ins, outs);
  affine_layer2_2(ins2, outs2);
  affine_layer2_soft(ins_soft, outs_soft);

  // outs, outs_soft を dot[] と fdot[] に代入して比較する
  int errcnt = 0;
  for(int i=0; i<NUMBER_OF_OUTPUT_LAYER; i++){
    outs >> pdata;
    outs2 >> pdata2;
    outs_soft >> fpdata;

    dot.data[i] = pdata.data[0];
    fdot.data[i] = fpdata.data[0];

    printf("i = %d, HW = %f, HW2 = %f, SW = %f\n", i, (float)dot.data[i], (float)pdata2.data[0], fdot.data[i]);
    if(pdata.data[0] != pdata2.data[0]){ // 2つの実装の値が合わない
      printf("ERROR HW and SW results mismatch i = %d, HW = %f, HW2 = %f, SW = %f\n", i, (float)dot.data[i], (float)pdata2.data[0], fdot.data[i]);
      errcnt++;
      //return(1);
    }
  }

  cout << "Error Count = " << errcnt << endl;

  // max_pooling の結果をヘッダファイルに出力
  ofstream OH("affine_layer2_output.h");
  OH << "// affine_layer2_output.h" << endl;
  time_t now = time(0);
  struct tm* localNow = localtime(&now);
  OH << "// " << localNow->tm_year+1900 << "/" << localNow->tm_mon+1 << "/" << localNow->tm_mday;
  OH << " " << setw(2) << setfill('0') << localNow->tm_hour << ":" << localNow->tm_min << ":" << localNow->tm_sec << " by marsee" << endl;
  OH << "//" << endl;
  OH << endl;
  OH << "#ifndef __AFFINE_LAYER2_OUTPUT_H__" << endl;
  OH << "#define __AFFINE_LAYER2_OUTPUT_H__" << endl;
  OH << endl;
  OH << "const float affine2_fout[" << NUMBER_OF_OUTPUT_LAYER  << "] = {" << endl;
  for (int i=0; i<NUMBER_OF_OUTPUT_LAYER ; i++){
    OH << "    " << fixed << setprecision(14) << fdot.data[i];
    if (i == NUMBER_OF_OUTPUT_LAYER-1)
      OH << endl;
    else
      OH << "," << endl;
  }
  OH << "};" << endl << endl;

  OH << "const ap_fixed<12,7,AP_TRN,AP_WRAP> affine2_out[" << NUMBER_OF_OUTPUT_LAYER << "] = {" << endl;
  for (int i=0; i<NUMBER_OF_OUTPUT_LAYER ; i++){
    OH << "    " << fixed << setprecision(14) << (float)dot.data[i];
    if (i == NUMBER_OF_OUTPUT_LAYER-1)
      OH << endl;
    else
      OH << "," << endl;
  }
  OH << "};" << endl << endl;
  OH << "#endif" << endl;

  return(0);
}

int affine_layer2_soft(hls::stream<float_axis<1,1> >& ins,
                       hls::stream<float_axis<1,1> >& outs){

  float_axis<1,1> stdata;
  float dot[NUMBER_OF_OUTPUT_LAYER];
  float_axis<1,1> outd;

 Loop1: do {
    // user が 1になった時にフレームがスタートする
    ins >> stdata;
  } while(stdata.user == 0);

 Loop2: for (int i=0; i<NUMBER_OF_MIDDLE_LAYER; i++){
    if (i != 0)    // 最初の入力はすでに入力されている
      ins >> stdata;    // AXI4-Stream からの入力

  Loop4: for (int col=0; col<NUMBER_OF_OUTPUT_LAYER; col++){
      if (i == 0) // 最初は 0 にクリアする
        dot[col] = 0;

      float dot_temp = stdata.data[0] * af2_fweight[i][col];
      dot[col] += dot_temp;

      if (i == NUMBER_OF_MIDDLE_LAYER-1){ // 最後はバイアスを加算する
        dot[col] += af2_fbias[col];

        outd.data[0] = dot[col];

        if(col == 0)
          outd.user = 1;
        else
          outd.user = 0;

        if(col == NUMBER_OF_OUTPUT_LAYER-1)
          outd.last = 1;
        else
          outd.last = 0;

        outs << outd;
      }
    }
  }

  return(0);
}

// 検証用 affine_layer1_2()
// 検証用に affine_layer1() とは異なる実装でコーディング
int affine_layer2_2(hls::stream<ap_fixed_axis<IN_W,IN_I,1,1> >& ins,
                    hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> >& outs){

  ap_fixed_axis<IN_W,IN_I,1,1> stdata;
  affine_type aff2_in[NUMBER_OF_MIDDLE_LAYER];
  out_type dot2[NUMBER_OF_OUTPUT_LAYER];
  ap_fixed_axis<OUT_W,OUT_I,1,1> outd;

  do {
    // user が 1になった時にフレームがスタートする
    ins >> stdata;
  } while(stdata.user == 0);

  for(int i=0; i<NUMBER_OF_MIDDLE_LAYER; i++){
    if(i != 0) // 最初の入力はすでに入力されている
      ins >> stdata;

    aff2_in[i] = stdata.data[0];
  }

  for(int i=0; i<NUMBER_OF_OUTPUT_LAYER; i++){
    dot2[i] = 0;
    for(int j=0; j<NUMBER_OF_MIDDLE_LAYER; j++){
      dot2[i] += aff2_in[j]*af2_weight[j][i];
    }
    dot2[i] += af2_bias[i];

    outd.data[0] = dot2[i];
    if(i == 0)
      outd.user = 1;
    else
      outd.user = 0;

    if(i == NUMBER_OF_OUTPUT_LAYER-1)
      outd.last = 1;
    else
      outd.last = 0;

    outs << outd;
  }

  return(0);
}
