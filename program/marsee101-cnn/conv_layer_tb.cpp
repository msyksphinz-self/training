// conv_layer_tb.cpp
// 2018/02/13 by marsee
// 2018/04/14 : HLS ストリーム対応
// 2018/04/24 : 検証用に異なる実装のconv_layer2()と比較
// 2018/05/06 : conv_layer_template.h に対応
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
#include "conv_layer1.h"
#include "bmp_header.h"

int conv_layer1 (hls::stream<ap_fixed_axis<9,1,1,1> >& ins,
                 hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >& outs);
int conv_layer_soft (hls::stream<ap_axiu<32,1,1,1> >& ins,
                     hls::stream<float_axis<NUMBER_OF_KERNEL,1> >& outs);
int conv_layer2 (hls::stream<ap_axiu<32,1,1,1> >&ins,
                 hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >&outs);

#define BMP_FILE_NAME   "straight_RED_rect0_00_rgb.bmp"

int main() 
{
  using namespace std;

  hls::stream<ap_fixed_axis<9,1,1,1> > ins;
  hls::stream<ap_axiu<32,1,1,1> > ins2;
  hls::stream<ap_axiu<32,1,1,1> > ins_soft;
  hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> > outs;
  hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> > outs2;
  hls::stream<float_axis<NUMBER_OF_KERNEL,1> > outs_soft;
  ap_axiu<32,1,1,1> pix;
  ap_fixed_axis<9,1,1,1> pixf;
  ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> vals;
  ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> vals2;
  float_axis<NUMBER_OF_KERNEL,1> vals_soft;

  BITMAPFILEHEADER bmpfhr; // BMPファイルのファイルヘッダ(for Read)
  BITMAPINFOHEADER bmpihr; // BMPファイルのINFOヘッダ(for Read)
  FILE *fbmpr, *fbmpw, *fbmpwf;
  int *rd_bmp;
  int *hw_conv[NUMBER_OF_KERNEL];
  int *sw_conv[NUMBER_OF_KERNEL];
  float *hw_convf[NUMBER_OF_KERNEL];
  float *sw_convf[NUMBER_OF_KERNEL];
  int blue, green, red;
  ap_uint<2> r_l;
  char fhname[100];
  char fsname[100];

  if ((fbmpr = fopen(BMP_FILE_NAME, "rb")) == NULL){ // test.bmp をオープン
    fprintf(stderr, "Can't open straight_RED_rect0_00.bmp by binary read mode\n");
    exit(1);
  }
  // bmpヘッダの読み出し
  fread(&bmpfhr.bfType, sizeof(uint16_t), 1, fbmpr);
  fread(&bmpfhr.bfSize, sizeof(uint32_t), 1, fbmpr);
  fread(&bmpfhr.bfReserved1, sizeof(uint16_t), 1, fbmpr);
  fread(&bmpfhr.bfReserved2, sizeof(uint16_t), 1, fbmpr);
  fread(&bmpfhr.bfOffBits, sizeof(uint32_t), 1, fbmpr);
  fread(&bmpihr, sizeof(BITMAPINFOHEADER), 1, fbmpr);

  // ピクセルを入れるメモリをアロケートする
  if ((rd_bmp =(int *)malloc(sizeof(int) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
    fprintf(stderr, "Can't allocate rd_bmp memory\n");
    exit(1);
  }
  for(int i=0; i<NUMBER_OF_KERNEL; i++){
    if ((hw_conv[i] =(int *)malloc(sizeof(int) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
      fprintf(stderr, "Can't allocate hw_conv[%d] memory\n", i);
      exit(1);
    }
    if ((sw_conv[i] =(int *)malloc(sizeof(int) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
      fprintf(stderr, "Can't allocate sw_conv[%d] memory\n", i);
      exit(1);
    }
  }

  for(int i=0; i<NUMBER_OF_KERNEL; i++){
    if ((hw_convf[i] =(float *)malloc(sizeof(float) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
      fprintf(stderr, "Can't allocate hw_convf[%d] memory\n", i);
      exit(1);
    }
    if ((sw_convf[i] =(float *)malloc(sizeof(float) * (bmpihr.biWidth * bmpihr.biHeight))) == NULL){
      fprintf(stderr, "Can't allocate sw_convf[%d] memory\n", i);
      exit(1);
    }
  }

  // rd_bmp にBMPのピクセルを代入。その際に、行を逆転する必要がある
  for (int y=0; y<bmpihr.biHeight; y++){
    for (int x=0; x<bmpihr.biWidth; x++){
      blue = fgetc(fbmpr);
      green = fgetc(fbmpr);
      red = fgetc(fbmpr);
      rd_bmp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] = (blue & 0xff) | ((green & 0xff)<<8) | ((red & 0xff)<<16);
    }
  }
  fclose(fbmpr);

  // ins に入力データを用意する
  for(int i=0; i<5; i++){    //　dummy data
    pix.user = 0;
    pixf.user = 0;

    pix.data = i;
    pixf.data[0] = i;

    pix.last = 0;
    pixf.last = 0;

    ins << pixf;
    ins2 << pix;
    ins_soft << pix;
  }

  // 1 画面分のデータを ins、ins_soft に入力する
  for(int j=0; j < bmpihr.biHeight; j++){
    for(int i=0; i < bmpihr.biWidth; i++){
      pix.data = (ap_uint<32>)rd_bmp[(j*bmpihr.biWidth)+i];
      pixf.data[0] = (ap_fixed<9,1,AP_TRN,AP_WRAP>)((ap_ufixed<16,8,AP_TRN,AP_WRAP>)(pix.data & 0xff) / 256);

      if (j==0 && i==0) {   // 最初のデータの時に TUSER を 1 にする
        pix.user = 1;
        pixf.user = 1;
      } else {
        pix.user = 0;
        pixf.user = 0;
      }

      if (i == bmpihr.biWidth-1) { // 行の最後でTLASTをアサートする
        pix.last = 1;
        pixf.last = 0;
      } else {
        pix.last = 0;
        pixf.last = 0;
      }

      ins << pixf;
      ins2 << pix;
      ins_soft << pix;
    }
  }

  // 畳み込み演算
  conv_layer1(ins, outs);
  conv_layer2(ins2, outs2);
  conv_layer_soft(ins_soft, outs_soft);

  // 画像サイズの縮小(畳み込みをすると行、列共に -4
  bmpfhr.bfSize = (HORIZONTAL_PIXEL_WIDTH-4) * (VERTICAL_PIXEL_WIDTH-4) * 3 + 54;
  bmpihr.biHeight = VERTICAL_PIXEL_WIDTH - 4;
  bmpihr.biWidth = HORIZONTAL_PIXEL_WIDTH - 4;

  // ハードウェアとソフトウェアのラプラシアン・フィルタの値のチェック
  out_type val[NUMBER_OF_KERNEL];
  out_type val2[NUMBER_OF_KERNEL];
  float val_soft[NUMBER_OF_KERNEL];

  cout << endl;
  cout << "outs" << endl;
  int errcnt=0;
  for(int j=0; j < bmpihr.biHeight; j++){
    for(int i=0; i < bmpihr.biWidth; i++){
      outs >> vals;
      outs2 >> vals2;
      outs_soft >> vals_soft;

      for(int k=0; k<NUMBER_OF_KERNEL; k++){
        val[k] = vals.data[k];
        val2[k] = vals2.data[k];
        val_soft[k] = vals_soft.data[k];

        int *hw_convp = hw_conv[k];
        int *sw_convp = sw_conv[k];
        hw_convp[(j*bmpihr.biWidth)+i] = ((int)val[k]+32)*4; // 32を足して負の符号を排除し、整数部6ビットなので、2ビット分補正する
        sw_convp[(j*bmpihr.biWidth)+i] = ((int)val_soft[k]+32)*4;

        float *hw_convfp = hw_convf[k];
        float *sw_convfp = sw_convf[k];
        hw_convfp[(j*bmpihr.biWidth)+i] = (float)val[k];
        sw_convfp[(j*bmpihr.biWidth)+i] = val_soft[k];
        if (val[k] != val2[k]){
          printf("ERROR val and val2 results mismatch i = %d, j = %d, val[%d] = %f, val2[%d] = %f\n", i, j, k, (float)val[k], k, (float)val2[k]);
          errcnt++;
          //return(1);
        }
        printf("HW and SW results i = %d, j = %d, HW[%d] = %f, HW2[%d] = %f, SW[%d] = %f\n", i, j, k, (float)val[k], k, (float)val2[k], k, val_soft[k]);
      }
    }
  }
  cout << "Error Count = " << errcnt << endl;
  cout << "Success HW and SW results match" << endl;
  cout << endl;

  // ハードウェアの畳み込み演算の結果を temp_conv0.bmp, temp_conv1.bmp に出力する
  for (int k=0; k<NUMBER_OF_KERNEL; k++){
    if (k==0){
      if ((fbmpw=fopen("temp_conv0.bmp", "wb")) == NULL){
        fprintf(stderr, "Can't open temp_conv0.bmp by binary write mode\n");
        exit(1);
      }
    } else {
      if ((fbmpw=fopen("temp_conv1.bmp", "wb")) == NULL){
        fprintf(stderr, "Can't open temp_conv1.bmp by binary write mode\n");
        exit(1);
      }
    }

    // BMPファイルヘッダの書き込み
    fwrite(&bmpfhr.bfType, sizeof(uint16_t), 1, fbmpw);
    fwrite(&bmpfhr.bfSize, sizeof(uint32_t), 1, fbmpw);
    fwrite(&bmpfhr.bfReserved1, sizeof(uint16_t), 1, fbmpw);
    fwrite(&bmpfhr.bfReserved2, sizeof(uint16_t), 1, fbmpw);
    fwrite(&bmpfhr.bfOffBits, sizeof(uint32_t), 1, fbmpw);
    fwrite(&bmpihr, sizeof(BITMAPINFOHEADER), 1, fbmpw);
    // RGB データの書き込み、逆順にする
    for (int y=0; y<bmpihr.biHeight; y++){
      for (int x=0; x<bmpihr.biWidth; x++){
        int *hw_convp = hw_conv[k];
        blue = hw_convp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] & 0xff;
        green = blue;
        red = blue;

        fputc(blue, fbmpw);
        fputc(green, fbmpw);
        fputc(red, fbmpw);
      }
    }
    fclose(fbmpw);
  }

  // ソフトウェアの畳み込み演算の結果を temp_conv_float0.bmp, temp_conv_float1.bmp に出力する
  for(int k=0; k<2; k++){
    if (k == 0){
      if ((fbmpwf=fopen("temp_conv_float0.bmp", "wb")) == NULL){
        fprintf(stderr, "Can't open temp_conv_float0.bmp by binary write mode\n");
        exit(1);
      }
    } else {
      if ((fbmpwf=fopen("temp_conv_float1.bmp", "wb")) == NULL){
        fprintf(stderr, "Can't open temp_conv_float1.bmp by binary write mode\n");
        exit(1);
      }
    }

    // BMPファイルヘッダの書き込み
    fwrite(&bmpfhr.bfType, sizeof(uint16_t), 1, fbmpwf);
    fwrite(&bmpfhr.bfSize, sizeof(uint32_t), 1, fbmpwf);
    fwrite(&bmpfhr.bfReserved1, sizeof(uint16_t), 1, fbmpwf);
    fwrite(&bmpfhr.bfReserved2, sizeof(uint16_t), 1, fbmpwf);
    fwrite(&bmpfhr.bfOffBits, sizeof(uint32_t), 1, fbmpwf);
    fwrite(&bmpihr, sizeof(BITMAPINFOHEADER), 1, fbmpwf);
    // RGB データの書き込み、逆順にする
    for (int y=0; y<bmpihr.biHeight; y++){
      for (int x=0; x<bmpihr.biWidth; x++){
        int *sw_convp = sw_conv[k];
        blue = sw_convp[((bmpihr.biHeight-1)-y)*bmpihr.biWidth+x] & 0xff;
        green = blue;
        red = blue;

        fputc(blue, fbmpwf);
        fputc(green, fbmpwf);
        fputc(red, fbmpwf);
      }
    }
    fclose(fbmpwf);
  }

  // ヘッダ出力
  ofstream OH("conv_layer_output.h");
  OH << "// conv_layer_output.h" << endl;
  time_t now = time(0);
  struct tm* localNow = localtime(&now);
  OH << "// " << localNow->tm_year+1900 << "/" << localNow->tm_mon+1 << "/" << localNow->tm_mday;
  OH << " " << setw(2) << setfill('0') << localNow->tm_hour << ":" << localNow->tm_min << ":" << localNow->tm_sec << " by marsee" << endl;
  OH << "//" << endl;
  OH << endl;
  OH << "#ifndef __CONV_LAYER_OUTPUT_H__" << endl;
  OH << "#define __CONV_LAYER_OUTPUT_H__" << endl;
  OH << endl;
  OH << "const float conv_layer_fout[" << bmpihr.biHeight*bmpihr.biWidth << "][" << NUMBER_OF_KERNEL << "] = {" << endl;
  for (int y=0; y<bmpihr.biHeight; y++){
    for (int x=0; x<bmpihr.biWidth; x++){
      OH << "    {" << fixed << setprecision(12) << sw_convf[0][bmpihr.biWidth*y+x];
      for(int i=1; i<NUMBER_OF_KERNEL; i++){
        OH << ", " << sw_convf[i][bmpihr.biWidth*y+x];
      }
      OH << "}";
      if (y==bmpihr.biHeight-1 && x==bmpihr.biWidth-1)
        OH << endl;
      else
        OH << "," << endl;
    }
  }
  OH << "};" << endl << endl;

  OH << "const ap_fixed<16, 6, AP_TRN, AP_WRAP> conv_layer_out[" << bmpihr.biHeight*bmpihr.biWidth << "][" << NUMBER_OF_KERNEL << "] = {" << endl;
  for (int y=0; y<bmpihr.biHeight; y++){
    for (int x=0; x<bmpihr.biWidth; x++){
      OH << "    {" << hw_convf[0][bmpihr.biWidth*y+x];
      for(int i=1; i<NUMBER_OF_KERNEL; i++){
        OH << ", " <<  hw_convf[i][bmpihr.biWidth*y+x];
      }
      OH << "}";
      if (y==bmpihr.biHeight-1 && x==bmpihr.biWidth-1)
        OH << endl;
      else
        OH << "," << endl;
    }
  }
  OH << "};" << endl << endl;
  OH << "#endif" << endl;

  free(rd_bmp);
  for(int k=0; k<NUMBER_OF_KERNEL; k++){
    free(hw_conv[k]);
    free(sw_conv[k]);
    free(hw_convf[k]);
    free(sw_convf[k]);
  }

  return(0);
}

int conv_layer_soft(hls::stream<ap_axiu<32,1,1,1> >& ins,
                    hls::stream<float_axis<NUMBER_OF_KERNEL,1> >& outs){
  ap_axiu<32,1,1,1> pix;
  float_axis<NUMBER_OF_KERNEL,1> conv_out;

  hls::LineBuffer<ARRAY_SIZE-1, HORIZONTAL_PIXEL_WIDTH, float> linebuf;
  hls::Window<ARRAY_SIZE, ARRAY_SIZE, float> mbuf;

  float ap_uf_pix;
  float val;

  do {
    // user が 1になった時にフレームがスタートする
    ins >> pix;
  } while(pix.user == 0);

 Loop1: for (int y=0; y<VERTICAL_PIXEL_WIDTH; y++){
  Loop2: for (int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
      if (!(x==0 && y==0))    // 最初の入力はすでに入力されている
        ins >> pix;    // AXI4-Stream からの入力

      ap_uf_pix = (float)(pix.data & 0xff) / 256.0;
      //printf("ap_uf_pix_soft = %f\n", ap_uf_pix);

      mbuf.shift_pixels_left();    // mbuf の列を1ビット左シフト
      for(int i=0; i<ARRAY_SIZE-1; i++){
        mbuf.insert_pixel(linebuf.getval(i,x), i, ARRAY_SIZE-1);
      }
      mbuf.insert_pixel(ap_uf_pix, ARRAY_SIZE-1, ARRAY_SIZE-1);

      // LineBuffer の更新
      linebuf.shift_pixels_up(x);
      linebuf.insert_bottom_row(ap_uf_pix, x);

      // conv_layer の演算
      for (int k=0; k<NUMBER_OF_KERNEL; k++){
        val=0.0;
        for (int j=0; j<ARRAY_SIZE; j++){
          for (int i=0; i<ARRAY_SIZE; i++){
            val += mbuf.getval(j,i) * conv1_fweight[k][0][j][i];
          }
        }
        val += conv1_fbias[k];
        conv_out.data[k] = val;
      }

      // 最初のARRAY_SIZE-1行とその他の行の最初のARRAY_SIZE-1列は無効データなので出力しない
      if (x<(ARRAY_SIZE-1) || y<(ARRAY_SIZE-1))
        continue;
      else { // 有効なデータの時
        if (x==(ARRAY_SIZE-1) && y==(ARRAY_SIZE-1)){ // 最初のデータでは、TUSERをアサートする
          conv_out.user = 1;
        } else {
          conv_out.user = 0;
        }

        if (x == (HORIZONTAL_PIXEL_WIDTH-1)){    // 行の最後で TLAST をアサートする
          conv_out.last = 1;
        } else {
          conv_out.last = 0;
        }

        outs << conv_out;
      }
    }
  }
  return(0);
}

// 検証用 conv_layer2()
// 検証用に conv_layer() とは異なる実装でコーディング
int conv_layer2(hls::stream<ap_axiu<32,1,1,1> >&ins,
                hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >&outs){

  ap_axiu<32,1,1,1> pix;
  val_type conv_val[NUMBER_OF_KERNEL][VERTICAL_PIXEL_WIDTH][HORIZONTAL_PIXEL_WIDTH];
  in_type ap_uf_pix[VERTICAL_PIXEL_WIDTH][HORIZONTAL_PIXEL_WIDTH];
  ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> conv_out;

 Loop1: do {
    // user が 1になった時にフレームがスタートする
    ins >> pix;
  } while(pix.user == 0);

 Loop2: for (int y=0; y<VERTICAL_PIXEL_WIDTH; y++){
  Loop3: for (int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
      if (!(x==0 && y==0))    // 最初の入力はすでに入力されている
        ins >> pix;    // AXI4-Stream からの入力

      ap_uf_pix[y][x] = (in_type)((ap_ufixed<16, 8, AP_TRN, AP_WRAP>)(pix.data & 0xff) / 256);
    }
  }

  for(int i=0; i<NUMBER_OF_KERNEL; i++){    // カーネルの個数
    for(int j=0; j<VERTICAL_PIXEL_WIDTH-(ARRAY_SIZE-1); j++){
      for(int k=0; k<HORIZONTAL_PIXEL_WIDTH-(ARRAY_SIZE-1); k++){
        conv_val[i][j][k] = 0;
        for(int m=0; m<ARRAY_SIZE; m++){
          for(int n=0; n<ARRAY_SIZE; n++){
            conv_val[i][j][k] += (val_type)ap_uf_pix[j+m][k+n] * (val_type)conv1_weight[i][0][m][n];
          }
        }
        conv_val[i][j][k] += (val_type)conv1_bias[i];
      }
    }
  }

  for(int y=0; y<VERTICAL_PIXEL_WIDTH-(ARRAY_SIZE-1); y++){
    for(int x=0; x<HORIZONTAL_PIXEL_WIDTH-(ARRAY_SIZE-1); x++){
      for(int i=0; i<NUMBER_OF_KERNEL; i++){
        conv_out.data[i] = conv_val[i][y][x];
      }

      if (x==0 && y==0){ // 最初のデータでは、TUSERをアサートする
        conv_out.user = 1;
      } else {
        conv_out.user = 0;
      }

      if (x == (HORIZONTAL_PIXEL_WIDTH - ARRAY_SIZE)){    // 行の最後で TLAST をアサートする
        conv_out.last = 1;
      } else {
        conv_out.last = 0;
      }

      outs << conv_out;
    }
  }

  return(0);
}

