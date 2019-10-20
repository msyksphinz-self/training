#[allow(unused_variables)]
fn main() {
    let c1 = 'A';               // char型
    let c2 = 'a';
    assert!(c1 < c2);           // 文字コード順で大小比較
    assert!(c1.is_uppercase()); // 大文字か検査

    let c3 = '0';
    assert!(c3.is_digit(10));   // 10進数の数字か検査

    let c4 = '\t';              // タブ文字
    let c5 = '\n';              // 改行（LF）文字
    let c6 = '\'';              // シングルクオート（'）
    let c7 = '\\';              // バックスラッシュ（\）
    let c8 = '\x7F';            // 制御文字delを8ビットコードで表現（16進数で2桁）

    let c9 = '漢';              // ソースコードに直接漢字も書ける（ファイルはUTF-8形式で
                                // エンコードしておくこと）
    let c10 = '\u{5b57}';       // '字'をユニコードのエスケープコードで表現（16進数で最大6桁）
    let c11 = '\u{1f600}';      // 絵文字 😀

    assert_eq!(std::mem::size_of::<char>(), 4);  // サイズは4バイト
}
