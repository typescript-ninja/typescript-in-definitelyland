// 注意！ここでやってるやり方よりもtype guardsを使うんだ…！！
// 型アサーションは悪い。常に悪い。なるべく使わないこと。

let obj: string | number | Date = null as any;

// string 扱いしてみる
(<string>obj).charAt(0);

// number 扱いしてみる
(<number>obj).toFixed(2);

// Date 扱いしてみる
(<Date>obj).getTime();

// 値の集合に含まれない型にしてみると普通に怒られる
// error TS2352: Type 'string | number | Date' cannot be converted to type 'RegExp'.
//   Type 'Date' is not comparable to type 'RegExp'.
//     Property 'exec' is missing in type 'Date'.
// (<RegExp>obj).test("test");

export {}
