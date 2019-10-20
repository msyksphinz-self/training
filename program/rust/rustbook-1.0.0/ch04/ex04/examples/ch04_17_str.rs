fn main() {
    let s1 = "abc1";   // &'static str型
    let s2 = "abc2";
    assert!(s1 < s2);
    assert!(s1 != s2);

    let s3 = "文字列を複数行に渡って書くと
    改行やスペースが入る";
    let s4 = "行末にバックスラッシュを付けると\
            改行などが入らない";

    assert_eq!(s3, "文字列を複数行に渡って書くと\n    改行やスペースが入る");
    assert_eq!(s4, "行末にバックスラッシュを付けると改行などが入らない");

    let s5 = "文字列に\"と\\を含める";  // バックスラッシュでエスケープ
    let s6 = r#"文字列に"と\を含める"#; // raw文字列リテラル。正規表現などに便利
    assert_eq!(s5, s6);

    let s7 = r###"このように#の数を増やすと"##"があっても大丈夫"###;
    assert_eq!(s7, "このように#の数を増やすと\"##\"があっても大丈夫");

    #[allow(unused_variables)]
    let s8 = "もちろん絵文字\u{1f600}も使える"; // もちろん絵文字😀も使える

    let fruits = "あかりんご, あおりんご\nラズベリー, ブラックベリー";

    // lines()メソッドは改行コード（\n）を含む文字列から1行ずつ
    // 取り出せるイテレータを作る
    let mut lines = fruits.lines();
    // イテレータのnext()メソッドで次の行を得る
    let apple_line = lines.next();
    assert_eq!(apple_line,   Some("あかりんご, あおりんご"));
    assert_eq!(lines.next(), Some("ラズベリー, ブラックベリー"));
    // 次の行がないならNoneが返る。
    assert_eq!(lines.next(), None);

    // りんごの行（Some(..)）の中身を取り出す
    if let Some(apples) = apple_line {
        // 「あか」で始まるかチェック
        assert!(apples.starts_with("あか"));
        // 「りんご」の文字を含むかチェック
        assert!(apples.contains("りんご"));
        // 「あお」が最初に出現する位置（UTF-8表現で何バイト目）を得る
        assert_eq!(apples.find("あお"), Some(17)); // 0始まりなので18バイト目

        // 文字列をカンマ（,）で分割するイテレータを作る
        let mut apple_iter = apples.split(",");
        assert_eq!(apple_iter.next(), Some("あかりんご"));

        let green = apple_iter.next();
        // 左側に余白がある。
        assert_eq!(green, Some(" あおりんご"));
        // Some(..)の内容にstrのtrim()メソッドを適用して余白を取り除く
        assert_eq!(green.map(str::trim), Some("あおりんご"));

        assert_eq!(apple_iter.next(), None);
    } else {
        unreachable!();  // もしここに到達したらパニックで強制終了する
    }

    // s1からs4はどれも画面上では1文字として表示される
                    // UTF-8表現
    let s1 = "a";   // 61
    let s2 = "あ";  // E3 81 82
    let s3 = "😀";  // F0 9F 98 80
    let s4 = "🇯🇵";  // F0 9F 87 AF F0 9F 87 B5

    // len()メソッドはUTF-8のバイト数を返す
    assert_eq!(s1.len(), 1);
    assert_eq!(s2.len(), 3);
    assert_eq!(s3.len(), 4);
    assert_eq!(s4.len(), 8);

    let s = "abcあいう";
    assert_eq!(s.get(0..1), Some("a"));
    assert_eq!(s.get(3..6), Some("あ"));
    assert_eq!(s.get(3..4), None);  // UTF-8として解釈できない場合

    let s = "かか\u{3099}く";  // \u{3099}は濁点文字
    println!("{}", s);        // かがく

    let mut iter = s.chars();
    assert_eq!(iter.next(), Some('か'));
    assert_eq!(iter.next(), Some('か'));
    assert_eq!(iter.next(), Some('\u{3099}'));
    assert_eq!(iter.next(), Some('く'));
    assert_eq!(iter.next(), None);

    let utf8: [u8; 4] = [0x61, 0xe3, 0x81, 0x82];
    assert_eq!(std::str::from_utf8(&utf8), Ok("aあ"));

    let bad_utf8: [u8; 2] = [0x81, 0x33];  // でたらめなバイト列
    let result2 = std::str::from_utf8(&bad_utf8);
    assert!(result2.is_err());
    println!("{:?}", result2);
    // → "Err(Utf8Error { valid_up_to: 0, error_len: Some(1) })"

    // 文字列リテラル（&'static str）から&mut strは直接は得られない
    // まず文字列リテラルをStringへ変換し、そこから&mut strを取り出す
    let mut s1 = "abcあいう".to_string();  // String型

    // &mut strを得る。これはStringが持つUTF-8バイト列を指す可変スライス
    let s2 = s1.as_mut_str();         // &mut str型

    // 英小文字を大文字に変更
    s2.make_ascii_uppercase();
    assert_eq!(s2, "ABCあいう");

    // &mut strのUTF-8バイト列を直接操作して"あ"（3バイト）を"*a*"に変更する
    let b = unsafe { s2.as_bytes_mut() };
    b[3] = b'*';
    b[4] = b'a';
    b[5] = b'*';

    // 大元のStringが変更されている
    assert_eq!(s1, "ABC*a*いう");
}
