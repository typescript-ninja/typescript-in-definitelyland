={definition-file} JS資産と型定義ファイル

//comment{
npmにpublishする時のベストプラクティス
@types
//}

== JavaScriptの資産が使いたい！

TypeScriptはJavaScriptの上位互換であり、JavaScriptを置き換えるものです。
とはいえ、現時点ではWebアプリの世界はJavaScriptで成り立っていますし、すでに莫大な資産があります。
それらを放り出してしまうのはあまりにもったいないので、TypeScriptでも活用していきたいところです。
そのためにTypeScriptは既存のJavaScript用資産を活用するための仕組みを持っています。
それが、型定義ファイルです。

通常のTypeScriptコードは拡張子が.tsなのに対して、型定義ファイルは拡張子を.d.tsとします。
拡張子を.d.tsとしたファイルに実装を含むようなコードを書くとtscがエラーにするので、ケアレスミス予防のためにも型定義ファイルの拡張子は必ず.d.tsにします。

TypeScriptでは、JavaScriptの自由奔放（かつ、危険がてんこ盛り）の世界に後付で型を与えます。
もとからTypeScriptで書かれている場合、実装と型定義を同時に書いているようなものなので、実装と型がズレて（つまりバグって）しまうことはありません。
一方、型定義ファイルはすでに実装があるJavaScriptに後付かつ手書きで型をつけていくため、ズレる（バグる）可能性が大いに有ります。
そこのところを十分に気をつけないといけません。

== ツールを使って検索だ！

#@# TODO @types について書く

さて、まずは自分で型定義ファイルを作るよりも、既存のものを使ってみましょう。
有名どころはひととおり揃っています。

そのために、まずはtsdまたはdtsmというツールを導入しましょう@<fn>{tsd}@<fn>{dtsm}@<fn>{NuGet}。

//cmd{
# どちらか！
$ npm install -g tsd
$ npm install -g dtsm
//}

まずは、プロジェクトで使う型定義ファイルを記録するために依存関係を保存するための設定ファイルを生成します。

//cmd{
# どちらか！
$ tsd init
$ dtsm init
//}

ここではテストを書くときに使う便利ライブラリ、power-assertを題材にして型定義ファイルをダウンロードし、保存してみます。

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
tsdとdtsmでは微妙に生成されるファイルが異なりますが、おおむね次のような構成になっているはずです。

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

//footnote[tsd][tsdはbartvdsがメインに開発している型定義ファイル管理ツールで、広く使われています]
//footnote[dtsm][dtsmは筆者（vvakame）が作っているツールで、まだあまり宣伝していないためユーザは少ないです。みんな使ってね！]
//footnote[NuGet][WindowsユーザにはNuGetというツールもあるのですが、全然知らないため割愛します]

== 型定義ファイルを参照してみよう！

型定義ファイルを参照するには、tscコマンドでソースコードをコンパイルするときに一緒に指定するか、ソースコード中からリファレンスコメントで参照する必要があります。
リファレンスコメントとして参照するほうがコンパイル手順を簡素に保てるため、プロジェクトの構成をシンプルに保つことができます。

リファレンスコメントはソースコードの先頭に@<code>{/// <reference path="相対パスor絶対パス" />}の形式で指定します。

mocha+power-assertでテストを書く場合を例に、使い方を解説していきます。

テスト対象のコードは@<code>{usage/lib/index}です（@<list>{usage/lib/index}）。

//list[usage/lib/index][至って普通の外部モジュール]{
#@# TODO #@mapfile(../code/definition-file/usage/lib/index.ts)
"use strict";

export function hello(word = "TypeScript") {
  return "Hello, " + word;
}
#@# TODO #@end
//}

これに対して、テストコードを書いてみましょう（@<list>{usage/tests/indexSpec}）。
普通ですね。「特定のinputを与えるとoutputが得られる」ことを検証するコードです。

//list[usage/tests/indexSpec][mocha+power-assertでテストを書く]{
#@# TODO #@mapfile(../code/definition-file/usage/tests/indexSpec.ts)
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
#@# TODO #@end
//}

mochaでは、describeで何に対してのテストかを宣誓し、itでどういう性質をもつべきかを宣誓します。
power-assertは、適当にassertに真になってほしい式を突っ込んでおけばそれが本当に真になっているかを検証して報告してくれます。

ここで問題なのは、mochaとpower-assertについての情報がソースコード上に存在していないことです。
たとえば、assert関数はpower-assertが提供するものですし、describeとitはmochaが提供しています。
JavaScriptの世界では静的な型検査などありませんので問題ありませんが、TypeScriptではそうはいかないため型情報をぶっこんでやる必要があります。
そこで使われるのが型定義ファイルです。

型定義ファイルの抜粋を示します。
mocha（@<list>{usage/abstract/mocha}）とpower-assert（@<list>{usage/abstract/power-assert}）の型定義ファイル（抜粋）を見てみましょう。

//list[usage/abstract/mocha][mocha.d.ts抜粋]{
#@# TODO #@mapfile(../code/definition-file/usage/abstract/mocha.d.ts)
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
#@# TODO #@end
//}

//list[usage/abstract/power-assert][power-assert.d.ts抜粋]{
#@# TODO #@mapfile(../code/definition-file/usage/abstract/power-assert.d.ts)
declare function assert(value: any, message?: string): void;

declare module "power-assert" {
  export = assert;
}
#@# TODO #@end
//}

型定義ファイルを見るとmocha, power-assertそれぞれのAPIが表現されています。
@<list>{usage/tests/indexSpec}から「これらのAPIを使います！」と宣言すれば、矛盾なくコンパイルを通すことができそうです。
その意思を表すためにリファレンスコメントを使います。

== 型定義ファイルを書こう

さて、型定義ファイルの取得方法、使い方はわかりました。
しかし、世の中にあるJavaScriptライブラリのうち、まだまだ型定義ファイルが書かれていないものは数多くあります。
特に、門外不出の社内ライブラリなどは誰も手をつけていない前人未到の地に違いありません。

#@# @suppress InvalidExpression
しからば！自分で書くしかあるまいよ！
ぶっちゃけた話、めんどくさいのですが、後々の安心・安全を得るための投資として割りきりましょう。

なお、自分で型定義ファイルを書く覚悟無しにTypeScriptをやるのは茨の道だと思いますので頑張ってください。
「誰かがやってくれないと自分ではできません」なんて甘えた根性では型サバンナでは到底生きていけないのです（DefinitelyTypedメンテナの意見です）。

=== 型、実体、そして42。

TypeScriptはJavaScriptに対して後付で型による制約を付け足した言語です。
そのため、JavaやC#のような最初から型ありきの言語より少し考え方が複雑です。
具体的にいえば、型と実体（値）というものが分かれています。

すべてがTypeScriptで書かれたプログラムであれば、型と実体は基本的には一致しています。
クラスの定義を書いたとき、JavaScriptプログラムとしてのクラスと、TypeScriptで使う型としてのクラスが一度に誕生します。
これは大変素直かつ簡単な動作で、型と実体をひとつの記述から作成しているためこの2つが乖離してしまうことはありません。

#@# @suppress CommaNumber
一方、JavaScriptでコードを書いてTypeScriptで型定義ファイルを作成して使う場合、実装と型が個別に定義されることになります。
そのため、型と実体の2つが分離してしまい、この2つの間に乖離が生じると（つまりバグると）コンパイルが通るのに実行時エラーが多発する、というありさまになるわけです。
型定義ファイルを書いて"この変数は、あります！"と宣言したけれど、実際には存在せず実行時エラーになるというのは広く使われている型定義ファイルですらままある話です。

=== 良い型定義ファイル、悪い型定義ファイル

型定義ファイルにも、良し悪しがあります。
その基準は至って簡単です。

 1. 正しいライブラリの使い方を導くこと
 2. 他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと
 3. IDE上で使いやすいこと

筆者が考えるのはこの3つです。

正しいライブラリの使い方を導く、というのは裏を返せば間違った使い方ができないようにする、ということです。
これには、型と実体の定義に乖離が存在せず、コンパイルが通ったら実行時エラーが簡単には起こらないことも含まれます。

他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと、というのは、意図せぬインタフェースの統合などが起こらないことを指します。
このためには、汎用的な名前を使うのを避けたり、可読性が高く理解しやすい型定義を作り干渉した場合に容易に判断できるようにすることも含まれます。

IDE上で使いやすいこと、というのは、Visual Studio Codeなどでコードを書く上で入力補完の候補が不用意に出過ぎないようにして見通しのよい開発を助けることなどが含まれます。

#@# prh:disable
これら3つを守ることが"良い品質であること"に繋がるというのは、TypeScript自体が型指定を行うことで間違ったコードを書きにくいようにするツールであると考えると納得がいくでしょう。

慣れないうちはどうしても"上手く書けないので仕方なく"悪い型定義を書いてしまうことがあります。
DefinitelyTypedにpull requestを送ってくれる人にもそういう人は多くいます。

#@# prh:disable
これから説明するベストプラクティスを踏まえて、より良い型定義ファイルを作成できるように鍛錬していきましょう。

#@# @suppress ParagraphNumber SectionLength
== 型定義ファイルのベストプラクティス

#@# @suppress SentenceLength JapaneseStyle
#@# prh:disable
型定義ファイルを書く上でのベストプラクティスを解説していきます。
基本的には@<href>{http://www.typescriptlang.org/docs/handbook/writing-declaration-files.html,公式HandbookのWriting Declaration Files}@<fn>{official-handbook}と@<href>{http://definitelytyped.org/guides/best-practices.html,DefinitelyTypedのbest practices}@<fn>{dt-best-practice}に従っておけばよいです。
本書では、そこに書かれていることや筆者の経験則などを説明していきます。

#@# prh:disable
//footnote[official-handbook][@<href>{http://www.typescriptlang.org/docs/handbook/writing-declaration-files.html}]
//footnote[dt-best-practice][@<href>{http://definitelytyped.org/guides/best-practices.html}]

=== テキトーに、やろー！

一番最初にコレを書くのもどうかと思うのですが、まずは"使える"ようにすることが一番大切です。

型定義ファイルの品質の良さにこだわるあまり、完成しない、使いたいライブラリが使えない、というのがもっともよくない状態です。
型定義ファイルの良し悪しを判断する力は、TypeScript自体の理解度に大きく依存します。
TypeScriptを書き始めの頃は、品質を気にした所で後々粗が見えてくるのは避けられないのでまずは"使える"状態にすることを目指しましょう。

品質や"ライブラリ全体をカバーしている"かは気になるところではあります。
しかし、まずは使いたいところが使えればいいのです。
スゴいものになると、1万行を超える型定義ファイルがあります。また3000行程度のものはわりとごろごろしています…。
しかし、そんなにも頑張って書いてると、余裕で日が暮れてしまいます@<fn>{atom-dts}。

まずは、使いたいところが、使える！
それでよいのです。
ドラゴン・ゲンドーソー＝先生のインストラクション・ワンを思い出しましょう。
「百発のスリケンで倒せぬ相手だからといって、一発の力に頼ってはならぬ。一千発のスリケンを投げるのだ！」
最初は上手くできなくても、数をこなし学習を重ねれば、そのうち立派な型定義ファイルを書けるようになるでしょう。

//footnote[atom-dts][なお、筆者はGitHubの作っているエディタ、Atomの型定義ファイルを3日かけて書いたことがあります。アレがジゴクだ]

==== 最高に雑な型定義ファイルを作る

テキトーにやるためにまずは最高に雑な、とりあえず動く型定義ファイルを作ってみます（@<list>{wildcard/basic-invalid}）。
モジュール名しか指定しなかったり、anyな変数を用意したりしてコンパイルエラーを回避します。

//list[wildcard/basic-invalid][雑な型定義ファイルの例]{
#@mapfile(../code/definition-file/wildcard/basic-invalid.d.ts)
// 名前だけ定義すると全てanyでとりあえず使える
declare module "lodash";

// 必要な変数をとりあえずanyで生やす
declare let $: any;

// 特定のパッケージ配下をとりあえず全部anyで
declare module "sample/*";


// WebPackなど特殊なローダー用
declare module "json!*";

// 同上
// モジュール読んだらモジュールは文字列
declare module "*!text" {
  const _: string;
  export = _;
}
#@end
//}

この例だと、@<code>{--noImplicitAny}オプションを有効にするとエラーになってしまいます。
そのため、コンパイルエラーを通したらなるべく早く@<code>{--noImplicitAny}を有効にできるように頑張りたいところです。

この型定義ファイルの利用例を見てみます（@<code>{wildcard/basicUsage-ignore}）。

#@# tsc code/definition-file/wildcard/basicUsage-ignore.ts code/definition-file/wildcard/basic-invalid.d.ts

//list[wildcard/basicUsage-ignore][型定義ファイルの利用例]{
#@mapfile(../code/definition-file/wildcard/basicUsage-ignore.ts)
import * as _ from "lodash";
import * as sample from "sample/foobar";
import * as data from "json!./bar.json";
import * as text from "./foo.txt!text";

// _はany
_.map([1, 2, 3], n => n * 3);
// $はany
$("#id");
// sampleはany
sample;
// dataもany
data;
// textはstring
text.toUpperCase();
#@end
//}

anyばっかりですね。
しかし、コンパイルはとおります。

#@# @suppress ParagraphNumber SectionLength
=== インタフェースを活用する

インタフェースは大変使いやすいパーツです。
というのも、インタフェースには@<strong>{後から定義を拡張できる}という特性があるからです（@<list>{interface/declarationMerging}、@<list>{interface/declarationMergingUsage}）。

//list[interface/declarationMerging][定義を分割して書く]{
#@mapfile(../code/definition-file/interface/declarationMerging.d.ts)
interface Foo {
  hello(): string;
}

// 同名のインタフェースを定義すると、合体される！
interface Foo {
  bye(): string;
}
#@end
//}

//list[interface/declarationMergingUsage][定義が統合される！]{
#@mapfile(../code/definition-file/interface/declarationMergingUsage.ts)
/// <reference path="./declarationMerging.d.ts" />
// ↑ 昔はこのようにreferece commentを使ってファイル間の依存関係を明示していましたが、
//   最近はtsconfig.jsonに全ての依存関係を書くようにしたため見かける事が大変少なくなりました

let foo: Foo = null as any;

foo.hello();
foo.bye();

export { }
#@end
//}

このとおり、別々に定義したインタフェースがひとつに統合されています。
これを利用することで、既存の型であろうとも拡張が可能になるのです。

例をひとつ見てみましょう。
String#trimStartは、文字列の先頭にある空白文字を取り除く機能です。
本章執筆時点（2016年07月17日）では、この提案@<fn>{string-trimStart}はTC39のプロポーサルでstage 1で、TypeScriptにはまだ入ってきていません。
そのため、Stringインタフェースを拡張する形でコンパイルを通せるようにしてみましょう（@<list>{interface/stringTrimStart}）

//list[interface/stringTrimStart][String#trimStartを生やす]{
#@mapfile(../code/definition-file/interface/stringTrimStart.ts)
interface String {
  trimStart(): string;
}

let str = "  TypeScript  ";

// 文字列先頭の空白文字を削る
console.log(str.trimStart());
#@end
//}

あとは、実行時にString.prototype.trimStartを適当な実装で補ってやれば未サポートのブラウザでも利用可能になるでしょう。

この手法は、他人が作った型定義ファイルを拡張する場合にも活用できます。
相乗りできるのであれば遠慮なく乗っかっていってしまいましょう。

#@# prh:disable
//footnote[string-trimStart][@<href>{https://github.com/sebmarkbage/ecmascript-string-left-right-trim}]

=== 幽霊モジュール

幽霊モジュール@<fn>{ghost-module}という考え方があります。

namespaceを作ったとしても、即座に実体が生成されるとは限りません。
namespaceが抱えるのがインタフェースのみである場合、実体がある扱いにはならないのです（@<list>{ghost-module/invalid}）。

//list[ghost-module/invalid][幽霊モジュール]{
#@mapfile(../code/definition-file/ghostModule/invalid.ts)
declare namespace ghost {
  interface Test {
    str: string;
  }
}

// 型としては普通にアクセスできる
let test: ghost.Test;
test.str;

// 実体としては存在していない！
// invalid.ts(13,17): error TS2304: Cannot find name 'ghost'.
let notExists = ghost;

export { }
#@end
//}

これを活用して、大量のインタフェースをもつようなライブラリの定義をひとまとまりにできます。

実際の例を見てみましょう。
@<list>{ghostModule/jqueryWithoutGhostModule-ignore}はjQueryの型定義ファイルからの抜粋（＆一部改変）です。

//list[ghostModule/jqueryWithoutGhostModule-ignore][実際のjQueryの型定義の例]{
#@mapfile(../code/definition-file/ghostModule/jqueryWithoutGhostModule-ignore.d.ts)
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

トップレベルに複数の型がいくつも散乱してしまうのがよくありません。
それに、@<code>{JQuery}というprefixが乱舞していて目を惑わせます。
ライブラリ内部でAPI同士が参照する場合でも引数や返り値にプリフィクスが必要なのはめんどうくさいです。
IDE上で型注釈を手書きするときも候補がたくさんサジェストされてしまうことでしょう。

#@# @suppress ParenthesizedSentence
これを幽霊モジュールを使って書きなおしてみます（@<list>{ghostModule/jqueryWithGhostModule-ignore}）。

//list[ghostModule/jqueryWithGhostModule-ignore][幽霊モジュールを使ってみた]{
#@mapfile(../code/definition-file/ghostModule/jqueryWithGhostModule-ignore.d.ts)
declare namespace jquery {
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

#@# @suppress SuccessiveWord
インタフェース名が短く、かつわかりやすくなりました。
やっぱり、こういうのがいいですね。

もちろん、無理に幽霊モジュールを使う必要はありません。
クラスや変数や関数などを持ち、通常の実体をもつモジュールが存在している場合は、そのモジュールに相乗りしてしまったほうが楽でしょう。

#@# @suppress JapaneseStyle
#@# prh:disable
…なんでDefinitelyTyped上にある型定義ファイルでそうなってないものが多いかって？
よい質問です。
ひとつは幽霊モジュールの認知度が低いこと、もうひとつは型定義ファイルの大幅な書き換えは互換性の破壊を生み出すからです。
先で説明しましたが、インタフェースは定義の統合ができます。
この性質を利用して定義の拡張を行っているので、うかつにJQueryStaticからjquery.Staticに型名を変更するとjQueryの型定義に依存しているさまざまなライブラリの色々なところが壊れてしまうのです。
特にjQueryプラグインとかはインタフェースを拡張する形で型定義するのでその量たるや…。

ともあれ、過去の定義との互換性を壊すことに繋がるため、途中から幽霊モジュールに切り替えるのは難しい場合があります。
可能であれば最初から幽霊モジュールを使うようにしましょう。
将来的には、このパターンの検出はtslintなどで機械的に行えるようにしたいところですね。

//footnote[ghost-module][TypeScriptリファレンスでは非インスタンス化モジュールという名前で紹介しました。その後、DefinitelyTypedのbest practicesでghost moduleと表記されたのでそちらに統一]

#@# @suppress JapaneseStyle ParagraphNumber SectionLength
=== なんでもかんでもインタフェースにしてはならない

おうお前少し前の文章であんだけインタフェースを持ち上げといてこれかぁ！？
と、思われたかもしれませんが、なんでもかんでも乱用すればいいってものではありません。

#@# @suppress ParenthesizedSentence
具体的に、モジュール様の構造をインタフェースを使って作ってはいけません（@<list>{interfaceAntipattern/moduleByInterfaceBad-ignore}）。

//list[interfaceAntipattern/moduleByInterfaceBad-ignore][インタフェースでモジュールを表現してしまう。何故なのか…]{
#@mapfile(../code/definition-file/interfaceAntipattern/moduleByInterfaceBad-ignore.d.ts)
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

#@# @suppress InvalidExpression
この型定義ファイルを読み解いて一瞬で使えるのは、元のJavaScriptコードを熟知している人だけでしょう。
少なくとも、この型定義ファイルをヒントに実際のコードを書くことには大いなる苦痛を伴います。
筆者は絶対に使いません。絶対です。
普通に、@<list>{interfaceAntipattern/moduleByInterfaceGood-ignore}のように書きましょう。

//list[interfaceAntipattern/moduleByInterfaceGood-ignore][素直にこうしよう]{
#@mapfile(../code/definition-file/interfaceAntipattern/moduleByInterfaceGood-ignore.d.ts)
// 普通にコレでいいだろ！！
declare namespace foo.bar.buzz {
  let str: string;
}
#@end
//}

さて次です。
普通、@<list>{interfaceAntipattern/callableModuleUsage-ignore}のような型定義ファイルを書こうとは思わないと思いますが、こういうコードが必要になる場合が稀にあります。
関数としても呼べるし、namespaceのようにも振る舞うオブジェクトの型定義を作成したいときです。

#@# TODO 内部モジュールで全文を検索 module とか declare module でも

//list[interfaceAntipattern/callableModuleUsage-ignore][関数・namespace どっちなの？]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleUsage-ignore.ts)
// assertは関数としても呼べるしnamespaceのようにも見える
assert(foo === "foo");
assert.ok(value);
#@end
//}

呼び出し可能で、プロパティをもつ。
この場合、すぐに考えつく型定義は@<list>{interfaceAntipattern/callableModuleBad1-ignore}か、@<list>{interfaceAntipattern/callableModuleBad2-ignore}でしょう。

//list[interfaceAntipattern/callableModuleBad1-ignore][こうしてしまいたい、気持ち]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleBad1-ignore.d.ts)
declare var assert: {
  (value: any): void;
  ok(value: any): void;
};
#@end
//}

//list[interfaceAntipattern/callableModuleBad2-ignore][匿名型注釈よりはマシ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleBad2-ignore.d.ts)
declare var assert: Assert;

interface Assert {
  (value: any): void;
  ok(value: any): void;
}
#@end
//}

#@# @suppress ParenthesizedSentence
たしかに、この定義でも動きます（正直、assert関数だけの定義だとこのままでもいい気がしますが…）。

しかし、これには別のよいやり方があるのです（@<list>{interfaceAntipattern/callableModuleGood-ignore}）。

//list[interfaceAntipattern/callableModuleGood-ignore][関数とnamespace 両方やらなきゃいけないのが辛いところだ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleGood-ignore.d.ts)
declare function assert(value: any): void;
declare namespace assert {
  function ok(value: any): void;
}
#@end
//}

関数とnamespaceを同名で宣言できるのです。
これの効能は、階層構造を素直に表現できることと、前項で説明した幽霊モジュールの書き方を併用できるところです。

#@# TODO types-2.0がlandしたらパス変わる
#@# @suppress SentenceLength
この手法は、実際に@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/power-assert/power-assert.d.ts,power-assertの型定義ファイル}@<fn>{power-assert-dts}でも利用されています。
@<list>{interfaceAntipattern/powerAssertAbst-ignore}に抜粋&改変したものを示します。

//list[interfaceAntipattern/powerAssertAbst-ignore][関数+namespaceの実例]{
#@mapfile(../code/definition-file/interfaceAntipattern/powerAssertAbst-ignore.d.ts)
declare function assert(value: any, message?: string): void;
declare namespace assert {

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

外部に公開されている関数は@<code>{assert}のみで、そこに追加でプロパティが生えている形式です。
実体のある要素（関数）があるため、幽霊モジュールにはなりませんが、Optionsインタフェースが上手く取り込まれています。
余計な名前を階層の浅いところにバラ撒かず、厳密さも損なっていません。
この書き方は、案外よく登場するパターンなので覚えておくとよいでしょう。

実は、このやり方は型定義ファイルだけではなく、通常のTypeScriptコードでも使えます（@<list>{interfaceAntipattern/callableModule.ts}）。

//list[interfaceAntipattern/callableModule.ts][関数が先、namespaceは後！絶対！]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModule.ts)
function test() {
  return "test!";
}
namespace test {
  export function func() {
    return "function!";
  }
}
#@end
//}

コンパイル結果の@<list>{interfaceAntipattern/callableModule.js}を見ると、なぜ関数が先でnamespaceが後、という決まりになっているかがわかりますね。

//list[interfaceAntipattern/callableModule.js][JSとして正しい構造だ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModule.js)
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

//footnote[power-assert-dts][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/power-assert/power-assert.d.ts}]

=== クラスはクラスとして定義する

クラスを型定義として起こす方法について解説します。
歴史的経緯により、TypeScriptではクラスの型定義を行う時に2つの代表的なやり方が存在しています。
まずはその2つのやり方を見てみましょう（@<list>{declareClass/basic}）。

//list[declareClass/basic][素直にクラス定義 vs インタフェース+変数]{
#@mapfile(../code/definition-file/declareClass/basic.d.ts)
// A. 普通にクラスを定義する
declare class TestA {
}

// B. クラスの分解定義 変数 + インタフェース2つ
declare let TestB: TestBConstructor;
interface TestBConstructor {
  new (): TestB;
}
interface TestB {
}
#@end
//}

こんな感じです。
普通にクラス定義をするほうが素直ですね。

過去にはこの2つのやり方にそれぞれメリット・デメリットがありました。
しかし、現在のTypeScriptでは大幅に制限が緩和されたためメリット・デメリットの面で考える必要はなくなってきました（@<list>{declareClass/stretch}）。
よい時代になったものです。

//list[declareClass/stretch][相互運用性がある！]{
#@mapfile(../code/definition-file/declareClass/stretch.ts)
// classはopen-endedになったため同名のinterfaceで拡張可能に
class Person {
  name: string;
}
interface Person {
  age: number;
}
let p: Person = new Person();
// 両方アクセス可能！
console.log(p.name, p.age);

// interfaceを使ったクラスの構成でも
interface AnimalConstructor {
  new (): Animal;
}
interface Animal {
  speak(): string;
}
/* tslint:disable:variable-name */
let Animal: AnimalConstructor = class {
  speak() {
    return "???";
  }
};
/* tslint:enable:variable-name */
// Animalはただの変数だが普通に継承できる！
class Cat extends Animal {
  speak() {
    return "meow";
  }
}
let cat: Cat = new Cat();
console.log(cat.speak());

// ECMAScript 2015だと以下のような分すら書けるのでまぁできて然るべきだった
let cat2: Cat = new class extends class {
  speak() {
    return "???";
  }
} {
  speak() {
    return "meow";
  }
}();
console.log(cat2.speak());

export { }
#@end
//}

=== オーバーロードを上手く使おう！

#@# @suppress JapaneseAmbiguousNounConjunction
正しいライブラリの使い方を導くこと。
を心に秘めて、@<list>{overload/useOverload}を見てください。
どれが一番、元々の関数の仕様がわかりやすいですか？

//list[overload/useOverload][普通に使えます]{
#@mapfile(../code/definition-file/overload/useOverload.ts)
// 同じ実装に対して、どの型定義が一番便利かな？
// 1関数でget, set両方の役目を果たす場合…

// getのとき setのとき 仕様が違うことがよく分かる
declare function valueA(value: any): void;
declare function valueA(): any;

// setのときも値が取れる気がする…？
declare function valueB(value?: any): any;

// 詳細が不明だ…！
declare let valueC: Function;
#@end
//}

答え：一番最初のやつ。

JavaScriptのライブラリは1つの関数にさまざまな使い方をさせようとする場合がままあります。
つまり、1つの関数が複数の顔をもつということです。
その顔ひとつひとつに個別の型定義を割り振ってやるテクニックをオーバーロードと呼びます。

なお、普通にTypeScriptコードを書くときにオーバーロードはあまり使わないのがよいスタイルです。
実装が煩雑になってしまいますからね。
素直にメソッドを分けましょう。

union typesを使うと、@<list>{overload/overloadVsUnionTypes}のように書くこともできます。
簡単な例だとunion typesのほうがよいと思いますが、このケースではどっちがいいかは、今の知見ではまだわからないですね。

//list[overload/overloadVsUnionTypes][うーん、どっちがいいかは難しい]{
#@mapfile(../code/definition-file/overload/overloadVsUnionTypes.ts)
// union types未使用
declare function hello(word: string): string;
declare function hello(callback: () => string): string;

hello("TypeScript");
hello(() => "function");

// union typesあり
declare function bye(word: string | { (): string; }): string;

bye("JavaScript");
bye(() => "function");
#@end
//}

#@# @suppress SectionLength JapaneseAmbiguousNounConjunction
=== モジュールの定義の統合

#@# @<strong>{利用可能になったバージョン 1.3.0}

#@# @suppress SentenceLength
あんまり言及されることがないのでここで触れておきます。
モジュールの型定義はopen endedですので、@<list>{externalModuleDeclarationMerging/basic}と@<list>{externalModuleDeclarationMerging/usage}のようなコードが書けます。
めでたい。

//list[externalModuleDeclarationMerging/basic][モジュール定義を後から拡張可能]{
#@mapfile(../code/definition-file/externalModuleDeclarationMerging/basic.d.ts)
// モジュールの定義の統合ができます
declare module "foo" {
  let str: string;
}

declare module "foo" {
  let num: number;
}
#@end
//}

//list[externalModuleDeclarationMerging/usage][普通に使えます]{
#@mapfile(../code/definition-file/externalModuleDeclarationMerging/usage.ts)
import * as foo from "foo";
foo.str;
foo.num;
#@end
//}

#@# @suppress SuccessiveWord SentenceLength JapaneseAmbiguousNounConjunction
DefinitelyTypedではモジュールの型定義の外側にnamespaceを使った定義を掃き出し、モジュールの型定義の外側に拡張ポイントを設ける例がありました。
モジュールを利用しない、namespaceだけの構成。
たとえば、lodashやjQueryのようなグローバルな名前空間に変数を生やすような場合に、未だに有効です。

=== anyと{}とObject

もしも、型定義ファイルを書いていて具体的な型がわからなかったりとりあえずコンパイルを脳死状態で通したい場合、素直に@<code>{any}を使うことをお勧めします。
こういったシチュエーションで、稀にObjectを指定する人がいます。
これはJavaScriptの仕様として、プロトタイプチェーンの頂点にいるのでObjectを使おう！と思ったのでしょうが、これはやめたほうがよいです。

関数の引数にObjectや{}を指定するのは、結局どのような引数でも受け入れてしまいます。
本当にどのような値でも受け入れるのであれば、anyにするべきです。

関数の返り値にObjectや{}を指定するのは、結局どのようなプロパティも存在しないため型アサーションでもって適切な型にするしかありません。
これは、anyを指定するのと同程度に危険です。
素直にanyを使いましょう。

筆者は今のところ、Objectや{}が型注釈として適切な場面を見たことがありません。
もし、そういう存在を知っている！という方が居られましたら、ぜひご教示ください。
大抵の場合は、適切な型を定義してそちらを参照するほうが優れています。

#@# @suppress SuccessiveWord
そして、anyが気になるのであれば、よくよく調べて適切な型定義を与えるようにしましょう。

=== ドキュメントから書き起こす

もしライブラリにしっかりしたドキュメントがあるのであれば、実装コードから型定義ファイルを起こすのではなく、ドキュメントをベースに作成しましょう。
Visual StudioなどのIDEでは、型定義ファイル上に書かれたJSDocコメントも利用時に表示してくれる場合があります。
そのため、型定義を起こしつつ、あわせてJSDocを記述していくとよいでしょう。

#@# @suppress KatakanaSpellCheck
サンプルをテスト用コードとしてTypeScriptコードに移植し、ドキュメントどおりの記述が可能かも確かめるとよいです。
型定義ファイルは書き起こしたけれどもドキュメント中に書かれている利用例がコンパイルに通らないようであれば、それは悪い型定義であるといえます。
まぁ、たまにドキュメントのほうが間違っている場合があるのでその場合は修正のpull requestを送ったりするチャンスです。

とはいえ、世の中ドキュメントにコストをあまり掛けることのできないプロジェクトも多くあります。
その場合、コードから型定義ファイルを起こすことになるのは仕方のないことです。

現在、DefinitelyTypedにあるjQueryの型定義ファイルを熱心に面倒みてくれているJohn Reillyは特にドキュメントとの整合性を熱心に見ます。
そのため、もしjQueryのドキュメント自体が間違っている場合はjQueryのドキュメントを直すところから始めるとよいでしょう。
コントリビュートの輪！

=== コールバック関数の引数を無闇に省略可能（optional）にしない

まずは例を見てみましょう（@<list>{callback/basic}）。

//list[callback/basic][optionalはもしかしたら値がないことを表す]{
#@mapfile(../code/definition-file/callback/basic.ts)
// 良い例
declare function readFile(filePath: string, listener: (data: string) => void): void;
// 悪い例
declare function readFileOpt(filePath: string, listener: (data?: string) => void): void;

// 使ってみよう！
readFile("./test.txt", data => {
  // ここでのdataは必ず実体がある
  console.log(data.toUpperCase());
});
readFileOpt("./test.txt", data => {
  // ここでのdataはundefinedかもしれない… チェックしなければダメ
  if (!data) {
    data = "not found";
  }
  console.log(data.toUpperCase());
});

// 引数を無視するのは自由 optionalにする理由にはならない
readFile("./test.txt", () => {
  console.log("done");
});
readFileOpt("./test.txt", () => {
  console.log("done");
});
#@end
//}

両方とも、ファイルの読み取りを行うための関数を型定義として書き起こしたものです。
readFileはdataが省略不可、readFileOptはdataが省略可能（optional）になっています。
これは、readFileOptではdataがundefinedになるかもしれないことを表します。
dataがundefinedかもしれないため、if文などで中身をチェックし、undefinedだった場合の対応を入れなければなりません。
これがもし、本当にundefinedになりうるのであれば省略可能にするか、union typesでundefinedを与える必要があります。
しかし、そうではなく必ずdataの値が渡されてくる場合は、無用なチェック処理が発生することになります。

optionalとは、値が渡されるかどうかの指標であって、コールバックを受け取った側が使うかどうかではないのです。
そのことに留意しておきましょう。

=== インタフェースのプリフィクスとしてIをつけるのはやめよう！

#@# prh:disable
って@<href>{https://www.typescriptlang.org/docs/handbook/writing-declaration-files.html#naming-conventions,明記}@<fn>{writing-dts-files}された。

C#やJavaよりも、広い範囲でインタフェースが利用されるので"実装を強制させるパーツ"扱いしてはいけないからだそうです。

#@# @suppress JapaneseStyle
古くはTypeScriptコンパイラ本体のコードもC#の伝統に倣いIプリフィクスを使っていましたが、現在では取り除かれています。
また、DefinitelyTypedでも公式の記述に従い新しい型定義ファイルについてはIプリフィクスを使わぬようレビューしています。

#@# prh:disable
//footnote[writing-dts-files][@<href>{https://www.typescriptlang.org/docs/handbook/writing-declaration-files.html#naming-conventions}]

=== CommonJS形式でちょっと小難しいexport句の使い方

インタフェースやクラスのインスタンス単体をモジュールの外側に見せたい場合、@<list>{export/sample1}のように書きます。

//list[export/sample1][実はインタフェースBarも外から見えない]{
#@mapfile(../code/definition-file/export/sample1.d.ts)
declare module "bar" {
  interface Bar {
    num: number;
  }

  // この_は外部からは参照できない。exportしてないので。
  let _: Bar;
  export = _;
}
#@end
//}

呼び出し側では@<list>{export/sample1Usage}のように使います。
importした値がインタフェースFooのインスタンスになっていることがわかります。

//list[export/sample1Usage][使うとき。インタフェースBarのインスタンスが得られる]{
#@mapfile(../code/definition-file/export/sample1Usage.ts)
// b は "bar" の Barのインスタンス だよ！
import * as b from "bar";
b.num;
#@end
//}

よくやりがちな誤りは@<list>{export/sample2}のような書き方をしてしまうことです。
インタフェースのインスタンスをexportしたつもりが型がexportされてしまうのです。

//list[export/sample2][それは値ではなくて型だけexportしているぞ！]{
#@mapfile(../code/definition-file/export/sample2.d.ts)
declare module "buzz" {
  interface Buzz {
    num: number;
  }

  // よくやりがちな過ち
  export = Buzz;
}
#@end
//}

こういう悲しい目を回避するには、型定義ファイルのテストが有効です。
とりあえず型定義ファイルを書いたら適当なユースケースに当てはめて意図どおりコンパイルできるか確かめてみましょう。

#@# TODO ES2015とCommonJSの書き方の違いと正しい選択について

#@# @suppress SectionLength ParagraphNumber
=== グローバルに展開される型定義とモジュールの両立

グローバルに変数が展開されるのと、モジュールとしての利用が両立しているタイプのライブラリについて考えます。
具体的に、@<kw>{UMD,Universal Module Definition}と呼ばれる形式@<fn>{umd}です。
ライブラリ内部でモジュールとしての使い方が想定されているのか、そうではないのかを判断し展開の方法を変えます。

#@# @suppress JapaneseAmbiguousNounConjunction ParenthesizedSentence
TypeScriptではこういうパターンのときに使いやすい型定義ファイルの記述方法があります。
しかし、TypeScript 2.0.0までは任意の場所においてある型定義ファイルを特定の名前のモジュールだと認識される方法がなかったため、役に立ってはいませんでした。
この形式が使われているのはDefinitelyTypedの@typesパッケージシリーズ（本書執筆時点ではtypes-2.0ブランチ）だけではないでしょうか。

説明のために、strutilとstrutil-extraという架空のライブラリについて考えてみます。
strutilはrandomizeString関数を提供します。
strutil-extraはhappy関数を提供し、strutilを拡張します。

まずは型定義ファイルを見てみましょう（@<list>{augmentGlobal/typings/strutil/}、@<list>{augmentGlobal/typings/strutil-extra/}）。
ちょっと見慣れない書き方ですね。

//list[augmentGlobal/typings/strutil/][typings/strutil/index.d.ts]{
#@mapfile(../code/definition-file/augmentGlobal/typings/strutil/index.d.ts)
// importされなかった場合、globalにstrutilという名前で展開する
export as namespace strutil;

// 普通の型定義 declare module "..." の中と同じ書き味でよい
export interface Options {
  i?: number;
}
export declare function randomizeString(str: string, opts?: Options): string;

// グローバルな要素の拡張
declare global {
  // 既存のstring型にメソッドを生やす
  interface String {
    randomizeString(opts?: Options): string;
  }
}
#@end
//}

//list[augmentGlobal/typings/strutil-extra/][typings/strutil-extra/index.d.ts]{
#@mapfile(../code/definition-file/augmentGlobal/typings/strutil-extra/index.d.ts)
// 他のモジュールの型定義を参照する
import * as strutil from "strutil";

export as namespace strutilExtra;

export declare function happy(str: string): string;

// 他のモジュールの拡張
declare module "strutil" {
  // 既存の要素を拡張できる
  interface Options {
    reverse?: boolean;
  }

  // 自分ではないモジュールに勝手に新規の変数や関数を生やしたりはできない
  // 定義の拡張のみ可能
  // error TS1038: A 'declare' modifier cannot be used
  //   in an already ambient context.
  // export declare let test: any;
}

declare global {
  interface String {
    happy(): string;
  }
}
#@end
//}

既存モジュールの定義の拡張もできています。
この形式だと、どのライブラリを拡張しているのか明示されるところが利点となります。

さて、これらを@<code>{import ... from "strutil";}したりするためのtsconfig.jsonを確認しておきます（@<list>{augmentGlobal/tsconfig.json}）。
baseUrlとpathsの指定があります。
TypeScript 2.0.0からこうして任意の場所の型定義ファイルを任意の名前に紐付けられるようになったため、ローカル環境でも利用しやすくなりました。

//list[augmentGlobal/tsconfig.json][tsconfig.jsonの例]{
#@mapfile(../code/definition-file/augmentGlobal/tsconfig.json)
{
    "compilerOptions": {
        "module": "commonjs",
        "target": "es5",
        "noImplicitAny": true,
        "baseUrl": "./",
        "paths": {
            "strutil": ["./typings/strutil/"],
            "strutil-extra": ["./typings/strutil-extra/"]
        }
    },
    "exclude": [
        "node_modules"
    ]
}
#@end
//}

次に前述の型定義ファイルを利用する例を見てみます。
まずはグローバルに展開される例です（@<list>{augmentGlobal/lib/bare}）。

//list[augmentGlobal/lib/bare][lib/bare.ts]{
#@mapfile(../code/definition-file/augmentGlobal/lib/bare.ts)
// UMD形式のライブラリがglobalに展開されたときの動作に相当する
// import, export句がない場合、globalのstrutilが参照できる
strutil.randomizeString("TypeScript");
strutilExtra.happy("TypeScript");

// globalのStringも拡張されている
"TypeScript".randomizeString();
"TypeScript".happy();

// import、export が存在すると、ちゃんと読み込め！と怒られる
// error TS2686: Identifier 'strutil' must be imported from a module
// error TS2686: Identifier 'strutilExtra' must be imported from a module
#@end
//}

なるほど。
@<code>{export as namespace ...}形式を使わないUMD形式の対応方法もありますが、importと混ぜるとエラーになるところがよいですね。

モジュール形式も見てみましょう（@<list>{augmentGlobal/lib/module}）。
普通にモジュールであるかのように利用できますね。

//list[augmentGlobal/lib/module][lib/module.ts]{
#@mapfile(../code/definition-file/augmentGlobal/lib/module.ts)
// UMD形式のライブラリがglobaに展開されたときの動作に相当する
// importした時、普通のモジュールとして振る舞う
import { randomizeString } from "strutil";
import { happy } from "strutil-extra";

randomizeString("TypeScript");
happy("TypeScript");

// strutil-extra で追加したパラメータも反映されている
randomizeString("TypeScript", {
  i: 11,
  reverse: true, // これ
});

// globalのStringも拡張されている
"TypeScript".randomizeString();
"TypeScript".happy();
#@end
//}

この形式がどこまで普及するかはわかりませんが、時とともにDefinitelyTyped内部でも見かける頻度が増えていくでしょう。
ファイル名を見ただけではどういう名前に解決されるかがわかりにくいところだけ、注意が必要です。

#@# TODO /// <reference types="jquery" /> 的な記述の調査

//footnote[umd][@<href>{https://github.com/umdjs/umd}]

=== 最終チェック！

やった！型定義ファイルが書けたぞ！
己の出来高に満足する前に、もう少しだけやっておきたいことがあります。
それが、--noImplicitAnyをつけての試しコンパイルとtslintによるチェックです。
#@# TODO --strictNullChecks とかへの言及

==== --noImplicitAny

TypeScriptコンパイラの最重要オプション、--noImplicitAnyを使って型定義ファイルをコンパイルしてみましょう。
@<list>{noImplicitAny/basic-invalid}のような、メソッドの返り値の型を書き忘れた！という脇の甘いコードをコンパイルしてみます。

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
anyが紛れ込んで、型チェックが意味を成さなくなるとTypeScriptコードの意義が薄れてしまいます。
型定義ファイルを書くときも、通常の開発時も、常に--noImplicitAnyを使うようにしましょう。

#@# TODO 他のオプションについても触れたほうが

==== tslint

lintという種類のプログラムがあります。
ざっくり、プログラムを静的に解析してバグになりそうな箇所や悪いコードスタイルを見つけてくるツールを指します。

TypeScriptではtslintというプログラムが一般的に使われています。
tslintのリポジトリは@<href>{https://github.com/palantir/tslint,こちら}@<fn>{tslint-repo}です。

tslintはコンパイルだけでは見つけきれない、悪いにおいのするコードを検出してくれます。
tslintはちょくちょく新しいルールが追加されたりするため、本書では詳しくは取り上げません。
そのときそのときの最適な設定を突き詰めてみてください。

#@# @suppress SentenceLength
tslintは必ず設定ファイルを必要とします。
今のところ、TypeScript界における統一見解は存在していないので@<href>{https://github.com/palantir/tslint/blob/master/tslint.json,tslintが使ってる設定ファイル}@<fn>{tslint-example-config}か、@<href>{https://github.com/Microsoft/TypeScript/blob/master/tslint.json,TypeScript本体のtslint.json}@<fn>{tsc-tslint}を参照するとよいでしょう。

#@# TODO できれば本書お勧めの設定くらいは書きたい
#@# TODO 全設定項目解説とおすすめ設定を書きたいなぁ

//footnote[tslint-repo][@<href>{https://github.com/palantir/tslint}]
//footnote[tslint-example-config][@<href>{https://github.com/palantir/tslint/blob/master/tslint.json}]
//footnote[tsc-tslint][@<href>{https://github.com/Microsoft/TypeScript/blob/master/tslint.json}]

== Let's contribute!

ようこそ！@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped,DefinitelyTyped}@<fn>{dt}へ！
メンテナのvvakameです。

DefinitelyTypedではさまざまな型定義ファイルを取り揃えてございます！
世界中の人々が作った型定義ファイルは集積され、tsdや、NuGetや、dtsmなどを介して広く利用されています。
#@# TODO @types

貴方が作った型定義ファイルも、DefinitelyTypedでホストして世界中の人々に使ってほしいとは思いませんか？
もしくは、あなたがいつも使っている型定義ファイルのバグを治したい…そんな気持ちになることもあるでしょう。
その思い、すべてDefinitelyTypedにぶつけてみましょう！

本書を読んでいただいた紳士淑女の皆様はたぶん、よい感じの品質の型定義ファイルが書けるようになったことと思います。
品質と時間のトレードオフを考えて、いい感じの型定義ファイルを提供していただきたいです。

DefinitelyTypedはGitHub上のリポジトリなので、追加、修正についてはpull requestをご利用ください。

この節では、筆者がDefinitelyTypedに送られてきたときにどういう考えでpull requestのレビューをしているかを示したいと思います。
あくまで、ここに書いてあることは筆者固有の観点なので、レビュワーによって別のことを言われる場合もあるでしょう。
実際にpull requestを送ってみて、ここに書いてある以外の理由で拒否されたとしても、そこは実際のレビュワーのいうことを尊重して議論していただきたいと思います。

とはいえ、メンテナは全員@<href>{http://definitelytyped.org/guides/contributing.html,DefinitelyTypedのContribution guide}@<fn>{dt-contrib-guide}は読んでいるはずなので、こちらには目を通しておくとよいでしょう。

//footnote[dt][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped}]
//footnote[dt-contrib-guide][@<href>{http://definitelytyped.org/guides/contributing.html}]

#@# TODO TypeScript teamが参加してきていて今は彼らのほうが作業量が多いという話

=== 新規型定義ファイルの追加のレビューの観点

まずは今までなかった、新しいライブラリに対する型定義ファイルのレビューの観点を解説していきます。

 1. CIが通っているか
 2. npmまたはbowerに公開されている名前と同じになっているか、公開されていない場合は競合が発生しないか
 3. テストが存在しているか
 4. 幽霊モジュールを使ったほうが構造がきれいになるか

だいたいこんな感じです。

CIが通っているか。
これは、ヘッダが定められた形式で書かれているか、--noImplicitAny付きで型定義ファイルやテストがコンパイルできるか、を主に見ています。

npm, またはbowerに公開されている名前どおりか。
これは、そのまんまでnpmに公開されているライブラリはnpmで公開されている名前と同一のディレクトリ名、ファイル名にします。
もし、npmに公開されていない場合は適当に名前を選ぶしかありませんが、同名の別のライブラリがnpm上に存在していないかなどをチェックしています。

逆に、ここに書かれていないことはあまり見ていません。
たとえば、ライブラリの実装全体に対する、型定義ファイルのカバー率や、anyの多さなどはあまり見ていません。
それらは後から別の、それを必要とした人が補ってくれる可能性があるからです。
別に一人でやりきらなくてもいいよな！という発想ですね。
もちろん最初に高品質高カバー率のものが出てきたほうが「やりおる！」と感心はします。

なお、レビュワー次第ですがJSDocがきっちり書かれているか、というのを見る人もいます。
きちんとドキュメントから転記などしてあるものが送られてきたときはやはり感心しますね。

=== 既存型定義ファイルの修正のレビューの観点

 1. CIが通っているか
 2. 破壊的変更が含まれていないか
 3. ライブラリ本体のドキュメントまたは実装からその変更内容が妥当であるかの検証

これだけです。
新規追加の場合は比較的レビューがガバガバなのですが、既存のものの変更はすでに利用している人に影響があるため、勢い慎重になってしまいます。
そのため、結構レビューに時間が取られて辛いので、ドキュメントや実装が確認できるURLを添えてくれると大変嬉しいです。

いくつか補足しましょう。

破壊的変更が含まれていないか。
たとえば、コードスタイルの変更（インタフェースのプリフィクスにIをつける、つけない など）や、幽霊モジュールを使っていないスタイルから使っているスタイルへの変更など。
または、ある型定義を別の表現へと書き換える場合。
これらはレビュアーが妥当かどうかを判断します。
たいてい判断できないのでヘッダに書いてあるDefinitions by:に名前が書いてある人達にGitHub上でmentionが飛ばされ、レビューしてもらって決めます。
型定義ファイルを作った人は、たいていの場合、実際に使っている人、つまり破壊的変更を加えられると困る人達です。
ですので、もしある型定義ファイルに変な変更が入りそうな場合、きっと事前に相談してほしいに違いないと考えるわけです。

次に、変更内容が妥当であるかの検証です。
たいていは筆者が当該ライブラリのドキュメントまたは実装を読んで正しいかどうかをverifyします。
しかし、小規模でドキュメントのURLがちゃんと添付されている場合以外は、前項同様、Definitions by:の人たちにレビューの依頼を投げます。

では、皆様のpull request、お待ちしています！
