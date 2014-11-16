= TypeScriptの型システム

 * 1.1
 ** 仕様は変わってないけどバグを直した結果breaking changeになった系
 * BCTについて
 * declaration merging for external module
 * tuple types
 * union types
 * type alias

== 型宣言空間と変数宣言空間

TBD
訳語がアレな気がするので一回specを確認する

== クラス

TBD

== インタフェース

TBD

== オブジェクト型リテラル (object type literal)

TBD

== モジュール

=== 内部モジュール (internal modules)

TBD

=== 外部モジュール (external modules)

TBD

== 共通最適型 (Best Common Types)

TBD
BCTの略のexpandを一回確認すること

== タプル型 (tuple types)

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

tuple(たぷる)は、任意の数の要素の組です。
JavaScriptではtupleはサポートされていないため、TypeScriptでのtupleはただのArrayです。

既存のJavaScriptの資産を使おうとした時に、配列の形で多値を返してくるライブラリが稀にあります。
タプル型はおそらくそういった時に使うためのもので、TypeScriptでコードを書く時に多様するものではないでしょう。
というのも、普通にコードを書いている限りでは型推論の結果としてタプル型が出てこないためです。

タプル型は型の世界でしか登場しません。
記述方法は配列の型指定に @<code>{[typeA, typeB]} のように配列の要素の代わりに型名を記述していくだけです。
記述例を見てみましょう@<list>{tuple1}。

//list[tuple1][基本的な例]{
#@mapfile(../code/with-types/tuple1.ts)
// まずは今までどおりの配列から
// TypeScript 1.3.0 ではこれはいままで通りの {}[]
// TypeScript 1.4.0? ではこれは別の箇所で解説しているunion typesとあわせて(string | number | boolean)[]
var array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
var tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
    console.log(v);
});
#@end
//}

各要素の型を指定してやると、その要素のindexでアクセスした時に適切な型で扱われるようになります。

もちろん、タプル型はGenericsと組み合わせて利用することができます(@<list>{tuple3})。

//list[tuple3][Genericsでの利用も可]{
#@mapfile(../code/with-types/tuple3.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
    return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
#@end
//}

Good!いいですね。

タプル型のない今まで(1.1.0-1以前)のTypeScriptでは@<list>{without-tuple}のような、数値添字で型安全 or 配列として利用可能 という究極の選択をしなければならなかったですからね…。
辛かった。

//list[without-tuple][TypeScript 1.1.0-1 までの苦しい運用]{
#@mapfile(../code/with-types/tuple3.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
    return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
#@end
//}

さて、タプル型の重箱の隅を見て行きましょう。
要素数が多すぎる場合、指定されていない値の型はBCTになります。
その例を見てみましょう(@<list>{tuple2})。
TODO 1.4.0でのBCTの扱いが正しいか確認する

//list[tuple2][値の要素数が多すぎる場合]{
#@mapfile(../code/with-types/tuple2.ts)
// 要素が多い分にはOKだ！
var tuple: [string, number] = ["str", 1, "test"];

// 1.4.0? では範囲外の要素の型は全ての要素のunion、つまりここでは string | number になる。
// 1.3.0 では型指定されていない要素は BCT(Best Common Type) つまりここでは {} になる
var value = tuple[2];

// 1.3.0 では以下の記述は正しい
// 1.4.0? ではダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

お次は要素の順序がズレた場合、どうなるかを見てみましょう(@<list>{tuple-unshift})。

//list[tuple-unshift][絶望に身をよじれ…！]{
#@mapfile(../code/with-types/tuple-unshift.ts)
// 1.3.0 限定！！
var tuple: [string, number] = ["str", 1];

// 先頭を number に…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

…悲しい結果になりましたね。
unshiftやpopなど、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptでは全てをフォローすることは不可能です。
タプル型を扱う場合は要素数を変更するような操作をしないほうがよいでしょう。

TypeScript 1.3.0ではもうちょっと辛いコードを書くこともできます(@<list>{tuple-unshift-1.3.0})。

//list[tuple-unshift-1.3.0][1.3.0だともっと辛い]{
#@mapfile(../code/with-types/tuple-unshift-1.3.0.ts)
// 1.3.0 限定！！
var tuple: [string, number] = ["str", 1];

// 先頭を boolean に…
// unshiftの定義は Array<T>.unshift(...items: T[]): number
// 1.3.0 の場合 T が {} なのでコンパイルが通ってしまう
tuple.unshift(true);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

結論：タプル型を過信するのはやめろ繰り返すタプル型を過信するのはやめろ！

== 直和型 (union types)

TBD
本当によく語られる直和型と一緒なのかよくよく確認する
多相が入ったのか！という反応もされたのでその辺りも調べる… variant types という用語ではないんだよなぁ…
https://twitter.com/k_matsuzaki/statuses/533873787444285442

== 型の別名 (type alias)

TBD

== その他取りこぼし

随時追加すること
