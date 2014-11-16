= 型定義ファイルとは

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

TBD

== 型定義ファイルのベストプラクティス

TBD

型定義ファイルを書く上でのベストプラクティスを解説していきます。
基本的には@<href>{http://definitelytyped.org/guides/best-practices.html,DefinitelyTypedのbest practives}と@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,公式のHandbookのWriting .d.ts files}に従っておけばよいです。
そこに書かれていることや筆者の経験則を交えて説明します。

TODO external module でも declaration merging について

=== テキトーに、やろー！

一番最初にコレを書くのもどうかと思うのですが、まずは"使える"ようにすることが一番大切です。

型定義ファイルにも完成度というか、品質の良い・悪いがあります。
ですが、品質の良さにこだわるあまり、完成しない、つまり、使いたいライブラリが使えない、というのが一番よくない状態です。
最終的に、良し悪しが分かるようになるにはTypeScript自体の理解度に大きく依存します。
TypeScriptを書き始めの頃は、品質は気にするだけ無駄なのでまずは"使える"状態にすることを目指しましょう。

品質の他にも、"ライブラリ全体をカバーしている"かどうかなども気になるところではありますが、まずは使いたいところが使えればいいのです。
スゴいものになると、1万行超える型定義ファイルがあったり、3000行のはわりとごろごろしていたり…。そんなのを頑張って書いてると、余裕で日が暮れてしまいます@<fn>{atom-dts}。

まずは、使いたいところが、使える！
それで良いのです。

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

=== なんでもかんでもインタフェースにしてはならない

おうお前一個前であんだけ書いといていきなりこれかぁ！？
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
少なくとも、この型定義ファイルをヒントに実際のコードを書くことは大いなる苦痛が伴うでしょう。
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

呼び出し可能で、プロパティを持つ。ふむ、じゃあ@<list>{callable-module-bad}だ！

//list[callable-module-bad][こうしてしまいたい、気持ち]{
#@mapfile(../code/definition-file/callable-module-bad.d.ts)
declare var assert: Assert;

interface Assert {
    (value: any): void;
    ok(value: any): void;
}
#@end
//}

正直、これだけの定義だとこのままでもいい気がします。
ですが、もう一つ、別のやり方があるのです(@<list>{callable-module-good})。

//list[callable-module-good][関数と内部モジュール 両方やらなきゃいけないのが(ry]{
#@mapfile(../code/definition-file/callable-module-good.d.ts)
declare function assert(value: any): void;
declare module assert {
    function ok(value: any): void;
}
#@end
//}

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

=== 幽霊モジュール

TBD

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
