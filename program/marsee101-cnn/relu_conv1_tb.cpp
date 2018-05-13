// relu_conv1_tb.cpp
// 2018/02/20 by marsee (HLS stream)
// 2018/05/06 : relu_template.h に対応
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
#include "relu_conv1.h"
#include "conv_layer_output.h"

int relu_conv1(hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >& ins,
			   hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >& outs);

int relu_soft( hls::stream<float_axis<NUMBER_OF_KERNEL,1> >& ins,
			   hls::stream<float_axis<NUMBER_OF_KERNEL,1> >& outs);

int main(){
  using namespace std;

  hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> > ins;
  hls::stream<float_axis<NUMBER_OF_KERNEL,1> > ins_soft;
  hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> > outs;
  hls::stream<float_axis<NUMBER_OF_KERNEL,1> > outs_soft;

  float relu_fout[ALL_PIXELS][NUMBER_OF_KERNEL];
  conv_type relu_out[ALL_PIXELS][NUMBER_OF_KERNEL];

  ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> pix;
  float_axis<NUMBER_OF_KERNEL,1> fpix;

  // ins に入力データを用意する
  for(int i=0; i<5; i++){    //　dummy data
	pix.user = 0;
	for(int j=0; j<NUMBER_OF_KERNEL; j++){
	  pix.data[j] = (conv_type)i;
	}
	ins << pix;
	fpix.user = 0;
	for(int j=0; j<NUMBER_OF_KERNEL; j++){
	  fpix.data[j] = (float)i;
	}
	ins_soft << fpix;
  }

  // 1 画面分のデータを ins、ins_soft に入力する
  for(int j=0; j < VERTICAL_PIXEL_WIDTH; j++){
	for(int i=0; i < HORIZONTAL_PIXEL_WIDTH; i++){
	  for(int k=0; k<NUMBER_OF_KERNEL; k++){
		pix.data[k] = conv_layer_out[j*HORIZONTAL_PIXEL_WIDTH+i][k];
		fpix.data[k] = conv_layer_fout[j*HORIZONTAL_PIXEL_WIDTH+i][k];
	  }

	  if (j==0 && i==0){    // 最初のデータの時に TUSER を 1 にする
		pix.user = 1;
		fpix.user = 1;
	  } else {
		pix.user = 0;
		fpix.user = 0;
	  }

	  if (i == HORIZONTAL_PIXEL_WIDTH-1){ // 行の最後でTLASTをアサートする
		pix.last = 1;
		fpix.last = 1;
	  } else {
		pix.last = 0;
		fpix.last = 0;
	  }

	  ins << pix;
	  ins_soft << fpix;
	}
  }

  relu_conv1(ins, outs);
  relu_soft(ins_soft, outs_soft);

  // outs, outs_soft を　relu_out[][], relu_fout[][] に出力する
  int errcnt=0;
  for(int j=0; j < VERTICAL_PIXEL_WIDTH; j++){
	for(int i=0; i < HORIZONTAL_PIXEL_WIDTH; i++){
	  outs >> pix;
	  outs_soft >> fpix;

	  for(int k=0; k<NUMBER_OF_KERNEL; k++){
		relu_out[j*HORIZONTAL_PIXEL_WIDTH+i][k] = pix.data[k];
		relu_fout[j*HORIZONTAL_PIXEL_WIDTH+i][k] = fpix.data[k];
		if ((double)pow((double)pix.data[k]-(double)fpix.data[k],(double)2) > 4){ // 2乗誤差が4よりも大きい
		  printf("ERROR HW and SW results mismatch i = %d, j = %d, HW[%d] = %f, SW[%d] = %f\n", i, j, k, (float)pix.data[k], k, fpix.data[k]);
		  errcnt++;
		  return(1);
		}
		printf("HW and SW results i = %d, j = %d, HW[%d] = %f, SW[%d] = %f\n", i, j, k, (float)pix.data[k], k, fpix.data[k]);
	  }
	}
  }
  cout << "Error Count = " << errcnt << endl;
  cout << "Success HW and SW results match" << endl;
  cout << endl;

  // ReLU の結果をヘッダファイルに出力
  ofstream OH("relu_output.h");
  OH << "// relu_output.h" << endl;
  time_t now = time(0);
  struct tm* localNow = localtime(&now);
  OH << "// " << localNow->tm_year+1900 << "/" << localNow->tm_mon+1 << "/" << localNow->tm_mday;
  OH << " " << setw(2) << setfill('0') << localNow->tm_hour << ":" << localNow->tm_min << ":" << localNow->tm_sec << " by marsee" << endl;
  OH << "//" << endl;
  OH << endl;
  OH << "#ifndef __RELU_OUTPUT_H__" << endl;
  OH << "#define __RELU_OUTPUT_H__" << endl;
  OH << endl;
  OH << "const float relu_fout[" << VERTICAL_PIXEL_WIDTH*HORIZONTAL_PIXEL_WIDTH  << "][" << NUMBER_OF_KERNEL << "] = {" << endl;
  for (int y=0; y<VERTICAL_PIXEL_WIDTH ; y++){
	for (int x=0; x<HORIZONTAL_PIXEL_WIDTH ; x++){
	  OH << "    {" << fixed << setprecision(12) << relu_fout[HORIZONTAL_PIXEL_WIDTH*y+x][0];
	  for(int i=1; i<NUMBER_OF_KERNEL; i++){
		OH << ", " << relu_fout[HORIZONTAL_PIXEL_WIDTH*y+x][i];
	  }
	  OH << "}";
            
	  if (y==VERTICAL_PIXEL_WIDTH-1 && x==HORIZONTAL_PIXEL_WIDTH-1)
		OH << endl;
	  else
		OH << "," << endl;
	}
  }
  OH << "};" << endl << endl;

  OH << "const ap_fixed<16, 6, AP_TRN, AP_WRAP> relu_out[" << VERTICAL_PIXEL_WIDTH*HORIZONTAL_PIXEL_WIDTH  << "][" << NUMBER_OF_KERNEL << "] = {" << endl;
  for (int y=0; y<VERTICAL_PIXEL_WIDTH ; y++){
	for (int x=0; x<HORIZONTAL_PIXEL_WIDTH ; x++){
	  OH << "    {" << (float)relu_out[HORIZONTAL_PIXEL_WIDTH*y+x][0];
	  for(int i=1; i<NUMBER_OF_KERNEL; i++){
		OH << ", " <<  (float)relu_out[HORIZONTAL_PIXEL_WIDTH*y+x][1];
	  }
	  OH << "}";

	  if (y==VERTICAL_PIXEL_WIDTH -1 && x==HORIZONTAL_PIXEL_WIDTH -1)
		OH << endl;
	  else
		OH << "," << endl;
	}
  }
  OH << "};" << endl << endl;
  OH << "#endif" << endl;

  return(0);
}   


int relu_soft(hls::stream<float_axis<2,1> >& ins,
			  hls::stream<float_axis<2,1> >& outs){

  float_axis<2,1> fpix;

  do {
    // user が 1になった時にフレームがスタートする
	ins >> fpix;
  } while(fpix.user == 0);

 Loop1: for (int y=0; y<VERTICAL_PIXEL_WIDTH; y++){
  Loop2: for (int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
	  if (!(x==0 && y==0))    // 最初の入力はすでに入力されている
		ins >> fpix;    // AXI4-Stream からの入力
            
	  for(int i=0; i<NUMBER_OF_KERNEL; i++){
		if (fpix.data[i] < 0.0) // データが 0 以下だったら 0 にする
		  fpix.data[i] = 0.0;
	  }

	  outs << fpix;
	}
  }

  return(0);
}

