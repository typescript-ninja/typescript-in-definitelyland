= 型は便利だ楽しいな

#@# TODO プロパティの文字列定数によるアクセス
#@# TODO 省略可能引数

TypeScriptの華はやはり型！
@<chapref>{typescript-basic}など所詮児戯に過ぎぬッ！！

この章ではTypeScriptの型の仕組みのうち、日常的に使う箇所を重点的に解説していきます。
TypeScriptコードを書く分には使わない範囲(型定義ファイルで主に使う範囲)や、仕様的に少し複雑なものについては@<chapref>{types-advanced}で紹介します。

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
実用上のテクニックは@<chapref>{types-advanced}や@<chapref>{definition-file}でも言及します。

== オブジェクト型リテラル (object type literal)

オブジェクト型リテラルは、オブジェクトリテラルに似た記法で、匿名の型を作り出す機能です(@<list>{object-type-literal/basic})。

//list[object-type-literal/basic][基本的な例]{
#@mapfile(../code/types-basic/object-type-literal/basic.ts)
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

オブジェクト型リテラルは型を指定する箇所@<fn>{object-literal-type}であればどこでも使えます(@<list>{object-type-literal/basic-usage})。

//list[object-type-literal/basic-usage][でも、正直読みづらい]{
#@mapfile(../code/types-basic/object-type-literal/basic-usage.ts)
function move(
            value: {x: number; y: number; },
            delta: {dx?: number; dy?: number;}
        ): {x: number; y: number} {
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
//   "x": -1,
//   "y": 2
// }
console.log(JSON.stringify(result, null, 2));
#@end
//}

では、オブジェクト型リテラルで使える書き方5つを見ていきましょう。

//footnote[object-literal-type][interfaceのextendsの後とかtypeofの後の識別子とかは厳密に言うと型を指定する箇所ではありません。]

=== プロパティシグニチャ (property signatures)

1つ目は、実は既に登場しているプロパティを書くための書き方、プロパティシグニチャです(@<list>{object-type-literal/property-signiture})。

//list[object-type-literal/property-signiture][大将！いつものやつ！]{
#@mapfile(../code/types-basic/object-type-literal/property-signiture.ts)
var obj: {
    property: string;
};
// 当てはまる値はこんな感じ
obj = {
    property: "Hi!"
};
#@end
//}

素直でわかりやすいですね。

=== コールシグニチャ (call signatures)

2つ目は、そのオブジェクトが関数として呼び出し可能であることを示す書き方、コールシグニチャです(@<list>{object-type-literal/call-signature})。

//list[object-type-literal/call-signature][関数として利用できる]{
#@mapfile(../code/types-basic/object-type-literal/call-signature.ts)
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

オーバーロードも表現できます(@<list>{object-type-literal/call-signature-overload})。

//list[object-type-literal/call-signature-overload][オーバーロードも表現できる]{
#@mapfile(../code/types-basic/object-type-literal/call-signature-overload.ts)
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

=== コンストラクトシグニチャ (construct signatures)

3つ目は、そのオブジェクトがコンストラクタとして利用可能であることを示す書き方、コンストラクトシグニチャです(@<list>{object-type-literal/constructor-signature-invalid})。

//list[object-type-literal/constructor-signature-invalid][newできるんじゃよ]{
#@mapfile(../code/types-basic/object-type-literal/constructor-signature-invalid.ts)
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

TypeScriptのコードの書き方の範疇では、クラスを定義しなければコンストラクトシグニチャにマッチするコードを書くことはできません。
型アサーションを使って@<code>{<any>}で無理やり回避する方法はありますが、あまり使わないほうがよいでしょう。

コンストラクトシグニチャも、コールシグニチャ同様にオーバーロードが可能で、引数毎に別々の型が返るような定義も可能です。
しかし、実装するのがしちめんどくさいどころの話ではないのでほどほどにしましょう。

コンストラクトシグニチャは主に型定義ファイルの作成時にお世話になります。

=== インデックスシグニチャ (index signatures)

4つ目は、インデックスシグニチャです。
添字によるプロパティアクセスに対して、型を当てはめられます(@<list>{object-type-literal/index-signature})。

//list[object-type-literal/index-signature][プロパティアクセスカッコツキ]{
#@mapfile(../code/types-basic/object-type-literal/index-signature.ts)
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

インデックスシグニチャの型と、インデックスシグニチャ以外(例えばプロパティシグニチャ)の型との間に矛盾が生じないようにする必要があります。

ちなみに、TypeScriptは@<list>{object-type-literal/index-access-sample-invalid}のように、文字列リテラルによるアクセスも許可しています。

//list[object-type-literal/index-access-sample-invalid][実は . アクセスと同じ堅牢さを誇る]{
#@mapfile(../code/types-basic/object-type-literal/index-access-sample-invalid.ts)
var obj = {
    str: "string",
    num: 1
};

// 文字列リテラルによるアクセスだと普通に . アクセス同様の型情報が得られる
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

インデックスシグニチャの利用は静的な検証の恩恵からするりと外れる危険性が高いため、安易に使わないようにしましょう。

=== メソッドシグニチャ (method signatures)

最後の5つ目は、メソッドシグニチャです。
あるプロパティがメソッドであることを表現できます(@<list>{object-type-literal/method-signature})。

//list[object-type-literal/method-signature][メソッドの定義っぽい]{
#@mapfile(../code/types-basic/object-type-literal/method-signature.ts)
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

"プロパティシグニチャ + 関数な型"の組み合わせでも表現できますが、メソッドシグニチャのほうがぱっと見わかりやすいですね。

== 関数型リテラル(function type literals)

関数も型として表現できます(@<list>{function-types/basic})。

//list[function-types/basic][関数も型として表現できる]{
#@mapfile(../code/types-basic/function-types/basic.ts)
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

アロー関数式の実装は@<code>{(word: string): string => "Hello, " + word;}という記法なのに対して、関数型リテラルは@<code>{(word: string) => string}という記法で、返り値の型を置く場所の@<code>{=>}の前後が違うので間違えないように気をつけましょう。

== インタフェース

インタフェースは多くのOOPな言語に存在しているので、ご存知の方も多いでしょう。
TypeScriptのインタフェースは通常のインタフェース以上に色々な場面で登場します。
TypeScriptでの一番基本的な使い方は名前付きオブジェクト型リテラルを作ることです。
インタフェースの中に書ける記法はオブジェクト型リテラルそのまんまです。

TypeScriptでのインタフェースの酷使されっぷりを@<list>{interface/basic}で紹介します。

//list[interface/basic][酷使されるインタフェースさん]{
#@mapfile(../code/types-basic/interface/basic.ts)
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

== 構造的部分型 (Structural Subtyping)

構造的部分型は、乱暴に言うと静的型付け用のduck typingです。
TypeScriptでは、構造が一致するかどうかで型の互換性を判定します(@<list>{structural-subtypings/basic})。
そこに実際の継承関係は必要ありません。

//list[structural-subtypings/basic][大体一緒ならまぁ一緒ってことでいいじゃん]{
#@mapfile(../code/types-basic/structural-subtypings/basic.ts)
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

そのため、@<list>{structural-subtypings/class-compat}のようなコードもTypeScriptとしては正しいです。

//list[structural-subtypings/class-compat][Pointインタフェースが要求されているが？]{
#@mapfile(../code/types-basic/structural-subtypings/class-compat.ts)
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

通るには通りますが、コードの堅牢性としては、きちんとimplements節を使いクラスの仕様であると明示したほうが良いコーディングスタイルになります。

なお、optionalなプロパティは存在していなくても同じ型であるものとして扱われます(@<list>{structural-subtypings/optional})。

//list[structural-subtypings/optional][optional(?)なプロパティはなくてもよい]{
#@mapfile(../code/types-basic/structural-subtypings/optional.ts)
interface Point {
    x: number;
    y: number;
    color?: string; // なくてもいい
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

== 型アサーション (type assertions)

型アサーションは他の言語でいうところのキャストです。
@<list>{type-assertions/basic}のように、@<code>{<変換後型名>}と書くだけです。

//list[type-assertions/basic][型アサーション 基本例]{
#@mapfile(../code/types-basic/type-assertions/basic.ts)
var obj: any = 1;

// お前は今後 number として生きよ…
var num = <number>obj;

var str = "string";
// 非常に外道なコードを書くことができる… 人としての良識を疑う
// やめて！私をanyにしてnumberに打ち込む気なんでしょう！？anyみたいに！
num = <any>str;
#@end
//}

もちろん、互換性のない型に型アサーションで変換しようとすると怒られます(@<list>{type-assertions/invalid-assertions-invalid})。

//list[type-assertions/invalid-assertions-invalid][stringはnumberにはなれない]{
#@mapfile(../code/types-basic/type-assertions/invalid-assertions-invalid.ts)
var str = "str";
// anyを経由しない場合、整合性の無い型アサーションは成功しない！安全！
// error TS2352: Neither type 'string' nor type 'number' is assignable to the other.
var num: number = <number> str;
#@end
//}

ダウンキャストも実行できます(@<list>{type-assertions/class})。
TypeScriptコード中で一番よくみる型アサーションは、このinstanceofとの合わせ技のパターンでしょう。

//list[type-assertions/class][親クラスから子クラスへ変換]{
#@mapfile(../code/types-basic/type-assertions/class.ts)
class Base {
    str: string;
}

class InheritA extends Base {
    num: number;
}
class InheritB extends Base {
    bool: boolean;
}

// とりあえず型は親クラスとして…
var obj: Base;

// 中身が何か調べてから安全にダウンキャストする
if (obj instanceof InheritA) {
    var a = <InheritA>obj;
    a.num;
} else if(obj instanceof InheritB) {
    var b = <InheritB>obj;
    b.bool;
}
#@end
//}

<any>に一旦キャストすればなんにでも化けさせられるので、これは一種の切り札です。
型定義ファイルを使っている時に、その型定義ファイルに不足や誤りがある場合、型アサーションでとりあえず切り抜けたい場合があります(@<list>{type-assertions/buggy-definition-file})。

//list[type-assertions/buggy-definition-file][親クラスから子クラスへ変換]{
#@mapfile(../code/types-basic/type-assertions/buggy-definition-file.ts)
// こういう、型定義があるとする。実装はJavaScriptが与える。
declare class Base {
    str: string;
}

var obj = new Base();
// 本当は、Baseクラスがnumプロパティも持ってるんだけど…
// 型定義が不足しているなら、anyで誤魔化せばいいじゃない！！
// キレイ事だけじゃ世の中生きていけないんじゃよ…
var num: number = (<any>obj).num;
#@end
//}

== ジェネリクス (generics)

TBD
