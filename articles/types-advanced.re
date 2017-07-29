={types-advanced} アドバンスド型戦略

型のうち、難しいけど便利な話や、標準の型定義を読むために必要な知識や、あまり関わりたくないけど実用上たまにお世話になる内容を解説していきます。
@<kw>{タプル型,tuple types}や@<kw>{直和型,union types}についての解説もありますよ！

なお、普段書くコードではこの章で出てくる内容をなるべく使わずに済む設計こそよい設計だと筆者は考えています@<fn>{bad-code}。

TypeScriptでは、JavaScriptで書かれたコードを型定義ファイルを介して扱う場面があります。
そういったときに本章の内容が活きてくる場面があるでしょう。
しかし、本章で書かれた内容を活かさないと上手く扱えないJavaScriptコードは、元々の品質が微妙なコードだと考えてよいでしょう。

//footnote[bad-code][本章で触れる機能を使うほうがよい場合もあります。たとえば構文木の構築・分解時などです。自分の用途に本当にそれが必要かはよくよく考えてみてください]

#@# @suppress ParagraphNumber SectionLength
=={union-types} 直和型（Union Types）

#@# 備忘録 特に意識的に注意しないと正しいコードが書けない話じゃないので割愛していく
#@# 直和型と交差型を組み合わせのノーマライズ (Normalize union/intersection type combinations in 2.1.1

#@# @<strong>{導入されるバージョン 1.4.0}

はい、皆様待望の機能でございます。
"名前を言ってはいけないあの界隈"がよく使う用語を使って解説しないといけないのでビクビクですね。

一番最初に強く主張しておくと@<strong>{TypeScriptのコード書くときに積極的に使うものじゃあないぞ！！}
という感じなんですが、@<code>{--strictNullChecks}オプションを使う場合に避けて通れない要素なのでしっかり覚えましょう。

#@# @suppress InvalidExpression CommaNumber JapaneseAmbiguousNounConjunction
では解説していきましょう。
union typesはいわゆる直和型です。
たとえば@<code>{string | null | undefined}という型注釈がある場合、この変数の値はstringか、nullか、undefinedのいずれかを表します。
union typesはnullやundefinedとの組み合わせ、各種literal typesなどTypeScriptの中でも出番がたくさんあります。
JavaScriptという現実と安全な世界を構築するTypeScriptの橋渡しをしてくれる機能といえます。

union typesを使うためには型注釈を書く際に複数の型を｜で区切って書きます。
自分でコードを書くときによく書くのは@<code>{Subscription | null}のような型でしょうか。
@<code>{--strictNullChecks}オプションを使う場合、stringとnullやundefinedといった値は厳密に区別されます。
ゆえに、クラスのプロパティでnullをセットしたいシチュエーションがある場合、union typesのお世話になる必要があります。

既存のJavaScriptライブラリには返り値の型が複数ある困った関数がかなりあります。
これらに対して適切なコードを書くことを誘導したい時、@<code>{any}を指定するよりunion typesを使ったほうがよりよい型定義になります。

まずは簡単な例から見ていきましょう（@<list>{unionTypes/basic.ts}）。

//list[unionTypes/basic.ts][型A | 型B でAかBのどちらかを表す]{
#@mapfile(../code/types-advanced/unionTypes/basic.ts)
let a: string | boolean | undefined;
// string | boolean なので次はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable
//   to type 'string | boolean | undefined'.
// a = 1;

// b1 と b2 を合体させてみよう
let b1: string | boolean | undefined;
let b2: boolean | number | undefined;
// c の型は string | number | boolean | undefined となる
let c: typeof b1 | typeof b2;

export { b1, b2, c }
#@end
//}

ご覧のとおり、union types中の型の順番とかは関係ない（交換可能）し、union typesのunion typesなどは合体させてひとつのunion typesにできます。

TypeScriptを書いていて、意図せずunion typesを目にしてしまう機会があります。
|| 演算子を使ったとき、条件（三項）演算子を使ったとき、配列リテラルを使ったときです（@<list>{unionTypes/inferred.ts}）。

//list[unionTypes/inferred.ts][こういうときは目にしますね]{
#@mapfile(../code/types-advanced/unionTypes/inferred.ts)
// and の型は string | boolean
let and = "str" || true;
// cond の型は number | string
let cond = true ? 1 : "str";
// array の型は (number | boolean | string)[]
let array = [1, true, "str"];

export { and, cond, array }
#@end
//}

一番よくお目にかかるのは配列リテラルでしょうか。
TypeScriptのベストプラクティスとして1つの配列で複数の型の値を扱わないほうが堅牢なコードになるため、きれいなコードを書いている限りはあまり見ないかもしれません。

型注釈として関数を与えるときは記法にちょっと気をつけないとコンパイルエラーになります（@<list>{unionTypes/syntax.ts}）。

//list[unionTypes/syntax.ts][型名をカッコで囲うんです？]{
#@mapfile(../code/types-advanced/unionTypes/syntax.ts)
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

export { func, b, c, d }
#@end
//}

読みづらいコードになってしまいました。
型にも適切な名前をつけることの重要さが偲ばれます。

union typesな値を使うときは、型アサーションを使うこともできますがなるべくなら避けましょう（@<list>{unionTypes/typeAssertion.ts}）。

//list[unionTypes/typeAssertion.ts][一応使えるよ。こうすれば]{
#@mapfile(../code/types-advanced/unionTypes/typeAssertion.ts)
// 注意！ここでやってるやり方よりもtype guardsを使うんだ…！！
// 型アサーションは悪い。常に悪い。なるべく使わないこと。

let obj: string | number | Date = null as any;

// string 扱いしてみる
(obj as string).charAt(0);

// number 扱いしてみる
(obj as number).toFixed(2);

// Date 扱いしてみる
(obj as Date).getTime();

// 値の集合に含まれない型にしてみるとしっかり怒られる
// error TS2352: Type 'string | number | Date' cannot be converted to type 'RegExp'.
//   Type 'Date' is not comparable to type 'RegExp'.
//     Property 'exec' is missing in type 'Date'.
// (obj as RegExp).test("test");

export { }
#@end
//}

union typesを相手にする場合は、次に説明する@<hd>{type-guards}を使いましょう。話はそれからだ！

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

#@# @suppress JapaneseAmbiguousNounConjunction
=={type-guards} 型の番人（Type Guards）

#@# TODO 型の番人、最近あまり用語として出てこない気がする… 型の絞込 とかでよくない？

#@# mhidaka TODO 本節だけぶっちぎりで長い。構成を大胆に工夫する余地がある。「更に細かくする」「独立愚連して章になる」「削る」など
#@# mhidaka アドバンスド型戦略は悪いコードとつきあうためのテクニカルなアレを感じる。必然サンプルコードが増えるが全てを網羅的に必要としていないので悩ましい
#@# この辺は「コンパイルエラーレスキュー24時」的なくくりとして独立させたいようなそうでもないような… 単離しても11P分くらいだけなんだよね

#@# @<strong>{導入されるバージョン 1.4.0}

#@# @suppress SuccessiveWord JapaneseStyle
type guardsは、union typesが導入されたことで変数の型が一意ではなくなってしまったため、それを自然に解決するために導入された仕組みです。
type guardsは"変数Aが○○という条件を満たすとき、変数Aの型は××である"というルールを用いて、ガード（番人となる条件式など）の後の文脈で変数の型を××に狭めることができます。

==={control-flow-based-type-analysis} 処理フローに基づく型の解析（Control Flow Based Type Analysis）

#@# @suppress JapaneseStyle
さて、トップバッターがいきなり公式にtype guardsの一員なのか怪しいのですがいってみましょう。
名前が長いですが、JavaScriptとして素直にコードを書き、変数の型が確定するような分岐などがあると書いたとおりに変数の型が絞り込まれるというものです。

例を見ていきましょう。
TypeScriptを書いていて一番対処を迫られるunion typesのパターンはおそらく@<code>{T | undefined}のような、何か＋@<code>{undefined}の形式でしょう。
if文を用いてundefinedの値について対処してみます（@<list>{typeGuards/controlFlowBasedBasic.ts}）。

//list[typeGuards/controlFlowBasedBasic.ts][undefinedの可能性を潰す]{
#@mapfile(../code/types-advanced/typeGuards/controlFlowBasedBasic.ts)
function upperA(word?: string) {
  // wordは省略可能引数なので string | undefined
  // ここでwordをいきなり使おうとするとエラーになる
  // Object is possibly 'undefined'.
  // word.toUpperCase();

  if (word == null) { // word が null か undefined の時
    // undefinedの可能性をstringで上書き！
    word = "TypeScript";
  }

  // undefinedの可能性を潰したのでこの時点でwordはstring確定！
  console.log(word.toUpperCase());
}

function upperB(word?: string) {
  // 別解：JSで || 演算子は最初にtruthyになった値を返す
  // ので、undefined（falsy）な時は "TypeScript" で上書きされる
  word = word || "TypeScript";

  // undefinedの可能性を潰したのでこの時点でwordはstring確定！
  console.log(word.toUpperCase());
}

function upperC(word = "TypeScript") {
  // TypeScript的に一番素直なパターン
  console.log(word.toUpperCase());
}

export { upperA, upperB, upperC }
#@end
//}

もう一例見てみましょう。
引数に@<code>{string}と@<code>{string[]}を取り、これを@<code>{string[]}に統一して利用します（@<list>{typeGuards/controlFlowBasedArray.ts}）。

//list[typeGuards/controlFlowBasedArray.ts][変数の型を統一していく]{
#@mapfile(../code/types-advanced/typeGuards/controlFlowBasedArray.ts)
function upperAll(words: string | string[]) {
  if (typeof words === "string") {
    // string なら string[] に変換する
    words = [words];
  }

  // この時点ではwordsはstring[]に揃えられる
  return words.map(word => word.toUpperCase());
}

console.log(upperAll("TypeScript"));
console.log(upperAll(["TypeScript", "JavaScript"]));

export { }
#@end
//}

変数のプロパティに対してもtype guardsは利用可能です（@<list>{typeGuards/controlFlowBasedProperty.ts}）。
書いたコードがどういう型に絞り込まれているか想像できない場合、エディタ上のツールチップの表示でどういう変数になっているか確認します。
#@# プロパティアクセスについてtype guardsを行う（Type guards on property access）

//list[typeGuards/controlFlowBasedProperty.ts][変数のプロパティも絞り込める]{
#@mapfile(../code/types-advanced/typeGuards/controlFlowBasedProperty.ts)
interface Foo {
  value: number | string;
}

let foo: Foo = {
  value: "TypeScript",
};

// number | string では toUpperCase があるか確定できない
// error TS2339: Property 'toUpperCase' does not exist on type 'number | string'.
// foo.value.toUpperCase();

// 変数直だけではなくて、変数のプロパティでもtype guardsが使える
if (typeof foo.value === "string") {
  // ここでは foo.value は string に絞りこまれている！一時変数いらない！
  foo.value.toUpperCase();
}

export { }
#@end
//}

最後に、関数が絡んだ場合の例を見ておきます（@<list>{typeGuards/controlFlowBasedFunction-invalid.ts}）。
関数の内側と外側では、処理フローは別世界です。
関数はいつ実行されるかわからないため、変数の再代入が可能な場合、関数の内側で別途絞込みを行う必要があります。
一方、constを使うと変数の値を変えることができないため、この問題を回避できる場合があります。

//list[typeGuards/controlFlowBasedFunction-invalid.ts][関数の外側でのフローは内側では関係ない]{
#@mapfile(../code/types-advanced/typeGuards/controlFlowBasedFunction-invalid.ts)
let v1: string | number;
if (typeof v1 === "string") {
  let f = () => {
    // これはエラーになる！
    // プログラムの字面的にはstringに確定されていそう…
    // しかし、関数はいつ実行されるかわからない
    // error TS2339: Property 'toUpperCase'
    //   does not exist on type 'string | number'.
    console.log(v1.toUpperCase());
  };
  // ここではvはまだstring
  f();

  // ここでvがnumberに！fの思惑と食い違う
  v1 = 1;
  f();
}

// letではなくてconstを使うと…
const v2: string | number = null as any;
if (typeof v2 === "string") {
  let f = () => {
    // v2の中身が入れ替えられる可能性はないのでエラーにならない
    console.log(v2.toUpperCase());
  };
  f();
}
#@end
//}

さて、次項以降でどういう処理が絞り込みに繋がるのかの例を見ていきます。

==={typeof-type-guards} typeofによるType Guards

JavaScriptの@<code>{typeof}は指定した値がどういう性質のオブジェクトかを調べ、文字列で返す演算子です。
ECMAScript 5の範囲では、変換ルールは次のとおりです。
#@# NOTE: ES2015の範囲だとSymbolがあるとかSymbolである程度可変になってしまうとかがある

 * string のときは"string"を返す
 * boolean のときは"boolean"を返す
 * number のときは"number"を返す
 * undefined のときは"undefined"を返す
 * 関数として呼び出し可能な場合は"function"を返す
 * それ以外の場合（nullを含む！）は"object"を返す

これを利用して、変数の型を狭めます。

一番簡単な使い方から見ていきましょう（@<list>{typeGuards/typeofBasic.ts}）。
typeofの返り値次第で変数の型を絞り込むことができます。

//list[typeGuards/typeofBasic.ts][実際の型がわからないなら調べるしかないじゃない！]{
#@mapfile(../code/types-advanced/typeGuards/typeofBasic.ts)
let obj: number | string = null as any;
if (typeof obj === "string") {
  // ここでは string と確定されている！
  obj.charAt(0);
} else {
  // ここでは消去法で number と確定されている！
  obj.toFixed(2);
}

export { }
#@end
//}

変数objをtypeofで調べたときに値がstringだったので、変数objの型はstringである、という具合に絞りこまれています。

もう一例見てみましょう。
@<list>{typeGuards/typeof-invalid.ts}では、anyやnumberと指定された変数をtype guardsでstringに絞り込んでいます。

//list[typeGuards/typeof-invalid.ts][変なコードを書くとコンパイラが教えてくれる]{
#@mapfile(../code/types-advanced/typeGuards/typeof-invalid.ts)
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
この操作を行うと"ありえない"ことを表すnever型になるため、@<code>{obj.toFixed(0)}というstringには存在しないメソッドの呼び出しはコンパイルエラーとなります。

うーん、便利ですね。
変数に指定した型どおりの値が入ってくるのが健全なので、コンパイル時にミスが発見されるのは嬉しいことです。

==={instanceof-type-guards} instanceofによるType Guards

typeofでしかtype guardsが使えないと辛いので、instanceofを使ったtype guardsも、もちろんあります（@<list>{typeGuards/instanceofBasic.ts}）。

//list[typeGuards/instanceofBasic.ts][instanceofの挙動]{
#@mapfile(../code/types-advanced/typeGuards/instanceofBasic.ts)
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
  } else {
    // ここではAと確定している
    obj.str;
  }
}

export { }
#@end
//}

==={user-defined-type-guards} ユーザ定義のType Guards（User-defined Type Guards）

ユーザが定義した関数を使って、値の型をTypeScriptコンパイラに指示する方法があります（@<list>{typeGuards/userDefined.ts}）。
型判別用の関数を作成し、そこで返り値に@<code>{仮引数名 is 型名}という形式で判別結果を指定します。
この書き方をした場合、返り値はbooleanでなければなりません。

//list[typeGuards/userDefined.ts][ユーザ定義のtype guards]{
#@mapfile(../code/types-advanced/typeGuards/userDefined.ts)
class Sample {
  str: string;
}

// 構造的部分型！
let obj: Sample = {
  str: "Hi!",
};

// 独自にSample型である事の判定を実装する
function isSample(s: Sample): s is Sample {
  if (!s) {
    return false;
  }
  // とりあえず、strプロパティがあって値がstringなら
  // Sample型に互換性あり！という基準にする
  return typeof s.str === "string";
}

if (isSample(obj)) {
  console.log(obj.str);
}

export { }
#@end
//}

面白い記法として、isの左辺にthisを用いることもできます（@<list>{typeGuards/userDefinedWithThis.ts}）。

//list[typeGuards/userDefinedWithThis.ts][isの左辺にthisを使う]{
#@mapfile(../code/types-advanced/typeGuards/userDefinedWithThis.ts)
abstract class Node {
  isStringNode(): this is StringNode {
    return this instanceof StringNode;
  }
  isNumberNode(): this is NumberNode {
    return this instanceof NumberNode;
  }
}

class StringNode extends Node {
  constructor(public text: string) {
    super();
  }
}

class NumberNode extends Node {
  constructor(public value: number) {
    super();
  }
}

let nodes: Node[] = [new StringNode("TypeScript"), new NumberNode(8)];
// TypeScript と 8 と表示される
nodes.forEach(n => {
  if (n.isStringNode()) {
    // n is StringNode!
    console.log(n.text);
  } else if (n.isNumberNode()) {
    // n is NumberNode!
    console.log(n.value);
  }
});

export { }
#@end
//}

引数として渡された値の型名を明示する代わりに、thisの型を指定するわけです。
ツリー状の構造を作るときに活躍するかもしれませんね。

==={type-guards-and-logical-operators} Type Guardsと論理演算子

type guardsは@<code>{&&}とか@<code>{||}とか@<code>{?}とか@<code>{!}とかの論理演算子にもちゃんと対応しています（@<list>{typeGuards/operator.ts}）。

//list[typeGuards/operator.ts][ブール代数みたいな演算に対応してる]{
#@mapfile(../code/types-advanced/typeGuards/operator.ts)
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

// 三項演算子はif文と一緒の挙動
typeof obj === "string" ? obj.charAt(0) : obj;
// 次と等価
if (typeof obj === "string") {
  obj.charAt(0);
} else {
  obj;
}

// 一応、否定演算子にも対応している
if (!(typeof obj !== "string")) {
  // 否定の否定はそのまんまstringだな！ちゃんと絞り込まれます
  obj.charAt(0);
}

export { }
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
ややこしいですが、エディタ上で各分岐内での変数の型を確認できるため困ることは少ないでしょう。

==={type-guards-weakpoint} Type Guardsの弱点

type guardsは型システム上の仕組みだということを忘れてはいけません。
JavaScriptの実行環境とはまったく関係がないのです。

TypeScriptでは構造的部分型の仕組みにより、クラスが要求されている箇所に互換性のある別の値を代入できます。

その仕組みを使って、@<list>{typeGuards/weakspot.ts}のようなコードが書けてしまいます。

//list[typeGuards/weakspot.ts][構造的部分型とtype guards]{
#@mapfile(../code/types-advanced/typeGuards/weakspot.ts)
class Sample {
  str: string;
}

// 構造的部分型！
let obj: Sample = {
  str: "Hi!",
};

if (obj instanceof Sample) {
  // 型はSampleに絞られている しかし、絶対に到達しない
  // 現在のobjはSampleを親に持たない
  console.log(obj.str);
}

export { }
#@end
//}

#@# @suppress SentenceLength CommaNumber
objはSampleを型として持ち、その値として互換性のあるオブジェクトリテラルを持っています。
コンパイル後のJavaScriptコード（@<list>{typeGuards/weakspot.js}）を見ると、objの値がSampleクラスのインスタンスではないことが一目瞭然です。
しかし、TypeScriptコード上で見ると人間の脳としては型を元に判別していると誤解しがちなので注意が必要です。
不可解な動作に遭遇した場合、JSのコードを確認したりデバッガを併用して実際の処理フローを確認するようにしましょう。

//list[typeGuards/weakspot.js][コンパイル後のJS]{
#@mapfile(../code/types-advanced/typeGuards/weakspot.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Sample {
}
// 構造的部分型！
let obj = {
    str: "Hi!",
};
if (obj instanceof Sample) {
    // 型はSampleに絞られている しかし、絶対に到達しない
    // 現在のobjはSampleを親に持たない
    console.log(obj.str);
}
#@end
//}

これを回避する方法がいくつかあります。

ひとつ目は、ユーザ定義のtype guardsを使う方法。
ふたつ目はprivateな要素をクラスに突っ込んでしまうことです（@<list>{typeGuards/vsWeakspot2-invalid.ts}）。

//list[typeGuards/vsWeakspot2-invalid.ts][privateな要素があれば構造的部分型で値を偽造できない]{
#@mapfile(../code/types-advanced/typeGuards/vsWeakspot2-invalid.ts)
class Sample {
  str: string;
  private _tmp: any;
}

// privateなインスタンス変数があるクラスのインスタンスは偽造できない！
// error TS2322: Type '{ _tmp: null; str: string; }' is not
//     assignable to type 'Sample'. Property '_tmp' is private
//     in type 'Sample' but not in type '{ _tmp: null; str: string; }'.
let obj: Sample = {
  str: "Hi!",
  _tmp: null,
};
#@end
//}

#@# @suppress JapaneseStyle
色々書きましたが、一番の解決策はunion typesやanyをなるべく使わない、真っ当な設計をすることです。

=={intersection-types} 交差型（Intersection Types）

union typesに似た記法の@<kw>{交差型,intersection types}です。
intersection typesは2つの型を合成し、1つの型にできます。
union typesと違って利用頻度は低く、TypeScript的に使いたくなるシチュエーションもほとんどありません。

まずは例を見てみましょう。
ある関数に渡したオブジェクトを拡張し、新しいプロパティやメソッドを生やします（@<list>{intersectionTypes/basic.ts}）。

//list[intersectionTypes/basic.ts][型を合成する]{
#@mapfile(../code/types-advanced/intersectionTypes/basic.ts)
interface Storage {
  $save(): void;
}

function mixinStorage<T>(base: T): T & Storage {
  let modified = base as any;
  modified.$save = () => {
    // めんどいので保存したフリ
    console.log(`データを保存しました！ ${JSON.stringify(base)}`);
  };

  return modified;
}

// 何の変哲もないオブジェクト
let base = {
  name: "TypeScript",
};
// を、Storageを合成する関数に渡す
let obj = mixinStorage(base);

// baseに存在しないメソッドが呼べる！
// データを保存しました！ {"name":"TypeScript"} と表示される
obj.$save();

// もちろん、baseにあったプロパティにもアクセスできる
obj.name = "JavaScript";
// データを保存しました！ {"name":"JavaScript"} と表示される
obj.$save();

export { }
#@end
//}

intersection typesを使うと、型定義ファイルが書きやすくなる場合があります。
例を見てみます（@<list>{intersectionTypes/angularResource.ts}）。
intersection typesを使わない書き方とintersection typesを使った書き方、どちらのほうが理解しやすいでしょうか？

//list[intersectionTypes/angularResource.ts][型の合成で素直な定義を作る]{
#@mapfile(../code/types-advanced/intersectionTypes/angularResource.ts)
// intersection typesを使わない書き方
declare namespace angular.resource1 {
  interface ResourceProvider {
    create<T extends Resource<any>>(): T;
  }

  interface Resource<T> {
    $insert(): T;
  }
  let $resource: ResourceProvider;
}
// 上の定義を使ってみる
namespace sample1 {
  interface Sample {
    str: string;
  }
  // SampleResourceという型を1つ無駄に作らねばならぬ
  // なぜこれで動くのか、トリックがわかるだろうか？
  interface SampleResource extends Sample, angular.resource1.Resource<Sample> { }

  let $obj = angular.resource1.$resource.create<SampleResource>();
  $obj.str = "test";
  let obj = $obj.$insert();
  console.log(obj.str);
}

// intersection typesを使った書き方
declare namespace angular.resource2 {
  interface ResourceProvider {
    create<T>(): T & Resource<T>;
  }

  interface Resource<T> {
    $insert(): T;
  }
  let $resource: ResourceProvider;
}
// 上の定義を使ってみる
namespace sample2 {
  interface Sample {
    str: string;
  }

  // 超簡単…！！
  let $obj = angular.resource2.$resource.create<Sample>();
  $obj.str = "test";
  let obj = $obj.$insert();
  console.log(obj.str);
}

export { sample1, sample2 }
#@end
//}

intersection typesを使いこなした書き方のほうが、圧倒的に謎が少なく素直に書けています。

#@# @suppress ParagraphNumber
=={type-alias} 型の別名（Type Alias）

#@# @<strong>{導入されるバージョン 1.4.0}

@<kw>{型の別名,type alias}もunion typesの扱いを便利にするために導入された機能です。
機能としてはただ単に型をひとまとまりにして、それに名前が付けられるだけです。
それだけです。
例を見てみましょう（@<list>{typeAlias/basic.ts}）。

//list[typeAlias/basic.ts][頻出するunion typesに名前をつける]{
#@mapfile(../code/types-advanced/typeAlias/basic.ts)
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

tuple typesに名前をつけることもできます（@<list>{typeAlias/tuple.ts}）。

//list[typeAlias/tuple.ts][tuple typesに名前をつける]{
#@mapfile(../code/types-advanced/typeAlias/tuple.ts)
// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

let c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
{
  class Point {
    constructor(public x: number, public y: number) {
    }
  }
  class Circle {
    constructor(public p: Point, public r: number) {
    }
  }
  let c2: Circle = new Circle(new Point(1, 2), 3);
  console.log(c2.p, c2.r);
}

export { Point, Circle, c }
#@end
//}

こちらは素直にクラスでやればいいのに、という感じです。

type aliasは型に別名をつけるだけで、コンパイルされると消えてしまう存在です。
そのため、@<list>{typeAlias/doNotHaveInstance-invalid.ts}のようなコードは書くことができません。

//list[typeAlias/doNotHaveInstance-invalid.ts][type aliasは値を作らない]{
#@mapfile(../code/types-advanced/typeAlias/doNotHaveInstance-invalid.ts)
// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = Array<string>;

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
let strArray = new StringArray();
#@end
//}

type aliasは仕様上、interfaceと同じように利用できる場面もあります。
ですが、基本的にはtype aliasよりもinterfaceを使うべきです。
interfaceは定義の統合ができるので後から自由に拡張することができます。
柔軟性が高いのです。
interfaceが基本、type aliasは応用、と考えておきましょう。

#@# @suppress ParagraphNumber SectionLength ParenthesizedSentence
=={primitive-literal-types} プリミティブ値のリテラル型（String, Number, Boolean and Enum Literal Types）

#@# numberとbooleanもリテラル型に使えるようになった in 2.0.3
文字列リテラル、数値リテラル、真偽値のリテラルを型として使える機能です。
文字列以外の値をリテラル型として使える機能は2.0.3で入りました。
パッと読んだだけでは、意味がわからないですね。
まずは例を見てみましょう（@<list>{literalTypes/basic.ts}）。

//list[literalTypes/basic.ts][トランプのスート（マーク）を型として表す]{
#@mapfile(../code/types-advanced/literalTypes/basic.ts)
// "文字列" が 型 です。値ではない！
type Suit = "Heart" | "Diamond" | "Club" | "Spade";

// OK
let suit = "Heart";
// NG suitの型に含まれていない
// error TS2322: Type '"Joker"' is not
//   assignable to type '"Heart" | "Diamond" | "Club" | "Spade"'.
// suit = "Joker";

// number, boolean, enum
type Digit = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7;
type Bool = true | false;
type Falsy = false | 0 | "" | null | undefined;

enum SuitEnum {
  Heart,
  Diamond,
  Club,
  Spade,
}
type RedSuit = SuitEnum.Heart | SuitEnum.Diamond;

export { Suit, suit, Digit, Bool, Falsy, SuitEnum, RedSuit }
#@end
//}

文字列が型というのは見慣れないとなかなか気持ちが悪いですね。
しかし、この機能はTypeScriptがJavaScriptの現実と折り合いをつける上で重要な役割があります。
たとえば、DOMのaddEventListenerなどです。
指定するイベント名によって、イベントリスナーの型@<fn>{this-definition-was-changed}が変わります（@<list>{literalTypes/eventListener.d.ts}）。
#@# OK REVIEW lc: "指定するイベント名によって、イベントリスナーの型が変わります"？

//footnote[this-definition-was-changed][なお、現在のDOM周りの型定義は後述するkeyofを使って書き直されています]

//list[literalTypes/eventListener.d.ts][イベント名によって型が変わる]{
#@mapfile(../code/types-advanced/literalTypes/eventListener.d.ts)
// lib.dom.d.ts から抜粋
// 第一引数で指定するイベントによってリスナーで得られるイベントの型が違う
interface HTMLBodyElement extends HTMLElement {
  addEventListener(
    type: "change",
    listener: (this: this, ev: Event) => any,
    useCapture?: boolean): void;
  addEventListener(
    type: "click",
    listener: (this: this, ev: MouseEvent) => any,
    useCapture?: boolean): void;
  addEventListener(
    type: "keypress",
    listener: (this: this, ev: KeyboardEvent) => any,
    useCapture?: boolean): void;
  addEventListener(
    type: string,
    listener: EventListenerOrEventListenerObject,
    useCapture?: boolean): void;
}
#@end
//}

これにより、自然にTypeScriptでコードを書くだけでリスナーで受け取れるイベントの型が自動的に適切なものに絞りこまれます@<fn>{js-dirty-overload}。

//footnote[js-dirty-overload][こんなものが必要になってしまうJavaScriptの複雑さよ…]

#@# Discriminated union types タグ付きUnion型 in 2.0.0 Beta
またunion typesとリテラル型を組み合わせ、switchで条件分岐ができます（@<list>{literalTypes/switch.ts}）。

//list[literalTypes/switch.ts][Union Typesはswitchで優遇されている]{
#@mapfile(../code/types-advanced/literalTypes/switch.ts)
// 足し算
interface Add {
  type: "add";
  left: Tree;
  right: Tree;
}
// 末端の値
interface Leaf {
  type: "leaf";
  value: number;
}

type Tree = Add | Leaf;

// (10 + 3) + 5 を表現する
let node: Tree = {
  type: "add",
  left: {
    type: "add",
    left: { type: "leaf", value: 10 },
    right: { type: "leaf", value: 3 },
  },
  right: {
    type: "leaf",
    value: 5,
  },
};

// 18 と表示される
console.log(calc(node));

function calc(root: Tree): number {
  // プロパティの値で型の絞込ができる！
  switch (root.type) {
    case "leaf":
      // 型は Leaf で決定！
      return root.value;
    case "add":
      // 型は Add で決定！
      return calc(root.left) + calc(root.right);
    default:
      throw new Error("unknown node");
  }
}

export { }
#@end
//}

=={polymorphic-this-type} 多態性のあるthis型（Polymorphic 'this' Type）

@<code>{this}を型として用いることができます。
たとえば@<list>{polymorphicThisType/basic.ts}のようなコードです。

//list[polymorphicThisType/basic.ts][thisを型として用いる]{
#@mapfile(../code/types-advanced/polymorphicThisType/basic.ts)
// 自分自身を型として表す時、this を利用する
class A {
  _this: this;
  a(): this {
    return this;
  }

  d(arg: this): this {
    return arg;
  }

  e() { // thisをreturnした場合暗黙的に返り値もthisとなる
    return this;
  }
}

class B extends A {
  b() {
    console.log("B");
  }
}

interface C extends A {
  c(): void;
}

// a() はクラスAのメソッドだが返り値の型はB自身だ！
new B().a().e().b();

// d() もクラスAのメソッドだが引数はBでなければならぬ
new B().d(new B()).b();

// d() はクラスAのメソッドだが、Bに生えている限りAを渡したら怒られてしまう
// error TS2345: Argument of type 'A' is not assignable to parameter of type 'B'.
//   Property 'b' is missing in type 'A'.
// new B().d(new A()).b();

// プロパティについても同様にB自身になる
new B()._this.b();

// インタフェースでもOK C自身になる
let c: C = null as any;
c.a().c();

export { }
#@end
//}

thisを型として記述するという発想がすごいですね。
引数や返り値の型としてthisを利用しています。
fluentな、メソッドチェーンで使うAPIを組み立てる場合に役立ちそうです。
この書き方がないと、ジェネリクスなどを使ってごまかさなければならないところでしょう。

とはいえ、仮引数の型として使うと実際に受け取れる値に対して制約がきつくなる場合があるため乱用は控えましょう。
@<code>{return this;}を使った時に、メソッドの返り値が暗黙的に@<code>{this}になるのを利用する、くらいがちょうどよいかもしれません。

#@# @suppress JapaneseAmbiguousNounConjunction
=={specifying-this-types-for-functions} 関数のthisの型の指定（Specifying This Types For Functions）

JavaScriptでは@<code>{Function.prototype.bind}や@<code>{Function.prototype.call}、@<code>{Function.prototype.apply}などの関数により、関数呼び出し時のthisの値の型を変更できます。
この仕様は悪しき仕様だと筆者は思いますが、jQueryやDOMなど、古めのAPIではこの仕様をAPIとして組み込んだものが存在しています。
TypeScriptでもこの仕様に対して自然なサポートを与えようと頑張っています。

まずは簡単な例を見てみます（@<list>{thisTypeForFunctions/basic.ts}）。
関数の1つ目の仮引数の名前を@<code>{this}にするだけです。

//list[thisTypeForFunctions/basic.ts][thisの型を指定する]{
#@mapfile(../code/types-advanced/thisTypeForFunctions/basic.ts)
// 関数内部でのthisの型を偽の第一引数で指定
function testA(this: string) {
  console.log(this.toUpperCase());
}

// こういう利用を想定しているはず
// TYPESCRIPT と表示される
testA.bind("TypeScript")();

// 普通に呼び出すとエラーになる
// error TS2684: The 'this' context of type 'void'
//   is not assignable to method's 'this' of type 'string'.
// testA();

// 1つ目の仮引数がthisの型指定だった場合、それは偽物の仮引数
// 実際に何かを渡すとエラーになってしまう
// error TS2346: Supplied parameters do not match any signature of call target.
// testA("TypeScript");

function testB() {
  // --noImplicitThisオプション利用時、関数内でthisにアクセスすると怒られる
  // error TS2683: 'this' implicitly has type 'any'
  //   because it does not have a type annotation.
  // console.log(this.toUpperCase());
}

function testC(this: string, postfix: string) {
  console.log(`${this.toUpperCase()}${postfix}`);
}
// TYPESCRIPT! と表示される
testC.bind("TypeScript")("!");

export { testB }
#@end
//}

thisの値がすり替えられるときの挙動に対応できています。
@<code>{--noImplicitThis}オプションを利用すると、thisの型指定がない（曖昧な）関数内でthisへアクセスするとエラーになります。
thisを使わない限りはエラーにならないため、常用していきましょう。

この仕様が現実世界でどう役に立つかを紹介します（@<list>{thisTypeForFunctions/eventListener.ts}）。

#@# TODO このあたり抜粋するコードを選び直したほうがよさそう
//list[thisTypeForFunctions/eventListener.ts][thisの値が差し替えられるAPIに対応]{
#@mapfile(../code/types-advanced/thisTypeForFunctions/eventListener.ts)
// lib.dom.d.ts から抜粋
// listenerの仮引数の先頭が偽の仮引数で、thisの型の指定が行われている
interface HTMLBodyElement extends HTMLElement {
  addEventListener(
    type: "click",
    listener: (this: this, ev: MouseEvent) => any,
    useCapture?: boolean): void;
  addEventListener(
    type: string,
    listener: EventListenerOrEventListenerObject,
    useCapture?: boolean): void;
}

let el1: HTMLBodyElement = null as any;
el1.addEventListener("click", function() {
  // thisの型はHTMLBodyElement
  this.innerText = "Hi!";
});
el1.addEventListener("click", () => {
  // アロー関数の場合thisの値は変えられない
  // error TS2683: 'this' implicitly has type 'any'
  //   because it does not have a type annotation.
  // this.innerText = "Hi!";
});

let el2: HTMLDivElement = null as any;
el2.addEventListener("click", function() {
  // thisの型はHTMLDivElement
  this.innerText = "Hi!";
});

export { }
#@end
//}

イベント発生時のコールバック関数でthisが差し替えられる場合に対応できています。

自分でTypeScriptコードを書く時にこの記法が必要になる機会は少なくありたいものです。
しかし、型定義ファイルを作成する時にはお世話にならざるをえないときがあるでしょう。

=={thistype} ThisTypeでthisの型を制御する

#@# thisの型のコントロールがより柔軟に行われるようになった Controlling this in methods of object literals through contextual type in 2.3RC
@<code>{--noImplicitThis}オプションを利用した場合、オブジェクトリテラル中のthisがオブジェクトリテラル自体を指すよう正しく認識されます。
さらに、オブジェクトリテラル中でのthisの型を強力にコントロールしたい場合、@<code>{ThisType<T>}を利用できます（@<list>{types-advanced/thisType/basic.ts}）。

//list[types-advanced/thisType/basic.ts][ThisType<T>の利用例など]{
#@mapfile(../code/types-advanced/thisType/basic.ts)
const obj1 = {
  name: "maya",
  greeting() {
    console.log(`Hello, ${this.name}`);

    // 存在しないプロパティにアクセスするとちゃんとエラーにしてくれる
    // error TS2339: Property 'notExists' does not exist on
    //   type '{ name: string; greeting(): void; }'.
    // console.log(`Hello, ${this.notExists}`);
  },
};
console.log(obj1.greeting());

// ThisType を使ってthisの値を無理やり制御する
interface A {
  name: string;
}
interface B {
  hello(): void;
}

// objの型はBであり、obj内でのthisの型はAと明示的に指定する
const obj: B & ThisType<A> = {
  hello() {
    // this.name は A的には存在する！
    console.log(`Hello, ${this.name}`);
    // Aに存在しないものは存在しない扱い ちなみに this.hello も存在しない扱い
    // error TS2339: Property 'notExists' does not exist on type 'A'.
    // console.log(`Hello, ${this.notExists}`);
  },
};
// objは B なので問題なくhelloにアクセスできる
obj.hello();

export { }
#@end
//}

オブジェクトリテラルの中でthisの値が期待どおりの値になっています。
ThisTypeはいわゆるマーカーで、その型がついていること自体に意味があります。
あまり出番が必要ないほうが嬉しい機能ではありますが、これを使うことでthisの型付けをより安全にできる場合もあります。

#@# TODO https://github.com/Microsoft/TypeScript/issues/17041
#@# コンパイラのバグで、報告したら直してもらえたけどリリースは2.5.0だと思われるので今回は寝かせる

=={local-types} ローカル型（Local Types）

ローカル型は通常より小さい範囲で、クラスやインタフェースやenumやtype aliasを定義できます（@<list>{localType/basic.ts}）。

//list[localType/basic.ts][ローカル型を試す]{
#@mapfile(../code/types-advanced/localType/basic.ts)
{
  type Data = string | number;
  let obj: Data = 1;

  console.log(obj);
}
{
  type Data = number | Date;
  let obj: Data = 1;

  console.log(obj);
}

// ブロックスコープの外ではもはやData型を参照することはできない
// error TS2304: Cannot find name 'Data'.
// let obj: Data;

{
  // クラス、enum、Buzzなども
  class Foo { }
  enum Bar {
    a,
    b,
  }
  interface Buzz { }

  console.log(Foo, Bar.a, null as any as Buzz); // 警告消し
}
// もちろんブロックスコープの外では上記3つは参照できない

export { }
#@end
//}

使う機会は少ないかもしれませんが、@<list>{localType/inMethod.ts}のようにメソッドの中で簡易に別名を用意したい場合などに利用できるでしょう。

//list[localType/inMethod.ts][メソッド内でだけ通用する別名]{
#@mapfile(../code/types-advanced/localType/inMethod.ts)
// 現実的な活用例
class Foo {
  method() {
    // メソッド内でのみ使えるtype alias！
    type Data = string | number;
    let obj: Data = 1;

    console.log(obj);
  }
}

export { Foo }
#@end
//}

=={type-queries} 型クエリ（Type Queries）

@<kw>{型クエリ,Type Queries}は指定したクラスや変数などの型をコピーします。
たとえば、クラスそのものを型として指定したい場合、@<list>{typeQueries/basic.ts}のように型クエリを使います。

//list[typeQueries/basic.ts][クラスそのものの型を指定する]{
#@mapfile(../code/types-advanced/typeQueries/basic.ts)
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
  new(): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;

export { }
#@end
//}

メソッドなどの値も取れますが、thisを使うことはできないため、少しトリッキーなコードになる場合もあります。
@<list>{typeQueries/cheapTrick.ts}の例は、prototypeプロパティを使っているためJavaScriptの知識が試されます。

//list[typeQueries/cheapTrick.ts][prototypeを参照するとメソッドの型が取れる]{
#@mapfile(../code/types-advanced/typeQueries/cheapTrick.ts)
class Sample {
  hello = (word = "TypeScript") => `Hello, ${word}`;
  bye: typeof Sample.prototype.hello;
}

let obj = new Sample();
obj.bye = obj.hello;

export { }
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
型クエリはわざわざインタフェースを定義するのもめんどくさいけど…というときに使える場合があります。
@<list>{typeQueries/copy.ts}では、ひとつ目の引数の型をふたつ目の引数や返り値の型にコピーして使っています。

//list[typeQueries/copy.ts][ここまで複雑にするならインタフェース使って]{
#@mapfile(../code/types-advanced/typeQueries/copy.ts)
// このコードは（死ぬほど読みにくいけど）正しい
function move(
  p1: { x1: number; y1: number; x2: number; y2: number; },
  p2: typeof p1,
): typeof p1 {
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

export { }
#@end
//}

この例のレベルまでやってしまうとさすがに読みにくくなるのでインタフェースをひとつ定義したほうがよいですね。

#@# @suppress ParagraphNumber SectionLength
=={tuple-types} タプル型（Tuple Types）

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

#@# @<strong>{導入されたバージョン 1.3.0}

#@# @suppress JapaneseAmbiguousNounConjunction
@<kw>{タプル,tuple}は、任意の数の要素の組です。
JavaScriptではタプルはサポートされていないため、TypeScriptでのタプルはただのArrayで表現されます。

#@# OK mhidaka タプル型（Tuple Types）の名称がtupleまたはタプル型、タプルの3種類が登場するので統一を図ってほしい
#@# タプルとタプル型に統一

既存のJavaScript資産を使おうとしたときに、配列の形で多値を返してくるライブラリが稀にあります。
タプル型はそういったときに使うためのもので、TypeScriptでコードを書く際に多用するものではありません。
というのも、普通にコードを書いている限りでは型推論の結果としてタプル型が出てこないのです。

タプル型は型（TypeScript）の世界にしか登場せず、コンパイル後のJavaScriptコードでは消えてしまいます。
記述方法は配列の型指定へ@<code>{[typeA, typeB]}のように配列の要素の代わりに型名を記述していくだけです。
例を見てみましょう（@<list>{tuple/basic.ts}）。

//list[tuple/basic.ts][基本的なtupleの例]{
#@mapfile(../code/types-advanced/tuple/basic.ts)
// まずは今までどおりの配列から
// これは別の箇所で解説している union typesで表現され (number | string | boolean)[]
let array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
let tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesはArrayでもあるのだ
tuple.forEach(v => console.log(v));

export { array }
#@end
//}

各要素の型を指定すると、要素のindexでアクセスしたときに適切な型で扱われます。

さて、タプル型について重箱の隅をつついていきましょう。
要素数が多すぎる場合、指定されていない値の型はunion typesになります。
その例を見てみましょう（@<list>{tuple/manyValues.ts}）。

//list[tuple/manyValues.ts][値の要素数が多すぎる場合]{
#@mapfile(../code/types-advanced/tuple/manyValues.ts)
// 要素が多い分にはOKだ！
let tuple: [string, number] = ["str", 1, "test"];

// 範囲外の要素の型はすべての要素のunion、つまり string | number になる。
let value = tuple[2];

// 以下はダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

次は要素の順序がずれた場合、どうなるかを見てみましょう（@<list>{tuple/unshift.ts}）。

//list[tuple/unshift.ts][順序の変化には弱い]{
#@mapfile(../code/types-advanced/tuple/unshift.ts)
let tuple: [string, number] = ["str", 1];

// 先頭をnumberに…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);

export { }
#@end
//}

悲しい結果になりました。
@<code>{[1, true]}のような配列のリテラルをタプル型に推論しないのはおそらくこのためです。

@<code>{unshift}や@<code>{pop}など、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptではTypeScriptコンパイラ側ですべてをキャッチアップすることは不可能です。
タプル型を扱う場合は要素数を変更するような操作をしないほうがよいでしょう。

なるべくなら、タプルは使いたくないですね。

=={non-null-assertion-operator} 非null指定演算子（Non-null Assertion Operator）

非null指定演算子（@<code>{!}）は、変数やプロパティの末尾に指定して利用します。
値が@<code>{null}や@<code>{undefined}ではないことを人力でコンパイラに教えてやるための記法です。
新規にコードを書き起こすのであれば非null指定演算子は使わないほうがよいでしょう。
基本的に使わずに済ませたいものです。

しかしながら、昔からメンテしているTypeScriptコードについてはこの演算子に頼らざるをえない場合もあります。
@<code>{--strictNullChecks}オプションを有効にしたい場合、省略可能なプロパティではundefinedのチェックが必須になります。
警告を低コストに抑制したい場合、非null指定演算子は有効な対処法となります。
もちろん、将来的には徐々にリファクタリングして、この演算子の利用箇所を消滅させていくべきです。

例を見てみましょう（@<list>{nonNullAssertionOperator/basic.ts}）。

//list[nonNullAssertionOperator/basic.ts][!演算子を使う]{
#@mapfile(../code/types-advanced/nonNullAssertionOperator/basic.ts)
import * as fs from "fs";

interface Config {
  filePath?: string | null;
  verbose?: boolean;
}

// 呼び出し元で値をしっかり代入していても...
let config: Config = {};
config.filePath = "settings.json";
config.verbose = false;
processA(config);
function processA(config: Config = {}) {
  // 関数内部ではConfigのプロパティはundefinedの可能性が排除できない…
  // よって、! で無理やりエラーを消す必要がある
  if (fs.existsSync(config.filePath!)) {
    console.log(fs.readFileSync(config.filePath!, "utf8"));
  }
}

function processB(config: Config = {}) {
  // 関数内で初期値を設定してやるとエラーを解消できる（かしこい）
  config.filePath = config.filePath || "settings.json";
  config.verbose = config.verbose || false;

  // 初期値設定済なので ! 不要
  if (fs.existsSync(config.filePath)) {
    console.log(fs.readFileSync(config.filePath, "utf8"));
  }

  // undefinedではなくした結果は関数をまたいで引き継がれない
  // 残念だが当然…
  processA(config);
}

// Configのundefinedとnull無し版
interface ConfigFixed {
  filePath: string;
  verbose: boolean;
}

function processC(config: Config = {}) {
  // ? 除去版に値を詰め替える
  const fixed: ConfigFixed = {
    filePath: config.filePath || "settings.json",
    verbose: config.verbose || false,
  };

  if (fs.existsSync(fixed.filePath)) {
    console.log(fs.readFileSync(fixed.filePath, "utf8"));
  }
}

export { Config, processB, processC }
#@end
//}

人間がundefinedやnullではないと確信できる場合、エラーとなる箇所の末尾に@<code>{!}をつけていきます。
非null指定演算子をなるべく使わない手段として使う前に初期値を代入する、undefinedやnullを含まない型の値に詰め直すなどが考えられます。

他の方法も見てみます（@<list>{nonNullAssertionOperator/intersectionTypes.ts}）。
先に見た@<list>{nonNullAssertionOperator/basic.ts}もあわせ、undefined、nullフリーな型を用意して処理の途中からそちらに乗り換えるのが王道でしょう。

//list[nonNullAssertionOperator/intersectionTypes.ts][デフォルト値と付き合う]{
#@mapfile(../code/types-advanced/nonNullAssertionOperator/intersectionTypes.ts)
interface Config {
  filePath?: string | null;
  verbose?: boolean;
}

// Configのundefinedとnull無し版
interface ConfigFixed {
  filePath: string;
  verbose: boolean;
}

let config: Config = {
  verbose: true,
};
// filledの型は ConfigFixed & Config
// assignの定義が引数4つまではintersection typesで定義されているため
// assign<T, U, V>(target: T, source1: U, source2: V): T & U & V; が実際の定義
let defaultConfig: ConfigFixed = { filePath: "settings.json", verbose: false };
let filled = Object.assign({}, defaultConfig, config);

// filledはfilePathとverboseが存在することが確定しているのでConfigFixedと互換性がある！
let fixed: ConfigFixed = filled;
console.log(fixed);

export { ConfigFixed, fixed }
#@end
//}

Control flow based type analysisが賢く処理してくれることに賭けるか、@<code>{Object.assign}などを使い、intersection typesを上手く活用します。

#@# @suppress SentenceLength ParenthesizedSentence
他によい方法が思いついたら、ぜひ筆者にその方法を教えてください。
筆者としてはもう少しControl flow based type analysisと構造的部分型の相性がよいと楽だなと考え、TypeScriptリポジトリに提案もしてみました@<fn>{issue10065}。
しかし、設計上の制約（ようするに実装が難しい）ので現時点では対応策無し、とのことでした。
残念。

//footnote[issue10065][@<href>{https://github.com/Microsoft/TypeScript/issues/10065}]

=={mixin-classes} クラスのMixin

#@# クラスのMixinパターンのサポート Mixin classes in 2.2.1
クラスにMixinで要素や機能を追加できます。
拡張用の関数に対してコンストラクタを渡すと機能拡張する形で継承したものを返す、というだけの関数です（@<list>{mixin/basic.ts}）。

#@# OK mhidaka mixin Mixinで表記に揺れ
#@# ミックスインにするか悩んだけどMixinに統一するか…って感じ

//list[mixin/basic.ts][任意のクラスにMixinで機能を追加する]{
#@mapfile(../code/types-advanced/mixin/basic.ts)
type Constructor<T> = new (...args: any[]) => T;

function Tagged<T extends Constructor<object>>(Base: T) {
  return class extends Base {
    tag = "";
    constructor(...args: any[]) {
      super(...args);
    }
  };
}

class Score {
  constructor(public point: number) { }
}

// Mixinできる
const TaggedScore = Tagged(Score);

const ts = new TaggedScore(1);
ts.tag = "vv";
console.log(ts.tag, ts.point);

// これはちゃんと怒られる
// error TS2345: Argument of type '"s"' is not assignable to
//   parameter of type 'number'.
// new TaggedScore("s");

// Mixinしたクラスも分け隔てなく継承できる
class RankingScore extends TaggedScore {
  constructor(public rank: number, tag: string, point: number) {
    super(point);
    this.tag = tag;
  }
}

const rs = new RankingScore(1, "vv", 100);
console.log(rs.rank, rs.tag, rs.point);
#@end
//}

過去に存在した問題として、このやり方をしてもTypeScriptがreturnしている新しいクラスに対して上手に型付けができませんでした。
これを、TypeScriptでは@<list>{mixin/compat.ts}のようなルールを設けることで回避しました。

//list[mixin/compat.ts][Mixinのためのルール]{
#@mapfile(../code/types-advanced/mixin/compat.ts)
// 2つのコンストラクタとそれぞれの返り値の型
// コンストラクタの片方は ...args: any[] を引数に取る
type ConstructorA<T, U> = { new(s: string): T } & { new(...args: any[]): U };

// 1つのコンストラクタと返り値は2つの型の交差型
type ConstructorB<T, U> = { new(s: string): T & U };

// 2つの定義には互換性がある！
const A: ConstructorA<Date, RegExp> = null as any;
const B: ConstructorB<Date, RegExp> = A;
const A2: ConstructorA<Date, RegExp> = B;
#@end
//}

Mixinクラスのコンストラクタ引数が@<code>{...args: any[]}なのがポイントですね。
@<list>{mixin/basic.ts}で出てくるTagged関数は@<code>{ConstructorA}に似た型定義になりますが、他の箇所では@<code>{ConstructorB}のように扱われるため、意図どおりの活用ができるようになります。

Mixin用の関数について命名規則は言及されている媒体によってCamelCaseやcamelCaseのようにバラバラで、まだコンセンサスがないようです。
ここではTypeScriptのWhat's newの規則に則っています。

=={keyof-and-mapped-types} keyofと型の写像（keyof and Mapped Types）

#@# keyof と 型の切り出し Static types for dynamically named properties (keyof T and T[K]) in 2.1.4
#@# ある型のフィールドの修飾子の変換(Map処理)が可能に apped types (e.g. { [P in K]: T[P] }) in 2.1.4

keyofと型の写像の組み合わせは、複雑な機能です。
Mapped Typesにどういう訳語を当てるべきか大変悩んだ@<fn>{equivalent-word}のですが、型の写像という訳にしました。

//footnote[equivalent-word][訳語について悩むことが多いのですが、口頭で人と喋る時は原語のままの場合が多いので本でもそうしたほうがいいのかもしれない…]

この節ではざっくりと次の事柄について順に説明していきます。

 1. 型のルックアップ（Lookup Types）
 2. keyof演算子
 3. 型に対してMap処理をかける（型の写像の作成）
 4. 組み込みの型に対するMap処理の紹介
 5. 合せ技の紹介

まずは型のルックアップ（Lookup Types）です。
任意のプロパティの型を参照できる機能です。
コード例を見てみます（@<list>{keyofAndMappedTypes/lookupTypes.ts}）。

#@# OK mhidaka 切り出しは適切な訳文か？ lookは洗う？洗い出しのほうが日本語ぽくないかな？
#@# 切り出しも洗い出しも検索もちょっと微妙だったのでルックアップをそのまま使うことにした

//list[keyofAndMappedTypes/lookupTypes.ts][ある型のプロパティの型を参照できる]{
#@mapfile(../code/types-advanced/keyofAndMappedTypes/lookupTypes.ts)
interface Cat {
  kind: string;
  name: string;
  age: number;
}

// Catのkindプロパティの型を指す！
// つまり string である
type KindType = Cat["kind"];

// kindとageのどちらかの型なので string | number になる
type NameOrAge = Cat["kind" | "age"];
#@end
//}

型注釈でインデックスアクセスのような記法で指定したプロパティの型が参照できます。
ドットアクセスだとコンパイルエラーになるので注意しましょう。

全プロパティのキー名を列挙するにはkeyof演算子を使います（@<list>{keyofAndMappedTypes/keyof.ts}）。

//list[keyofAndMappedTypes/keyof.ts][keyof演算子である型のプロパティ名を列挙できる]{
#@mapfile(../code/types-advanced/keyofAndMappedTypes/keyof.ts)
interface Cat {
  kind: string;
  name: string;
  age: number;
}

// 全プロパティのキー名
// つまり "kind" | "name" | "age" になる
type CatPropertyNames = keyof Cat;
#@end
//}

型に対してプロパティが増減した時も自動的に対応できる点がいいですね。

型のルックアップやkeyof演算子は単体ではあまり使いみちが思いつきませんが、これから説明する型の写像処理やジェネリクスと組み合わせると力を発揮します。

型の写像処理には書き方の基本が4パターンあります。

//emlist{
{ [ P in K ] : T }
{ [ P in K ] ? : T }
{ readonly [ P in K ] : T }
{ readonly [ P in K ] ? : T }
//}

みただけでは全然わからないですね。
全体としてはインデックスシグニチャと同じ書式として読み解くことができます。

@<code>{K}に含まれる@<code>{P}という値の型にあたる@<code>{T}と読めばいいのでしょうか。
前半部分にあたる@<code>{[ P in K ]}相当の部分で操作対象となるプロパティの一覧を定義します。
それと対になるように変換したプロパティの値を定義します。

この書き方を理解するために、TypeScriptの標準ライブラリに入っているビルトインのパーツを確認していきます（@<list>{keyofAndMappedTypes/buildinTypes.ts}）。

#@# OK mhidaka 癖かもしれないがちょいちょいかかりつけが遠い修飾語を発見する。
#@# 自分では気がついてないけど羊の修正例見ると確かにそうだなーってなる 今後自力で直せる予感はあまりしない

//list[keyofAndMappedTypes/buildinTypes.ts][型の写像を作るビルトインのパーツたち]{
#@mapfile(../code/types-advanced/keyofAndMappedTypes/buildinTypes.ts)
// 指定した型の全ブロパティを省略可能にする
type Partial<T> = {
  [P in keyof T]?: T[P];
};

// 指定した型の全プロパティをreadonly扱いにする
type Readonly<T> = {
  readonly [P in keyof T]: T[P];
};

// 指定した型の、指定したプロパティだけを集めたサブセット型を作る
type Pick<T, K extends keyof T> = {
  [P in K]: T[P];
};

// 指定した型のプロパティの値の型を変換した型を作る
type Record<K extends string, T> = {
  [P in K]: T;
};

export { Partial, Readonly, Pick, Record }
#@end
//}

写像処理と型のルックアップを組み合わせ、実用的な変換処理を作り出しています。
これだけだとピンとこないでしょうから、利用例を見てみます（@<list>{keyofAndMappedTypes/buildinTypesUsage.ts}）。

//list[keyofAndMappedTypes/buildinTypesUsage.ts][ビルトインの型の利用例]{
#@mapfile(../code/types-advanced/keyofAndMappedTypes/buildinTypesUsage.ts)
interface Cat {
  kind: string;
  name: string;
  age?: number;
}

let c1: Cat = {
  kind: "NorwegianForestCat",
  name: "アルファ",
};

let c2: Partial<Cat> = {
  kind: "NorwegianForestCat",
  // name, age が欠けていてもエラーにならない
};

let c3: Readonly<Cat> = {
  kind: "NorwegianForestCat",
  name: "アルファ",
  age: 3,
};
// readonly なので代入しようとするとエラーになる
// error TS2540: Cannot assign to 'name'
//   because it is a constant or a read-only property.
// c3.kind = "TypeScript";

let c4: Pick<Cat, "name" | "age"> = {
  // kind は K に含まれていないので不要
  name: "アルファ",
  // age はもともとOptional
};

let c5: Record<keyof Cat, boolean> = {
  // 全てのプロパティの型はbooleanを要求される
  kind: true,
  name: true,
  age: true, // 必須になる
};

export { c1, c2, c3, c4, c5 }
#@end
//}

雰囲気が掴めてきませんか。
とりあえずビルトイン型の存在を認識して、便利に使えるようになるところから始めるのがよさそうです。

これらの武器をうまく使うと、JavaScriptの自然なコードに対して複雑な型チェックを実施できます（@<list>{keyofAndMappedTypes/usage-invalid.ts}）。

//list[keyofAndMappedTypes/usage-invalid.ts][この節のパーツを組み合わせてより強い安全さを作り出す]{
#@mapfile(../code/types-advanced/keyofAndMappedTypes/usage-invalid.ts)
interface PropertyDescriptor<T> {
  configurable?: boolean;
  enumerable?: boolean;
  value?: T;
  writable?: boolean;
  get?(): T;
  set?(v: T): void;
}
function defineProperty<T, K extends keyof T>(o: T, p: K,
    attributes: PropertyDescriptor<T[K]>): any {
  return Object.defineProperty(o, p, attributes);
}

interface Foo {
  a?: string;
}

let foo: Foo = {};

// 正しい組み合わせ a に string
defineProperty(foo, "a", {
  enumerable: false,
  value: "a",
});

// ダメ a に number
// error TS2345: Argument of type '{ enumerable: false; value: number; }'
//   is not assignable to parameter of type 'PropertyDescriptor<string>'.
//  Types of property 'value' are incompatible.
//    Type 'number' is not assignable to type 'string'.
defineProperty(foo, "a", {
  enumerable: false,
  value: 1,
});

// ダメ b は存在しない
// error TS2345: Argument of type '"b"' is not assignable to
//   parameter of type '"a"'.
defineProperty(foo, "b", {
  enumerable: false,
  value: "a",
});

export { }
#@end
//}

このような工夫をこらしたコードを自力で1からひねり出せるかというとかなりの難易度です。
功夫が必要です。
やっていきましょう。
