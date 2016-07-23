= アドバンスド型戦略

#@# TODO 文脈依存型
#@# TODO 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.11.8
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
  new (): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;

export { }
#@end
//}

メソッドなどの値も取れますが、thisを使うことはできないため、少しトリッキーなコードになる場合もあります。
@<list>{typeQueries/cheapTrick}の例は、prototypeプロパティを使っているためJavaScript力が多少ないと思いつかないかもしれません。

//list[typeQueries/cheapTrick][prototypeを参照するとメソッドの型が取れる]{
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
@<list>{typeQueries/copy}では、ひとつ目の引数の型をふたつ目の引数や返り値の型にもコピーして使っています。

//list[typeQueries/copy][ここまで複雑にするならインタフェース使って]{
#@mapfile(../code/types-advanced/typeQueries/copy.ts)
// このコードは（死ぬほど読みにくいけど）正しい
function move(p1: { x1: number; y1: number; x2: number; y2: number; },
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

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
  console.log(v);
});

export { array }
#@end
//}

各要素の型を指定すると、その要素のindexでアクセスしたときに適切な型で扱われます。

もちろん、タプル型はGenericsと組み合わせて利用できます（@<list>{tuple/withGenerics}）。

//list[tuple/withGenerics][Genericsでの利用も可]{
#@mapfile(../code/types-advanced/tuple/withGenerics.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
  return [v1, v2];
}

let tuple = zip("str", { hello(): string { return "Hello!"; } });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello();   // おー、静的に検証される！

export { }
#@end
//}

Good！いいですね。

さて、タプル型について重箱の隅を見ていきましょう。
要素数が多すぎる場合、指定されていない値の型はunion typesになります。
その例を見てみましょう（@<list>{tuple/manyValues}）。

//list[tuple/manyValues][値の要素数が多すぎる場合]{
#@mapfile(../code/types-advanced/tuple/manyValues.ts)
// 要素が多い分にはOKだ！
let tuple: [string, number] = ["str", 1, "test"];

// 範囲外の要素の型はすべての要素のunion、つまり string | number になる。
let value = tuple[2];

// 以下はダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

お次は要素の順序がズレた場合、どうなるかを見てみましょう（@<list>{tuple/unshift}）。

//list[tuple/unshift][絶望に身をよじれ…！]{
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

#@# @suppress InvalidExpression CommaNumber JapaneseAmbiguousNounConjunction
では解説していきましょう。
union typesはいわゆる直和型です。
この変数の値の型は、アレか、コレか、ソレのどれかが入っています！ということを表します。

なんのために入ったのかというと、まずは既存JavaScriptによりよい型定義を与えるためでしょう。
そして、nullやundefined、string literal typesなどTypeScriptの中でも適用領域が広がっています。
JavaScriptという現実と、安全な世界を構築するTypeScriptの橋渡しを上手にしてくれる機能といえます。

ちなみに、自分でTypeScriptコード書いてるときに欲しくなる機能ではあまりありません。

まずは簡単な例から見ていきましょう（@<list>{unionTypes/basic}）。

//list[unionTypes/basic][型A | 型B でAかBのどちらかを表す]{
#@mapfile(../code/types-advanced/unionTypes/basic.ts)
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

export { b1, b2, c }
#@end
//}

型注釈を書く際に複数の型を｜で区切って書けます。
既存のJavaScriptライブラリだとこういった、返り値の型が複数ある困った関数がかなりあります。
あとは普通にTypeScriptを書いているときでもSyntaxTreeとかをコードから構築するときにはあったほうが便利かもしれません。

ご覧のとおり、union types中の型の順番とかは関係ない（交換可能）し、union typesのunion typesなどは単純にまとめてひとつのunion typesに統合できます。

自然にTypeScriptを書いていて、union typesを目にする機会は3種類あります。
|| 演算子を使ったとき、条件（三項）演算子を使ったとき、配列リテラルを使ったときです（@<list>{unionTypes/inferred}）。

//list[unionTypes/inferred][こういうときは目にしますね]{
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
TypeScript一般のベストプラクティスとして1つの配列で複数の型の値を扱わないほうが堅牢なコードになるため、きれいなコードを書いている限りはあまり見ないかもしれません。

型注釈として関数を与えるときは記法にちょっと気をつけないとコンパイルエラーになります（@<list>{unionTypes/syntax}）。

//list[unionTypes/syntax][型名をカッコで囲うんです？]{
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

見づらいコードになってしまいました。
適切に型にも名前をつけることの重要さが偲ばれます。

union typesな値を使うときは、型アサーションを使うこともできますがなるべくなら避けてとおりましょう（@<list>{unionTypes/typeAssertion}）。
union typesを相手にする場合は、次に説明する@<hd>{typeGuards}を使いましょう。話はそれからだ！

//list[unionTypes/typeAssertion][一応使えるよ こうすれば]{
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

// 値の集合に含まれない型にしてみると普通に怒られる
// error TS2352: Type 'string | number | Date' cannot be converted to type 'RegExp'.
//   Type 'Date' is not comparable to type 'RegExp'.
//     Property 'exec' is missing in type 'Date'.
// (<RegExp>obj).test("test");

export { }
#@end
//}

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

== 交差型（Intersection Types）

union typesに続いて、intersection types（交差型）です。
intersection typesは2つの型を合成し、1つの型にすることができます。
union typesと違って、利用頻度は低く、TypeScript的に使いたくなるシチュエーションもほとんどありません。

まずは例を見てみましょう。
ある関数に渡したオブジェクトを拡張し、新しいプロパティやメソッドを生やします（@<list>{intersectionTypes/basic}）。

//list[intersectionTypes/basic][型を合成する]{
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
例を見てみます（@<list>{intersectionTypes/angularResource}）。
intersection typesを使わない書き方とintersection typesを使った書き方、どちらのほうが理解しやすいでしょうか？

//list[intersectionTypes/angularResource][型の合成で素直な定義を作る]{
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

#@# @suppress SectionLength ←なんかこれ実装バグってない？
== 文字列リテラル型（String Literal Types）

文字列リテラルを、型として、使える機能です。
パッと聞き、意味がわからないですね。
まずは例を見てみましょう（@<list>{stringLiteralTypes/basic}）。

//list[stringLiteralTypes/basic][カードのスートを型として表す]{
#@mapfile(../code/types-advanced/stringLiteralTypes/basic.ts)
// "文字列" が 型 です。値ではない！
let suit: "Heart" | "Diamond" | "Club" | "Spade";

// OK
suit = "Heart";
// NG suitの型に含まれていない
// error TS2322: Type '"Joker"' is not assignable to type '"Heart" | "Diamond" | "Club" | "Spade"'.
// suit = "Joker";

export { }
#@end
//}

文字列が型、というのは見慣れないとすごく気持ちが悪いですね。
しかし、この機能はTypeScriptがJavaScriptの現実と折り合いをつける上で重要な役割があります。
たとえば、DOMのaddEventListenerなどです。
指定するイベント名によって、イベントリスナーの型が変わります（@<list>{stringLiteralTypes/eventListener}）。

#@# OK REVIEW lc: "指定するイベント名によって、イベントリスナーの型が変わります"？

//list[stringLiteralTypes/eventListener][イベント名によって型が変わる]{
#@mapfile(../code/types-advanced/stringLiteralTypes/eventListener.d.ts)
// lib.dom.d.ts から抜粋
// 第一引数で指定するイベントによってリスナーで得られるイベントの型が違う
interface HTMLBodyElement extends HTMLElement {
  addEventListener(type: "change", listener: (this: this, ev: Event) => any, useCapture?: boolean): void;
  addEventListener(type: "click", listener: (this: this, ev: MouseEvent) => any, useCapture?: boolean): void;
  addEventListener(type: "keydown", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
  addEventListener(type: "keypress", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
  addEventListener(type: "keyup", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
  addEventListener(type: "wheel", listener: (this: this, ev: WheelEvent) => any, useCapture?: boolean): void;
  addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
#@end
//}

これにより、自然にTypeScriptでコードを書くだけでリスナーで受け取れるイベントの型が自動的に適切なものに絞りこまれます。
こんなものが必要になってしまうJavaScriptの複雑さよ…。

また、union typesと文字列リテラル型を組み合わせ、switchで条件分岐ができます（@<list>{stringLiteralTypes/switch}）。
switch文によるtype guards（後述）はTypeScript 2.1.0からのサポートが予定されているので、現時点で若干えこひいきされていますね。

//list[stringLiteralTypes/switch][Union Typesはswitchでえこひいきされている]{
#@mapfile(../code/types-advanced/stringLiteralTypes/switch.ts)
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

なお、執筆時点でアンダース・ヘルスバーグ御大が@<i>{Number, enum, and boolean literal types}というpull requestを作成、作業しています@<fn>{primitive-literal-types}。

//footnote[primitive-literal-types][@<href>{https://github.com/Microsoft/TypeScript/pull/9407}]

#@# @suppress JapaneseAmbiguousNounConjunction
=={typeGuards} 型の番人（Type Guards）

#@# @<strong>{導入されるバージョン 1.4.0}

#@# @suppress SuccessiveWord JapaneseAmbiguousNounConjunction
type guardsは、union typesが導入されたことで変数の型が一意ではなくなってしまったため、それを自然に解決するために導入された仕組みです。
type guardsは"変数Aが○○という条件を満たすとき、変数Aの型は××である"というルールを用いて、ガード（番人となる条件式など）の後の文脈で変数の型を××に狭めることができます。

=== 処理フローに基づく型の解析（Control flow based type analysis）

さて、トップバッターがいきなり公式にtype guardsの一員なのか怪しいのですがいってみましょう。
名前が長いですが、要するに普通にコードを書いていった時に、値の型を判別するコードを書くとその分岐にしたがって本当に変数の型が絞り込まれるというものです。

例を見ていきましょう。
TypeScriptを書いていて一番対処に迫られるunion typesのパターンはおそらく@<code>{T | undefined}のような、何か+undefinedの形式でしょう。
if文を用いてundefinedのな値について対処を入れてみます（@<list>{typeGuards/controlFlowBasedBasic}）。

//list[typeGuards/controlFlowBasedBasic][undefinedの可能性を潰す]{
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
引数に@<code>{string}と@<code>{string[]}を取り、これを@<code>{string[]}に統一して利用します（@<list>{typeGuards/controlFlowBasedArray}）。

//list[typeGuards/controlFlowBasedArray][変数の型を統一していく]{
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

変数のプロパティに対してもtype guardsは利用可能です（@<list>{typeGuards/controlFlowBasedProperty}）。
コンパイラの実装を想像すると、なにげに大変そうなことをやっていて思わず感心してしまいます。
#@# プロパティアクセスについてtype guardsを行う（Type guards on property access）

//list[typeGuards/controlFlowBasedProperty][変数のプロパティも絞り込める]{
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

最後に、関数が絡んだ場合の例を見ておきます（@<list>{typeGuards/controlFlowBasedFunction-invalid}）。
関数の内側と外側では、処理フローは別世界です。
言われてみれば当然ですが、関数はいつ実行されるかわからないため、関数の内側で別途絞込みを行う必要があります。

//list[typeGuards/controlFlowBasedFunction-invalid][関数の外側でのフローは内側では関係ない]{
#@mapfile(../code/types-advanced/typeGuards/controlFlowBasedFunction-invalid.ts)
let v: string | number;

v = "string";

let f = () => {
  // これはエラーになる！
  // プログラムの字面的にはstringに確定されていそうだが、関数はいつ実行されるかわからない
  // error TS2339: Property 'toUpperCase' does not exist on type 'string | number'.
  console.log(v.toUpperCase());
};
// ここではvはまだstring
f();

// ここでvがnumberに！
v = 1;
f();
#@end
//}

さて、次以降の項でどういう処理が絞り込みに繋がるのかの例を見ていきます。

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

一番簡単な使い方から見ていきましょう（@<list>{typeGuards/typeofBasic}）。
TypeScriptのtype guardsでは、typeofの結果がstring, boolean, numberの場合、その型に絞り込むことができます。

//list[typeGuards/typeofBasic][実際の型がわからないなら調べるしかないじゃない！]{
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
@<list>{typeGuards/typeof-invalid}では、anyやnumberと指定された変数をtype guardsでstringに絞り込んでいます。
この操作を行うと"ありえない"ことを表すnever型になるため、@<code>{obj.toFixed(0)}というstringには存在しないメソッドの呼び出しはコンパイルエラーとなります。

//list[typeGuards/typeof-invalid][変なコードを書くとコンパイラが教えてくれる]{
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

うーん、便利ですね。
変数に指定した型どおりの値が入ってくるのが健全なので、コンパイル時にミスが発見されるのは嬉しいことです。

=== instanceofによるtype guards

primitive typesだけtype guardsが使えてもあんまり嬉しくないので、instanceofを使ったtype guardsももちろんあります。

JavaScriptにおけるinstanceofは、ある値が指定した関数のインスタンスであるかを調べる演算子です。
プロトタイプチェーンも遡ってみていくので、親子関係にある場合もインスタンスかどうかを調べることができます。

動作例を確認してみましょう（@<list>{typeGuards/instanceof}）。

//list[typeGuards/instanceof][instanceof の挙動]{
#@mapfile(../code/types-advanced/typeGuards/instanceof.ts)
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

export { }
#@end
//}

オブジェクトのprototypeと一致するか順番どおり見ていくだけですね。

instanceofで型を絞り込みます（@<list>{typeGuards/instanceofBasic}）。

//list[typeGuards/instanceofBasic][instanceofの挙動]{
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

昔のTypeScriptと違って、instanceofのelse句でも型の絞込が行われます。
挙動として納得感があり大変よいですね。

=== ユーザ定義のtype guards（User-defined Type Guards）

ユーザが定義した関数によって、ある値がなんの型なのかをTypeScriptコンパイラに教える方法があります（@<list>{typeGuards/userDefined}）。
型判別用の関数を作成し、そこで返り値に@<code>{仮引数名 is 型名}という形式で判別結果を指定します。
この書き方をした場合、返り値はbooleanでなければなりません。

//list[typeGuards/userDefined][ユーザ定義のtype guards]{
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
  // とりあえず、strプロパティがあって値がstringならSample型コンパチということでOK という基準にする
  return typeof s.str === "string";
}

if (isSample(obj)) {
  console.log(obj.str);
}

export { }
#@end
//}

面白い記法として、isの左辺にthisを用いることもできます（@<list>{typeGuards/userDefinedWithThis}）。

//list[typeGuards/userDefinedWithThis][isの左辺にthisを使う]{
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
これも利用する機会は少なさそうですが、ツリー状の構造を作るときなどに活躍しそうです。

=== type guardsと論理演算子

type guardsは@<code>{&&}とか@<code>{||}とか@<code>{?}とか@<code>{!}とかの論理演算子にもちゃんと対応しています（@<list>{typeGuards/operator}）。

//list[typeGuards/operator][ブール代数みたいな演算に対応してる]{
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

export { }
#@end
//}

あんまり使わないかもしれませんが、他の人がこの書き方を使った時に戸惑わぬよう頭の片隅にはとどめておいたほうがよいかもしれません。

=== type guardsの弱点

type guardsは型システム上の仕組みだということを忘れてはいけません。
JavaScriptの実行環境とは全く関係がないのです。

TypeScriptでは構造的部分型の仕組みにより、クラスが要求されている箇所に互換性のある別の値を代入することができます。

その仕組みを使って、@<list>{typeGuards/weakspot}のようなコードが書けてしまいます。

//list[typeGuards/weakspot][構造的部分型とtype guards]{
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
コンパイル後のJavaScriptコード（@<list>{typeGuards/weakspot.js}）を見ると、objの値がSampleクラスのインスタンスではないことが一目瞭然ですが、TypeScript上で見ると型を元に判別されていると勘違いしやすい、ということを頭の片隅においておきましょう。

//list[typeGuards/weakspot.js][コンパイル後のJS]{
#@mapfile(../code/types-advanced/typeGuards/weakspot.js)
"use strict";
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

ひとつ名は、ユーザ定義のtype guardsを使う方法。
ふたつ目はprivateな要素をクラスに突っ込んでしまうことです（@<list>{typeGuards/vsWeakspot2-invalid}）。

//list[typeGuards/vsWeakspot2-invalid][privateな要素があれば構造的部分型で値を偽造できない]{
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
色々書きましたが、一番の解決策はunion typesやanyを多用せず、真っ当なコードを書けるよう設計することです。

#@# @suppress ParagraphNumber
== 型の別名（Type Alias）

#@# @<strong>{導入されるバージョン 1.4.0}

最初に書いておきます。
@<strong>{可能な限りtype aliasを使うな！interface使え！}
筆者はtype aliasの乱用を恐れています！

type aliasもunion typesの扱いを便利にするために導入された機能です。
機能としてはただ単に、型をひとまとまりにして、それに名前が付けられるだけです。
それだけです。

type aliasは仕様上、interfaceと同じように利用できる場面もあります。
ですが、基本的にtype aliasはinterfaceより機能が貧弱であるため、なるべく避けるほうがよいでしょう。

代表例を見てみましょう（@<list>{typeAlias/basic}）。

//list[typeAlias/basic][頻出するunion typesに名前をつける]{
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

tuple typesに名前をつけることもできます（@<list>{typeAlias/tuple}）。

//list[typeAlias/tuple][tuple typesに名前をつける]{
#@mapfile(../code/types-advanced/typeAlias/tuple.ts)
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
  let c2: Circle = new Circle(new Point(1, 2), 3);
  console.log(c2.p, c2.r);
}

export { Point, Circle, c, alternative }
#@end
//}

素直にクラスでやればいいのに、という感じです。

type aliasは型に別名をつけるだけで、コンパイルされると消えてしまう存在です。
そのため、@<list>{typeAlias/doNotHaveInstance-invalid}のようなコードは書くことができません。

//list[typeAlias/doNotHaveInstance-invalid][type aliasは値を作らない]{
#@mapfile(../code/types-advanced/typeAlias/doNotHaveInstance-invalid.ts)
// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = string[];

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
let strArray = new StringArray();
#@end
//}

#@# @suppress CommaNumber
TypeScriptの仕様書にのっているtype aliasの利用例について、interfaceでの書き換えができるものを示します（@<list>{typeAlias/specExample}）。
union typesが絡むもの、tuple typesが絡むもの、型クエリが絡むものだけが、interfaceで置き換えることができません。

//list[typeAlias/specExample][interfaceを使うんだ！]{
#@mapfile(../code/types-advanced/typeAlias/specExample.ts)
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

export {
  StringOrNumber, TextObject, Coord, ObjectStatics, Pair,
  Coordinates, HolidayLookup, AltHolidayLookup, Callback, AltCallback,
}
#@end
//}

最後に、type aliasではなくインタフェースを使ったほうがいい理由を掲げておきます。

 * interfaceが絡んだときのコンパイルエラーにはinterface名が表示されてわかりやすい
 ** type aliasは展開されて表示されちゃうので無理
 * interfaceは定義の統合ができるので後から自由に拡張できる
 ** type aliasは無理

@<strong>{interfaceでできることをtype aliasでやるな！}

== 多態性のあるthis型（Polymorphic 'this' Type）

@<code>{this}を型として用いることができます。
たとえば@<list>{polymorphicThisType/basic}のようなコードです。

//list[polymorphicThisType/basic][thisを型として用いる]{
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
引数や返り値の型として、普通にthisを利用しています。
fluentな、メソッドチェーンで使うAPIを組み立てる場合に役立ちそうです。

この書き方がないと、ジェネリクスなどを使ってごまかさなければならないところでしょう。
とはいえ、便利になる代わりに仮引数に対して使ったりすると無駄に制約がきつくなったりする場合があるため、乱用は控えましょう。
@<code>{return this;}を使った時に、メソッドの返り値が暗黙的に@<code>{this}になるのを利用する、くらいがよい塩梅かもしれません。

#@# @suppress JapaneseAmbiguousNounConjunction
== 関数のthisの型の指定（Specifying this types for functions）

JavaScriptでは@<code>{Function.prototype.bind}や@<code>{Function.prototype.call}、@<code>{Function.prototype.apply}などの関数により、関数呼び出し時のthisの型を変更することができます。
この仕様は悪しき仕様だと筆者は思いますが、jQueryやDOMなど、古めのAPIではこの仕様をAPIとして組み込んだものが存在しています。
TypeScriptではこの変更も頑張ってサポートしようとしています。

まずは簡単な例を見てみます（@<list>{thisTypeForFunctions/basic}）。
関数の1つ目の仮引数の名前を@<code>{this}にするだけです。

//list[thisTypeForFunctions/basic][thisの型を指定する]{
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
@<code>{--noImplicitThis}オプションを利用すると、thisの型指定がない関数内でthisへアクセスするとエラーになります。
thisを使わない限りはエラーにならないため、常用してしまってよいでしょう。

この仕様が現実世界でどう役に立つかを紹介します（@<list>{thisTypeForFunctions/eventListener}）。

//list[thisTypeForFunctions/eventListener][thisの値が差し替えられるAPIに対応]{
#@mapfile(../code/types-advanced/thisTypeForFunctions/eventListener.ts)
// lib.dom.d.ts から抜粋
// listenerの仮引数の先頭が偽の仮引数で、thisの型の指定が行われている
interface HTMLBodyElement extends HTMLElement {
  addEventListener(type: "click", listener: (this: this, ev: MouseEvent) => any, useCapture?: boolean): void;
  addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
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
自分でTypeScriptコードを書く時に必要になる場合は少なくありたいものです。
しかし、型定義ファイルを作成する時にはお世話にならざるをえないときがあるでしょう。

== ローカル型（Local Types）

ローカル型は通常より小さい範囲で、クラスやインタフェースやenumやtype aliasを定義することができます（@<list>{localType/basic}）。

//list[localType/basic][ローカル型を試す]{
#@mapfile(../code/types-advanced/localType/basic.ts)
{
  type Data = string | boolean;
  let obj: Data = true;

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

使う機会は少ないかもしれませんが、@<list>{localType/inMethod}のようにメソッドの中で簡易に別名を用意したい場合などに利用できるでしょう。

//list[localType/inMethod][メソッド内でだけ通用する別名]{
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
