= 型定義ファイルとは

TODO .d.ts にすると実装が書けなくて便利

== JavaScriptの資産が使いたい！

TypeScriptはJavaScriptの上位互換であり、JavaScriptを置き換えるものかもしれません。
とはいえ、現時点のWebアプリの世界はJavaScriptで成り立っていますし、TypeScriptでコードを書いてそのまま公開している人は圧倒的少数です。
そこで、TypeScriptは既存のJavaScript用資産をTypeScriptで活用するための仕組みを持っています。
それが、型定義ファイルです。

JavaScriptの、自由奔放(かつ、危険がてんこ盛り)の世界に後付で型を与えます。
元からTypeScriptで書かれている場合、実装と型定義を同時に書いているようなものなので、実装と型がズレて(つまりバグって)しまうことはありません。
一方、型定義ファイルは既に実装があるJavaScriptに後付で手書きで型をつけていくため、ズレる(バグる)可能性が大いに有ります。
そこのところだけ、ちょっち気をつけないといけません。

== ツールを使って検索だ！

さて、まずは自分で型定義ファイルを作るよりも、既存のものを使ってみましょう。
有名ドコロはひと通りあります。

そのために、まずは tsd または dtsm というツールを導入しましょう@<fn>{dtsm}@<fn>{NuGet}。

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

ではpower-assertの型定義ファイルをダウンロードし、保存してみます。

//cmd{
# どちらか！
$ tsd query power-assert --action install --save --resolve

>> tsd 0.5.7

 - power-assert/power-assert.d.ts : <head> : 2014-11-14 02:03

   >> empower/empower.d.ts : <head> : 2014-11-14 02:03
   >> power-assert-formatter/power-assert-formatter.d.ts : <head> : 2014-11-14 02:03

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
│   └── empower.d.ts
├── power-assert
│   └── power-assert.d.ts
└── power-assert-formatter
    └── power-assert-formatter.d.ts
//}

あとは、これら型定義ファイルを自分の使うコードから参照するだけでコンパイルが可能になります。
残念ながらライブラリの実体は含まれていないため、npmやbowerなどで別途取得する必要があるでしょう。

TODO mochaも入れて、簡単なテストを書く例(型定義ファイルを使う例)を示す

//footnote[dtsm][dtsmは筆者(vvakame)が作っているツールで、まだあまり宣伝していないためユーザは少ないです。みんな使ってね！]
//footnote[NuGet][WindowsユーザにはNuGetというツールもあるのですが、全然知らないため割愛します。]

== 型定義ファイルを書こう

さて、型定義ファイルの使い方はわかりました。
しかし、世の中にあるJavaScriptライブラリのうち、まだまだ型定義ファイルが書かれていないものは数多くあります。
特に、門外不出の社内内製ライブラリなどは誰も手をつけていない前人未到の地かもしれません。

しからば！自分で書くしかあるまいよ！
ぶっちゃけた話めんどくさいのですが、後々の安心・安全のための投資として割りきりましょう。

なお、自分で型定義ファイルを書く覚悟無しにTypeScriptをやるのは茨の道だと思いますので頑張ってください。
おう甘えた根性では生きていけねぇんだよ！！オラオラ！！！(DefinitelyTypedメンテナの意見です。)

=== 良い型定義ファイル、悪い型定義ファイル

型定義ファイルにも、良し悪しがあります。
その基準は至って簡単です。

 1. 正しいライブラリの使い方を導くこと。
 2. 他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと。
 3. IDE上で使いやすいこと。

こんな感じです。
正しいライブラリの使い方を導く、というのは、裏を返せば間違った使い方ができないようにする、ということです。
TypeScript自体が型を使って間違ったコードを書きにくいようにするツールであると考えると、納得ですよね。

=== 型、そして 実体。

TypeScriptは、JavaScriptに対して後付けで型による制約を付け足した言語です。
そのため、JavaやC#のような、最初から型ありきの言語より、少し考え方が複雑です。
具体的に言えば、型と実体(値)というものが分かれています。

全てがTypeScriptで書かれているプログラムであれば、型と実体はわりと一致しています。
クラスを定義した時、JavaScriptプログラムとしてのクラス(OOPするためのコンストラクタ関数)と、TypeScriptで使う型としてのクラスが一度に爆誕します。
これは非常に素直かつ簡単で、型と実体を1つの記述から作成しているので、この2つが分離してぐちゃぐちゃになってしまい、コンパイルが通るのに実行時エラーが多発する、ということは起こりにくくなります。

一方、JavaScriptで書いて、型定義ファイルを当てて使う場合、実装と型がバラバラに定義されているため、バグる可能性があります。

また、型定義ファイルを書いて"この変数は、あります！"と宣言したけど、実際には存在せず実行時エラーというのもままある話です。

"上手く書けないので仕方なく"悪い型定義を書いてしまうことがあります。
DefinitelyTypedにpull requestを送ってくれる人にもそういう人は多くいます。

これから説明するベストプラクティスを踏まえて、より良い型定義ファイルを作成できるように鍛錬していきましょう。

== 型定義ファイルのベストプラクティス

TBD

型定義ファイルを書く上でのベストプラクティスを解説していきます。
基本的には@<href>{http://definitelytyped.org/guides/best-practices.html,DefinitelyTypedのbest practives}と@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,公式のHandbookのWriting .d.ts files}に従っておけばよいです。
そこに書かれていることや筆者の経験則を交えて説明します。

TODO external module でも declaration merging について

=== テキトーに、やろー！

一番最初にコレを書くのもどうかと思うのですが、まずは"使える"ようにすることが一番大切です。

型定義ファイルの品質の良さにこだわるあまり、完成しない、つまり、使いたいライブラリが使えない、というのが一番よくない状態です。
最終的に、良し悪しが分かるようになるにはTypeScript自体の理解度に大きく依存します。
TypeScriptを書き始めの頃は、品質は気にするだけ無駄なのでまずは"使える"状態にすることを目指しましょう。

品質の他にも、"ライブラリ全体をカバーしている"かどうかなども気になるところではありますが、まずは使いたいところが使えればいいのです。
スゴいものになると、1万行超える型定義ファイルがあったり、3000行のはわりとごろごろしていたり…。そんなのを頑張って書いてると、余裕で日が暮れてしまいます@<fn>{atom-dts}。

まずは、使いたいところが、使える！
それで良いのです。
ドラゴン・ゲンドーソー＝先生のインストラクション・ワンを思い出しましょう。
「百発のスリケンで倒せぬ相手だからといって、一発の力に頼ってはならぬ。一千発のスリケンを投げるのだ！」
最初上手くできなくても、数をこなし学習を重ねれば、そのうちニンジャをも打倒しうる型定義ファイルが書けるようになるでしょう。

//footnote[atom-dts][なお、筆者はGitHubの作っているエディタ、Atomの型定義ファイルを3日かけて書いたことがあります。アレがジゴクだ。]

=== インタフェースを活用する

インタフェースは大変使いやすいパーツです。
というのも、インタフェースは@<strong>{後から定義を拡張できる}という特性があるからです(@<list>{declaration-merging}、@<list>{declaration-merging-usage})。

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

これこの通り、別々に定義したインタフェースが1つに統合されています。
これを利用することで、既存の型定義を拡張することができます。

例を1つ見てみましょう。
Array#find は、指定した方法に基づき要素を1つ探す関数です。
TypeScript 1.3.0ではデフォルトの型定義ファイル(lib.d.ts)にはまだこのメソッドが定義されていません。
そのため、Arrayインタフェースを拡張する形でコンパイルを通すことができるようになります@<list>{array-find}。

//list[array-find][Array#find を生やす]{
#@mapfile(../code/definition-file/array-find.ts)
// TypeScript 1.3.0 では Array.prototype.find の定義はまだ存在していない
interface Array<T> {
    find(callback: (element: T, index: number, array: T[]) => boolean, thisArg?: any): T;
}

var array = [1, 2, 3];

// 最初に見つかる奇数を得る
array.find(v => v % 2 === 1);
#@end
//}

あとは、実装側を@<href>{https://github.com/paulmillr/es6-shim/,es6-shim}などで埋めてやれば古いブラウザでも利用可能になるでしょう。

この手法は、他人が作った型定義ファイルを拡張する場合にも活用することができます。
全部自分で書いて、自分でメンテするのは、悲しいもんな…。

=== 幽霊モジュール

幽霊モジュール@<fn>{ghost-module}という考え方があります。

内部モジュールを作ったとしても、即座に実体が生成されるとは限りません。
内部モジュールが抱えるのがインタフェースのみである場合、実体がある扱いにはならないのです@<list>{ghost-module-invalid}。

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
@<list>{jquery-without-ghost-module}はjQueryの型定義ファイルの抜粋(&一部改変)です。

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
    then<U>(doneFilter: (value: T) => U, failFilter?: (...reasons: any[]) => U, progressFilter?: (...progression: any[]) => any): JQueryPromise<U>;
}

interface JQueryDeferred<T> extends JQueryPromise<T> {
    reject(...args: any[]): JQueryDeferred<T>;
    resolve(value?: T, ...args: any[]): JQueryDeferred<T>;
}

declare var $: JQueryStatic;
#@end
//}

トップレベルに複数の型がいくつも散乱してしまうのがよくありません。
それに、@<code>{JQuery}というprefixが連打していて、わかりにくいですね。
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
        then<U>(doneFilter: (value: T) => U, failFilter?: (...reasons: any[]) => U, progressFilter?: (...progression: any[]) => any): Promise<U>;
    }

    interface Deferred<T> extends Promise<T> {
        reject(...args: any[]): Deferred<T>;
        resolve(value?: T, ...args: any[]): Deferred<T>;
    }
}

declare var $: jquery.Static;
#@end
//}

インタフェース名が短く、かつわかりやすくなりました。
そうそう、こういうのでいいんだよこういうので！

もちろん、普通にクラスや変数、関数など実体が伴う定義が絡む場合はその限りではありません。
通常の実体のあるモジュールに相乗りしていってしまったほうが楽でしょう。

…なんでDefinitelyTyped上にある型定義ファイルでそうなってないものが多いかって？
良い質問です。一つ目は幽霊モジュールの認知度が低いこと、もう一つは型定義ファイルの大幅な書き換えは互換性の破壊を生み出すからです。
先で説明しましたが、インタフェースは定義の統合ができます。
この性質を利用して定義の拡張を行っているので、うかつに JQueryStatic から jquery.Static に変更すると色々なところが壊れてしまうのです。
特にjQueryプラグインとかはその方法で型定義しますからね。
魔窟ですよ…。

ともあれ、過去の定義との互換性を壊す事に繋がるため、途中から幽霊モジュールに切り替えるのは難しい場合があります。
可能であれば最初から幽霊モジュールを使うようにしましょう。
将来的には、このパターンの検出にはtslintなどで機械的に行えるようにしたいところですね。

//footnote[ghost-module][TypeScriptリファレンスでは非インスタンス化モジュールでした。DefnitelyTypedのbest practiceでghost moduleと表記されたのでそちらに統一。]

=== なんでもかんでもインタフェースにしてはならない

おうお前少し前であんだけインタフェースを持ち上げといてこれかぁ！？
と、思われたかと思いますが、なんでもかんでも使っていいわけねぇだルルォ！？

具体的に、モジュール様の構造をインタフェースを使って作ってはいけません(@<list>{module-by-interface-bad})。

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

この型定義ファイルを読み解いて一瞬で使える人は、元のJavaScriptコードを熟知している人だけでしょう。
少なくとも、この型定義ファイルをヒントに実際のコードを書くことは大いなる苦痛が伴います。
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
具体的には、@<list>{callable-module-usage}のように関数としても呼べるし、内部モジュールのようにも振る舞うオブジェクトです。

//list[callable-module-usage][関数・内部モジュール どっちなの？]{
#@mapfile(../code/definition-file/callable-module-usage-invalid.ts)
// 関数としても呼べるしモジュールにもなってる
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
}
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

たしかに、この定義でも動きます。
正直、これだけの定義だとこのままでもいい気がします。

ですが、もう一つ、別の良いやり方があるのです(@<list>{callable-module-good})。

//list[callable-module-good][関数と内部モジュール 両方やらなきゃいけないのが(ry]{
#@mapfile(../code/definition-file/callable-module-good.d.ts)
declare function assert(value: any): void;
declare module assert {
    function ok(value: any): void;
}
#@end
//}

関数と内部モジュールを同名で宣言できるのです。
これの効能は、層構造を素直に表現できることと、前項で説明した幽霊モジュールの書き方を併用できるところです。

この手法は、実際に@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/master/power-assert/power-assert.d.ts,power-assertの型定義ファイル}でも利用されています。
@<list>{power-assert-abst}に抜粋&改変したものを示します。

//list[power-assert-abst][関数と内部モジュール 両方やらなきゃいけないのが(ry]{
#@mapfile(../code/definition-file/power-assert-abst.d.ts)
declare function assert(value: any, message?: string): void;
declare module assert {

    export function deepEqual(actual: any, expected: any, message?: string): void;
    export function notDeepEqual(acutal: any, expected: any, message?: string): void;

    export interface Options {
        assertion?: any;
        output?: any;
    }

    export function customize(options: Options): typeof assert;
}
#@end
//}

外部に公開されている関数は@<code>{assert}のみで、そこに追加でプロパティが生えている形式です。
実体のある要素(関数)があるため、幽霊モジュールにはなりませんが、Optionsインタフェースが上手く取り込まれています。
余計な名前を階層の浅いところにバラ撒かず、厳密さも残っていません。
この書き方は、案外よく利用するパターンなので覚えておくと良いでしょう。

実は、このやり方は型定義ファイルだけではなく、通常のTypeScriptコードでも使えます(@<list>{callable-module-ts})。

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

コンパイル結果の@<list>{callable-module-js}を見ると、内部モジュールが先、関数が後、がダメな理由がわかりますね。

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

=== クラスを定義するには？

普通に定義すればええやろ！！と思うかもしれませんが、現在のTypeScriptにはなかなか難しい問題を抱えています。
先に、どういう選択肢が存在するかを見てみましょう(@<list>{declare-class})。

//list[declare-class][素直にクラス定義 vs インタフェース+変数]{
#@mapfile(../code/definition-file/declare-class.d.ts)
// A. 普通にクラスを定義する
declare class TestA { }

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
シビア。

==== 素直にクラスのメリット・デメリット

 * ライブラリ利用時に普通に継承できる
 * 定義の拡張ができない
 ** 別ライブラリが拡張する設計(プラグインとか)のライブラリには向かない
 * 別途インタフェースの実装を型定義に盛り込む時めんどくさい(定義の二重記述が必要)

@<list>{declare-vanilla-class-invalid}みたいな感じ。

//list[declare-vanilla-class-invalid][クラスで定義]{
#@mapfile(../code/definition-file/declare-vanilla-class-invalid.ts)
declare class BaseA {
    str: string;
}
// ○利用時に普通に継承できる！
class Inherit extends BaseA {
    number: number;
}
// ☓ クラスはopen endedじゃないので定義の拡張ができない…
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
 ** new するだけの利用法なら特に不便ではない
 * インタフェース定義の統合が使えるので別ライブラリの拡張にも対応できる！
 * インタフェースを実装するのが(継承するだけなので)めっちゃ簡単

@<list>{declare-decompose-class-invalid}みたいな感じ。

//list[declare-decompose-class-invalid][インタフェース+変数で定義]{
#@mapfile(../code/definition-file/declare-decompose-class-invalid.ts)
var BaseA: BaseAStatic;
interface BaseAStatic {
    new (): BaseA;
}
interface BaseA {
    str: string;
}
// ☓ 利用時に継承できない…(しょせんインタフェース
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

==== けつ☆論

クラスの型定義がopen endedになって定義を拡張可能にして、@<href>{https://github.com/Microsoft/TypeScript/issues/371,インタフェースのオプショナルな実装}サポートしてくれたらクラス定義で全部賄える！

今のところはどうしようもないので、用途に応じて適切なほうを選びましょう。

=== オーバーロードを上手く使おう！

正しいライブラリの使い方を導くこと。
を心に秘めて、@<list>{use-overload}を見るのじゃ。
どれが一番わかりやすいだろうか…？

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

JavaScriptのライブラリという奴は1つの関数に様々な使い方をさせようとします。
つまり、1つの関数が複数の顔を持つということです。
その顔1つ1つに個別の型定義を割り振ってやるためのテクニックとして、オーバーロードの存在を覚えておくとよいでしょう。

なお、普通にTypeScriptコードを書く時にオーバーロードはあまり使わん模様。

union typesが使えるようになると、@<list>{overload-vs-union-types}のように書くこともできます。
簡単な例だとunion typesのほうがよいと思いますが、このケースではどっちがいいかは、まだわからないですね。

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

#@# TODO http://goo.gl/LHRXAr なぜ全部OKじゃないのか？

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

ちなみに、それまでは内部モジュールとして拡張ポイントを@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b66f2750b10f262a698098cabbf210f7f2a/express/express.d.ts#L15,外出しして用意し}、@<href>{https://github.com/borisyankov/DefinitelyTyped/blob/e3b19b66f2750b10f262a698098cabbf210f7f2a/passport/passport.d.ts#L8,他所で拡張する}という頑張り方をしていました。
泣けますね。
既存ライブラリにメソッド生やすという黒魔術はNode.js上でもできてしまうのが辛いところです。
とはいえ、1.3.0移行は多少楽になったのが嬉しいところです。

=== ドキュメントから書き起こす

もし、ライブラリにしっかりしたドキュメントがあるのであれば、実装コードから型定義ファイルを起こすのではなく、ドキュメントをベースに作成しましょう。
Visual StudioなどのIDEでは、型定義ファイル上に書かれたJSDocコメントも利用時に表示してくれる場合があるため、あわせて記述していくとなおよいでしょう。
少なくとも、ドキュメント中に書かれている利用例がコンパイルに通らないような型定義ファイルは悪い型定義ファイルであるといえます。
サンプルをテスト用コードとしてTypeScriptコードに移植し、ドキュメント通りの記述が可能かも確かめるとよいでしょう。

とはいえ、世の中ドキュメントにコストをあまり書けることのできないプロジェクトも多くあります。
その場合、コードから型定義ファイルを起こすことになるのは仕方のないことです。

なお、DefinitelyTypedのjqueryの型定義ファイルを熱心に面倒みてくれている John Reilly は特にドキュメントとの整合性を熱心に見るので、もしjQueryのドキュメント自体が間違っている場合はjQueryのドキュメントを治すところから始めるとよいでしょう。
コントリビュートの輪！

=== その他なんか

TBD ガイドライン見て書き足して、どうぞ。

=== 最終チェック！

TBD
tslintと--noImplicitAnyについては必ず書く

== Let's contribution!

TBD
品質の話
全体カバー率の話

=== 郷に入るには郷に従うのが楽

TBD
http://definitelytyped.org/guides/contributing.html
互換性 & 一貫性を保つ

=== pull requestのレビュー実施チャート

TBD
