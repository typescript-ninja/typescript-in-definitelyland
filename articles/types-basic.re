= 型は便利だ楽しいな

#@# TODO プロパティの文字列定数によるアクセス
#@# TODO 省略可能引数

TypeScriptの華はやはり型！
@<chapref>{typescript-basic}など所詮児戯に過ぎぬッ！！

この章ではTypeScriptの型の仕組みのうち、日常的に使う箇所を重点的に解説していきます。
TypeScriptコードを書く分には使わない範囲（型定義ファイルで主に使う範囲）や、仕様的に少し複雑なものについては@<chapref>{types-advanced}で紹介します。

#@# NOTE https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#23-declarations
まず、TypeScriptに熟達していく上で必ず意識しなければいけないのが@<kw>{型の宣言空間,type declaration space}と@<kw>{値の宣言空間,variable declaration space}の存在です。
別の言い方をすると、型定義と実装の差です。

JavaやC#だと、おおむね両者は密接に結びついていて、そこの差で困ることは少ないです。
#@# OK REVIEW muo: 概ね続きは少々読みづらいので片方を別の表現にしたほうが良いです
筆者が簡単に思いつく範囲では、似たような事例はJavaでのGenericsの型パラメータのtype erasureぐらいでしょうか。
JavaのGenericsの型パラメータは実行時には消されてしまうため、たとえば@<code>{new T();}というコードを書くことはできません。
TypeScriptの型と値の区別は、Javaの例に近いかもしれません。

型情報としては存在していても、値として存在していない。
そういう場面は実はTypeScriptではいっぱいあるため、差を理解し積極的に活用できるようになるとTypeScript上級者といえるでしょう。

この章では、そんなTypeScriptの型の宣言空間で活躍する要素を紹介していきます。
実用上のテクニックは@<chapref>{types-advanced}や@<chapref>{definition-file}でも言及します。

== オブジェクト型リテラル（Object Type Literals）

オブジェクト型リテラルは、オブジェクトリテラルに似た記法で、匿名の型を作り出す機能です（@<list>{object-type-literal/basic}）。

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

値の代わりに型名を、要素の終わりに,ではなく;を書くだけです。
簡単ですね。

オブジェクト型リテラルは型を指定する箇所@<fn>{object-literal-type}であればどこでも使えます（@<list>{object-type-literal/basic-usage}）。

//list[object-type-literal/basic-usage][でも、正直読みづらい]{
#@mapfile(../code/types-basic/object-type-literal/basic-usage.ts)
function move(
      value: { x: number; y: number; },
      delta: { dx?: number; dy?: number; }
    ): { x: number; y: number } {
  if (delta.dx) {
    value.x += delta.dx;
  }
  if (delta.dy) {
    value.y += delta.dy;
  }
  return value;
}

var result = move({ x: 1, y: 2 }, { dx: -2 });
// 次のように表示される
// {
//   "x": -1,
//   "y": 2
// }
console.log(JSON.stringify(result, null, 2));
#@end
//}

では、オブジェクト型リテラルで使える書き方5つを見ていきましょう。

//footnote[object-literal-type][interfaceのextendsの後とかtypeofの後の識別子とかは厳密に言うと型を指定する箇所ではありません]

=== プロパティシグニチャ（Property Signatures）

1つ目は、実はすでに登場しているプロパティを示す記法、プロパティシグニチャです（@<list>{object-type-literal/property-signiture}）。
#@# OK REVIEW muo: 書くための書き方 は少々冗長です。プロパティの記法とするか、片方を別の表現にしたほうがよさそうです

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

=== コールシグニチャ（Call Signatures）

2つ目は、そのオブジェクトが関数として呼び出し可能であることを示す記法、コールシグニチャです（@<list>{object-type-literal/call-signature}）。
#@# OK REVIEW muo: ひとつ前と揃えて「示す書き方」→「示す記法」が良いかも

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

オーバーロードも表現できます（@<list>{object-type-literal/call-signature-overload}）。

//list[object-type-literal/call-signature-overload][オーバーロードも表現できる]{
#@mapfile(../code/types-basic/object-type-literal/call-signature-overload.ts)
var obj: {
  // overloadもできるよ
  (word: string): string;
  (): number;
};
// 当てはまる値はこんな感じ
// すべての引数と返り値に矛盾しないようにしなければならない…
obj = (word?: string): any => {
  if (typeof word === "string") {
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

=== コンストラクトシグニチャ（Construct Signatures）

3つ目は、そのオブジェクトがコンストラクタとして利用可能であることを示す記法、コンストラクトシグニチャです（@<list>{object-type-literal/constructor-signature-invalid}）。
#@# OK REVIEW muo: 例によって「示す書き方」→「示す記法」が良いかも

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

function Hello() {
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
しかし、実装するのがしちめんどくさいどころの話ではないので、ほどほどにしましょう。

コンストラクトシグニチャは主に型定義ファイルの作成時にお世話になります。

=== インデックスシグニチャ（Index Signatures）

4つ目は、インデックスシグニチャです。
添字によるプロパティアクセスに対して、型を当てはめられます（@<list>{object-type-literal/index-signature}）。

//list[object-type-literal/index-signature][プロパティアクセスカッコツキ]{
#@mapfile(../code/types-basic/object-type-literal/index-signature.ts)
var objA: {
  [index: number]: string;
};
var objB: {
  [index: string]: string;
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
  // インデックスの型が number の場合、数値以外のプロパティがインデックスシグニチャで
  // 取れる値に反していてもよい
  num: 1
};
objB = {
  0: "str",
  str: "str"
  // インデックスの型が string の場合、すべてのプロパティの型がインデックスシグニチャに
  // 反しないようにしなければならない
  // error TS2322: Type
  //  '{ [x: string]: string | number; 0: string; str: string; num: number; }'
  //     is not assignable to type
  //  '{ [x: string]: string; }'.
  //     Index signatures are incompatible.
  //         Type 'string | number' is not assignable to type 'string'.
  //     Type 'number' is not assignable to type 'string'.
  // num: 1
};
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

インデックスシグニチャの型と、インデックスシグニチャ以外（たとえばプロパティシグニチャ）の型との間に矛盾が生じないようにする必要があります。

ちなみに、TypeScriptは@<list>{object-type-literal/index-access-sample-invalid}のように、文字列リテラルによるアクセスも許可しています。

//list[object-type-literal/index-access-sample-invalid][実は . アクセスと同じ堅牢さを誇る]{
#@mapfile(../code/types-basic/object-type-literal/index-access-sample-invalid.ts)
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

インデックスシグニチャの利用は静的な検証の恩恵からするりと外れる危険性が高いため、安易に使わないようにしましょう。

=== メソッドシグニチャ（Method Signatures）

最後の5つ目は、メソッドシグニチャです。
あるプロパティがメソッドであることを表現できます（@<list>{object-type-literal/method-signature}）。

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
  hello: function(word: string) {
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

"プロパティシグニチャ+関数な型"の組み合わせでも表現できますが、メソッドシグニチャのほうがぱっと見わかりやすいですね。

== 関数型リテラル（Function Type Literals）

関数も型として表現できます（@<list>{function-types/basic}）。

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

== インタフェース（Interfaces）

インタフェースは多くのOOPな言語に存在しているので、ご存知の方も多いでしょう。
TypeScriptのインタフェースは通常のインタフェース以上に色々な場面で登場します。
TypeScriptでの一番基本的な使い方は名前付きオブジェクト型リテラルを作ることです。
インタフェースの中で許される記法はオブジェクト型リテラルそのまんまです。
#@# OK REVIEW muo: 〜に書ける→〜で許される かなー

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
  constructor(public num: number) {
  }
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

== 構造的部分型（Structural Subtyping）

構造的部分型は、乱暴に言うと静的型付け用のduck typingです。
TypeScriptでは、構造が一致するかどうかで型の互換性を判定します（@<list>{structural-subtypings/basic}）。
そこに実際の継承関係は必要ありません。

//list[structural-subtypings/basic][大体一緒ならまぁ一緒ってことでいいじゃん]{
#@mapfile(../code/types-basic/structural-subtypings/basic.ts)
// クラス Foo はstring型のstrという名前のプロパティと
// number型のnumという名前のプロパティを持つ
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
  constructor(public x: number, public y: number) {
  }
}
// Point の実装が強制されないけど互換性はある！
class PointImpl2 {
  constructor(public x: number, public y: number) {
  }
}

// 引数に Point をとる
function double(p: Point): Point {
  return {
    x: p.x * 2,
    y: p.y * 2
  };
}
// 次のすべては正しいコードとなります
double(new PointImpl1(1, 2));
double(new PointImpl2(3, 4));
double({
  x: 5,
  y: 6,
  z: 7  // 要素が多すぎる分には問題ない
});
#@end
//}

とおるにはとおりますが、コードの堅牢性としては、きちんとimplements節を使いクラスの仕様であると明示したほうが良いコーディングスタイルになります。

なお、省略可能なプロパティは存在していなくても同じ型であるものとして扱われます（@<list>{structural-subtypings/optional}）。
#@# OK REVIEW muo: 基礎のところでだいぶ「オプショナル」という表記をしてたけどこれは統一すべきなのか悩ましい感じ

//list[structural-subtypings/optional][optional(?)なプロパティはなくてもよい]{
#@mapfile(../code/types-basic/structural-subtypings/optional.ts)
interface Point {
  x: number;
  y: number;
  color?: string; // なくてもいい
}
function printPoint(p: Point): void {
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
// error TS2345: Argument of type '{ x: number; y: number; color: boolean; }'
//   is not assignable to parameter of type 'Point'.
//   Types of property 'color' are incompatible.
//     Type 'boolean' is not assignable to type 'string'.
// printPoint({
//   x: 1,
//   y: 2,
//   color: true
// });
#@end
//}

== 型アサーション（Type Assertions）

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

もちろん、互換性のない型に型アサーションで変換しようとすると怒られます（@<list>{type-assertions/invalid-assertions-invalid}）。

//list[type-assertions/invalid-assertions-invalid][stringはnumberにはなれない]{
#@mapfile(../code/types-basic/type-assertions/invalid-assertions-invalid.ts)
var str = "str";
// anyを経由しない場合、整合性の無い型アサーションは成功しない！安全！
// error TS2352: Neither type 'string' nor type 'number'
// is assignable to the other.
var num: number = <number> str;
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

ダウンキャストも実行できます（@<list>{type-assertions/class}）。
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
} else if (obj instanceof InheritB) {
  var b = <InheritB>obj;
  b.bool;
}
#@end
//}

<any>に一旦キャストすればなんにでも化けさせられるので、これは一種の切り札です。
型定義ファイルを使っているときに、その型定義ファイルに不足や誤りがある場合、型アサーションでとりあえず切り抜けたい場合があります（@<list>{type-assertions/buggy-definition-file}）。

//list[type-assertions/buggy-definition-file][親クラスから子クラスへ変換]{
#@mapfile(../code/types-basic/type-assertions/buggy-definition-file.ts)
// こういう、型定義があるとする。実装はJavaScriptが与える。
declare class Base {
  str: string;
}

var obj = new Base();
// 本当は、Baseクラスがnumプロパティも持ってるんだけど…
// 型定義が不足しているなら、anyで誤魔化せばいいじゃない！！
// キレイごとだけじゃ世の中生きていけないんじゃよ…
var num: number = (<any>obj).num;
#@end
//}

== ジェネリクス（Generic Types）

いよいよ来ました。
最後の大ボスです。
Javaなどでは総称型とも呼ばれます。

ジェネリクスなんて知らんわい！
という人も、実はすでに色々なところでお世話になっています。

=== ジェネリクスの基本

TypeScriptで一番よく使うジェネリクスを使ったクラスは、Arrayです。
例を見てみましょう(@<list>{generic-types/basic})。

//list[generic-types/basic][配列はジェネリクスに支えられております]{
#@mapfile(../code/types-basic/generic-types/basic.ts)
// string[] は実は Array<string> と同じ意味なのだ！(Arrayだけ特別に！
var strArray: Array<string> = ["a", "b", "c"];
var numArray: Array<number> = [1, 2, 3];

// ArrayのメソッドとしてforEachがある
// forEachで渡される値の型はそれぞれ違う(forEachは汎用的だ！
strArray.forEach(v => v.charAt(0));
numArray.forEach(v => v.toFixed(2));

// 頑張ってこうやって書いてもいいけど、めんどいよね
strArray.forEach((v: string) => v.charAt(0));
numArray.forEach((v: number) => v.toFixed(2));

// あと、間違った型を指定したときにエラーにならないとこわい…
// error TS2345: Argument of type '(v: RegExp) => boolean' is not assignable to
// parameter of type '(value: string, index: number, array: string[]) => void'.
// strArray.forEach((v: RegExp) => v.test("str"));
#@end
//}

実は、@<code>{string[]}という型は@<code>{Array<string>}と同じ意味なのです！
ArrayだけはTypeScriptの中で特別扱いされています。

ここで出てくる@<code>{<string>}という部分がジェネリクスの肝です。
@<code>{Array<string>}を声に出して読むと、"stringのArray"になります。
ただのArrayではないのです。
"stringの"という所が重要です。
stringを別のものにして"numberのArray"とか"RegExpのArray"と言うこともできます。
つまり、色々な型に対して、"○○のArray"と言うことができるのです。
これをプログラム上で表現すると@<code>{Array<T>}という表現になります。

ここで新しく出てきた@<code>{T}を@<kw>{型パラメータ,type parameters}と呼びます。
実際、ここで出てくるアルファベットは@<code>{T}じゃなくてもかまいせん。
@<code>{Type}でもいいですし、なんでもよいです。
ただ、慣習として他の既存の型とかぶらないようにアルファベット大文字1文字を使う場合が多いです。
代表的な例ではTypeの頭文字であるTや、アルファベット的にTの次の文字であるUや、Returnの頭文字であるRなどが使われます。

さて、ではlib.d.tsから一部を抜粋した@<list>{generic-types/array-declaration-invalid}を見てみます@<fn>{array-forEach}。

//list[generic-types/array-declaration-invalid][Array<T>が登場する]{
#@mapfile(../code/types-basic/generic-types/array-declaration-invalid.ts)
declare var Array: {
  new <T>(...items: T[]): T[];
};

interface Array<T> {
  length: number;
  push(...items: T[]): number;
  pop(): T;
  forEach(callbackfn: (value: T) => void, thisArg?: any): void;
  [n: number]: T;
}
#@end
//}

色々な所でTが使われています。
pushの定義を見ると、"○○のArrayに対して、○○の値いくつかを追加するメソッドpush"とか、"○○のArrayに対して、末尾の○○の値を1つ取得するメソッドpop"、"○○のArrayに対して、○○の値それぞれに対してcallbackFnを適用するメソッドforEach"などの、汎用化された要素がたくさんあります。

ここで、型パラメータTを実際にstringで具体化します(@<list>{generic-types/array-declaration-string-invalid})。

//list[generic-types/array-declaration-string-invalid][Array<T>が登場する]{
#@mapfile(../code/types-basic/generic-types/array-declaration-string-invalid.ts)
declare var Array: {
  new (...items: string[]): string[];
};

interface Array {
  length: number;
  push(...items: string[]): number;
  pop(): string;
  forEach(callbackfn: (value: string) => void, thisArg?: any): void;
  [n: number]: string;
}
#@end
//}

"stringのArrayに対して、stringの値をいくつか追加するメソッドpush"や、"stringのArrayに対して、末尾のstringの値を1つ取得するメソッドpop"、"stringのArrayに対して、stringの値それぞれに対してcallbackFnを適用するメソッドforEach"などになりました。
ジェネリクス、使う分にはめっちゃ簡単ですね！

このように、ジェネリクスを使うと柔軟性と堅牢さを両立することができます。

//footnote[array-forEach][紙面の都合上横幅が辛かったのでforEachの定義を大胆に切り詰めてあります…。ごめんなさい！]

=== ジェネリクスの書き方色々

ここでジェネリクスの書き方を確認しておきます（@<list>{generic-types/variation}）。

//list[generic-types/variation][Array<T>が登場する]{
#@mapfile(../code/types-basic/generic-types/variation.ts)
// それぞれのTは独立していて、関連性はない

// クラスとジェネリクス
class SampleA<T> {
  constructor(public data: T) {
  }
}
// 使い方 <string>は省略しても引数から推論可能
var objA = new SampleA<string>("str");

// インタフェースとジェネリクス
interface SampleB<T> {
  data: T;
}
// 使い方
var objB: SampleB<number> = { data: 1 };

// オブジェクト型リテラル各種
var obj: {
  new <T>(value: T): any;
  <T>(value: T): any;
  methodA<T>(value: T): any;
  // 型パラメータは複数あってもよい
  methodB<T, U>(value: T): U;
};

// 関数型
var func: <T>(array: T[]) => T;
// 使い方
func = <T>(array: T[]) => array[0];
func<number>([1, 2, 3]);

// コンストラクタ型
var ctor: new <T>(value: T) => any;
// 使い方
ctor = SampleA;
new ctor<string>("str");
#@end
//}

この中でよく使うのは、クラスとインタフェースとメソッドシグニチャの書き方でしょう。
まずはこの3パターンの書き方を覚えておくべきです。

=== 型パラメータと制約

型パラメータには満たすべき制約を設けることができます。
例を見てみましょう（@<list>{generic-types/limitation-invalid}）。

//list[generic-types/limitation-invalid][型パラメータTはBaseを継承していなければならない]{
#@mapfile(../code/types-basic/generic-types/limitation-invalid.ts)
class Base {
  str: string;
}
class InheritA extends Base {
  num: number;
}

interface Sample<T extends Base> {
  method(): T;
}

// これはOK
var objA: Sample<InheritA>;

// これはダメ RegExpはBaseを継承していない
// error TS2344: Type 'RegExp' does not satisfy the constraint 'Base'.
// Property 'str' is missing in type 'RegExp'.
var objB: Sample<RegExp>;

// これはOK 指定したオブジェクト型リテラルはBaseクラスの要件を満たす
var objC: Sample<{ str: string; }>;
#@end
//}

型パラメータが満たすべき制約をextendsの形式で指定することができます。
これにより、Tに何が指定されようとも、Baseに存在するプロパティには安全にアクセスできることがわかります。

=== 自分でジェネリクス有りのコードを書く

ジェネリクスで一番難しいのは、使うことではなく、使わせることです。
何故ならば、ジェネリクスを提供するコードというのは、何かしらの要素を抽象的なまま扱わねばならないからです。
たとえば、"○○のArray"のように、型パラメータ部分が何になっても上手く動くような設計です。

逆に言うと、実際に使うときには具体化しなければいけないわけで、ジェネリクス有りのコードは"必ず何かと組み合わせて具体化する"必要があります。
これを上手に使いこなすには一段上の設計力が要求されます。

通常の範囲では自分でジェネリクスを提供するコードを作る機会はさほど多くはありません。
ですが、そこができるようになったらだいぶ型に慣れ親しんできたといえます。
