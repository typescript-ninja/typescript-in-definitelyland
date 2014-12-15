= JS資産と型定義ファイル

== JavaScriptの資産が使いたい！

TypeScriptはJavaScriptの上位互換であり、JavaScriptを置き換えるものです。
とはいえ、現時点のWebアプリの世界はJavaScriptで成り立っていますし、既に莫大な資産があります。
それらを放り出してしまうのはあまりに勿体無いので、TypeScriptでも活用していきたいところです。
そのためにTypeScriptは既存のJavaScript用資産を活用するための仕組みを持っています。
それが、型定義ファイルです。

通常のTypeScriptのコードの拡張子が .ts なのに対して、型定義ファイルは拡張子を .d.ts とします。
拡張子を .d.ts としたファイルに実装を含むようなコードを書くとtscがエラーにしてくれますので、ケアレスミス予防のためにも型定義ファイルの拡張子は必ず .d.ts にします。
#@# OK REVIEW muo: 少々言葉不足でつながり悪く感じた。「拡張子を.d.tsとしたファイルに実装を含むようなコードを書くと」かなぁ

TypeScriptではJavaScriptの自由奔放（かつ、危険がてんこ盛り）の世界に後付で型を与えます。
元からTypeScriptで書かれている場合、実装と型定義を同時に書いているようなものなので、実装と型がズレて（つまりバグって）しまうことはありません。
一方、型定義ファイルは既に実装があるJavaScriptに後付かつ手書きで型をつけていくため、ズレる（バグる）可能性が大いに有ります。
#@# OK REVIEW muo: 後付で手書きで とつながるのが嫌な感じ。「JavaScriptに後付の型定義を手書きしていくため」かな。気に食わなかったらスルーでok
そこのところを十分に気をつけないといけません。

== ツールを使って検索だ！

さて、まずは自分で型定義ファイルを作るよりも、既存のものを使ってみましょう。
有名どころはひと通り揃っています。

そのために、まずはtsdまたはdtsmというツールを導入しましょう@<fn>{tsd}@<fn>{dtsm}@<fn>{NuGet}。

//cmd{
# どちらか！
$ npm install -g tsd
$ npm install -g dtsm
//}

まずは、プロジェクトで使う型定義ファイルを記録するために依存関係保存用のファイルを生成します。

//cmd{
# どちらか！
$ tsd init
$ dtsm init
//}

ここではテストを書く時に使う便利ライブラリ、power-assertを題材にして型定義ファイルをダウンロードし、保存してみます。
#@# OK REVIEW muo: power-assetどっから出てきたの!? となるので、「ここでは〜〜なpower-assertを題材として型定義ファイルの〜〜をやってみましょう」的な前説を付近に追加すべき。

//cmd{
# どちらか！
$ tsd query power-assert --action install --save --resolve

>> tsd 0.5.7

 - power-assert/power-assert.d.ts : <head> : 2014-11-14 02:03

   >> empower/empower.d.ts : <head> : 2014-11-14 02:03
   >> power-assert-formatter/power-assert-formatter.d.ts : <head> : (略)

>> running install..

>> written 3 files:

    - empower/empower.d.ts
    - power-assert-formatter/power-assert-formatter.d.ts
    - power-assert/power-assert.d.ts

$ dtsm install power-assert --save
power-assert/power-assert.d.ts
empower/empower.d.ts
power-assert-formatter/power-assert-formatter.d.ts
//}

デフォルトでは、typings/ フォルダに型定義ファイルが保存されます。
tsdとdtsmでは微妙に生成されるファイルが異なりますが、概ね以下のような構成になっているはずです。

//cmd{
$ tree typings
typings
├── empower
│   └── empower.d.ts
├── power-assert
│   └── power-assert.d.ts
└── power-assert-formatter
    └── power-assert-formatter.d.ts
//}

あとは、これら型定義ファイルを自分の使うコードから参照するだけでコンパイルが可能になります。
残念ながらライブラリの実体は含まれていないため、npmやbowerなどで別途取得する必要があるでしょう。

//footnote[tsd][tsdはbartvdsがメインに開発している型定義ファイル管理ツールで、広く使われています。]
//footnote[dtsm][dtsmは筆者（vvakame）が作っているツールで、まだあまり宣伝していないためユーザは少ないです。みんな使ってね！]
#@# OK REVIEW muo: こう書くならtsdについても一言紹介しておいたほうが良いように思った
//footnote[NuGet][WindowsユーザにはNuGetというツールもあるのですが、全然知らないため割愛します。]

== 型定義ファイルを参照してみよう！

型定義ファイルを参照するには、tscコマンドでソースコードをコンパイルする時に一緒に指定するか、ソースコード中からリファレンスコメントで参照する必要があります。
#@# OK REVIEW muo: コマンドでソースをコンパイル あたりが良さそう(言葉足りてない)
リファレンスコメントとして参照するほうがコンパイル手順を簡素に保てるため、プロジェクトの構成をシンプルに保つことができます。
#@# OK REVIEW muo: 開発に適す?運用に適す?どういうことか補記あったほうが嬉しい

リファレンスコメントはソースコードの先頭に@<code>{/// <reference path="相対パスor絶対パス" />}の形式で指定します。

mocha+power-assertでテストを書く場合を例に、使い方を解説していきます。

テスト対象のコードは@<code>{usage/lib/index}です。

//list[usage/lib/index][至って普通の外部モジュール]{
#@mapfile(../code/definition-file/usage/lib/index.ts)
"use strict";

export function hello(word = "TypeScript") {
  return "Hello, " + word;
}
#@end
//}

これに対して、テストコードを書いてみましょう（@<list>{usage/tests/indexSpec}）。
普通ですね。
特定のinputを与えるとoutputが得られる。
そのことを検証するコードです。

//list[usage/tests/indexSpec][mocha+power-assertでテストを書く]{
#@mapfile(../code/definition-file/usage/tests/indexSpec.ts)
/// <reference path="../typings/mocha/mocha.d.ts" />
/// <reference path="../typings/power-assert/power-assert.d.ts" />

import assert = require("power-assert");

import lib = require("../lib/index");

describe("lib", () => {
  describe("hello function", () => {
    it("generate string with default value", () => {
      var str = lib.hello();
      assert(str === "Hello, TypeScript");
    });
    it("generate string with parameter", () => {
      var str = lib.hello("JavaScript");
      assert(str === "Hello, JavaScript");
    });
  });
});
#@end
//}

mochaでは、describeで何に対してのテストかを宣誓し、itでどういう性質を持つべきかを宣誓します。
power-assertは、適当にassertに真になってほしい式を突っ込んでおけばそれが本当に真になっているかを検証して報告してくれます。

ここで問題なのは、mochaとpower-assertについての情報がソースコード上に存在していないことです。
例えば、assert関数はpower-assertが提供するものですし、describeとitはmochaが提供しています。
JavaScriptの世界では静的な型検査などありませんので問題ありませんが、TypeScriptではそうはいかないため型情報をぶっこんでやる必要があります。
そこで使われるのが型定義ファイルです。

型定義ファイルの抜粋を示します。
mocha（@<list>{usage/abstract/mocha}）とpower-assert（@<list>{usage/abstract/power-assert}）の型定義ファイル（抜粋）を見てみましょう。

//list[usage/abstract/mocha][mocha.d.ts抜粋]{
#@mapfile(../code/definition-file/usage/abstract/mocha.d.ts)
interface MochaDone {
  (error?: Error): void;
}

declare var describe: {
  (description: string, spec: () => void): void;
  only(description: string, spec: () => void): void;
  skip(description: string, spec: () => void): void;
};

declare var it: {
  (expectation: string, assertion?: (done: MochaDone) => void): void;
  only(expectation: string, assertion?: (done: MochaDone) => void): void;
  skip(expectation: string, assertion?: (done: MochaDone) => void): void;
};
#@end
//}

//list[usage/abstract/power-assert][power-assert.d.ts抜粋]{
#@mapfile(../code/definition-file/usage/abstract/power-assert.d.ts)
declare function assert(value: any, message?: string): void;

declare module "power-assert" {
  export = assert;
}
#@end
//}

型定義ファイルを見るとmocha, power-assertそれぞれのAPIが表現されています。
@<list>{usage/tests/indexSpec}から「これらのAPIを使います！」と宣言すれば、矛盾なくコンパイルを通すことができそうです。
その意思を表すためにリファレンスコメントを使います。

== 型定義ファイルを書こう

さて、型定義ファイルの取得の仕方・使い方はわかりました。
しかし、世の中にあるJavaScriptライブラリのうち、まだまだ型定義ファイルが書かれていないものは数多くあります。
特に、門外不出の社内ライブラリなどは誰も手をつけていない前人未到の地に違いありません。
#@# OK REVIEW muo: 社内製 内製 社内 は良いけど社内内製はいけない

しからば！自分で書くしかあるまいよ！
ぶっちゃけた話めんどくさいのですが、後々の安心・安全のための投資として割りきりましょう。

なお、自分で型定義ファイルを書く覚悟無しにTypeScriptをやるのは茨の道だと思いますので頑張ってください。
誰かがやってくれないと自分ではできません、なんて甘えた根性では型サバンナでは到底生きていけないのです（DefinitelyTypedメンテナの意見です）。

=== 型、実体、そして42。

TypeScriptはJavaScriptに対して後付で型による制約を付け足した言語です。
そのため、JavaやC#のような最初から型ありきの言語より少し考え方が複雑です。
具体的にいえば、型と実体（値）というものが分かれています。

全てがTypeScriptで書かれたプログラムであれば、型と実体は基本的には一致しています。
#@# OK REVIEW muo: 書かれている→書かれた かな
クラスの定義を書いた時、JavaScriptプログラムとしてのクラス（OOPするためのコンストラクタ関数）と、TypeScriptで使う型としてのクラスが一度に誕生します。
これは非常に素直かつ簡単で、型と実体を1つの記述から作成しているので、この2つが乖離してしまうことはありません。

一方、JavaScriptでコードを書いて、TypeScriptで型定義ファイルを作成して使う場合は実装と型が個別に定義されることになります。
#@# OK REVIEW muo: JavaScriptで書いて→JavaScriptでコードを書いて
#@# OK REVIEW muo: 「当てて使う」の表現は少々感覚寄りすぎかなーと
そのため、型と実体の2つが分離してしまい、この2つの間に乖離が生じると（つまりバグると）コンパイルが通るのに実行時エラーが多発する、という有り様に成るわけです。
型定義ファイルを書いて"この変数は、あります！"と宣言したけれど、実際には存在せず実行時エラーになるというのは広く使われている型定義ファイルですらままある話です。

=== 良い型定義ファイル、悪い型定義ファイル

型定義ファイルにも、良し悪しがあります。
その基準は至って簡単です。

 1. 正しいライブラリの使い方を導くこと
 2. 他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと
 3. IDE上で使いやすいこと

こんな感じです。

正しいライブラリの使い方を導く、というのは、裏を返せば間違った使い方ができないようにする、ということです。
これには、型と実体の定義に乖離が存在せず、コンパイルが通ったら実行時エラーが簡単には起こらないことも含まれます。

他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと、というのは、意図せぬインタフェースの統合などが起こったりしないことです。
このためには、あまりにも汎用的な名前を避けたり、可読性が高く理解しやすい型定義を作り干渉するかしないかを容易に判断できるようにすることも含まれます。

IDE上で使いやすいこと、というのは、Visual StudioやWebStorm上で入力補完の候補が不用意にたくさん出過ぎないようにして見通しのよい開発を助けることなどが含まれます。

これら3つを守ることが"良い品質であること"に繋がるというのは、TypeScript自体が型指定を行うことで間違ったコードを書きにくいようにするツールであると考えると納得がいくでしょう。
#@# OK REVIEW muo: 「型を使って間違ったコードを書きにくいように」の部分が構文解析しづらいので、ちょい冗長になるけど「型指定をおこなうことで間違った」のほうが良さそう
#@# OK REVIEW muo: 納得できると思います→納得できるでしょう(または、納得がいくでしょう)

慣れないうちはどうしても"上手く書けないので仕方なく"悪い型定義を書いてしまうことがあります。
#@# OK REVIEW muo: 人は誰しも→人間、どうしても あたり
DefinitelyTypedにpull requestを送ってくれる人にもそういう人は多くいます。

これから説明するベストプラクティスを踏まえて、より良い型定義ファイルを作成できるように鍛錬していきましょう。

== 型定義ファイルのベストプラクティス

型定義ファイルを書く上でのベストプラクティスを解説していきます。
基本的には@<href>{http://definitelytyped.org/guides/best-practices.html,DefinitelyTypedのbest practices}@<fn>{dt-best-practice}と@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,公式のHandbookのWriting .d.ts files}@<fn>{official-handbook}に従っておけばよいです。
本書では、そこに書かれていることや筆者の経験則などを説明していきます。

//footnote[dt-best-practice][http://definitelytyped.org/guides/best-practices.html]
//footnote[official-handbook][http://www.typescriptlang.org/Handbook#writing-dts-files]

=== テキトーに、やろー！

一番最初にコレを書くのもどうかと思うのですが、まずは"使える"ようにすることが一番大切です。

型定義ファイルの品質の良さにこだわるあまり、完成しない・使いたいライブラリが使えない、というのが一番よくない状態です。
型定義ファイルの良し悪しを判断する力はTypeScript自体の理解度に大きく依存します。
#@# OK REVIEW muo: 「最終的に、良し悪しが分かるようになるには」→「型定義ファイルの良し悪しを判断する力は」
TypeScriptを書き始めの頃は、品質を気にした所で後々粗が見えてくるのは避けられないのでまずは"使える"状態にすることを目指しましょう。
#@# OK REVIEW muo: 品質は→品質を

品質や"ライブラリ全体をカバーしている"かは気になるところではあります。
しかし、まずは使いたいところが使えればいいのです。
スゴいものになると、1万行を超える型定義ファイルがあったり、3000行程度のものはわりとごろごろしていたり…。そんなのを頑張って書いてると、余裕で日が暮れてしまいます@<fn>{atom-dts}。

まずは、使いたいところが、使える！
それでよいのです。
ドラゴン・ゲンドーソー＝先生のインストラクション・ワンを思い出しましょう。
「百発のスリケンで倒せぬ相手だからといって、一発の力に頼ってはならぬ。一千発のスリケンを投げるのだ！」
最初は上手くできなくても、数をこなし学習を重ねれば、そのうち立派な型定義ファイルを書けるようになるでしょう。

//footnote[atom-dts][なお、筆者はGitHubの作っているエディタ、Atomの型定義ファイルを3日かけて書いたことがあります。アレがジゴクだ。]

=== インタフェースを活用する

インタフェースは大変使いやすいパーツです。
というのも、インタフェースには@<strong>{後から定義を拡張できる}という特性があるからです（@<list>{declaration-merging}、@<list>{declaration-merging-usage}）。

//list[declaration-merging][定義を分割して書く]{
#@mapfile(../code/definition-file/declaration-merging.d.ts)
interface Foo {
  hello(): string;
}

// 同名のインタフェースを定義すると、合体される！
interface Foo {
  bye(): string;
}
#@end
//}

//list[declaration-merging-usage][定義が統合される！]{
#@mapfile(../code/definition-file/declaration-merging-usage.ts)
/// <reference path="./declaration-merging.d.ts" />

var foo: Foo;

foo.hello();
foo.bye();
#@end
//}

この通り、別々に定義したインタフェースが1つに統合されています。
これを利用することで、既存の型であろうとも拡張が可能な場合があります。

例を1つ見てみましょう。
Array#findは、指定した方法に基づき要素を1つ探す関数です。
TypeScript 1.3.0ではデフォルトの型定義ファイル（lib.d.ts）にはまだこのメソッドが定義されていません。
そのため、Arrayインタフェースを拡張する形でコンパイルを通せるようにしてみましょう（@<list>{array-find}）。

//list[array-find][Array#find を生やす]{
#@mapfile(../code/definition-file/array-find.ts)
// TypeScript 1.3.0 では Array.prototype.find の定義はまだ存在していない
interface Array<T> {
  find(callback: (e: T, idx: number, array: T[]) => boolean, thisArg?: any): T;
}

var array = [1, 2, 3];

// 最初に見つかる奇数を得る
array.find(v => v % 2 === 1);
#@end
//}

あとは、実装側を@<href>{https://github.com/paulmillr/es6-shim/,es6-shim}@<fn>{es6-shim}などで埋めてやれば古いブラウザでも利用可能になるでしょう。

この手法は、他人が作った型定義ファイルを拡張する場合にも活用できます。
相乗りできるのであれば遠慮なく乗っかっていってしまいましょう。

//footnote[es6-shim][https://github.com/paulmillr/es6-shim/]

=== 幽霊モジュール

幽霊モジュール@<fn>{ghost-module}という考え方があります。

内部モジュールを作ったとしても、即座に実体が生成されるとは限りません。
内部モジュールが抱えるのがインタフェースのみである場合、実体がある扱いにはならないのです（@<list>{ghost-module-invalid}）。

//list[ghost-module-invalid][幽霊モジュール]{
#@mapfile(../code/definition-file/ghost-module-invalid.ts)
declare module ghost {
  interface Test {
    str: string;
  }
}

// 型としては普通にアクセスできる
var test: ghost.Test;
test.str;

// 実体としては存在していない！
// ghost-module-invalid.ts(13,17): error TS2304: Cannot find name 'ghost'.
var notExists = ghost;
#@end
//}

これを活用して、大量のインタフェースを持つようなライブラリの定義をひとまとまりにできます。

実際の例を見てみましょう。
@<list>{jquery-without-ghost-module}はjQueryの型定義ファイルの抜粋（＆一部改変）です。

//list[jquery-without-ghost-module][実際のjQueryの型定義の例]{
#@mapfile(../code/definition-file/jquery-without-ghost-module.d.ts)
interface JQuery {
  addClass(className: string): JQuery;
  html(htmlString: string): JQuery;
  val(): any;
  empty(): JQuery;
  append(content1: JQuery, ...content2: any[]): JQuery;
  appendTo(target: JQuery): JQuery;
}

interface JQueryStatic {
  ajax(settings: JQueryAjaxSettings): any;
  (selector: string, context?: Element): JQuery;
  (element: Element): JQuery;
}

interface JQueryAjaxSettings {
  data?: any;
  type?: string;
  url?: string;
}

interface JQueryPromise<T> {
  state(): string;
  then<U>(
    fullfill: (value: T) => U,
    reject?: (...reasons: any[]) => U
  ): JQueryPromise<U>;
}

interface JQueryDeferred<T> extends JQueryPromise<T> {
  reject(...args: any[]): JQueryDeferred<T>;
  resolve(value?: T, ...args: any[]): JQueryDeferred<T>;
}

declare var $: JQueryStatic;
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

トップレベルに複数の型がいくつも散乱してしまうのがよくありません。
それに、@<code>{JQuery}というprefixが乱舞していて目を惑わせます。
ライブラリ内部のAPI同士の参照でも引数や返り値にプリフィクスが必要なのはめんどうくさいです。
IDE上で型注釈を手書きする時も候補がたくさんサジェストされてしまうことでしょう。

これを幽霊モジュールを使って書きなおしてみます。

//list[jquery-with-ghost-module][幽霊モジュールを使ってみた]{
#@mapfile(../code/definition-file/jquery-with-ghost-module.d.ts)
declare module jquery {
  interface Element {
    addClass(className: string): Element;
    html(htmlString: string): Element;
    val(): any;
    empty(): Element;
    append(content1: Element, ...content2: any[]): Element;
    appendTo(target: Element): Element;
  }

  interface Static {
    ajax(settings: AjaxSettings): any;
    (selector: string, context?: Element): Element;
    (element: Element): Element;
  }

  interface AjaxSettings {
    data?: any;
    type?: string;
    url?: string;
  }

  interface Promise<T> {
    state(): string;
    then<U>(
      fullfill: (value: T) => U,
      reject?: (...reasons: any[]) => U
    ): Promise<U>;
  }

  interface Deferred<T> extends Promise<T> {
    reject(...args: any[]): Deferred<T>;
    resolve(value?: T, ...args: any[]): Deferred<T>;
  }
}

declare var $: jquery.Static;
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

インタフェース名が短く、かつわかりやすくなりました。
そうそう、こういうのでいいんだよこういうので！

もちろん、無理に幽霊モジュールを使う必要はありません。
クラスや変数、関数などを持ち、通常の実体を持つモジュールが存在している場合は、そのモジュールに相乗りしてしまったほうが楽でしょう。
#@# OK REVIEW muo: 実体のある→実体を持つ
#@# OK REVIEW muo: 相乗りしていってしまったほうが→相乗りしてしまったほうが

…なんでDefinitelyTyped上にある型定義ファイルでそうなってないものが多いかって？
良い質問です。
一つ目は幽霊モジュールの認知度が低いこと、もう一つは型定義ファイルの大幅な書き換えは互換性の破壊を生み出すからです。
先で説明しましたが、インタフェースは定義の統合ができます。
この性質を利用して定義の拡張を行っているので、うかつにJQueryStaticからjquery.Staticに型名を変更するとjQueryの型定義に依存しているさまざまなライブラリの色々なところが壊れてしまうのです。
#@# OK REVIEW muo: うかつにJQueryStaticからjquery.Staticに変更すると(なにを?)
特にjQueryプラグインとかはインタフェースを拡張する形で型定義するのでその量たるや…。

ともあれ、過去の定義との互換性を壊す事に繋がるため、途中から幽霊モジュールに切り替えるのは難しい場合があります。
可能であれば最初から幽霊モジュールを使うようにしましょう。
将来的には、このパターンの検出はtslintなどで機械的に行えるようにしたいところですね。

//footnote[ghost-module][TypeScriptリファレンスでは非インスタンス化モジュールという名前で紹介しました。その後、DefinitelyTypedのbest practicesでghost moduleと表記されたのでそちらに統一]

=== なんでもかんでもインタフェースにしてはならない

おうお前少し前の文章であんだけインタフェースを持ち上げといてこれかぁ！？
と、思われたかもしれませんが、なんでもかんでも乱用すればいいってものではありません。

具体的に、モジュール様の構造をインタフェースを使って作ってはいけません（@<list>{module-by-interface-bad}）。

//list[module-by-interface-bad][インタフェースでモジュールを表現してしまう。何故なのか…]{
#@mapfile(../code/definition-file/module-by-interface-bad.d.ts)
interface Foo {
  bar: FooBar;
}

interface FooBar {
  buzz: FooBarBuzz;
}

interface FooBarBuzz {
  str: string;
}

declare var foo: Foo;

// foo.bar.buzz.str という使い方ができる。わかりにくくてユーザは死ぬ。
#@end
//}

この型定義ファイルを読み解いて一瞬で使えるのは、元のJavaScriptコードを熟知している人だけでしょう。
#@# OK REVIEW muo: 使える人は→使えるのは
少なくとも、この型定義ファイルをヒントに実際のコードを書くことには大いなる苦痛を伴います。
#@# OK REVIEW muo: ×実際のコードを書くことは大いなる苦痛が ○実際のコードを書くことには大いなる苦痛が ○実際のコードを書くには大いなる苦痛が ○実際のコードを書くことは大いなる苦痛を
俺は絶対使わんぞ！
普通に、@<list>{module-by-interface-good}のように書くのだ！

//list[module-by-interface-good][素直にこうしよう]{
#@mapfile(../code/definition-file/module-by-interface-good.d.ts)
// 普通にコレでいいだろ！！
declare module foo.bar.buzz {
  var str: string;
}
#@end
//}

普通、こんな型定義ファイルを書こうとは思わないと思いますが、こういうコードを書きたくなってしまう時が稀にあります。
具体的には、@<list>{callable-module-usage}のように関数としても呼べるし、内部モジュールのようにも振る舞うオブジェクトの型定義を作成したい時です。

//list[callable-module-usage][関数・内部モジュール どっちなの？]{
#@mapfile(../code/definition-file/callable-module-usage-invalid.ts)
// assertは関数としても呼べるしモジュールのようにも見える
assert(foo === "foo");
assert.ok(value);
#@end
//}

呼び出し可能で、プロパティを持つ。ふむ、じゃあ@<list>{callable-module-bad1}か、@<list>{callable-module-bad2}だ！

//list[callable-module-bad1][こうしてしまいたい、気持ち]{
#@mapfile(../code/definition-file/callable-module-bad1.d.ts)
declare var assert: {
  (value: any): void;
  ok(value: any): void;
};
#@end
//}

//list[callable-module-bad2][匿名型注釈よりはマシ]{
#@mapfile(../code/definition-file/callable-module-bad2.d.ts)
declare var assert: Assert;

interface Assert {
  (value: any): void;
  ok(value: any): void;
}
#@end
//}

たしかに、この定義でも動きます（正直、assert関数だけの定義だとこのままでもいい気がしますが…）。

しかし、これには別の良いやり方があるのです（@<list>{callable-module-good}）。

//list[callable-module-good][関数と内部モジュール 両方やらなきゃいけないのが(ry]{
#@mapfile(../code/definition-file/callable-module-good.d.ts)
declare function assert(value: any): void;
declare module assert {
  function ok(value: any): void;
}
#@end
//}

関数と内部モジュールを同名で宣言できるのです。
これの効能は、階層構造を素直に表現できることと、前項で説明した幽霊モジュールの書き方を併用できるところです。
#@# OK REVIEW muo: 層構造→階層構造 かな、ここの流れだと。

この手法は、実際に@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/master/power-assert/power-assert.d.ts,power-assertの型定義ファイル}@<fn>{power-assert-dts}でも利用されています。
@<list>{power-assert-abst}に抜粋&改変したものを示します。

//list[power-assert-abst][関数+内部モジュールの実例]{
#@mapfile(../code/definition-file/power-assert-abst.d.ts)
declare function assert(value: any, message?: string): void;
declare module assert {

  export function deepEqual(actual: any, expected: any): void;
  export function notDeepEqual(acutal: any, expected: any): void;

  export interface Options {
    assertion?: any;
    output?: any;
  }

  export function customize(options: Options): typeof assert;
}
#@end
//}
#@# OKREVIEW muo: 前のとキャプション同じだけど意図してる? 話的にはつながってるので良いかもしらんけど念のため

外部に公開されている関数は@<code>{assert}のみで、そこに追加でプロパティが生えている形式です。
実体のある要素（関数）があるため、幽霊モジュールにはなりませんが、Optionsインタフェースが上手く取り込まれています。
余計な名前を階層の浅いところにバラ撒かず、厳密さも損なっていません。
この書き方は、案外よく登場するパターンなので覚えておくとよいでしょう。

実は、このやり方は型定義ファイルだけではなく、通常のTypeScriptコードでも使えます（@<list>{callable-module-ts}）。

//list[callable-module-ts][関数が先、内部モジュールは後！絶対！]{
#@mapfile(../code/definition-file/callable-module.ts)
function test() {
  return "test!";
}
module test {
  export function func() {
    return "function!";
  }
}
#@end
//}

コンパイル結果の@<list>{callable-module-js}を見ると、なぜ関数が先、内部モジュールが後、という決まりになっているかがわかりますね。

//list[callable-module-js][コンパイル結果を見れば、理由がわかる。これ、正しいJSだ！]{
#@mapfile(../code/definition-file/callable-module.js)
function test() {
    return "test!";
}
var test;
(function (test) {
    function func() {
        return "function!";
    }
    test.func = func;
})(test || (test = {}));
#@end
//}

//footnote[power-assert-dts][https://github.com/borisyankov/DefinitelyTyped/blob/master/power-assert/power-assert.d.ts]

=== クラスを定義するには？

普通に定義すればええやろ！！と思うかもしれませんが、現在のTypeScriptはなかなか難しい問題を抱えています。
先に、どういう選択肢が存在するかを見てみましょう（@<list>{declare-class}）。

//list[declare-class][素直にクラス定義 vs インタフェース+変数]{
#@mapfile(../code/definition-file/declare-class.d.ts)
// A. 普通にクラスを定義する
declare class TestA {
}

// B. クラスの分解定義 変数 + インタフェース2つ
declare var TestB: TestBStatic;
interface TestBStatic {
  new (): TestB;
}
interface TestB {
}
#@end
//}

こんな感じです。
ぱっと見、普通にクラス定義をするほうが素直ですね。

しかし、それぞれの手法にメリット・デメリットがあるのです。
そのライブラリのユースケースにとって、どちらが適切か見極めねばなりません。

==== 素直にクラスのメリット・デメリット

 * ライブラリ利用時に普通に継承できる
 * 定義の拡張ができない
 ** 別ライブラリが（プラグインなどで）拡張する設計のライブラリには向かない
 * 別途インタフェースの実装を型定義に盛り込む時めんどくさい（定義の二重記述が必要）

@<list>{declare-vanilla-class-invalid}みたいな感じです。
#@# OK REVIEW muo: 感じ。→感じです。

//list[declare-vanilla-class-invalid][クラスで定義]{
#@mapfile(../code/definition-file/declare-vanilla-class-invalid.ts)
declare class BaseA {
  str: string;
}
// ○利用時に普通に継承できる！
class Inherit extends BaseA {
  number: number;
}
// × クラスはopen endedじゃないので定義の拡張ができない…
// error TS2300: Duplicate identifier 'BaseA'.
declare class BaseA {
  num: number;
}

interface FooListener {
  on(eventName: string, callback: (data: any) => void): void;
}
declare class FooListenerImpl implements FooListener {
  // インタフェースの要素全部書かないとコンパイル通らない…
  on(eventName: string, callback: (data: any) => void): void;
}
#@end
//}

==== インタフェース+変数に分解

 * ライブラリ利用時に継承できない
 ** newするだけの利用法なら特に不便ではない
 * インタフェース定義の統合が使えるので別ライブラリの拡張にも対応できる！
 * インタフェースを実装するのが（継承するだけなので）めっちゃ簡単

@<list>{declare-decompose-class-invalid}みたいな感じです。
#@# OK REVIEW muo: 感じ。→感じです。

//list[declare-decompose-class-invalid][インタフェース+変数で定義]{
#@mapfile(../code/definition-file/declare-decompose-class-invalid.ts)
var BaseA: BaseAStatic;
interface BaseAStatic {
  new (): BaseA;
}
interface BaseA {
  str: string;
}
// × 利用時に継承できない…(しょせんインタフェース
// error TS2311: A class may only extend another class.
class Inherit extends BaseA {
  number: number;
}
// ○ インタフェースは定義の拡張ができる！！
interface BaseA {
  num: number;
}

interface FooListener {
  on(eventName: string, callback: (data: any) => void): void;
}
var FooListenerImpl: FooListenerImplStatic;
interface FooListenerImplStatic {
  new (): FooListenerImpl;
}
interface FooListenerImpl extends FooListener {
  // FooListenerの要素何も転記しなくて済む！
}
#@end
//}

==== まとめ

クラスの型定義がopen endedになって定義を拡張可能にして、@<href>{https://github.com/Microsoft/TypeScript/issues/371,インタフェースのオプショナルな実装}@<fn>{optional-interface}サポートしてくれたらクラス定義で全部賄える！

今のところはどうしようもないので、用途に応じて適切なほうを選びましょう。

//footnote[optional-interface][https://github.com/Microsoft/TypeScript/issues/371]

=== オーバーロードを上手く使おう！

正しいライブラリの使い方を導くこと。
を心に秘めて、@<list>{use-overload}を見てください。
どれが一番わかりやすいですか？

//list[use-overload][普通に使えます]{
#@mapfile(../code/definition-file/use-overload.ts)
// 同じ実装に対して、どの型定義が一番便利かな？

// getの時 setの時 仕様が違うことがよくわかる
declare function valueA(value: any): void;
declare function valueA(): any;

// setの時も値が取れるっぽい？
declare function valueB(value?: any): any;

// 詳細が不明だ…！
declare var valueC: Function;
#@end
//}

答え：一番最初のやつ。

JavaScriptのライブラリは1つの関数にさまざまな使い方をさせようとする場合が多くあります。
つまり、1つの関数が複数の顔を持つということです。
その顔1つ1つに個別の型定義を割り振ってやるテクニックをオーバーロードと呼びます。

なお、普通にTypeScriptコードを書く時にオーバーロードはあまり使わないのがよいスタイルです。
実装が煩雑になっちゃうからね！素直にメソッドを分けましょう。

union typesが使えるようになると、@<list>{overload-vs-union-types}のように書くこともできます。
簡単な例だとunion typesのほうがよいと思いますが、このケースではどっちがいいかは、今の知見ではまだわからないですね。

//list[overload-vs-union-types][うーん、どっちがいいかは難しい]{
#@mapfile(../code/definition-file/overload-vs-union-types.ts)
// union types 以前
declare function hello(word: string): string;
declare function hello(callback: () => string): string;

hello("TypeScript");
hello(() => "function");

// union types 以後 好みの問題かもね
declare function bye(word: string | { (): string; }): string;

bye("JavaScript");
bye(() => "function");
#@end
//}

=== 外部モジュールの定義の統合

@<strong>{利用可能になったバージョン 1.3.0}

あんまり言及される事がないのでここで触れておきます。
1.1.0-1までの時代は、外部モジュールがopen endedじゃありませんでした。
1.3.0からはopen endedになったので、@<list>{external-module-declaration-merging}と@<list>{external-module-declaration-merging-usage}のようなコードが書けます。
めでたい。

//list[external-module-declaration-merging][これ、1.1.0時代はできなかったのよね]{
#@mapfile(../code/definition-file/external-module-declaration-merging.d.ts)
// 外部モジュールの定義の統合ができます！！
declare module "foo" {
  var str: string;
}

declare module "foo" {
  var num: number;
}
#@end
//}

//list[external-module-declaration-merging-usage][普通に使えます]{
#@mapfile(../code/definition-file/external-module-declaration-merging-usage.ts)
/// <reference path="./external-module-declaration-merging.d.ts" />

import foo = require("foo");
foo.str;
foo.num;
#@end
//}

ちなみに、それまでは内部モジュールとして拡張ポイントを@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b66f2750b10f262a698098cabbf210f7f2a/express/express.d.ts#L15,外出しして用意}@<fn>{extract-module}し、@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b66f2750b10f262a698098cabbf210f7f2a/passport/passport.d.ts#L8,他所で拡張する}@<fn>{use-internal-module}という頑張り方をしていました。
泣けますね。
既存ライブラリにメソッドを生やすという黒魔術はNode.js上でもできてしまうのが辛いところです。
とはいえ、1.3.0以降は型定義の作成が多少楽になったのが嬉しいところです。

//footnote[extract-module][https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b/express/express.d.ts#L15]
//footnote[use-internal-module][https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b/passport/passport.d.ts#L8]
#@# OK REVIEW muo: 上記2点のURL端っこ入る気がしないのでハッシュ削るなりしたほうが良い

=== any と {} と Object

もし、型定義ファイルを書いていて具体的な型がわからない場合、調べるのがめんどくさい場合、なんでもいいばあい、そういう時にはanyを使いましょう。

たまに、Objectを指定する人がいます。
これはJavaScriptの仕様として、プロトタイプチェーンの頂点にいるのでObjectを使おう！と思うのでしょうが、これはやめたほうがよいでしょう。

関数の引数にObjectや {} を指定するのは、結局どのような引数でも受け入れてしまいます。
本当にどのような値でも受け入れる関数であれば、anyにするべきです。

関数の返り値にObjectや {} を指定するのは、結局どのようなプロパティも存在しないため型アサーションでもって適切な型にするしかありません。
これは、anyを指定するのと同程度に危険です。
#@# OK REVIEW muo: 危険度としては〜とするなら「同程度です。」で止める。逆に「危険です。」で止めるなら「危険度としては」を削る
素直にanyを使いましょう。

筆者は今のところ、Objectや {} が型注釈として適切な場面を見たことがありません。
もし、そういう存在を知っている！という方が居られましたら、ぜひご教示ください。
大抵の場合は、適切な型を定義してそちらを参照するほうが優れているはずです。

そして、anyが気になるのであれば、よくよく調べて適切な型定義を与えるようにしましょう。

=== ドキュメントから書き起こす

もし、ライブラリにしっかりしたドキュメントがあるのであれば、実装コードから型定義ファイルを起こすのではなく、ドキュメントをベースに作成しましょう。
Visual StudioなどのIDEでは、型定義ファイル上に書かれたJSDocコメントも利用時に表示してくれる場合があります。
そのため、型定義を起こしつつ、あわせてJSDocを記述していくとよいでしょう。

少なくとも、型定義には既に書き起こしてあるのにドキュメント中に書かれている利用例がコンパイルに通らないような型定義は悪い型定義であるといえます。
サンプルをテスト用コードとしてTypeScriptコードに移植し、ドキュメント通りの記述が可能かも確かめるとよいでしょう。

とはいえ、世の中ドキュメントにコストをあまり掛けることのできないプロジェクトも多くあります。
その場合、コードから型定義ファイルを起こすことになるのは仕方のないことです。

なお、DefinitelyTypedのjQueryの型定義ファイルを熱心に面倒みてくれているJohn Reillyは特にドキュメントとの整合性を熱心に見るので、もしjQueryのドキュメント自体が間違っている場合はjQueryのドキュメントを直すところから始めるとよいでしょう。
#@# OK REVIEW muo: ここ1個目の"jquery"は意図してる?それともjQuery?
コントリビュートの輪！

=== コールバック関数の引数を無闇に省略可能（optional）にしない

まずは例を見てみましょう（@<list>{callback/basic}）。

//list[callback/basic][optionalはもしかしたら値がない事を表す]{
#@mapfile(../code/definition-file/callback/basic.ts)
// 良い例
declare function onClick(listener: (e: MouseEvent) => void): void;
// 悪い例
declare function onClickOpt(listener: (e?: MouseEvent) => void): void;

// 使ってみよう！
onClick(e => {
  // ここでのeは必ず実体がある
  console.log("clicked!" + e.srcElement.textContent);
});
onClickOpt(e => {
  // ここでのeはundefinedかもしれない… チェックしなくていいの？
  console.log("clicked!" + e.srcElement.textContent);
});

// 引数を無視するのは自由 optionalにする理由にはならぬ！
onClick(() => {
  console.log("clicked!");
});
onClickOpt(() => {
  console.log("clicked!");
});
#@end
//}

両方とも、クリックイベントをハンドリングするための関数を型定義として書き起こしたものです。
onClickはeが省略不可、onClickOptはeが省略可能（optional）になっています。
これは、onClickOptではeがundefinedになるかもしれない事を表します。
eがundefinedかもしれないなら、if文とかで中身があるかチェックしなくていいの？という不安が生じます。

執筆時点ではこれを検出してくれる程度に頭のいいlintツールは筆者の知るかぎり存在していません。
ですが、正しい意思表示を行うためにも引数が確実に与えられると分かっている場合は、省略可能にしないようにしましょう。

optionalとは、値が渡されるかどうかの指標であって、コールバックを受け取った側が使うかどうかではないのです。
そのことに留意しておきましょう。

=== インタフェースのプリフィクスとして I をつけるのはやめろ！

って@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,明記}@<fn>{writing-dts-files}された。

C#やJavaよりも、広い範囲でインタフェースが利用されるので"実装を強制させるパーツ"扱いしてはいけないからである。

らしいが、そもそもC#の文化持ち込んで I つけるのやってたのMicrosoftさんじゃないんですかぁー！？
という気持ちになるがまぁ広い心で受け入れていこうなのだ。
過去にもboolをbooleanに変えたり、先頭大文字のメソッド名を先頭小文字にしたり、C#文化をWeb界隈にあわせようとしてくれてますからね…。

そんなこんなで、まだ微妙に古いスタイルが残ってたりするのでTypeScriptコンパイラのコードを見てTypeScriptのコードスタイルを勉強してはいけないゾ☆
本当に色々なスタイルが混在していてヤバイんですよ。1.4.0で内部APIが公開されるまでには修正されると思うのですけれど。

//footnote[writing-dts-files][http://www.typescriptlang.org/Handbook#writing-dts-files]

=== ちょっと小難しいexport句の使い方

インタフェースやクラスのインスタンス単体を外部モジュールの外側に見せたい場合、@<list>{export-sample1}のように書きます。

//list[export-sample1][実はインタフェースFooも外から見えない]{
#@mapfile(../code/definition-file/export-sample1.d.ts)
declare module "foo" {
  interface Foo {
    num: number;
  }

  // この _ は外部からは参照できない。exportしてないので。
  var _: Foo;
  export = _;
}
#@end
//}

呼び出し側では@<list>{export-sample1-usage}のように使います。
importした値がインタフェースFooのインスタンスになっていることがわかります。

//list[export-sample1-usage][使う時。インタフェースFooのインスタンスが得られる。]{
#@mapfile(../code/definition-file/export-sample1-usage.ts)
/// <reference path="./export-sample1.d.ts" />

// f は "foo" の Fooのインスタンス だよ！
import f = require("foo");
f.num;
#@end
//}

よくやりがちな誤りは@<list>{export-sample2}みたいな書き方をしてしまうことです。
インタフェースのインスタンスをexportしたつもりが型がexportされてしまうのです。
そして@<list>{export-sample2-usage-invalid}のようなエラーに出くわすことになります。

//list[export-sample2][それは値ではなくて型だけ輸出しているぞ！]{
#@mapfile(../code/definition-file/export-sample2.d.ts)
declare module "foo" {
  interface Foo {
    num: number;
  }

  // よくやりがちな過ち
  export = Foo;
}
#@end
//}

//list[export-sample2-usage-invalid][ｱｰｯ! らめぇ！]{
#@mapfile(../code/definition-file/export-sample2-usage-invalid.ts)
/// <reference path="./export-sample2.d.ts" />

// f は "foo" の Fooそのもの だよ！
import f = require("foo");

// export-sample2-usage-invalid.ts(7,1): error TS2304: Cannot find name 'f'.
f.num;

// この書き方は正しい
import Foo = require("foo");
var foo: Foo;
foo.num;
#@end
//}

こういう悲しい目を回避するには、型定義ファイルのテストが有効です。
とりあえず型定義ファイルを書いたら適当なユースケースに当てはめて意図通りコンパイルできるか確かめてみましょう。

#@# OK REVIEW muo: ここの2行ゆるっゆるで文意がババロア感してるので一度見直しplz
#@# ↑文章全体を削って対処

=== 最終チェック！

やった！型定義ファイルが書けたぞ！
己の出来高に満足する前に、もう少しだけやっておきたいことがあります。
それが、--noImplicitAnyをつけての試しコンパイルとtslintによるチェックです。

==== --noImplicitAny

TypeScriptコンパイラの最重要オプション、--noImplicitAnyを使って型定義ファイルをコンパイルしてみましょう。
@<list>{noImplicitAny/basic-invalid}のような、メソッドの返り値の型を書き忘れた！という脇の甘いコードを突っ込んでみます。

//list[noImplicitAny/basic-invalid][メソッドの返り値を書き忘れた！]{
#@mapfile(../code/definition-file/noImplicitAny/basic-invalid.d.ts)
declare class Sample {
  // 返り値の型を指定し忘れている！
  // error TS7010: 'method', which lacks return-type annotation,
  //               implicitly has an 'any' return type.
  method();
}
#@end
//}

//cmd{
$ tsc --noImplicitAny definition.d.ts
definition.d.ts(3,5): error TS7010: 'method', which lacks return-type
    annotation, implicitly has an 'any' return type.
//}

返り値の型を書いていないため暗黙的にanyになってしまいました。
これに対して、それはあかん！とコンパイラが教えてくれます。
anyが紛れ込んできて、型チェックが意味を成さなくなるとTypeScriptコードの意義が薄れてしまいます。
型定義ファイルを書くときも、通常の開発時も、常に--noImplicitAnyを使うようにしましょう。

==== tslint

lintという種類のプログラムがあります。
#@# OK REVIEW muo: プログラムの種類→種類のプログラム
ざっくり、プログラムを静的に解析してバグになりそうな箇所や悪いコードスタイルを見つけてくるツールを指します。
#@# OK REVIEW muo: のことを→トルツメ

TypeScriptではtslintというプログラムが一般的に使われています。
tslintのリポジトリは@<href>{https://github.com/palantir/tslint,こちら}@<fn>{tslint-repo}です。

tslintはコンパイルだけでは見つけきれない、悪い臭いのするコードを検出してくれます。
#@# OK REVIEW muo: 匂い→臭いかな。ネガティブなニュアンスなので
例を見てみましょう（@<list>{tslint/basic}）。

//list[tslint/basic][ん？何かおかしなコードがあるぞ？]{
#@mapfile(../code/definition-file/tslint/basic.ts)
// name must be in pascal case
class foo {
}
// unused variable: 'bar'
var bar: any;
#@end
//}

このコードの悪いところは、クラス名が先頭大文字じゃない！一回も参照していない変数がある！というところです。
その両方とも、tslintは検出してくれています。

tslintは必ず設定ファイルを必要とします。
今のところ、TypeScript界における統一見解は存在していないので、@<href>{https://github.com/palantir/tslint/blob/master/tslint.json,tslintが使ってる設定ファイル}@<fn>{tslint-example-config}を参考にしてみるのがよいかもしれません。

#@# TODO 全設定項目解説とおすすめ設定を書きたいなぁ

//footnote[tslint-repo][https://github.com/palantir/tslint]
//footnote[tslint-example-config][https://github.com/palantir/tslint/blob/master/tslint.json]

== Let's contribute!
#@# OK REVIEW muo: Let'sに噛ませるなら contribute! では

ようこそ！@<href>{https://github.com/borisyankov/DefinitelyTyped,DefinitelyTyped}@<fn>{dt}へ！
メンテナのvvakameです。

DefinitelyTypedではさまざまな型定義ファイルを取り揃えてございます！
世界中の人々が作った型定義ファイルは集積され、tsdや、NuGetや、dtsmなどを介して広く利用されています。

貴方が作った型定義ファイルも、DefinitelyTypedでホストして世界中の人々に使ってほしいとは思いませんか？
もしくは、あなたがいつも使っている型定義ファイルのバグを治したい…そんな気持ち。

全て、DefinitelyTypedにぶちまけましょう！

本書を読んでいただいた紳士淑女の皆様はたぶん、よい感じの品質の型定義ファイルが書けるようになったことと思うんですよね。
品質と時間のトレードオフを考えて、いい感じの型定義ファイルを提供していただきたく存じます。

DefinitelyTypedはGitHub上のリポジトリなので、追加・修正についてはpull requestをご利用ください。

具体的にpull requestを送るまでの流れは以前@<href>{http://qiita.com/vvakame/items/1980d4b6cc222e03fdcb,Qiitaに書いた}@<fn>{guide-on-qiita}ので、そちらを参照していただけると幸いです。

#@# TODO 転記して再構成したい…

ここでは、筆者がDefinitelyTypedに送られてきた時にどういう考えでpull requestのレビューをしているかを示したいと思います。
あくまで、ここに書いてあることは筆者固有の観点なので、レビュワーによって別の事を言われる場合もあるでしょう。
実際にpull requestを送ってみて、ここに書いてある以外の理由で拒否されたとしても、そこは実際のレビュワーの言うことを尊重して議論していただきたいと思います。

とはいえ、メンテナは全員@<href>{http://definitelytyped.org/guides/contributing.html,DefinitelyTypedのContribution guide}@<fn>{dt-contrib-guide}は読んでいるはずなので、こちらには目を通しておくとよいでしょう。

//footnote[dt][https://github.com/borisyankov/DefinitelyTyped]
//footnote[guide-on-qiita][http://qiita.com/vvakame/items/1980d4b6cc222e03fdcb]
//footnote[dt-contrib-guide][http://definitelytyped.org/guides/contributing.html]

=== 新規型定義ファイルの追加のレビューの観点

まずは今までなかった、新しいライブラリに対する型定義ファイルのレビューの観点を解説していきます。

 1. CIが通っているか
 2. npmまたはbowerに公開されている名前と同じになっているか 公開されていない場合は競合が発生しないか
 3. テストが存在しているか
 4. 幽霊モジュールを使ったほうが構造が綺麗になるか

だいたいこんな感じです。

CIが通っているか。
これは、ヘッダが定められた形式で書かれているか、--noImplicitAny付きで型定義ファイルやテストがコンパイルできるか、を主に見ています。

npm, またはbowerに公開されている名前通りか。
これは、そのまんまでnpmに公開されているライブラリはnpmで公開されている名前と同一のディレクトリ名・ファイル名にします。
もし、npmに公開されていない場合は適当に名前を選ぶしかありませんが、同名の別のライブラリがnpm上に存在していないかなどをチェックしています。

逆に、ここに書かれていないことはあまり見ていません。
例えば、ライブラリの実装全体に対する、型定義ファイルのカバー率や、anyの多さなどはあまり見ていません。
それらは後から別の、それを必要とした人が補ってくれる可能性があるからです。
別に一人でやりきらなくてもいいよな！という発想ですね。
もちろん最初に高品質高カバー率のものが出てきたほうが やりおる！ とは思います。

なお、レビュワー次第ですがJSDocがきっちり書かれているか、というのを見る人もいます。
筆者はWebStormで開発していてJSDocの恩恵が薄いため、自然とJSDocをあまり重要視しなくなってしまいました。
とはいえ、きちんとドキュメントから転記などしてあるものが送られてきた時はやはり感心します。

=== 既存型定義ファイルの修正のレビューの観点

 1. CIが通っているか
 2. 破壊的変更が含まれていないか
 3. ライブラリ本体のドキュメントまたは実装からその変更内容が妥当であるかの検証

これだけです。
新規追加の場合は比較的レビューがガバガバなのですが、既存のものの変更は既に利用している人に影響があるため、勢い慎重になってしまいます。
そのため、結構レビューに時間が取られて辛いので、ドキュメントや実装が確認できるURLを添えてくれると大変嬉しいです。

いくつか補足しましょう。

破壊的変更が含まれていないか。
例えば、コードスタイルの変更（インタフェースのプリフィクスにIをつける、つけない など）や、幽霊モジュールを使っていないスタイルから使っているスタイルへの変更など。
または、別に間違っていないメソッドなどの型定義から別表現への書き換えなど。
これらはレビュアーが妥当かどうかを判断します。
大抵、判断できないのでヘッダに書いてある Definitions by: に名前が書いてある人達にGitHub上でmentionを飛ばして相談します。
型定義ファイルを作った人は、大抵の場合実際に使っている人、つまり破壊的変更を加えられると困る人達です。
ですので、もしある型定義ファイルに変な変更が入りそうな場合、きっと事前に相談してほしいに違いないと考えるわけです。

次に、変更内容が妥当であるかの検証 です。
大抵は筆者が当該ライブラリのドキュメントまたは実装を読んで正しいかどうかをverifyします。
しかし、大規模な更新やライブラリ固有の事情が入り込む場合、レビューできない or めっちゃくちゃ大変であると判断した場合は前項同様、 Definitions by: の人たちにレビューの依頼を投げます。

では、皆様のpull request、お待ちしています！
