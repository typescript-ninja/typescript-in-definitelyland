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

TBD

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

TBD

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
