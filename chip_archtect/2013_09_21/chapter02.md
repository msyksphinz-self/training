# 第2章 Opteronの浮動小数点ユニット

- 2.1 浮動小数点のリネームレジスタファイル
- 2.2 浮動小数点リネームステージ1 : x87のスタックから絶対FPレジスタファイルへのマッピング
- 2.3 浮動小数点リネームステージ2 : 通常のレジスタリネーミング
- 2.4 浮動小数点命令スケジューラ
- 2.5 5ポート読み込みと5ポート書き込みの浮動小数点リネームレジスタファイル
- 2.6 浮動小数点演算ユニット
- 2.7 変換と分類ユニット
- 2.8 x87状態処理 : FCOMI/FCMOV と FCOM/FSTSWペア

![Opteron's Floating Point Processing Unit's](Opteron_FloatPnt_Core.jpg)

# 2.1 浮動小数点のリネームレジスタファイル

Opteronの浮動小数点リネームレジスタファイルは、88から120エントリに増加している。
このリネームレジスタファイルは言葉通りの意味のレジスタファイルである。
単一のエンティティを持っており、すべてのアーキテクチャ的な(非投機的な)値と、命令セットにより定義されたレジスタの投機的な値を保持している。

Opteronは再度72個の投機的な命令を格納することができる。
AthlonXPコアにより、投機的な命令のサポートは72命令から56命令に削減された。
AthlonXPではSSEのための128個のXMMレジスタファイルも含まれているが、88個のリネームレジスタファイルの数が増えることはなかった。

128bitのXMMレジスタはリネームレジスタファイル内の2つのエントリを使用する。
したがってOpteronは16個のXMMアーキテクチャ(リタイアした)レジスタを保持するために32エントリを利用する。
これにより、88+32エントリで120エントリとなる。

120エントリのうち40エントリはアーキテクチャの(非投機的な)レジスタの状態を保持するために使用される。
32エントリは16個のXMMレジスタ用である。残りの8つは8つのx87/MMXレジスタ用である。

さらに8つのレジスタエントリを、マイクロコードのスクラッチレジスタとして使用する。
これは時々マイクロアーキテクチャレジスタと呼ばれる。
これらのレジスタは命令セットとしては定義されておらず、プログラマからは直接見ることはできない。
これらのレジスタはマイクロコードが、三角関数や対数関数などの複雑な計算を行うために利用される。

48(40+8)エントリにより定義されるプロセッサのアーキテクチャ的な状態は、「アーキテクチャタグアレイ(Architectural Tag Array)」
として定義されている。
このエントリは、48個のアーキテクチャ的なレジスタエントリの最新の「投機的な」値を保持しており、「フューチャーファイルタグアレイ
(Future File Tag Array)」と呼ばれる。

プロセッサの投機的な状態は、分岐予測や例外の発生により解放されることがある。
これはフューチャーファイルの48個エントリをアーキテクチャタグアレイで上書きすることで実現される。

リネームレジスタファイルは90bit幅である。
浮動小数点のデータは全体で90bitまで拡張される(68bitと仮数部、18ビットの指数部、1ビットの符号ビット、3ビットの分類ビットである)。
3つの分類ビット委は、浮動小数点数の情報が付加される。
この分類ビットは、非浮動小数点数(整数)など、リネームレジスタファイルに書き込む際に拡張する必要のない値を識別するときなどに利用される。

|    | The 120 registers                  |   |    | The 90 bit registers              |   |   | Definition of the 3bit Class Code  |
|----|------------------------------------|---|----|-----------------------------------|---|---|------------------------------------|
|    | non speculative registers:         |   |    | subdivision of the 90 bits for FP |   | 0 | Zero                               |
| 8  | FP/MMX registers(arch.)            |   | 68 | Mantisse bits                     |   | 1 | Inifinity                          |
| 32 | SSE/SSE2 registers(arch.)          |   | 18 | Exponent bits                     |   | 2 | Quit NAN (Not A Number)            |
| 8  | Micro Code Scratch Registers(arch) |   | 1  | Sign bit                          |   | 3 | Signaling NAN (Not A Number)       |
|    | speculative registers              |   | 3  | Class Code bits                   |   | 4 | Denormal (very small FP number)    |
| 8  | FP/MMX registers(latest)           |   |    |                                   |   | 5 | MMX/MMX (non FP contents)          |
| 32 | SSE/SSE2 registers(lates)          |   |    |                                   |   | 6 | Normal (FP number, not very small) |
| 8  | Micro Code Scratch reg.(latest)    |   |    |                                   |   | 7 | Unsupported                        |
| 24 | Renaming speculative               |   |    |                                   |   |   |                                    |

![Opteron's Floating Point Core](Opteron_FloatPnt_Core_Ill.jpg)

