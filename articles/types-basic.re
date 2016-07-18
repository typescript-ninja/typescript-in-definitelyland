= 型は便利だ楽しいな

#@# TODO プロパティの文字列定数によるアクセス
#@# TODO 省略可能引数

#@# prh:disable
TypeScriptの華はやはり型！
@<chapref>{typescript-basic}など所詮児戯に過ぎぬッ！！

#@# @suppress JapaneseAmbiguousNounConjunction
この章ではTypeScriptの型の仕組みのうち、日常的に使う箇所を重点的に解説していきます。
TypeScriptコードを書く分には使わない範囲（型定義ファイルで主に使う範囲）や、仕様的に少し複雑なものについては@<chapref>{types-advanced}で紹介します。

#@# NOTE https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#23-declarations
まず、TypeScriptに熟達していく上で必ず意識しなければいけないのが@<kw>{型の宣言空間,type declaration space}と@<kw>{値の宣言空間,variable declaration space}の存在です。
別の言い方をすると、型定義と実装の差です。

JavaやC#だと、おおむね両者は密接に結びついていて、そこの差で困ることは少ないです。
筆者が簡単に思いつく範囲では、似たような事例はJavaでのGenericsの型パラメータのtype erasureぐらいでしょうか。
JavaのGenericsの型パラメータは実行時には消されてしまうため、たとえば@<code>{new T();}というコードを書くことはできません。
TypeScriptの型と値の区別は、Javaの例に近いかもしれません。

情報として型が存在していても、値として存在していない。
そういう場面はTypeScriptではたくさん遭遇するため、差を理解し積極的に活用できるようになるとTypeScript上級者といえるでしょう。

この章では、そんなTypeScriptの型の宣言空間で活躍する要素を紹介していきます。
実用上のテクニックは@<chapref>{types-advanced}や@<chapref>{definition-file}でも言及します。

== オブジェクト型リテラル（Object Type Literals）

オブジェクト型リテラルは、JSのオブジェクトリテラルに似た記法で、匿名の型を作り出す機能です（@<list>{objectTypeLiteral/basic}）。

//list[objectTypeLiteral/basic][基本的な例]{
#@mapfile(../code/types-basic/objectTypeLiteral/basic.ts)
// オブジェクトリテラルで値を作成！
let objA = {
  x: 1,
  y: 2,
};

// オブジェクト型リテラルで型を作成！(値は無し
// 上記の objA の型は型推論で objB と同一になる
let objB: {
  x: number;
  y: number;
};
objB = { x: 1, y: 2 };

// おんなじ！
objA = objB;
objB = objA;

export { }
#@end
//}

値の代わりに型名を、要素の終わりに,ではなく;を書くだけです。
簡単ですね。

オブジェクト型リテラルは型を指定する箇所@<fn>{object-literal-type}であればどこでも使えます（@<list>{objectTypeLiteral/basic-usage}）。

//list[objectTypeLiteral/basic-usage][でも、正直読みづらい]{
#@mapfile(../code/types-basic/objectTypeLiteral/basicUsage.ts)
// 関数の仮引数や返り値に対して
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

let result = move({ x: 1, y: 2 }, { dx: -2 });
// 次のように表示される
// {
//   "x": -1,
//   "y": 2
// }
console.log(JSON.stringify(result, null, 2));

export { }
#@end
//}

では、オブジェクト型リテラルで使える書き方5つを見ていきましょう。

//footnote[object-literal-type][interfaceのextendsの後とかtypeofの後の識別子とかは厳密にいうと型を指定する箇所ではありません]

=== プロパティシグニチャ（Property Signatures）

1つ目は、実はすでに登場しているプロパティを示す記法、プロパティシグニチャです（@<list>{objectTypeLiteral/propertySigniture}）。

//list[objectTypeLiteral/propertySigniture][大将！いつものやつ！]{
#@mapfile(../code/types-basic/objectTypeLiteral/propertySigniture.ts)
let obj: {
  property: string;
};
// 当てはまる値はこんな感じ
obj = {
  property: "Hi!",
};

export { }
#@end
//}

素直でわかりやすいですね。

=== コールシグニチャ（Call Signatures）

2つ目は、そのオブジェクトが関数として呼び出し可能であることを示す記法、コールシグニチャです（@<list>{objectTypeLiteral/callSignature}）。

//list[objectTypeLiteral/callSignature][関数として利用できる]{
#@mapfile(../code/types-basic/objectTypeLiteral/callSignature.ts)
let obj: {
  (word: string): string;
};
// 当てはまる値はこんな感じ
obj = word => `Hello, ${word}`;
obj = (word: string): string => {
  return `Hello, ${word}`;
};
obj = function(word: string): string {
  return `Hello, ${word}`;
};
// 呼び出してみよう！
let str = obj("TypeScript");
console.log(str);

export { }
#@end
//}

#@# @suppress ParenthesizedSentence
オーバーロードも表現できます（@<list>{objectTypeLiteral/call-signature-overload}）。

//list[objectTypeLiteral/call-signature-overload][オーバーロードも表現できる]{
#@mapfile(../code/types-basic/objectTypeLiteral/callSignatureOverload.ts)
let obj: {
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
let str = obj("TypeScript");
// Hello, TypeScript と表示される
console.log(str);

let num = obj();
// 42 と表示される
console.log(num);

export { }
#@end
//}

実装が煩雑になるのでなるべくオーバーロードを自分のコード内で利用することは避けたいところです。

=== コンストラクトシグニチャ（Construct Signatures）

#@# @suppress ParenthesizedSentence
3つ目は、対象オブジェクトがコンストラクタとして利用可能であることを示す記法、コンストラクトシグニチャです（@<list>{objectTypeLiteral/constructorSignature}）。

//list[objectTypeLiteral/constructorSignature][newできる]{
#@mapfile(../code/types-basic/objectTypeLiteral/constructorSignature.ts)
let clazz: {
  new (): any;
};

// 当てはまる値はこんな感じ
class Sample {
}
clazz = Sample;
let obj = new clazz();

// クラス式だとこんな感じ
clazz = class { };
obj = new clazz();
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
TypeScriptでできるコードの書き方の範疇では、クラスを定義しなければコンストラクトシグニチャにマッチするコードを書くことはできません。
関数+型アサーションを使って@<code>{<any>}で無理やり回避する方法はありますが、使わないほうがよいでしょう。

#@# @suppress SuccessiveWord
コンストラクトシグニチャも、コールシグニチャ同様にオーバーロードが可能で、引数毎に別々の型が返るような定義も可能です。
しかし、実装するのも正しく利用するのもめんどうなのでほどほどにしましょう。

コンストラクトシグニチャは主に型定義ファイルの作成時にお世話になります。

=== インデックスシグニチャ（Index Signatures）

4つ目は、インデックスシグニチャです。
添字によるプロパティアクセスに対して、型を当てはめられます（@<list>{objectTypeLiteral/indexSignature}）。

//list[objectTypeLiteral/indexSignature][プロパティアクセスの例]{
#@mapfile(../code/types-basic/objectTypeLiteral/indexSignature.ts)
let objA: {
  [index: number]: string;
} = {};
let objB: {
  [index: string]: string;
} = {};

// どういった使い方ができるの？
let s1 = objA[1];
// --noImplicitAny 付きだとちゃんとエラーになる
// error TS7015: Element implicitly has an 'any' type because index expression is not of type 'number'.
// var s2 = objA["test"];

// インデックスの型指定が string の場合 string でも number でもOK
let s3 = objB[1];
let s4 = objB["test"];

// 当てはまる値はこんな感じ
objA = {
  0: "str",
  // オブジェクトリテラルで直接変数に代入する場合、変数の型に存在しない値があるとエラーになる
  // error TS2322: Type
  //  '{ 0: string; str: string; }'
  //     is not assignable to type '{ [index: number]: string; }'.
  //  Object literal may only specify known properties,
  //     and 'str' does not exist in type '{ [index: number]: string; }'.
  // str: "str",
};
// 変数にオブジェクトリテラル直で代入でなければ余計なパラメータがついていても許される
let tmp = {
  0: "str",
  str: "str",
};
objA = tmp;

objB = {
  0: "str",
  str: "str",
  // インデックスの型が string の場合、すべてのプロパティの型がインデックスシグニチャに
  // 反しないようにしなければならない
  // error TS2322: Type
  //  '{ 0: string; str: string; num: number; }'
  //     is not assignable to type
  //  '{ [index: string]: string; }'.
  //  Property 'num' is incompatible with index signature.
  //    Type 'number' is not assignable to type 'string'.
  // num: 1,
};

export { s1, s3, s4 }
#@end
//}

インデックスシグニチャの型と、インデックスシグニチャ以外（たとえばプロパティシグニチャ）の型との間に矛盾が生じないようにする必要があります。

ちなみに、TypeScriptは@<list>{objectTypeLiteral/indexAccessSample}のように、文字列リテラルによるアクセスも許可しています。

//list[objectTypeLiteral/indexAccessSample][実は . アクセスと同じ堅牢さを誇る]{
#@mapfile(../code/types-basic/objectTypeLiteral/indexAccessSample.ts)
let obj = {
  str: "string",
  num: 1,
};

// 文字列リテラルによるアクセスだと普通に . アクセス同様に型情報が得られる
let str: string = obj["str"];
let num: number = obj["num"];
// ちなみに、存在しない要素にアクセスすると any になる
// --noImplicitAny を使うと暗黙的anyとしてちゃんと怒られる 萌え
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let any = obj["notExists"];

// 即値じゃないとダメ！コンパイラが処理できないのです
let propertyName1 = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let str2 = obj[propertyName1];

// なお、string literal typesを使っても怒られます
let propertyName2: "str" = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let str3 = obj[propertyName2];

export { str, num, propertyName1, propertyName2 }
#@end
//}

インデックスシグニチャの利用は静的な検証の恩恵からするりと外れる危険性が高いため、安易に使わないようにしましょう。

=== メソッドシグニチャ（Method Signatures）

最後の5つ目は、メソッドシグニチャです。
あるプロパティがメソッドであることを表現できます（@<list>{objectTypeLiteral/methodSignature}）。

//list[objectTypeLiteral/methodSignature][メソッドの定義っぽい]{
#@mapfile(../code/types-basic/objectTypeLiteral/methodSignature.ts)
let obj: {
  hello(word: string): string;
};

// 当てはまる値はこんな感じ
obj = {
  hello(word: string) {
    return "Hello, " + word;
  },
};
obj = {
  hello: (word: string) => "Hello, " + word,
};
obj = {
  hello: function(word: string) {
    return "Hello, " + word;
  },
};

// プロパティシグニチャ + 関数型 の別の書き方なだけだな！
let obj2: {
  hello: (word: string) => string;
};
obj2 = obj;
obj = obj2;

export { }
#@end
//}

"プロパティシグニチャ+関数な型"の組み合わせでも表現できますが、メソッドシグニチャのほうがぱっと見わかりやすいですね。

#@# @suppress
=== オブジェクトリテラルと厳密なチェック

オブジェクト型リテラルの話とは厳密には関係はないですが、関わりが深いのでここでこの話題に触れていきます。

オブジェクトリテラルでもって値を作る時に、厳密なチェックが行われる場合があります。
それは、値を直接何らかの型に当てはめた場合です。
例を見てみましょう（@<list>{objectTypeLiteral/strictCheck-invalid}）。

//list[objectTypeLiteral/strictCheck-invalid][厳密にチェックされる場合、されない場合]{
#@mapfile(../code/types-basic/objectTypeLiteral/strictCheck-invalid.ts)
// OK！変数の型に対して、過不足なし
let obj: { name: string; } = {
  name: "TypeScript",
};

// NG… 変数の型に対してプロパティが多すぎる
// error TS2322: Type '{ name: string; version: string; }'
//    is not assignable to type '{ name: string; }'.
//    Object literal may only specify known properties,
//    and 'version' does not exist in type '{ name: string; }'.
obj = {
  name: "JavaScript",
  version: "2016",
};

// オブジェクトリテラルの直接代入じゃなければOK 互換性はあるのだ
let tmp = {
  name: "JavaScript",
  version: "2016",
};
obj = tmp;

// この制約はオプション名のtypoの検出に役立つ
interface FooOptions {
  fileName?: string;
  checkBar?: boolean;
}
declare function foo(opts: FooOptions): void;

// fileName の大文字小文字を間違えている！
// Object literal may only specify known properties,
// and 'filename' does not exist in type 'FooOptions'.
foo({
  filename: "vvakame.txt",
  checkBar: true,
});

export { }
#@end
//}

この制約はなかなか強力で、慣れないうちはコンパイルエラーを回避する方法がわからない場合があるかもしれません。
型定義ファイルを使っている場合、型定義ファイルに不足がある場合などもあり、正規の方法で攻略するのが難しい場合すらあります。
その場合、型定義ファイルを修正したりしてほしいところですが、急いでいる場合は一旦別の変数に代入してから再代入することで回避できます。
このやり方だと、anyにキャストするやり方よりは型の不整合の検出などの点で有利なため、まだしもマシなやり方だといえます。

== 関数型リテラル（Function Type Literals）

関数も型として表現できます（@<list>{function-types/basic}）。

//list[function-types/basic][関数も型として表現できる]{
#@mapfile(../code/types-basic/functionTypes/basic.ts)
let func: (value: string) => string;
// 当てはまる値はこんな感じ
func = word => `Hello, ${word}`;
func = (word: string) => {
  return `Hello, ${word}`;
};
func = function(word: string) {
  return `Hello, ${word}`;
};

// 型に対して実装の引数の数が少ないのはOK
func = () => "Hello, TypeScript";

// 型に対して実装の引数の数が多い場合、省略可能かデフォルト値付きでなければならない
func = (v1: string, v2 = "JavaScript") => `Hello, ${v1} & ${v2}`;
#@end
//}

#@# @suppress SentenceLength ParenthesizedSentence
アロー関数の実装は@<code>{(word: string): string => "Hello, " + word;}という記法なのに対して、関数型リテラルは@<code>{(word: string) => string}という記法で、返り値の型を置く位置が@<code>{=>}の前後という違いがあるので間違えないように気をつけましょう。

== インタフェース（Interfaces）

インタフェースは多くのOOPな言語に存在しているので、ご存知の方も多いでしょう。
TypeScriptのインタフェースは通常のインタフェース以上に色々な場面で登場します。
TypeScriptでの一番基本的な使い方は名前付きオブジェクト型リテラルを作ることです。
インタフェースの中で許される記法はオブジェクト型リテラルそのまんまです。

#@# @suppress JapaneseAmbiguousNounConjunction
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
let objA: A = new AImpl();

// インタフェースは他のインタフェースを拡張できる
interface B1 {
  str: string;
}
interface B2 extends B1 {
  num: number;
}
// 代入する値はインタフェースを実装したクラスに限らない
let objB: B2 = {
  str: "string",
  num: 42,
};

// interfaceはクラスすら拡張できる！(実装はなかったことになる
class FooClass {
  constructor(public num: number) {
  }
}
interface C extends FooClass {
  str: string;
}
let objC: C = {
  num: 42,
  str: "string",
};
#@end
//}

== 構造的部分型（Structural Subtyping）

構造的部分型は、乱暴にいうと静的型付け用のduck typingです。
TypeScriptでは、構造が一致するかどうかで型の互換性を判定します（@<list>{structuralSubtypings/basic}）。
そこに実際の継承関係は必要ありません。

//list[structuralSubtypings/basic][大体一緒ならまぁ一緒ってことでいいよね]{
#@mapfile(../code/types-basic/structuralSubtypings/basic.ts)
// クラス Foo はstring型のstrという名前のプロパティと
// number型のnumという名前のプロパティを持つ
class Foo {
  str = "string";
  num = 1;
}

// 構造が完全に一致…！！！
// 構造が同じならもう同じってことでいいよね
let obj: Foo = {
  str: "Hi!",
  num: 42,
};

export { obj }
#@end
//}

そのため、@<list>{structuralSubtypings/classCompat}のようなコードもTypeScriptとしては正しいです。

//list[structuralSubtypings/classCompat][Pointインタフェースが要求されているが？]{
#@mapfile(../code/types-basic/structuralSubtypings/classCompat.ts)
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
    y: p.y * 2,
  };
}
// 次のすべては正しいコードとなります
double(new PointImpl1(1, 2));
double(new PointImpl2(3, 4));
double({
  x: 5,
  y: 6,
});
let p = {
  x: 7,
  y: 8,
  z: 9,
};
double(p); // オブジェクトリテラルを直で渡す場合余計な要素があると怒られる
#@end
//}

#@# prh:disable
コンパイル自体は通ります。
Pointインタフェースに適合させることが目的のクラスであれば、きちんとimplements節を使ったほうが意図的な仕様である事が明示できるためより好ましいです。

なお、省略可能なプロパティは存在していなくても同じ型であるものとして扱われます（@<list>{structuralSubtypings/optional}）。

//list[structuralSubtypings/optional][optional(?)なプロパティはなくてもよい]{
#@mapfile(../code/types-basic/structuralSubtypings/optional.ts)
interface Point {
  x: number;
  y: number;
  color?: string; // なくてもいい
}
function printPoint(p: Point): void {
  let message = `x=${p.x}, y=${p.y}`;
  if (p.color) {
    message = `${p.color}(${message})`;
  }
  console.log(message);
}
// optinal なプロパティはなくても大丈夫！
// x=1, y=2 と表示される
printPoint({
  x: 1,
  y: 2,
});
// red(x=1, y=2) と表示される
printPoint({
  x: 1,
  y: 2,
  color: "red",
});
// とはいえ、型が不一致だと怒られる。
// error TS2345: Argument of type '{ x: number; y: number; color: boolean; }'
//   is not assignable to parameter of type 'Point'.
//   Types of property 'color' are incompatible.
//     Type 'boolean' is not assignable to type 'string'.
// printPoint({
//   x: 1,
//   y: 2,
//   color: true,
// });
#@end
//}

== 型アサーション（Type Assertions）

#@# TODO 後置の as any 的な奴の記述を追加する
#@# TODO let foo = <Test>{} ; みたいな書き方はやめろという話を書く

型アサーションは他の言語でいうところのキャストです。
@<list>{typeAssertions/basic}のように、@<code>{<変換後型名>}と書くだけです。

//list[typeAssertions/basic][型アサーション 基本例]{
#@mapfile(../code/types-basic/typeAssertions/basic.ts)
let obj: any = 1;

// お前は今後 number として生きよ…
let num = <number>obj;

let str = "string";
// 非常に外道なコードを書くことができる… 人としての良識を疑う
// stringをnumberな型の変数に代入するだなんてなんて卑劣な…！
num = <any>str;
// 後置のキャストもあります as 型名という形式
num = str as any;

export { }
#@end
//}

もちろん、互換性のない型に型アサーションで変換しようとすると怒られます（@<list>{typeAssertions/invalidAssertions-invalid}）。

//list[typeAssertions/invalidAssertions-invalid][stringはnumberにはなれない]{
#@mapfile(../code/types-basic/typeAssertions/invalidAssertions-invalid.ts)
let str = "str";
// anyを経由しない場合、整合性の無い型アサーションは成功しない！安全！
// error TS2352: Type 'string' cannot be converted to type 'number'.
let num: number = <number>str;
#@end
//}

ダウンキャストも実行できます（@<list>{typeAssertions/class}）。
TypeScriptコード中で一番よくみる型アサーションは、このinstanceofとの合わせ技のパターンでしょう。

//list[typeAssertions/class][親クラスから子クラスへ変換]{
#@mapfile(../code/types-basic/typeAssertions/class.ts)
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
let obj: Base = new InheritA();

// objの中身はInheritAだ！人間はそれを知っているので無理やりダウンキャスト
(obj as InheritA).num;

// なお、instanceofで調べると勝手に対象の型にダウンキャストされる（型のnarrowing）
// もちろん、キャストするよりこっちのやり方のほうが安全
if (obj instanceof InheritA) {
  obj.num;
} else if (obj instanceof InheritB) {
  obj.bool;
} else {
  obj.str;
}

export { }
#@end
//}

anyに一旦キャストすればなんにでも化けさせられるので、これは一種の切り札です。
型定義ファイルを使っているときに、その型定義ファイルに不足や誤りがある場合、型アサーションでとりあえず切り抜けたい場合があります（@<list>{typeAssertions/buggyDefinitionFile}）。

//list[typeAssertions/buggyDefinitionFile][無理やりなんとかする例]{
#@mapfile(../code/types-basic/typeAssertions/buggyDefinitionFile.ts)
// こういう、型定義があるとする。実装は他のライブラリなどが与える。
declare class Base {
  str: string;
}

let obj = new Base();
// 本当は、Baseクラスがnumプロパティも持ってるんだけど…
// 型定義が不足しているので10秒で解決するにはanyで誤魔化すしかない
// キレイごとだけじゃ世の中生きていけないんじゃよ…
// でも、本当は型定義のほうを修正するのが正しいです
let num: number = (obj as any).num;

export { num }
#@end
//}

== ジェネリクス（Generic Types）

いよいよ来ました。
最後の大ボスです。
Javaなどでは総称型とも呼ばれます。

ジェネリクスなんて知らんわい！
という人も、実はすでに色々なところでお世話になっています。

#@# @suppress SectionLength
=== ジェネリクスの基本

TypeScriptで一番よく使うジェネリクスを使ったクラスは、Arrayです。
例を見てみましょう(@<list>{genericTypes/basic})。

//list[genericTypes/basic][配列はジェネリクスに支えられております]{
#@mapfile(../code/types-basic/genericTypes/basic.ts)
// string[] は実は Array<string> と同じ意味なのだ！(Arrayだけ特別に！
let strArray: Array<string> = ["a", "b", "c"];
let numArray: Array<number> = [1, 2, 3];

// ArrayのメソッドとしてforEachがある
// forEachで渡される値の型はそれぞれ違う(forEachは汎用的だ！
strArray.forEach(v => v.charAt(0));
numArray.forEach(v => v.toFixed(2));

// 頑張ってこうやって書いてもいいけど、めんどいよね
strArray.forEach((v: string) => v.charAt(0));
numArray.forEach((v: number) => v.toFixed(2));

// あと、間違った型を指定したときにエラーにならないとこわい…
// error TS2345: Argument of type '(v: RegExp) => boolean'
//     is not assignable to parameter of type
//     '(value: string, index: number, array: string[]) => void'.
//   Types of parameters 'v' and 'value' are incompatible.
//     Type 'string' is not assignable to type 'RegExp'.
// strArray.forEach((v: RegExp) => v.test("str"));
#@end
//}

実は、@<code>{string[]}という型は@<code>{Array<string>}と同じ意味なのです！
ArrayだけはTypeScriptの中で特別扱いされています。

ここで出てくる@<code>{<string>}という部分がジェネリクスの肝です。
@<code>{Array<string>}を声に出して読むと、"stringのArray"になります。
ただのArrayではないのです。
"stringの"という所が重要です。
stringを別のものにして"numberのArray"とか"RegExpのArray"とすることもできます。
つまり、色々な型に対して、"○○のArray"ということができるのです。
これをプログラム上で表現すると@<code>{Array<T>}という表現になります。

#@# @suppress JapaneseAmbiguousNounConjunction
ここで新しく出てきた@<code>{T}を@<kw>{型パラメータ,type parameters}と呼びます。
実際、ここで出てくるアルファベットは@<code>{T}ではなくてもかまいせん。
@<code>{Type}でもいいですし、なんでもよいです。
ただ、慣習として既存の型とかぶらないようにするためにアルファベット大文字1文字を使う場合が多いです。
代表的な例ではTypeの頭文字であるT、アルファベット的にTの次の文字であるUや、Returnの頭文字であるRなどが使われます。

さて、ではlib.d.tsから一部を抜粋した@<list>{genericTypes/arrayDeclaration-invalid}を見てみます@<fn>{array-forEach}。

//list[genericTypes/arrayDeclaration-invalid][Array<T>が登場する]{
#@mapfile(../code/types-basic/genericTypes/arrayDeclaration-invalid.ts)
interface ArrayConstructor {
  new <T>(...items: T[]): T[];
};

interface Array<T> {
  length: number;
  push(...items: T[]): number;
  pop(): T | undefined;
  forEach(callbackfn: (value: T) => void, thisArg?: any): void;
  [n: number]: T;
}
#@end
//}

#@# @suppress SuccessiveWord SentenceLength CommaNumber
色々な所でTが使われています。
pushの定義を見ると、"○○のArrayに対して、○○の値いくつかを追加するメソッドpush"とか、"○○のArrayに対して、末尾の○○の値を1つ取得するメソッドpop"、"○○のArrayに対して、○○の値それぞれに対してcallbackFnを適用するメソッドforEach"などの、汎用化された要素がたくさんあります。

ここで、型パラメータTを実際にstringで具体化します(@<list>{genericTypes/arrayDeclarationString-invalid})。

//list[genericTypes/arrayDeclarationString-invalid][string専用Arrayに変身]{
#@mapfile(../code/types-basic/genericTypes/arrayDeclarationString-invalid.ts)
interface ArrayConstructor {
  new (...items: string[]): string[];
};

interface Array {
  length: number;
  push(...items: string[]): number;
  pop(): string | undefined;
  forEach(callbackfn: (value: string) => void, thisArg?: any): void;
  [n: number]: string;
}
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction SentenceLength CommaNumber
"stringのArrayに対して、stringの値をいくつか追加するメソッドpush"や、"stringのArrayに対して、末尾のstringの値を1つ取得するメソッドpop"、"stringのArrayに対して、stringの値それぞれに対してcallbackFnを適用するメソッドforEach"などになりました。
ジェネリクス、使う分にはめっちゃ簡単ですね！

このように、ジェネリクスを使うと柔軟性と堅牢さを両立させることができます。

//footnote[array-forEach][紙面の都合上横幅が辛かったのでforEachの定義を大胆に切り詰めてあります…。ごめんなさい！]

=== ジェネリクスの書き方色々

ここでジェネリクスの書き方を確認しておきます（@<list>{genericTypes/variation}）。

//list[genericTypes/variation][Array<T>が登場する]{
#@mapfile(../code/types-basic/genericTypes/variation.ts)
// それぞれのTは独立していて、関連性はない

// クラスとジェネリクス
class SampleA<T> {
  constructor(public data: T) {
  }
}
// 使い方 <string>は省略しても引数から推論可能
let objA = new SampleA<string>("str");

// インタフェースとジェネリクス
interface SampleB<T> {
  data: T;
}
// 使い方
let objB: SampleB<number> = { data: 1 };

// オブジェクト型リテラル各種
let obj: {
  new <T>(value: T): any;
  <T>(value: T): any;
  methodA<T>(value: T): any;
  // 型パラメータは複数あってもよい
  methodB<T, U>(value: T): U;
};

// 関数型
let func: <T>(array: T[]) => T;
// 使い方
func = <T>(array: T[]) => array[0];
func<number>([1, 2, 3]);

// コンストラクタ型
let ctor: new <T>(value: T) => any;
// 使い方
ctor = SampleA;
new ctor<string>("str");

export { objA, objB, obj }
#@end
//}

この中でよく使うのは、クラスとインタフェースとメソッドシグニチャの書き方でしょう。
まずはこの3パターンの書き方を覚えておくべきです。

=== 型パラメータと制約

型パラメータには満たすべき制約を設けることができます。
例を見てみましょう（@<list>{genericTypes/limitation}）。

//list[genericTypes/limitation][型パラメータTはBaseを継承していなければならない]{
#@mapfile(../code/types-basic/genericTypes/limitation.ts)
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
let objA: Sample<InheritA>;

// これはダメ RegExpはBaseを継承していない
// error TS2344: Type 'RegExp' does not satisfy the constraint 'Base'.
// Property 'str' is missing in type 'RegExp'.
// let objB: Sample<RegExp>;

// これはOK 指定したオブジェクト型リテラルはBaseクラスの要件を満たす
let objC: Sample<{ str: string; }>;

export { objA, objC };
#@end
//}

型パラメータが満たすべき制約をextendsの形式で指定することができます。
これにより、Tに何が指定されようとも、Baseに存在するプロパティには安全にアクセスできることがわかります。

=== 自分でジェネリクス有りのコードを書く

#@# @suppress SuccessiveWord JapaneseAmbiguousNounConjunction
ジェネリクスを使いこなすにあたり、一番難しいのは使うことではなく、使わせることです。
なぜならば、ジェネリクスを提供するコードというのは、何かしらの要素を抽象的なまま扱わねばならないからです。
たとえば、"○○のArray"のように、型パラメータ部分が何になっても上手く動くような設計です。

逆にいうと、実際に使うときには具体化しなければならないわけで、ジェネリクス有りのコードは"必ず何かと組み合わせて具体化する"必要があります。
これを上手に使いこなすには一段上の設計力が要求されます。

通常の範囲では自分でジェネリクスを提供するコードを作る機会はさほど多くはありません。
ですが、そこができるようになったらだいぶ型に慣れ親しんできたといえます。
