= TypeScriptの型システム

#@# TODO 文脈依存型
#@# TODO 文字列により特殊化されたオーバーロード
#@# TODO Generics
#@# TODO 型クエリ
#@# TODO 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.10.7
#@# TODO プロパティの文字列定数によるアクセス
#@# TODO || 演算子あたりのだるさ
#@# TODO 省略可能引数
#@# TODO オーバーロードの選択アルゴリズム

っしゃオラー！
TypeScriptの華はやはり型！
@<chapref>{typescript-basic}など所詮児戯に過ぎぬわッ！！

#@# NOTE https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#23-declarations
まず、TypeScriptに熟達していく上で必ず意識しなければいけないのが@<kw>{型の宣言空間,type declaration space}と@<kw>{値の宣言空間,variable declaration space}の存在です。
別の言い方をすると、型定義と実装の差です。

JavaやC#だと、概ね両者は密接に結びついていて、そこの差で困ることは概ねありません。
筆者が簡単に思いつく範囲では、似たような事例はJavaでのGenericsの型パラメータのtype erasureぐらいでしょうか。
JavaのGenericsの型パラメータは実行時には消されてしまうため、例えば@<code>{new T();}というコードを書くことはできません。
TypeScriptの型と値の区別は、Javaの例に近いかもしれません。

型情報としては存在していても、値として存在していない。
そういう場面は実はTypeScriptではいっぱいあるため、その差を理解し積極的に活用できるようになるとTypeScript上級者と言えるでしょう。

この章では、そんなTypeScriptの型の宣言空間で活躍する要素を紹介していきます。
実用上のテクニックは@<chapref>{definition-file}でも言及しています。

== オブジェクト型リテラル (object type literal)

オブジェクト型リテラルは、オブジェクトリテラルに似た記法で、匿名の型を作り出す機能です(@<list>{object-type-literal/basic})。

//list[object-type-literal/basic][基本的な例]{
#@mapfile(../code/with-types/object-type-literal/basic.ts)
// オブジェクトリテラルで値を作成！
var objA = {
    x: 1,
    y: 2
};

// オブジェクト型リテラルで型を作成！(値は無し
// 上記の objA の型は型推論で objB と同一になる
var objB: {
    x: number;
    y: number;
};

// おんなじ！
objA = objB;
objB = objA;
#@end
//}

値の代わりに型名を、要素の終わりに , ではなく ; を書くだけです。
簡単ですね。

オブジェクト型リテラルは型を指定する箇所@<fn>{object-literal-type}であればどこでも使うことができます(@<list>{object-type-literal/basic-usage})。

//list[object-type-literal/basic-usage][でも、正直読みづらい]{
#@mapfile(../code/with-types/object-type-literal/basic-usage.ts)
function move(value: {x: number; y: number; }, delta: {dx?: number; dy?: number;}): {x: number; y: number} {
    if(delta.dx) {
        value.x += delta.dx;
    }
    if(delta.dy) {
        value.y += delta.dy;
    }
    return value;
}

var result = move({x: 1, y: 2}, {dx: -2});
// 以下のように表示される
// {
// "x": -1,
// "y": 2
// }
console.log(JSON.stringify(result, null, 2));
#@end
//}

これから、オブジェクト型リテラルで使える書き方5つを見ていきます。

//footnote[object-literal-type][interfaceのextendsの後とかtypeofの後の識別子とかは厳密に言うと型を指定する箇所ではないのだ！！]

=== プロパティシグニチャ

1つ目は、実は既に登場しているプロパティを書くための書き方、プロパティシグニチャです(@<list>{object-type-literal/property-signiture})。

//list[object-type-literal/property-signiture][大将！いつものやつ！]{
#@mapfile(../code/with-types/object-type-literal/property-signiture.ts)
var obj: {
    property: string;
};
// 当てはまる値はこんな感じ
obj = {
    property: "Hi!"
};
#@end
//}

めっちゃわかりやすいですね。

=== コールシグニチャ

2つ目は、そのオブジェクトが関数として呼び出し可能であることを示す書き方、コールシグニチャです(@<list>{object-type-literal/call-signature})。

//list[object-type-literal/call-signature][関数として利用できる]{
#@mapfile(../code/with-types/object-type-literal/call-signature.ts)
var obj: {
    (word: string): string;
};
// 当てはまる値はこんな感じ
obj = word => "Hello, " + word;
obj = (word: string) => {
    return "Hello, " + word;
};
obj = function(word: string): string {
    return "Hello, " + word;
};
// 呼び出してみよう！
var str = obj("TypeScript");
console.log(str);
#@end
//}

一応、オーバーロードも表現可能です(@<list>{object-type-literal/call-signature-overload})。

//list[object-type-literal/call-signature-overload][オーバーロードも表現可能]{
#@mapfile(../code/with-types/object-type-literal/call-signature-overload.ts)
var obj: {
    // overloadもできるよ
    (word: string): string;
    (): number;
};
// 当てはまる値はこんな感じ
// 全ての引数と返り値に矛盾しないようにしなければならない…
obj = (word?: string): any => {
    if(typeof word === "string") {
        return "Hello, " + word;
    } else {
        return 42;
    }
};
// 呼び出してみよう！
var str = obj("TypeScript");
// Hello, TypeScript と表示される
console.log(str);

var num = obj();
// 42 と表示される
console.log(num);
#@end
//}

実装が煩雑になるのでなるべくオーバーロードは避けたいですね。

=== コンストラクタシグニチャ

3つ目は、そのオブジェクトがコンストラクタとして利用可能であることを示す書き方、コンストラクタシグニチャです(@<list>{object-type-literal/constructor-signature})。

//list[object-type-literal/constructor-signature][newできるんじゃよ]{
#@mapfile(../code/with-types/object-type-literal/constructor-signature.ts)
var clazz: {
    new (): any;
};

// 当てはまる値はこんな感じ
class Sample {
}
clazz = Sample;
var obj = new clazz();

function Hello(){
}
// 返り値が void な関数は new できるけど…
new Hello();
// 型としての互換性はない
// error TS2322: Type '() => void' is not assignable to type 'new () => any'.
// clazz = Hello;
#@end
//}

TypeScriptのコードの書き方の範疇では、クラスを定義しなければコンストラクタシグニチャにマッチするコードを書くことはできません。
型アサーションを使って@<code>{<any>}で無理やり回避する方法はありますが、あまり使わないほうがよいでしょう。

コンストラクタシグニチャも、コールシグニチャ同様にオーバーロードが可能で、引数毎に別々の型が返るような定義も可能ですが、実装するのがしちめんどくさいどころの話ではないのでほどほどにしましょう。

コンストラクタシグニチャは主に型定義ファイルの作成時にお世話になります。

=== インデックスシグニチャ

4つ目は、インデックスシグニチャです。
添字によるプロパティアクセスに対する型を表すことができます(@<list>{object-type-literal/index-signature})。

//list[object-type-literal/index-signature][プロパティアクセスカッコツキ]{
#@mapfile(../code/with-types/object-type-literal/index-signature.ts)
var objA: {
    [index:number]: string;
};
var objB: {
    [index:string]: string;
};

// どういった使い方ができるの？
var s1 = objA[1];
// --noImplicitAny 付きだとちゃんとエラーになる
// error TS7017: Index signature of object type implicitly has an 'any' type.
// var s2 = objA["test"];

// インデックスの型指定が string の場合 string でも number でもOK
var s3 = objB[1];
var s4 = objB["test"];

// 当てはまる値はこんな感じ
objA = {
    0: "str",
    // インデックスの型が number の場合、数値以外のプロパティがインデックスシグニチャで取れる値に反していてもよい
    num: 1
};
objB = {
    0: "str",
    str: "str"
    // インデックスの型が string の場合、全てのプロパティの型がインデックスシグニチャに反しないようにしなければならない
    // error TS2322: Type '{ [x: string]: string | number; 0: string; str: string; num: number; }' is not assignable to type '{ [x: string]: string; }'.
    //     Index signatures are incompatible.
    //         Type 'string | number' is not assignable to type 'string'.
    //     Type 'number' is not assignable to type 'string'.
    // num: 1
};
#@end
//}

インデックスシグニチャの型と、インデックスシグニチャ以外の(例えばプロパティシグニチャ)の型と矛盾が生じないようにする必要があります。

ちなみに、TypeScriptは@<list>{object-type-literal/index-access-sample-invalid}のように、文字列リテラルによるアクセスも許可しています。

//list[object-type-literal/index-access-sample-invalid][実は . アクセスと同じ堅牢さを誇る]{
#@mapfile(../code/with-types/object-type-literal/index-access-sample-invalid.ts)
var obj = {
    str: "string",
    num: 1
};

// 文字列リテラルによるアクセスだと普通に . アクセス同様に型情報が得られる
var str: string = obj["str"];
var num: number = obj["num"];
// ちなみに、存在しない要素にアクセスすると any になる
// --noImplicitAny を使うと暗黙的anyとしてちゃんと怒られる 萌え
// error TS7017: Index signature of object type implicitly has an 'any' type.
var any = obj["notExists"];

// 即値じゃないとダメ！コンパイラが処理できないのです
var propertyName = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
var str2 = obj[propertyName];
#@end
//}

インデックスシグニチャの利用は静的な検証の恩恵からするりと外れるを考慮し、安易に行わないようにしましょう。

=== メソッドシグニチャ

最後の5つ目は、メソッドシグニチャです。
あるプロパティがメソッドであるものとして表現することができます(@<list>{object-type-literal/method-signature})。

//list[object-type-literal/method-signature][メソッドの定義っぽい]{
#@mapfile(../code/with-types/object-type-literal/method-signature.ts)
var obj: {
    hello(word: string): string;
};

// 当てはまる値はこんな感じ
obj = {
    hello(word: string) {
        return "Hello, " + word;
    }
};
obj = {
    hello: (word: string) => "Hello, " + word
};
obj = {
    hello: function (word: string) {
        return "Hello, " + word;
    }
};

// プロパティシグニチャ + 関数型 の別の書き方なだけだな！
var obj2: {
    hello: (word: string) => string;
};
obj = obj2;
obj2 = obj;
#@end
//}

プロパティシグニチャ + 関数な型 の組み合わせでも表現できますが、メソッドシグニチャのほうがぱっと見わかりやすいですね。

== 関数型リテラル(function type literals)

関数も型として表現することができます(@<list>{function-types/basic})。

//list[function-types/basic][関数も型として表現可能]{
#@mapfile(../code/with-types/function-types/basic.ts)
var func: (value: string) => string;
// 当てはまる値はこんな感じ
func = word => "Hello, " + word;
func = (word: string) => {
    return "Hello, " + word;
};
func = function(word: string) {
    return "Hello, " + word;
};

// 型に対して実装の引数の数が少ないのはOK
func = () => "Hello, TypeScript";

// 型に対して実装の引数の数が多い場合、省略可能かデフォルト値付きでなければならない
func = (v1: string, v2 = "JavaScript") => "Hello, " + v1 + " & " + v2;
#@end
//}

アロー関数式の実装は@<code>{(word: string): string => "Hello, " + word;}という記法なのに対して、関数型リテラルは@<code>{(word: string) => string}という記法で、返り値の型を置く場所が違うので間違えないように注意が必要です。

== インタフェース

インタフェースは多くのOOPな言語に存在しているので、ご存知の型も多いでしょう。
TypeScriptのインタフェースは通常のインタフェース以上に色々な場面で登場します。
一番基本的な使い方は名前付きオブジェクト型リテラルを作ることでしょうか。
インタフェースの中に書ける記法はオブジェクト型リテラルそのまんまです。

TypeScriptでのインタフェースの酷使されっぷりを、@<list>{interface/basic}で確かみてみろ！！

//list[interface/basic][酷使されるインタフェースさん]{
#@mapfile(../code/with-types/interface/basic.ts)
// 一般的な用法
interface A {
    str: string;
}
// クラスに特定の実装を強制する
class AImpl implements A {
    str: string;
}
var objA: A = new AImpl();

// インタフェースは他のインタフェースを拡張できる
interface B1 {
    str: string;
}
interface B2 extends B1 {
    num: number;
}
// 代入する値は実装したクラスとかじゃなくてもええんじゃ！
var objB: B2 = {
    str: "string",
    num: 42
};

// interfaceはクラスすら拡張する！(実装はなかったことになる
class FooClass {
    constructor(public num: number) {}
}
interface C extends FooClass {
    str: string;
}
var objC: C = {
    num: 42,
    str: "string"
};
#@end
//}

==　構造的部分型 (Structural Subtyping)

構造的部分型は、乱暴に言うと静的型付け用のduck typingです。
TypeScriptでは、オブジェクトの互換性を型の一致ではなく、構造が一致するかどうかで判定します(@<list>{structural-subtypings/basic})。

//list[structural-subtypings/basic][大体一緒ならまぁ一緒ってことでいいじゃん]{
#@mapfile(../code/with-types/structural-subtypings/basic.ts)
// クラス Foo はstring型のstrという名前のプロパティとnumber型のnumという名前のプロパティを持つ
class Foo {
    str = "string";
    num = 1;
}

// 構造が完全に一致…！！！
// 構造が同じならもう同じってことでいいんじゃね！？
var obj: Foo = {
    str: "Hi!",
    num: 42
};
#@end
//}

そのため、@<list>{structural-subtypings/class-compat}のようなコードもコンパイルが通ります。

//list[structural-subtypings/class-compat][Pointインタフェースが要求されているが？]{
#@mapfile(../code/with-types/structural-subtypings/class-compat.ts)
interface Point {
    x: number;
    y: number;
}
// Point の実装を強制する！！！
class PointImpl1 implements Point {
    constructor(public x: number, public y: number) {}
}
// Point の実装が強制されないけど互換性はある！
class PointImpl2 implements Point {
    constructor(public x: number, public y: number) {}
}

// 引数に Point をとる
function double(p: Point): Point {
    return {
        x: p.x * 2,
        y: p.y * 2
    };
}
// 以下の全ては正しいコードとなります
double(new PointImpl1(1, 2));
double(new PointImpl2(3, 4));
double({
    x: 5,
    y: 6,
    z: 7  // 要素が多すぎる分には問題ない
});
#@end
//}

通るには通りますが、コードの堅牢性としては、きちんとimplements節を使い、クラスの仕様であると主張したほうが良いコーディングスタイルになる場合のほうが多いでしょう。

なお、optionalなプロパティは存在していなくても同じ型であるものとして扱われます(@<list>{structural-subtypings/optional})。

//list[structural-subtypings/optional][optional(?)なプロパティはなくてもよい]{
#@mapfile(../code/with-types/structural-subtypings/optional.ts)
interface Point {
    x: number;
    y: number;
    color?: string;
}
function printPoint(p:Point):void {
    var message = "x=" + p.x + ", y=" + p.y;
    if (p.color) {
        message = p.color + "(" + message + ")";
    }
    console.log(message);
}
// optinal なプロパティはなくても大丈夫！
// x=1, y=2 と表示される
printPoint({
    x: 1,
    y: 2
});
// red(x=1, y=2) と表示される
printPoint({
    x: 1,
    y: 2,
    color: "red"
});
// とはいえ、型が不一致だと怒られる。
// error TS2345: Argument of type '{ x: number; y: number; color: boolean; }' is not assignable to parameter of type 'Point'.
//   Types of property 'color' are incompatible.
//     Type 'boolean' is not assignable to type 'string'.
// printPoint({
//     x: 1,
//     y: 2,
//     color: true
// });
#@end
//}

== 共通最適型 (Best Common Type)

#@# [1.3.0での仕様](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.10)

@<strong>{導入されたバージョン 1.0.0より前}

@<strong>{消滅したバージョン 1.4.0}

後述の tuple types, 及び union types が導入されたので、Best Common Typeの概念がどうなったのか調べてみてびっくりしました。
現在の最新仕様ではBCTという概念そのものが消滅してますね…。
こぇー…。
本書執筆時点では1.3.0しかリリースされていないため、一応現行では生きてる仕様なのですが。
まぁ、そりゃunion typesに置き換えられてしまいますよね…。

#@# TODO 最新の記述にあわせる
#@# TODO 本校執筆時 https://github.com/Microsoft/TypeScript/blob/9a5df8585bdb46427074b53fc0e46bc4f52dd261/doc/spec.md#4.12.2

まぁ、消える仕様のことを書いても詮無いことなので、ここではざっくり書くにとどめます。

Best Common Type の名の響き通り、複数要素の間で型の統一がされない場合、共通最適型のアルゴリズムの元に型が決定されていました。
例えば、@<list>{bct-basic-1.3.0}のような感じ。

//list[bct-basic-1.3.0][基本的な例]{
#@mapfile(../code/with-types/bct-basic-1.3.0.ts)
// {}[] でございます。
var array = [1, true];

// No best common type exists among return expressions. と怒られます。
// つまり 返り値が {} に推論されたので怒られた。
function test() {
    if (Math.random() < 0.5) {
        return 1;
    } else {
        return true;
    }
}
#@end
//}

1つ目は配列の要素の型が一致しないため、BCTを求めた結果、共通のsuper typeがなかったので {} になっています。
2つ目は関数のreturnステートメントが2つありますが、両者で型が一致しなかったので {} になり、結果コンパイルエラーとして怒られています。

本当に共通の要素がある場合、それに収束したりします。
例えば、親クラスA、その子クラスB, Cがある場合、BCTはAになります。

//list[bct-class-1.3.0][あまり見かけないBCTが役に立つ例]{
#@mapfile(../code/with-types/bct-class-1.3.0.ts)
class A { }
class B extends A {
    str: string;
}
class C extends A {
    num: number;
}

// array: A[] だ！
var array = [new A(), new B(), new C()];
#@end
//}

以上、解散！

== タプル型 (tuple types)

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

@<strong>{導入されたバージョン 1.3.0}

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
var tuple: [string, number] = ["str", 1];

// 先頭を number に…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

…悲しい結果になりました。
@<code>{[1, true]}のような配列のリテラルをtuple typesに推論しないのはおそらくこういった事情でしょう。

unshiftやpopなど、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptではTypeScriptコンパイラ側で全てをフォローすることは不可能です。
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

@<strong>{導入されるバージョン 1.4.0}

はい、皆様待望の機能でございます。
言ってはならないあの界隈がよく使う用語を使って解説しないといけないのでビクビクですね。

一番最初に書いておくけど@<strong>{積極的にTypeScriptのコード書く時に使うもんじゃないぞ！！}

じゃあ解説していきましょう。
union typesはいわゆる直和型でございます。
この変数の値は、アレか、コレか、ソレ！のどれか。
どれかは知らん。
みたいな感じ。

なんのために入ったのか？というと、既存JavaScriptにより良い型定義を与えるために入った…！！と言ってしまっていいでしょう。
実際、自分でTypeScriptコード書いてる時は欲しくならないしね。
ECMAScriptさん、パターンマッチもないしー。みたいなー(@<list>{union-types-basic})。

//list[union-types-basic][型A | 型B → 新食感！]{
#@mapfile(../code/with-types/union-types-basic.ts)
var a: string | boolean;
// string | number なので以下はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean'.
// Type 'number' is not assignable to type 'boolean'.
// a = 1;

var b: boolean | number;
// a と b を合体させると string | boolean | boolean | number だ！
// 実際にはc の型は string | number | boolean となる
var c: typeof a | typeof b;
#@end
//}

ハイ、型注釈で複数の型を | で区切って書ける感じです。
既存のJavaScriptライブラリだとこういう感じの困った返り値の関数、ちょいちょいありますね。
あとは普通にTypeScriptを書いている時でもSyntaxTreeとかをコードから構築する時にはあったほうが便利かもしれません。

ご覧のとおり、union types中の型の順番とかは関係ない(交換可能)だし、union typesのunion typesとかは単純にまとめられます。
次に見る@<list>{union-types-subtype}のように、union typesに含まれる型同士が親子関係にある場合、単に親にまとめられます。
これも実用上問題ありません。
というのも、@<hd>{type-guards}で紹介する仕組みがあるからです(後で読んでね！)。

//list[union-types-subtype][要素Bが要素Aのサブタイプの場合Aにまとまる]{
#@mapfile(../code/with-types/union-types-subtype.ts)
class Base {
    str: string;
}
class Inherit extends Base {
    num: number;
}

// 明示的に指定しとくと Base | Inherit となる(今のところ
var obj: Base | Inherit;

// Base にも Inherit にも存在する要素には普通にアクセスできる
obj.str;
// num は Base には存在しないのでアウトー！
// error TS2339: Property 'num' does not exist on type 'Base | Inherit'.
// obj.num;

// Base | Inherit は実質単に Base とするのと変わらないのでこんなんやると Base に丸められる
var base: typeof obj;
// まぁ型が Base でも子クラスだから普通に代入できるけどね
base = new Inherit();
#@end
//}

この辺り、仕様書上は若干小難しく書かれているのですが、単に最も少ない要素数になるように型がまとめられていくだけです。

自然にTypeScriptを書いていて、union typesを目にする機会は3種類あります。
|| 演算子を使った時、条件(三項)演算子を使った時、配列リテラルを使った時です(@<list>{union-types-infered})。

//list[union-types-infered][要素Bが要素Aのサブタイプの場合Aにまとまる]{
#@mapfile(../code/with-types/union-types-infered.ts)
// and の型は string | boolean
var and = "str" || true;
// cond の型は string | number
var cond = true ? 1 : "str";
// array の型は (string | number | boolean)[]
var array = [1, true, "str"];
#@end
//}

一番よくお目にかかるのは配列リテラルでしょうか。
TypeScript一般のベストプラクティスとして1つの配列に複数の型の値をツッコまないほうが堅牢なコードになるため、そこをしっかり守るとあんま見ないかもしんない。

型注釈として関数を与える時は記法にちょっと気をつけないとコンパイルエラーになります(@<list>{union-types-syntax})。

//list[union-types-syntax][型名をカッコで囲うんです？]{
#@mapfile(../code/with-types/union-types-syntax.ts)
// 引数無しの返り値stringな関数 な型注釈
var func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// var a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
var b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
var c: { (): string; } | { (): boolean; };

// union typesじゃない時でも使えるけど見づらいな！
var d: (() => string);
#@end
//}

ぶっちゃけ見づらいですよね。
仕様書上でも@<href>{https://github.com/Microsoft/TypeScript/issues/1267,カッコの対応ミスってた}りするので、頑張って気をつけましょう。
まぁ、コンパイルすればわかるしー…みたいなー。

union typesな値を使う時は、一応型アサーションも使えますがなるべくなら避けて通りましょう(@<list>{union-types-type-assertion})。
次に説明する@<hd>{type-guards}を使おう！まずはそっちだ！

//list[union-types-type-assertion][一応使えるよ こうすれば]{
#@mapfile(../code/with-types/union-types-type-assertion.ts)
// 注意！ここでやってるやり方よりもtype guardsを使うんだ…！！
// 型アサーションは悪い。常に悪い。なるべく使わないこと。

var obj: string | number | Date;

// string 扱いしてみる
(<string>obj).charAt(0);

// number 扱いしてみる
(<number>obj).toFixed(2);

// Date 扱いしてみる
(<Date>obj).getTime();

// 値の集合に含まれない型にしてみると普通に怒られる
// error TS2352: Neither type 'string | number | Date' nor type 'RegExp' is assignable to the other.
// (<RegExp>obj).test("test");
#@end
//}

色々試してみるが、期待以上に頭がよかったりはしない(@<list>{union-types-cant-infered-invalid})。

//list[union-types-cant-infered-invalid][こういうのは型推論できない]{
#@mapfile(../code/with-types/union-types-cant-infered-invalid.ts)
function test<T>(...args:T[]):T[] {
    return args;
}
// (number | boolean)[] にはならなかった。残念。
// error TS2453: The type argument for type parameter 'T' cannot be inferred from the usage. Consider specifying the type arguments explicitly.
var v = test(1, true);
#@end
//}

#@# TODO Contextual Union Types https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.4.1

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

=={type-guards} type guards

@<strong>{導入されるバージョン 1.4.0}

type guardsは、union typesが導入されたことで変数の型が一意ではなくなってしまったため、それを自然に解決するために導入された仕組みです。
type guardsは"変数Aが○○という条件を満たす時、変数Aの型は××である"というルールを用いて、条件チェックを行った後の変数の型を××に狭めることができます。

=== typeof による type guards

JavaScriptの typeof は指定した値がどういう性質のオブジェクトかを調べ、文字列で返す演算子です。
ECMAScript 5の範囲では、変換ルールは以下の通り。

 * string の時は "string" を返す。
 * boolean の時は "boolean" を返す。
 * number の時は "number" を返す。
 * undefine の時は "undefined" を返す。
 * 関数として呼び出し可能な場合 "function" を返す。
 * それ以外の場合(nullを含む！)は "object" を返す。

これを利用して、変数の型を狭めます。

==== 使い方

一番簡単な使い方から見ていきましょう(@<list>{type-guards-typeof-basic})。
TypeScriptのtype guardsでは、typeofの結果が string, boolean, number の場合、その型に絞り込むことができます。

//list[type-guards-typeof-basic][実際の型がわからないなら調べるしかないじゃない！]{
#@mapfile(../code/with-types/type-guards-typeof-basic.ts)
var obj: number | string;
if (typeof obj === "string") {
    // ここでは string と確定されている！
    obj.charAt(0);
} else {
    // ここでは消去法で number と確定されている！
    obj.toFixed(2);
}
#@end
//}

TypeScript 1.4.0 以前のTypeScriptであれば、このif文のthen節の中でも変数objの型はそのままでした。
type guardsが導入された後は"変数objがtypeofで調べた時にstringであるという条件を満たす時、変数objの型はstringである"というルールに基づき、if文のthen節の中では変数objはstringと型付けされます。
なお、この時の比較は必ず@<code>{===}を使う必要があります。
@<code>{==}ではダメです。

さて、実際にtype guardsが起こっている例を見てみます。
@<list>{type-guards-typeof-invalid}では、anyと指定された変数をtype guardsでstringに絞り込んでいます。
そのため、@<code>{obj.toFixed(0)}というstringには存在しないメソッドを呼びだそうとするとコンパイルの段階でエラーにしてくれます。

//list[type-guards-typeof-invalid][1.3.0ではエラーとして検出できぬ]{
#@mapfile(../code/with-types/type-guards-typeof-invalid.ts)
var obj:any;
if (typeof obj === "string") {
    // ここでは string と確定されている！
    // number にしか存在しないメソッドを呼ぶとコンパイルエラー！
    // error TS2339: Property 'toFixed' does not exist on type 'string'.
    obj.toFixed(0);
}
#@end
//}

うーん、便利ですね。

ちなみに、本当に型を"狭める"だけなので、@<list>{type-guards-typeof-cant-narrow}のような互換性のない型に狭めることはできません。

//list[type-guards-typeof-cant-narrow]["狭める"だけなんだなぁ]{
#@mapfile(../code/with-types/type-guards-typeof-cant-narrow.ts)
var obj:number;
if (typeof obj === "string") {
    // number を string に"狭める"ことはできない…
    // コンパイル通っちゃう
    obj.toFixed(0);
}
#@end
//}

まぁ、指定した型通りの値が入ってくるのであればなにも問題ないな！

==== 後続の型の絞込み

typeof による type guards 特有の仕様として、後続の型の絞込があります(@<list>{type-guards-typeof-removes})。

//list[type-guards-typeof-removes][型の絞込み！]{
#@mapfile(../code/with-types/type-guards-typeof-removes.ts)
var obj: number | string | boolean;

if (typeof obj === "string") {
    // ここでは string と確定されている！
    obj.charAt(0);
} else {
    // ここでは string が引かれ number | boolean;
    obj;
}

if (typeof obj === "string") {
    // ここでは string と確定されている！
    obj.charAt(0);
} else if(typeof obj === "number") {
    // ここでは number と確定されている！
    obj.toFixed(2);
} else {
    // ここでは string, number が引かれ boolean となる！
    obj;
}
#@end
//}

最初にstringとわかったら、後続のelse句ではstringは絶対入ってこないって、はっきり分かるんだね。

=== instanceof による type guards

primitive typesだけtype guardsが使えてもあんまり嬉しくないっすからね。
instanceof を使った type guards、もちろんあります！！
でも、仕様が完璧とはちょっと言い難いんですよね。

JavaScriptにおける instanceof は、ある値が指定した関数のインスタンスであるかを調べる演算子です。
プロトタイプチェーンも遡ってみていくので、親子関係にある場合もインスタンスかどうかを調べることができます。

動作例を確認しておきます(@<list>{instanceof})。

//list[instanceof][instanceof の挙動]{
#@mapfile(../code/with-types/instanceof.ts)
class Base {
}

class InheritA extends Base {
}
class InheritB extends Base {
}

var obj = new InheritA();

// true と表示される
console.log(obj instanceof Base);
// true と表示される
console.log(obj instanceof InheritA);
// false と表示される
console.log(obj instanceof InheritB);

// ｵﾗｧﾝ!!! 無理矢理親を差し替える！
InheritA.prototype = new InheritB();
obj = new InheritA();
// true と表示される
console.log(obj instanceof InheritB);
#@end
//}

オブジェクトのprototypeと一致するか順繰り見ていくだけですね。

==== 使い方

instanceofで型を絞り込みます(@<list>{type-guards-instanceof-basic})。

//list[type-guards-instanceof-basic][instanceof の挙動]{
#@mapfile(../code/with-types/type-guards-instanceof-basic.ts)
class A {
    str: string;
}
class B {
    num: number;
}

var obj: A | B;
if (obj instanceof  A) {
    // ここでは A と確定されている！
    obj.str;
}else if(obj instanceof B) {
    // ここでは B と確定されている！
    obj.num;
}
#@end
//}

typeof の type guards と違って、else句でもちゃんと調べないとダメです。
自動的に後続の型が絞り込まれたりはしません。
まぁ、primitiveな型の値と違って、親子関係があるので後続の型を絞ってよいと断言できないパターンがちょいちょいありますからね。
仕方ないね。

==== 自分で定義した型で使うには？

TypeScriptが標準で提供する(lib.d.tsに書いてある)型や、TypeScript上で定義したクラスだけが type guards の対象になる、そんなの悲しすぎ！
というわけで、それを解消する方法が用意されています。

最も簡単なのは、型定義上でも、クラスとして定義することです。
クラスはデフォルトでinstanceof による type guards に対応しています。

もう一つは、Functionと互換性をもたせたうえでprototypeプロパティを生やす(@<list>{type-guards-instanceof-prototype})！

//list[type-guards-instanceof-prototype][prototype の型が参照される]{
#@mapfile(../code/with-types/type-guards-instanceof-prototype.ts)
interface AStatic {
    new (): AInstance;
    prototype: AInstance;
}
interface AInstance {
    str: string;
}
declare var A: AStatic;

var obj: AInstance | {};
if (obj instanceof A) {
    // ここでは AInstance と確定されている！
    obj.str;
}
#@end
//}

instanceof の右側の値の、その型の、prototypeプロパティの、型！
つまり、instanceof の右側の A の型の AStatic の prototypeプロパティの、型(AInstance)！
…まわりくどい！

そもそも、公式のTypeScript Handbookの@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,Writing .d.ts files}のクラスの分割定義の箇所でも、prototypeなんかわざわざ定義してないんだなぁ…。

それどころか、執筆時点(2014/11/28)でのlib.d.tsでは組み込みオブジェクトのRegExpにprototypeプロパティが定義されてなくてコンパイルエラーになるんですよ！！@<fn>{missing-prototype-properties}
流石にこれは草生えざるを得ない…。
そもそも、これだとDefinitelyTypedのほぼ全ての型定義ファイルがtype guards未対応になっちゃうし、今あるものを頑張って対応したとしても今後送られてくる型定義ファイルについて全てに指摘して回るのはダルすぎんよ〜〜〜ｗｗｗ

というわけで、prototype propertyの代わりに、constructor signatureを持っている場合はそちらの返り値を参照するのはどう？という@<href>{https://github.com/Microsoft/TypeScript/issues/1283,提案}を行っています。
コレがそのまま通るかはわからないけど、1.4.0リリース時に仕様が改善されてたら俺のことめっちゃ褒めてくれてもいいと思います( ｰ`дｰ´)ｷﾘｯ

話を戻しましょう。
prototypeプロパティを持っているだけではダメで、Functionとの互換性を持たせる必要があります。
一番簡単なのは、インタフェースにconstructor signatureかcall signatureのどちらか、または両方を持たせることです。
もし、このどちらも行わず、Function型との互換性がなくなると、以下のようなエラーになります(@<list>{type-guards-instanceof-prototype-invalid})。

//list[type-guards-instanceof-prototype-invalid][右側はanyかFunctionと互換性のある型にしろってさ]{
#@mapfile(../code/with-types/type-guards-instanceof-prototype-invalid.ts)
interface AStatic {
    // 以下のどちらかがないと Function との互換性が無い
    // (): AInstance;
    // new (): AInstance;
    prototype: AInstance;
}
interface AInstance {
    str: string;
}
declare var A: AStatic;

var obj: any;
// error TS2359: The right-hand side of an 'instanceof' expression must be of type 'any' or of a type assignable to the 'Function' interface type.
if (obj instanceof A) {
}
#@end
//}

もう一つ、重要なコツを示しておきます。
それは、エラーメッセージの読み方です(@<list>{type-guards-instanceof-failed-invalid})。

//list[type-guards-instanceof-failed-invalid][んん？なんだこのエラーは？]{
#@mapfile(../code/with-types/type-guards-instanceof-failed-invalid.ts)
interface AStatic {
    new (): AInstance;
    // prototype: AInstance; がない！
}
interface AInstance {
    str: string;
}
declare var A: AStatic;

var obj: AInstance | Date;
if (obj instanceof A) {
    // ここでは AInstance に確定されていてほしかった…
    // error TS2339: Property 'str' does not exist on type 'Date | AInstance'.
    obj.str;
}
#@end
//}

instanceofを使ってtype guardsで型の絞込みをしたつもりのシチュエーションです。
しかし、AStaticはprototypeプロパティを持っていません。
つまり、type guardsは効力を発揮しなかったんだよ！ΩΩΩ＜な、なんだってー！
ここでのエラー原因は、"AInstanceに型を絞ることに失敗したよ！"ということです。
ですが、実際のエラーは"AInstance | Date だとstrプロパティにアクセスして安全かわかんなかったっすわ"というメッセージです。
type guardsの失敗が、別のエラーとなって間接的に表れてしまっています。
慣れていないと、このエラーとtype guardsに実は失敗している！ということが結びつきにくいので気をつけましょう。

#@# TODO https://github.com/Microsoft/TypeScript/issues/1283 が解決されない限り、definition-file.re に注意書きを書き足す

//footnote[missing-prototype-properties][https://github.com/Microsoft/TypeScript/issues/1282 として報告済み]

==== Generics と type guards

さて、ちょっと前に書いた"instanceof の右側の値の、その型の、prototypeプロパティの、型！"という表現は、実はちょっと不正確です。

prototypeのプロパティの型に、Genericsが絡むと、話がややこしくなります。
話がややこしい奴代表として、Array<T>に登場してもらいましょう(@<list>{array-declaration-invalid})。

//list[array-declaration-invalid][lib.d.tsからArrayの定義を抜粋]{
#@mapfile(../code/with-types/array-declaration-invalid.d.ts)
// lib.d.tsから抜粋 本当はもうちょっと色々ある
declare var Array: {
    new (arrayLength?: number): any[];
    (arrayLength?: number): any[];
    isArray(arg: any): boolean;
    prototype: Array<any>;
};

interface Array<T> {
    length: number;
    push(...items:T[]): number;
    pop(): T;
}
#@end
//}

instanceof で type guards で型を狭めた時、 any[] になるのかな…？と一瞬思いますが、事はそう簡単ではありません(@<list>{type-guards-instanceof-array-invalid}、@<list>{type-guards-instanceof-array}、@<list>{type-guards-instanceof-empty-array-invalid})。

//list[type-guards-instanceof-array-invalid][絞込み、失敗！]{
#@mapfile(../code/with-types/type-guards-instanceof-array-invalid.ts)
var array: number[] | Date;

if(array instanceof Array) {
    // Array.prototype の型は Array<any> つまりは any[] …！
    // any[] は number[] に代入可能だな！！型を狭められたに違いない！
    // error TS2339: Property 'length' does not exist on type 'number[] | Date'.
    // (つд⊂)ｺﾞｼｺﾞｼ (；ﾟ Дﾟ) !?
    array.length;
}
#@end
//}

//list[type-guards-instanceof-array][any[\] に絞り込むのはできる]{
#@mapfile(../code/with-types/type-guards-instanceof-array.ts)
var array: any[] | Date;

if(array instanceof Array) {
    // any[] に絞り込まれる
    array.length;
}
#@end
//}

//list[type-guards-instanceof-empty-array-invalid][{}[\] には絞り込めない…！]{
#@mapfile(../code/with-types/type-guards-instanceof-empty-array-invalid.ts)
var array: {}[] | Date;

if(array instanceof Array) {
    // error TS2339: Property 'length' does not exist on type 'Date | {}[]'.
    array.length;
}
#@end
//}

うーん、これもうわかんねぇな？
TypeScriptコンパイラのソースコードをざっくり読んだ感じだと、Array<any>を更に型パラメタをインスタンス化する前に戻して、Array<T>にしてから絞り込みのチェックをしているようなのですが、その場合Array<{}>として評価されてるのかなー、と思いきや@<list>{type-guards-instanceof-empty-array-invalid}を見る限り、そうとも言えなさそうなんですよね…。

この辺り、わかりやすいルールが提示されないと実用上使いにくくて困りますね。
とりあえず、Genericsが絡む場合はtype guardsに頼らないほうが詰まらずにすむ…という認識で良いでしょう@<fn>{type-guards-with-generics}。

#@# TODO Overloadは？状況に変化が生じていないか確認する
#@# TODO だるすぎるからなんとかして

//footnote[type-guards-with-generics][https://github.com/Microsoft/TypeScript/issues/1283 で議論されるかも？]

=== type guardsと論理演算子

&& とか || とか ? とか ! とかの論理演算子にもちゃんと対応しているよ！(@<list>{type-guards-operator})

//list[type-guards-operator][ブール代数みたいな演算に対応してる]{
#@mapfile(../code/with-types/type-guards-operator.ts)
var obj: number | boolean | string;

// && 演算子で絞込み
typeof obj === "string" && obj.charAt(0);
// 以下のようなコードはエラーになる！
// error TS2339: Property 'charAt' does not exist on type 'number'.
// typeof obj === "number" && obj.charAt(0);

// || 演算子でunion typesに
if(typeof obj === "string" || typeof obj === "boolean") {
    // string | boolean に絞り込まれる
} else {
    // 消去法で number ！ (typeof による type guards だけ)
}

// 三項演算子は普通にif文と一緒の挙動
typeof obj === "string" ? obj.charAt(0) : obj;
// 以下と等価
if (typeof obj === "string") {
    obj.charAt(0);
} else {
    obj;
}

// 一応、否定演算子にも対応している
if (!(typeof obj !== "string")) {
    // 否定の否定は普通にそのまんまstringだな！ちゃんと絞りこまれます
    obj.charAt(0);
}
#@end
//}

あんま使わない気がするな！

=== type guardsの弱点

type guardsは型システム上の仕組みだということを忘れてはいけません。
JavaScriptの実行環境とは全く関係がないのです。

TypeScriptでは、構造的部分型の仕組みにより、クラスが要求されている箇所に、互換性のある別の値を代入することができます。

その仕組を使って、@<list>{type-guards-weakspot}のようなコードが書けてしまいます。

//list[type-guards-weakspot][構造的部分型とtype guards]{
#@mapfile(../code/with-types/type-guards-weakspot.ts)
class Sample {
    str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
    str: "Hi!"
};

if (obj instanceof Sample) {
    // 型は Sample に絞られている が！ 絶対に到達しない！
    // 現在の obj はプロトタイプチェーンにSampleを持たない！
    console.log(obj.str);
}
#@end
//}

objはSampleを型として持ち、その値として互換性のあるオブジェクトリテラルを持っています。
コンパイル後のJavaScriptコード(@<list>{type-guards-weakspot.js})を見ると、objの値がSampleクラスのインスタンスではないことが一目瞭然ですが、TypeScript上で見ると勘違いしやすいです。

//list[type-guards-weakspot.js][コンパイル後のJS]{
#@mapfile(../code/with-types/type-guards-weakspot.js)
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
var obj = {
    str: "Hi!"
};
if (obj instanceof Sample) {
    console.log(obj.str);
}
#@end
//}

これを回避する方法は2つあります。
1つ目はtype guardsに頼らず、今まで通りに処理することです(@<list>{type-guards-vs-weakspot1})。
2つ目はprivateな要素をクラスに突っ込んでしまうことです(@<list>{type-guards-vs-weakspot2-invalid})。

//list[type-guards-vs-weakspot1][type guardsに頼らず生きる]{
#@mapfile(../code/with-types/type-guards-vs-weakspot1.ts)
class Sample {
    str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
    str: "Hi!"
};

if (obj !== null) {
    var str = (<Sample>obj).str;
    if(typeof str === "string") {
        console.log(str);
    }
}
#@end
//}

//list[type-guards-vs-weakspot2-invalid][privateな要素があれば構造的部分型で値を偽造できない]{
#@mapfile(../code/with-types/type-guards-vs-weakspot2-invalid.ts)
class Sample {
    private _tmp: any;
    str: string;
}

// privateなインスタンス変数があるクラスのインスタンスは偽造できない！
// error TS2322: Type '{ _tmp: null; str: string; }' is not assignable to type 'Sample'.
//     Property '_tmp' is private in type 'Sample' but not in type '{ _tmp: null; str: string; }'.
var obj: Sample = {
    _tmp: null,
    str: "Hi!"
};
#@end
//}

色々書きましたが、一番の解決策はunion typesやanyを多様せず、真っ当なコードを書けるよう設計することですね。

== 型の別名 (type alias)

@<strong>{導入されるバージョン 1.4.0}

最初に書いておきましょう。@<strong>{可能な限りtype aliasを使うな！interface使え！}
筆者はtype aliasの乱用を恐れています！

type aliasもunion typesの扱いを便利にするために導入された機能です。
機能としてはただ単に、型をひとまとまりにして、それに名前が付けられるだけです。
そんだけ。

type aliasは仕様上、interfaceと同じように利用できる場面もあります。
ですが、基本的にtype aliasはinterfaceより機能が貧弱であるためなるべく避けるほうがよいでしょう。

代表例を見てみましょう(@<list>{type-alias-basic})。

//list[type-alias-basic][頻出するunion typesに名前をつける]{
#@mapfile(../code/with-types/type-alias-basic.ts)
type FooReturns = string | number | boolean;

interface Foo {
    bar(): FooReturns;
    buzz(): FooReturns;
    barbuzz(): FooReturns;
}
#@end
//}

わかりやすいですね。
1ヶ所変更すると、関連箇所が全て更新されるのも便利です。

tuple typesに名前をつけることもできます(@<list>{type-alias-tuple})。

//list[type-alias-tuple][tuple typesに名前をつける]{
#@mapfile(../code/with-types/type-alias-tuple.ts)
// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

var c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
module alternative {
    class Point {
        constructor(public x: number, public y: number) {}
    }
    class Circle {
        constructor(public p: Point, public r: number) {}
    }
    var c: Circle = new Circle(new Point(1, 2), 3);
}
#@end
//}

素直にクラスでやれ！って感じですね。

type aliasは型に別名をつけるだけで、コンパイルされると消えてしまう存在です。
そのため、@<list>{type-alias-do-not-have-instance-invalid}のようなコードは書くことができません。

//list[type-alias-do-not-have-instance-invalid][import句とは違うのだよ！import句とは！]{
#@mapfile(../code/with-types/type-alias-do-not-have-instance-invalid.ts)
// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = string[];

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
var strArray = new StringArray();
#@end
//}

TypeScriptの仕様書にのっているtype aliasの利用例について、interfaceでの書き換えができるものを示します(@<list>{type-alias-spec-example})。
union typesが絡むもの、tuple typesが絡むもの、型クエリが絡むものだけ、interfaceで置き換えができません。

//list[type-alias-spec-example][interfaceを使うんだ！]{
#@mapfile(../code/with-types/type-alias-spec-example.ts)
// これらはinterfaceで表現不可 type aliasで正解
type StringOrNumber = string | number;
type TextObject = string | { text: string };
type Coord = [number, number];
type ObjectStatics = typeof Object;

// これらはinterfaceで表現可能
type HolidayLookup = Map<string, Date>;
interface AltHolidayLookup extends Map<string, Date> {}

type Callback = (data: string) => void;
interface AltCallback {
    (date: string): void;
}

type RecFunc = () => RecFunc;
interface AltRecFunc {
    (): AltRecFunc;
}
#@end
//}

また、type aliasではGenericsを使った名前を定義することはできません。
つまり、@<list>{type-alias-with-type-parameters-invalid}みたいなコードは文法的に正しくないためコンパイルが通りません。

//list[type-alias-with-type-parameters-invalid][こういうコードは書けないんじゃ]{
#@mapfile(../code/with-types/type-alias-with-type-parameters-invalid.ts)
// "type" Identifier "=" Type ";" という文法だからダメ
// Identifier は TypeParameters を含まない
// type-alias-with-type-parameters-invalid.ts(4,9): error TS1005: '=' expected.
type Foo<T> = Array<T> | T;

// こういうのは書けないんだ…すまんな…
var obj: Foo<number>;
obj = [1, 2];
obj = 1;
#@end
//}

こういう場合は、名前をつけずに関係各所にコピペして頑張るしかないですね…。@<fn>{type-alias-with-generics}

最後に、type aliasがいかにinterfaceに劣るかをまとめておきます。

 * interface は extends や implements とあわせて使える
 ** type aliasは無理
 * interface は定義の統合ができるので後から自由に拡張できる
 ** type aliasは無理
 * interface は Genericsが使えて型パラメータが持てる
 ** type aliasは無理
 * interface が絡んだ時のコンパイルエラーにはinterface名が表示されてわかりやすい
 ** type aliasは展開されて表示されちゃうので無理

@<strong>{interfaceでできることをtype aliasでやるな！}

//footnote[type-alias-with-generics][こういうコードをmizchi君が書きたがってた https://twitter.com/mizchi/statuses/537908273865703424]
