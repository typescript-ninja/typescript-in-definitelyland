= アドバンスド型戦略

#@# TODO 文脈依存型
#@# TODO 文字列により特殊化されたオーバーロード
#@# TODO 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.10.7
#@# TODO || 演算子あたりのだるさ
#@# TODO オーバーロードの選択アルゴリズム

型のうち、難しいけど便利な話や、あまり関わりたくないけど実用上たまにお世話になる内容を解説していきます。
タプル型（tuple types）や直和型（union types）についての解説もありますよ！
なお、普段書くコードではこの章で出てくる内容をなるべく使わずに済む設計こそよい設計だと筆者は考えています@<fn>{bad-code}。

TypeScriptでコードを書く中で、JavaScriptで書かれたコードを型定義ファイルを介して扱う場面があります。
そういったときに本章の内容が活きてくる場面があるでしょう。
しかし、本章で書かれた内容を活かさないと上手く扱えないJavaScriptコードは、元々の品質が微妙なコードだと考えてよいでしょう。

//footnote[bad-code][本章で触れる機能を使うほうがよい場合もあります。たとえば構文木の構築・分解時などです。自分の用途に本当にそれが必要かはよくよく考えてみてください]

== 型クエリ（Type Queries）

型クエリは指定した変数（やメソッドなど）の型をコピーします。
たとえば、@<list>{typeQueries/basic}のようなクラスそのものを型として指定したい場合、それ専用の書き方は用意されていません。
そういうときに、型クエリを使います。

//list[typeQueries/basic][クラスそのものの型だよ！]{
#@mapfile(../code-2.0/types-advanced/typeQueries/basic.ts)
class Sample {
  str: string;
}

// この書き方だとSampleのインスタンスになる Sampleクラスそのものではない
let obj: Sample;
// Sample自体の型をコピー！ つまりこれはSampleクラスそのものです
let clazz: typeof Sample;

// それぞれに当てはまる値は次のとおり なるほど
obj = new Sample();
clazz = Sample;

obj = new clazz();

// clazz を頑張って手で書くと次に等しい
let alterClazz: {
  new (): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;

export {}
#@end
//}

メソッドなどの値も取れますが、thisを使うことはできないため、少しトリッキーなコードになる場合もあります。
@<list>{typeQueries/cheapTrick}の例は、prototypeプロパティを使っているためJavaScript力が多少ないと思いつかないかもしれません。

//list[typeQueries/cheapTrick][prototypeを参照するとメソッドの型が取れる]{
#@mapfile(../code-2.0/types-advanced/typeQueries/cheapTrick.ts)
class Sample {
  hello = (word = "TypeScript") => "Hello, " + word;
  bye: typeof Sample.prototype.hello;
}

let obj = new Sample();
obj.bye = obj.hello;

export {}
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
型クエリはわざわざインタフェースを定義するのもめんどくさいけど…というときに使える場合があります。
@<list>{typeQueries/copy}では、ひとつ目の引数の型をふたつ目の引数や返り値の型にもコピーして使っています。

//list[typeQueries/copy][ここまで複雑にするならインタフェース使って]{
#@mapfile(../code-2.0/types-advanced/typeQueries/copy.ts)
// このコードは（死ぬほど読みにくいけど）正しい
function move(p1: { x1: number; y1: number; x2: number; y2: number; },
              p2: typeof p1
             ):   typeof p1 {
  return {
    x1: p1.x1 + p2.x1,
    y1: p1.y1 + p2.y1,
    x2: p1.x2 + p2.x2,
    y2: p1.y2 + p2.y2,
  };
}

let rect = move({
  x1: 1, y1: 1, // 無駄に多い
  x2: 2, y2: 2, // プロパティ
}, {
  x1: 3, y1: 3,
  x2: 4, y2: 4,
});
rect.x1;
rect.x2;

export {}
#@end
//}

ここまで来るとさすがに読みにくくなるのでインタフェースをひとつ定義したほうが断然いいですね。

#@# @suppress ParagraphNumber SectionLength
== タプル型（Tuple Types）

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

#@# @<strong>{導入されたバージョン 1.3.0}

#@# @suppress JapaneseAmbiguousNounConjunction
tuple（タプル）は、任意の数の要素の組です。
JavaScriptではtupleはサポートされていないため、TypeScriptでのtupleはただのArrayで表現されます。

既存のJavaScript資産を使おうとしたときに、配列の形で多値を返してくるライブラリが稀にあります。
タプル型はそういったときに使うためのもので、TypeScriptでコードを書く際に多用するものではないでしょう。
というのも、普通にコードを書いている限りでは型推論の結果としてタプル型が出てこないためです。

タプル型は型（TypeScript）の世界にしか登場せず、コンパイル後のJavaScriptコードでは消えてしまいます。
記述方法は配列の型指定へ @<code>{[typeA, typeB]} のように配列の要素の代わりに型名を記述していくだけです。
例を見てみましょう（@<list>{tuple/basic}）。

//list[tuple/basic][基本的な例]{
#@mapfile(../code-2.0/types-advanced/tuple/basic.ts)
// まずは今までどおりの配列から
// これは別の箇所で解説している union typesで表現され (number | string | boolean)[]
let array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
let tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
  console.log(v);
});

export {array}
#@end
//}

各要素の型を指定すると、その要素のindexでアクセスしたときに適切な型で扱われます。

もちろん、タプル型はGenericsと組み合わせて利用できます(@<list>{tuple/withGenerics})。

//list[tuple/withGenerics][Genericsでの利用も可]{
#@mapfile(../code-2.0/types-advanced/tuple/withGenerics.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
  return [v1, v2];
}

let tuple = zip("str", { hello(): string { return "Hello!"; } });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello();   // おー、静的に検証される！

export {}
#@end
//}

Good！いいですね。

さて、タプル型について重箱の隅を見ていきましょう。
要素数が多すぎる場合、指定されていない値の型はunion typesになります。
その例を見てみましょう(@<list>{tuple/manyValues})。

//list[tuple/manyValues][値の要素数が多すぎる場合]{
#@mapfile(../code-2.0/types-advanced/tuple/manyValues.ts)
// 要素が多い分にはOKだ！
let tuple: [string, number] = ["str", 1, "test"];

// 範囲外の要素の型はすべての要素のunion、つまり string | number になる。
let value = tuple[2];

// 以下はダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

お次は要素の順序がズレた場合、どうなるかを見てみましょう(@<list>{tuple/unshift})。

//list[tuple/unshift][絶望に身をよじれ…！]{
#@mapfile(../code-2.0/types-advanced/tuple/unshift.ts)
let tuple: [string, number] = ["str", 1];

// 先頭をnumberに…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);

export {}
#@end
//}

…悲しい結果になりました。
@<code>{[1, true]}のような配列のリテラルをタプル型に推論しないのはおそらくこのためでしょう。

unshiftやpopなど、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptではTypeScriptコンパイラ側ですべてをキャッチアップすることは不可能です。
タプル型を扱う場合は要素数を変更するような操作をしないほうがよいでしょう。

なるべくなら、タプルは使いたくないですね。

#@# @suppress ParagraphNumber SectionLength
== 直和型（Union Types）

#@# @<strong>{導入されるバージョン 1.4.0}

はい、皆様待望の機能でございます。
"名前を言ってはいけないあの界隈"がよく使う用語を使って解説しないといけないのでビクビクですね。

一番最初に書いておくけど@<strong>{TypeScriptのコード書くときに積極的に使うものじゃあないぞ！！}
という感じなんですが、@<code>{--strictNullChecks}オプションを使う場合に避けて通れない要素であるためしっかり覚えましょう。

#@# TODO intersection types 書かなきゃどこかで

#@# @suppress InvalidExpression CommaNumber JapaneseAmbiguousNounConjunction
じゃあ解説していきましょう。
union typesはいわゆる直和型です。
この変数の値の型は、アレか、コレか、ソレのどれかです。
みたいな感じ。

なんのために入ったのかというと、まずは既存JavaScriptによりよい型定義を与えるためでしょう。
そして、nullやundefined、string literal typesなどTypeScriptの中でも適用領域が広がっています。
JavaScriptという現実と、安全な世界を構築するTypeScriptの橋渡しを上手にしてくれる機能といえます。

ちなみに、自分でTypeScriptコード書いてるときに欲しくなる機能ではあまりありません。

まずは簡単な例から見ていきましょう(@<list>{unionTypes/basic})。

//list[unionTypes/basic][型A | 型B でAかBのどちらかを表す]{
#@mapfile(../code-2.0/types-advanced/unionTypes/basic.ts)
let a: string | boolean | undefined;
// string | boolean なので次はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean | undefined'.
// a = 1;

// b1 と b2 を合体させてみよう
let b1: string | boolean | undefined;
let b2: boolean | number | undefined;
// c の型は string | number | boolean | undefined となる
let c: typeof b1 | typeof b2;

export {c}
#@end
//}

型注釈を書く際に複数の型を｜で区切って書けます。
既存のJavaScriptライブラリだとこういった、返り値の型が複数ある困った関数がかなりあります。
あとは普通にTypeScriptを書いているときでもSyntaxTreeとかをコードから構築するときにはあったほうが便利かもしれません。

ご覧のとおり、union types中の型の順番とかは関係ない（交換可能）し、union typesのunion typesなどは単純にまとめてひとつのunion typesに統合できます。

自然にTypeScriptを書いていて、union typesを目にする機会は3種類あります。
|| 演算子を使ったとき、条件（三項）演算子を使ったとき、配列リテラルを使ったときです(@<list>{unionTypes/inferred})。

//list[unionTypes/inferred][こういうときは目にしますね]{
#@mapfile(../code-2.0/types-advanced/unionTypes/inferred.ts)
// and の型は string | boolean
let and = "str" || true;
// cond の型は number | string
let cond = true ? 1 : "str";
// array の型は (number | boolean | string)[]
let array = [1, true, "str"];

export {and, cond, array}
#@end
//}

一番よくお目にかかるのは配列リテラルでしょうか。
TypeScript一般のベストプラクティスとして1つの配列で複数の型の値を扱わないほうが堅牢なコードになるため、きれいなコードを書いている限りはあまり見ないかもしれません。

型注釈として関数を与えるときは記法にちょっと気をつけないとコンパイルエラーになります(@<list>{unionTypes/syntax})。

//list[unionTypes/syntax][型名をカッコで囲うんです？]{
#@mapfile(../code-2.0/types-advanced/unionTypes/syntax.ts)
// 引数無しの返り値stringな関数 な型注釈
let func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// let a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
let b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
let c: { (): string; } | { (): boolean; };

// union typesじゃないときでも使えるけど見づらいな！
let d: (() => string);

export {func, b, c, d}
#@end
//}

見づらいです。適切に型にも名前をつけることの重要さが偲ばれます。

#@# TODO この辺にcontrol flow based type analysisの話あってもいいかも？

union typesな値を使うときは、一応型アサーションも使えますがなるべくなら避けてとおりましょう(@<list>{unionTypes/typeAssertion})。
次に説明する@<hd>{typeGuards}を使いましょう。話はそれからだ！

//list[unionTypes/typeAssertion][一応使えるよ こうすれば]{
#@mapfile(../code-2.0/types-advanced/unionTypes/typeAssertion.ts)
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
#@end
//}

#@# TODO Contextual Union Types https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.4.1

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

//footnote[spec-example-bug][@<href>{https://github.com/Microsoft/TypeScript/issues/1267}]

=={typeGuards} 型のためのガード（Type Guards）

#@# TODO 和訳が微妙…
#@# @<strong>{導入されるバージョン 1.4.0}

type guardsは、union typesが導入されたことで変数の型が一意ではなくなってしまったため、それを自然に解決するために導入された仕組みです。
type guardsは"変数Aが○○という条件を満たすとき、変数Aの型は××である"というルールを用いて、ガード（条件チェックなど）の後の文脈で変数の型を××に狭めることができます。

=== typeofによるtype guards

JavaScriptの typeof は指定した値がどういう性質のオブジェクトかを調べ、文字列で返す演算子です。
ECMAScript 5の範囲では、変換ルールは次のとおりです。

 * string のときは"string"を返す
 * boolean のときは"boolean"を返す
 * number のときは"number"を返す
 * undefined のときは"undefined"を返す
 * 関数として呼び出し可能な場合は"function"を返す
 * それ以外の場合（nullを含む！）は"object"を返す

これを利用して、変数の型を狭めます。

===={typeof-typeguards-usage} 使い方

一番簡単な使い方から見ていきましょう（@<list>{typeGuards/typeofBasic}）。
TypeScriptのtype guardsでは、typeofの結果がstring, boolean, numberの場合、その型に絞り込むことができます。

//list[typeGuards/typeofBasic][実際の型がわからないなら調べるしかないじゃない！]{
#@mapfile(../code-2.0/types-advanced/typeGuards/typeofBasic.ts)
let obj: number | string = null as any;
if (typeof obj === "string") {
  // ここでは string と確定されている！
  obj.charAt(0);
} else {
  // ここでは消去法で number と確定されている！
  obj.toFixed(2);
}

export {}
#@end
//}

変数objをtypeofで調べたときに値がstringだったので、変数objの型はstringである、という具合に絞りこまれています。

もう一例見てみましょう。
@<list>{typeGuards/typeof-invalid}では、anyやnumberと指定された変数をtype guardsでstringに絞り込んでいます。
そのため、@<code>{obj.toFixed(0)}というstringには存在しないメソッドを呼びだそうとするとコンパイルの段階でエラーにしてくれます。
#@# TODO never型についての説明をどこかに

//list[typeGuards/typeof-invalid][変なコードを書くとコンパイラが教えてくれる]{
#@mapfile(../code-2.0/types-advanced/typeGuards/typeof-invalid.ts)
let objA: any;
if (typeof objA === "string") {
  // ここでは string と確定されている！
  // number にしか存在しないメソッドを呼ぶとコンパイルエラー！
  // error TS2339: Property 'toFixed' does not exist on type 'string'.
  objA.toFixed(0);
}

let objB: number = 1;
if (typeof objB === "string") {
  // "ありえない" パターンだとnever型になり怒られる
  // error TS2339: Property 'toFixed' does not exist on type 'never'.
  objB.toFixed(0);
}
#@end
//}

うーん、便利ですね。
変数に指定した型どおりの値が入ってくるのが健全なので、コンパイル時にミスが発見されるのは嬉しいことです。

#@# @suppress JapaneseAmbiguousNounConjunction
==== 後続の型の絞込み

typeofによるtype guards特有の仕様として、後続の型の絞込みがあります（@<list>{typeGuards/typeofRemoves}）。

//list[typeGuards/typeofRemoves][型の絞込み！]{
#@mapfile(../code-2.0/types-advanced/typeGuards/typeofRemoves.ts)
let obj: number | string | boolean = null as any;

if (typeof obj === "string") {
  // ここではstringと確定されている！
  obj.charAt(0);
} else {
  // ここではstringが引かれ number | boolean;
  obj;
}

if (typeof obj === "string") {
  // ここではstringと確定されている！
  obj.charAt(0);
} else if (typeof obj === "number") {
  // ここではnumberと確定されている！
  obj.toFixed(2);
} else {
  // ここではstring, numberが引かれbooleanとなる！
  obj;
}

export {}
#@end
//}

最初にstringとわかったら、後続のelse句ではstringは絶対入ってこないことは、わかりきっています。
親切な仕様ですね。

=== instanceofによるtype guards

primitive typesだけtype guardsが使えてもあんまり嬉しくないので、instanceofを使ったtype guardsももちろんあります。

JavaScriptにおけるinstanceofは、ある値が指定した関数のインスタンスであるかを調べる演算子です。
プロトタイプチェーンも遡ってみていくので、親子関係にある場合もインスタンスかどうかを調べることができます。

動作例を確認してみましょう(@<list>{typeGuards/instanceof})。

//list[typeGuards/instanceof][instanceof の挙動]{
#@mapfile(../code-2.0/types-advanced/typeGuards/instanceof.ts)
class Base {
}

class InheritA extends Base {
}
class InheritB extends Base {
}

let obj = new InheritA();

// trueと表示される
console.log(obj instanceof Base);
// trueと表示される
console.log(obj instanceof InheritA);
// falseと表示される
console.log(obj instanceof InheritB);

// 無理矢理親を差し替える！
InheritA.prototype = new InheritB();
obj = new InheritA();
// trueと表示される
console.log(obj instanceof InheritB);

export {}
#@end
//}

オブジェクトのprototypeと一致するか順番どおり見ていくだけですね。

==== 使い方

instanceofで型を絞り込みます(@<list>{typeGuards/instanceofBasic})。

//list[typeGuards/instanceofBasic][instanceofの挙動]{
#@mapfile(../code-2.0/types-advanced/typeGuards/instanceofBasic.ts)
class A {
  str: string;
}
class B {
  num: number;
}
class C extends A {
  bool: boolean;
}

let obj: A | B | C = null as any;
if (obj instanceof A) {
  // ここでは A（含むC) と確定している
  obj.str;
  if (obj instanceof C) {
    // ここではCと確定している
    obj.bool;
  }
}

if (obj instanceof C) {
  // ここではCと確定している
  obj.bool;
} else {
  // ここではまだ A | B
  if (obj instanceof B) {
    // ここではBと確定している
    obj.num;
  }else{
    // ここではAと確定している
    obj.str;
  }
}

export {}
#@end
//}

昔のTypeScriptと違って、instanceofのelse句でも型の絞込が行われます。
挙動として納得感があり大変よいですね。

=== type guardsと論理演算子

type guardsは@<code>{&&}とか@<code>{||}とか@<code>{?}とか@<code>{!}とかの論理演算子にもちゃんと対応しています(@<list>{typeGuards/operator})。

//list[typeGuards/operator][ブール代数みたいな演算に対応してる]{
#@mapfile(../code-2.0/types-advanced/typeGuards/operator.ts)
let obj: number | boolean | string = null as any;

// &&演算子で絞込み
typeof obj === "string" && obj.charAt(0);
// 次のようなコードはエラーになる！
// error TS2339: Property 'charAt' does not exist on type 'number'.
// typeof obj === "number" && obj.charAt(0);

// ||演算子でunion typesに
if (typeof obj === "string" || typeof obj === "boolean") {
  // string | boolean に絞り込まれる
} else {
  // 消去法でnumber！
}

// 三項演算子は普通にif文と一緒の挙動
typeof obj === "string" ? obj.charAt(0) : obj;
// 次と等価
if (typeof obj === "string") {
  obj.charAt(0);
} else {
  obj;
}

// 一応、否定演算子にも対応している
if (!(typeof obj !== "string")) {
  // 否定の否定は普通にそのまんまstringだな！ちゃんと絞り込まれます
  obj.charAt(0);
}

export {}
#@end
//}

あんまり使わないかもしれませんが、他の人がこの書き方を使った時に戸惑わぬよう頭の片隅にはとどめておいたほうがよいかもしれません。

=== type guardsの弱点

type guardsは型システム上の仕組みだということを忘れてはいけません。
JavaScriptの実行環境とは全く関係がないのです。

TypeScriptでは構造的部分型の仕組みにより、クラスが要求されている箇所に互換性のある別の値を代入することができます。

その仕組みを使って、@<list>{typeGuards/weakspot}のようなコードが書けてしまいます。

//list[typeGuards/weakspot][構造的部分型とtype guards]{
#@mapfile(../code-2.0/types-advanced/typeGuards/weakspot.ts)
class Sample {
  str: string;
}

// 構造的部分型！
let obj: Sample = {
  str: "Hi!"
};

if (obj instanceof Sample) {
  // 型はSampleに絞られている しかし、絶対に到達しない
  // 現在のobjはSampleを親に持たない
  console.log(obj.str);
}

export {}
#@end
//}

#@# @suppress SentenceLength CommaNumber
objはSampleを型として持ち、その値として互換性のあるオブジェクトリテラルを持っています。
コンパイル後のJavaScriptコード(@<list>{typeGuards/weakspot.js})を見ると、objの値がSampleクラスのインスタンスではないことが一目瞭然ですが、TypeScript上で見ると型を元に判別されていると勘違いしやすい、ということを頭の片隅においておきましょう。

//list[typeGuards/weakspot.js][コンパイル後のJS]{
#@mapfile(../code-2.0/types-advanced/typeGuards/weakspot.js)
"use strict";
class Sample {
}
// 構造的部分型！
let obj = {
    str: "Hi!"
};
if (obj instanceof Sample) {
    // 型はSampleに絞られている しかし、絶対に到達しない
    // 現在のobjはSampleを親に持たない
    console.log(obj.str);
}
#@end
//}

これを回避する方法がいくつかあります。
ひとつ目はユーザ定義のtype guardを利用することです(@<list>{typeGuards/userDefined})。
型判別用の関数を作成し、そこで返り値に@<code>{仮引数名 is 型名}という形式で判別結果を指定します。
この書き方をした場合、返り値はbooleanでなければなりません。

//list[typeGuards/userDefined][ユーザ定義のtype guards]{
#@mapfile(../code-2.0/types-advanced/typeGuards/userDefined.ts)
class Sample {
  str: string;
}

// 構造的部分型！
var obj: Sample = {
  str: "Hi!"
};

// 独自にSample型である事の判定を実装する
function isSample(obj: Sample): obj is Sample {
  if (!obj) {
    return false;
  }
  // とりあえず、strプロパティがあって値がstringならSample型コンパチということでOK という基準にする
  return typeof obj.str === "string";
}

if (isSample(obj)) {
  console.log(obj.str);
}


export { }
#@end
//}

ふたつ目はprivateな要素をクラスに突っ込んでしまうことです(@<list>{typeGuards/vsWeakspot2-invalid})。

//list[typeGuards/vsWeakspot2-invalid][privateな要素があれば構造的部分型で値を偽造できない]{
#@mapfile(../code-2.0/types-advanced/typeGuards/vsWeakspot2-invalid.ts)
class Sample {
  private _tmp: any;
  str: string;
}

// privateなインスタンス変数があるクラスのインスタンスは偽造できない！
// error TS2322: Type '{ _tmp: null; str: string; }' is not
//     assignable to type 'Sample'. Property '_tmp' is private
//     in type 'Sample' but not in type '{ _tmp: null; str: string; }'.
let obj: Sample = {
  _tmp: null,
  str: "Hi!",
};
#@end
//}

#@# @suppress JapaneseStyle
色々書きましたが、一番の解決策はunion typesやanyを多用せず、真っ当なコードを書けるよう設計することです。

#@# @suppress ParagraphNumber
== 型の別名（type alias）

#@# TODO @<strong>{導入されるバージョン 1.4.0}

最初に書いておきます。@<strong>{可能な限りtype aliasを使うな！interface使え！}
筆者はtype aliasの乱用を恐れています！

type aliasもunion typesの扱いを便利にするために導入された機能です。
機能としてはただ単に、型をひとまとまりにして、それに名前が付けられるだけです。
それだけです。

type aliasは仕様上、interfaceと同じように利用できる場面もあります。
ですが、基本的にtype aliasはinterfaceより機能が貧弱であるため、なるべく避けるほうがよいでしょう。

代表例を見てみましょう(@<list>{typeAlias/basic})。

//list[typeAlias/basic][頻出するunion typesに名前をつける]{
#@mapfile(../code-2.0/types-advanced/typeAlias/basic.ts)
type FooReturns = string | number | boolean;

interface Foo {
  bar(): FooReturns;
  buzz(): FooReturns;
  barbuzz(): FooReturns;
}
#@end
//}

わかりやすいですね。
1ヶ所変更すると、関連箇所がすべて更新されるのも便利です。

tuple typesに名前をつけることもできます(@<list>{typeAlias/tuple})。

//list[typeAlias/tuple][tuple typesに名前をつける]{
#@mapfile(../code-2.0/types-advanced/typeAlias/tuple.ts)
// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

let c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
namespace alternative {
  class Point {
    constructor(public x: number, public y: number) {
    }
  }
  class Circle {
    constructor(public p: Point, public r: number) {
    }
  }
  let c: Circle = new Circle(new Point(1, 2), 3);
  console.log(c.p, c.r);
}

export {c, alternative}
#@end
//}

素直にクラスでやればいいのに、という感じです。

type aliasは型に別名をつけるだけで、コンパイルされると消えてしまう存在です。
そのため、@<list>{typeAlias/doNotHaveInstance-invalid}のようなコードは書くことができません。

//list[typeAlias/doNotHaveInstance-invalid][type aliasは値を作らない]{
#@mapfile(../code-2.0/types-advanced/typeAlias/doNotHaveInstance-invalid.ts)
// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = string[];

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
let strArray = new StringArray();
#@end
//}

#@# @suppress CommaNumber
TypeScriptの仕様書にのっているtype aliasの利用例について、interfaceでの書き換えができるものを示します(@<list>{typeAlias/specExample})。
union typesが絡むもの、tuple typesが絡むもの、型クエリが絡むものだけが、interfaceで置き換えることができません。

//list[typeAlias/specExample][interfaceを使うんだ！]{
#@mapfile(../code-2.0/types-advanced/typeAlias/specExample.ts)
// これらはinterfaceで表現不可 type aliasで正解
type StringOrNumber = string | number;
type TextObject = string | { text: string };
type Coord = [number, number];
type ObjectStatics = typeof Object;
type Pair<T> = [T, T];
type Coordinates = Pair<number>;
type Tree<T> = T | { left: Tree<T>, right: Tree<T> };

// これらはinterfaceで表現可能
type HolidayLookup = Map<string, Date>;
interface AltHolidayLookup extends Map<string, Date> {
}

type Callback<T> = (data: T) => void;
interface AltCallback<T> {
  (date: T): void;
}

type RecFunc = () => RecFunc;
interface AltRecFunc {
  (): AltRecFunc;
}

export {StringOrNumber, TextObject, Coord, ObjectStatics,
   Coordinates, HolidayLookup, AltHolidayLookup, Callback, AltCallback, }
#@end
//}

最後に、type aliasの辛いところを再掲しておきます。

 * interfaceが絡んだときのコンパイルエラーにはinterface名が表示されてわかりやすい
 ** type aliasは展開されて表示されちゃうので無理
 * interfaceは定義の統合ができるので後から自由に拡張できる
 ** type aliasは無理

@<strong>{interfaceでできることをtype aliasでやるな！}
