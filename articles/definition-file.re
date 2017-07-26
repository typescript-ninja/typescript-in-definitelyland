={definition-file} JavaScriptの資産と型定義ファイル

#@# REVIEW muo: 6章けっこう長めなので章として切断できないかな。
#@# REVIEW muo: 基礎編・応用編と分けるのはむずかしいかもしれないけど、「このへんまで頑張ったら一息つける」「ここまでやってくれる人はほめてあげよう」ぐらいに分かれていると、トピック拾いやすいかもしらん

=={use-js-assets} JavaScriptの資産が使いたい

TypeScriptはJavaScriptの上位互換であり、JavaScriptを置き換えるものです。
とはいえ、現時点ではWebアプリの世界はJavaScriptで成り立っていますし、すでに莫大な資産があります。
それらを放り出してしまうのはあまりにもったいないので、TypeScriptでも活用したいものです。
そのためにTypeScriptは既存のJavaScript用資産を活用するための仕組みを持っています。
それが、型定義ファイルです。

TypeScriptコードの拡張子.tsに対して、型定義ファイルは拡張子を.d.tsとします。
拡張子を.d.tsとしたファイルに実装を含むコードを書くとtscがエラーにするので、ケアレスミス予防のためにも型定義ファイルの拡張子は必ず.d.tsにします。

TypeScriptでは、JavaScriptの自由奔放（かつ、危険がてんこ盛り）の世界に後付で型を与えます。
もとからTypeScriptで書かれている場合、実装と型定義を同時に書いているためこのふたつがズレて（つまりバグって）しまうことはありません。
一方、型定義ファイルはすでに実装があるJavaScriptに後付かつ手書きで型をつけていくため、ズレる（バグる）可能性が大いに有ります。
この可能性に十分気をつけないといけません。

#@# @suppress ParagraphNumber SectionLength
=={use-at-types} @typesを使う

まずは自分で型定義ファイルを作るよりも、既存のものを使ってみましょう。
Node.jsやlodashなどの有名どころはひととおり揃っています。

ライブラリの作者がTypeScriptユーザで、npm package自体に型定義ファイルがバンドルされていて何も考えずにTypeScriptから使える場合も増えてきました@<fn>{definition-bundled}。
ですが、現時点では一般的にはDefinitelyTyped@<fn>{definitelytyped}というコミュニティベースの型定義ファイル集積リポジトリを（間接的に）利用することになるでしょう。

//footnote[definition-bundled][たとえばAngularやRxJSなど]
//footnote[definitelytyped][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped}]

#@# @suppress JapaneseStyle
DefinitelyTypedから型定義ファイルをダウンロードしてくるための方法は複数用意されています。
TypeScript 2.0.0からは@typesというnpmのscoped package@<fn>{scoped-package}を使って型定義ファイルを利用します。
2.0.0以前ではtsd@<fn>{tsd}やdtsm@<fn>{dtsm}やtypings@<fn>{typings}というツールを使っていましたが、これらは不要になりました。
もし、ここで紹介する方法でうまくいかない場合、利用事例やブログ記事などが出回っている旧ツール群のいずれかを使ってみるとよいでしょう。

//footnote[scoped-package][@xxx/ から始まる名前空間が区切られたnpm packageのこと @<href>{https://docs.npmjs.com/misc/scope}]
//footnote[tsd][@<href>{https://www.npmjs.com/package/tsd}]
//footnote[dtsm][@<href>{https://www.npmjs.com/package/dtsm}]
//footnote[typings][@<href>{https://www.npmjs.com/package/typings}]

さて、前置きが長くなりましたが実際に型定義ファイルをダウンロードしてきて使ってみましょう。
ここではテストで使う便利ライブラリ、power-assertを題材にして型定義ファイルをダウンロードしてみます。

#@# TODO power-assertは直接参照されたく思想になっているはずなので別の例を探したい

//cmd{
# 型定義ファイルをinstall
$ npm install --save-dev @types/power-assert
+ @types/power-assert@1.4.29
added 3 packages in 2.706s
//}
#@# TODO power-assertの型定義ファイルに記載のライブラリ本体のバージョン更新しないと…

power-assertの型定義ファイルが依存しているモジュールの型定義も芋づる式に取得できています。
便利ですね。
型定義ファイルのパッケージには残念ながらライブラリの実体は含まれていないため@<code>{npm install power-assert}で別途インストールする必要があります。

既存ライブラリに対する型定義ファイルは@types/の下に元ライブラリのパッケージ名と同じ名前で公開される運用です。
パッケージの検索は@<href>{https://microsoft.github.io/TypeSearch/,TypeSearch}@<fn>{typesearch}で行うか、npm searchを使うとよいでしょう。

//footnote[typesearch][@<href>{https://microsoft.github.io/TypeSearch/}]

また、@typesで導入した型定義ファイルの検索は、モジュールの解決方法（@<code>{--moduleResolution}）がnodeのときのみ行われます@<fn>{issue9831}。
AMDなどを利用したい場合も@<code>{--moduleResolution node}を指定するようにしましょう。

//footnote[issue9831][@<href>{https://github.com/Microsoft/TypeScript/issues/9831}]

注意点として、npmのscopedパッケージを扱う場合、@<code>{vvakame/foobar}に対する型定義ファイルがほしいケースでは@<code>{@types/vvakame__foobar}パッケージを参照するネーミングルールになっています。

=={use-definition-files} 型定義ファイルを参照してみよう

型定義ファイルを参照するには、tscコマンドでコンパイル対象に含める必要があります。
node_modules/@typesにある型定義ファイルは特別扱いされ、モジュールをimportした時や、tsconfig.jsonのtypesに記述したモジュールの解決時に自動的に走査されます。
要するにnpm installしたら、後は何も気にしなくてもTypeScriptコンパイラが型定義ファイルを探しだしてきてくれるのです。

古くはリファレンスコメントとして、ソースコードの先頭に@<code>{/// <reference path="相対パスor絶対パス" />}の形式で書く方法もありましたがtsconfig.jsonの登場により廃れました。
基本として依存性の解決はtsconfig.jsonで行うようにします。

mocha＋power-assertでテストを書く場合を例に、使い方を解説していきます。

テスト対象のコードは@<code>{./lib/index.ts}です（@<list>{usage/lib/index.ts}）。

//list[usage/lib/index.ts][至って普通のモジュール]{
#@mapfile(../code/definition-file/usage/lib/index.ts)
export function hello(word = "TypeScript") {
  return `Hello, ${word}`;
}
#@end
//}

これに対してテストコードとして@<code>{./test/indexSpec.ts}を書いてみましょう（@<list>{usage/test/indexSpec.ts}）。

//list[usage/test/indexSpec.ts][mocha+power-assertでテストを書く]{
#@mapfile(../code/definition-file/usage/test/indexSpec.ts)
import * as assert from "power-assert";

import { hello } from "../lib/";

describe("lib", () => {
  describe("hello function", () => {
    it("generate string with default value", () => {
      let str = hello();
      assert(str === "Hello, TypeScript");
    });
    it("generate string with parameter", () => {
      let str = hello("JavaScript");
      assert(str === "Hello, JavaScript");
    });
  });
});
#@end
//}

普通ですね。「特定のinputを与えるとそれに応じたoutputが得られる」ことを検証するコードです。

ここで問題なのは、TypeScriptコンパイラが安全にコードを処理するためには、mochaとpower-assertについての情報が必要であることです。
たとえば、assert関数はpower-assertが提供するものですし、describeとit関数はmochaが提供しています。
JavaScriptの世界では静的な型検査などもちろんありません。
そこをTypeScriptと組み合わせたときにどう解決すればよいか、コンパイラに教えてあげる必要があります。
このようなときが型定義ファイルの出番です。

#@# @suppress ParenthesizedSentence
mocha（@<list>{usage/abstract/mocha.d.ts}）とpower-assert（@<list>{usage/abstract/power-assert.d.ts}）の型定義ファイルを抜粋・簡略化したものを見てみましょう。

//list[usage/abstract/mocha.d.ts][mocha.d.ts抜粋]{
#@mapfile(../code/definition-file/usageAbstract/mocha.d.ts)
interface MochaDone {
  (error?: any): void;
}
declare let describe: {
  (description: string, callback: () => void): any;
};
declare let it: {
  (expectation: string, callback?: () => void): any;
  (expectation: string, callback?: (done: MochaDone) => void): any;
};
#@end
//}

//list[usage/abstract/power-assert.d.ts][power-assert.d.ts抜粋]{
#@mapfile(../code/definition-file/usageAbstract/power-assert.d.ts)
export = assert;
export as namespace assert;

declare function assert(value: any, message?: string): void;
#@end
//}

型定義ファイルを見るとmochaとpower-assertそれぞれのAPIが表現されています。
TypeScriptコンパイラがこれらの型定義ファイルを認識できれば、矛盾なくコンパイルを通すことができそうです。
そのためのpackage.json（@<list>{usage/package.json}）とtsconfig.json（@<list>{usage/tsconfig.json}）を確認します。

//list[usage/package.json][package.json]{
#@mapfile(../code/definition-file/usage/package.json)
{
  "name": "typescript-in-definitelyland-sample",
  "private": true,
  "version": "1.0.0",
  "main": "lib/index.js",
  "scripts": {
    "build": "tsc",
    "pretest": "npm run build",
    "test": "mocha"
  },
  "author": "vvakame",
  "license": "MIT",
  "devDependencies": {
    "@types/mocha": "^2.2.28",
    "@types/power-assert": "^1.4.29",
    "mocha": "^3.4.2",
    "power-assert": "^1.4.1"
  }
}
#@end
//}

//list[usage/tsconfig.json][tsconfig.json]{
#@mapfile(../code/definition-file/usage/tsconfig.json)
{
    "compilerOptions": {
        "module": "commonjs",
        "target": "es5",
        "strict": true,
        "types": [
            "mocha"
        ]
    },
    "exclude": [
        "node_modules"
    ]
}
#@end
//}

#@# @suppress JapaneseStyle
power-assertはテストコード中でimportしますが、テストランナーであるmochaの定義はソースコード中からの参照がありません。
そのため、power-assertについてはTypeScriptコンパイラが必要であることを判別し、型定義ファイルを探しにいってくれます。

しかしmochaはそのような機会がないため、TypeScriptコンパイラは型定義を探しにいってくれません。
このままコンパイルするとmochaがグローバルに値を展開しているdescribeやitなどが見つからないといわれてしまいます。
これを解決するためにtsconfig.jsonのtypesプロパティ中でmochaを参照するよう指定します。

あわせて型定義ファイルへの参照が意図どおり処理されずに困った場合のデバッグ方法を紹介しておきます。
コンパイルに利用したファイルをリスト表示する@<code>{--listFiles}オプションと、型定義ファイル探索の様子を表示する@<code>{--traceResolution}オプションを試してみてください。

#@# TODO できればmochaの型定義バージョンあげたい
なお、DefinitelyTypedは規約上、実装本体のパッケージと型定義ファイルのバージョンのmajorとminorが一致します。
本書の例のようにmochaの実装と型定義のメジャーバージョンすらずれているのは本来はあまりよくありません。

#@# mhidaka TODO @typesをつかって使うはなしがここまでで、型定義を書くはなしがここから下で大分性質がことなるので章の分離があってもいいかもしれない。

=={writing-dts-files} 型定義ファイルを書こう

さて型定義ファイルの取得方法、使い方はわかりました。
しかし、世の中にあるJavaScriptライブラリのうち、型定義ファイルが書かれていないものはまだまだあります。
特に、門外不出の社内ライブラリなどは誰も手をつけて（いるわけが）ない前人未到の地です。

#@# @suppress InvalidExpression
しからば！自分で書くしかあるまいよ！
ぶっちゃけた話、めんどくさいのですが、後々の安心・安全を得るための投資として割りきりましょう。

なお、自分で型定義ファイルを1行も書かずにTypeScriptをやると、どこかの時点で不便に感じるでしょう。
最初のうちは無理かもしれませんが、まずは人が書いた型定義ファイルを読んで知識を蓄え、この世界に入ってきてくれると嬉しいです。

==={types-and-values} 型、実体、そして42。

TypeScriptはJavaScriptに対して後付で型による制約を付け足した言語です。
そのため、JavaやC#のような最初から型ありきの言語より少し考え方が複雑です。
具体的にいえば型と実体（値）というものが分かれています。

すべてがTypeScriptで書かれたプログラムであれば、型と実体は基本的には一致しています。
クラスの定義を書いたとき、JavaScriptプログラムとしてのクラスと、TypeScriptで使う型としてのクラスが一度に誕生します。
これは大変素直かつ簡単な動作で、ひとつの記述から型と実体を作成しているためこの2つが乖離してしまうことはありません。

#@# @suppress CommaNumber
一方、JavaScriptでコードを書いてTypeScriptで型定義ファイルを作成して使う場合、実装と型が個別に定義されることになります。
そのため、型と実体が分離してしまい、この2つの間に乖離が生じると（つまりバグると）コンパイルが通るのに実行時エラーが多発する、というありさまになるわけです。
型定義ファイルを書いて"この変数は、あります！"と宣言したけれど、実際には存在せず実行時エラーになるというのは広く使われている型定義ファイルですらままある話です。
これはもうどうしようもない話ですので、我慢するか自分で修正するしかありません。
全世界がTypeScriptに制覇されることによりこの葛藤は解消される予定です。
#@# OK mhidaka このような不均衡な状態とどうやってつきあっていってるの？という一言があるといいかもしれない。基本は我慢？

==={good-definitions-bad-definitions} 良い型定義ファイル、悪い型定義ファイル

型定義ファイルにも良し悪しがあります。
その基準は至って簡単です。

 1. 正しいライブラリの使い方を導くこと
 2. 他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと
 3. IDE上で使いやすいこと

正しいライブラリの使い方を導く、というのは裏を返せば間違った使い方ができないようにする、ということです。
これには型と実体の定義に乖離が存在せず、コンパイルが通ったら実行時エラーが簡単には起こらないことも含まれます。

他のコードや型定義ファイルに意図せぬ干渉を引き起こさないこと、というのは意図せぬインタフェースの統合などが起こらないことを指します。
このためには汎用的な名前を使うのを避け、可読性が高く理解しやすい型定義を作り、干渉した場合に容易に判断できるようにすることも含まれます。

IDE上で使いやすいことというのは、Visual Studio Codeなどでコードを書く上で入力補完の候補が不用意に出過ぎないようにして見通しのよい開発を助けることなどが含まれます。

#@# @suppress JapaneseStyle
#@# prh:disable
これら3つを守ることが"良い品質であること"に繋がるというのは、TypeScript自体が型指定を行うことで間違ったコードを書きにくいようにするツールであると考えると納得がいくでしょう。

慣れないうちはどうしても"うまく書けないので仕方なく"悪い型定義を書いてしまうことがあります。
DefinitelyTypedにpull requestを送ってくれる人にもそういう人は多くいます。

#@# prh:disable
これから説明するベストプラクティスを踏まえて、より良い型定義ファイルを作成できるように鍛錬していきましょう。

#@# @suppress ParagraphNumber SectionLength
=={writing-instruction} 型定義ファイルを書くための心得

#@# @suppress SentenceLength JapaneseStyle
型定義ファイルを書く上でのベストプラクティスを解説していきます。
基本的には公式Handbookの@<href>{http://www.typescriptlang.org/docs/handbook/writing-declaration-files.html,Writing Declaration Files}@<fn>{official-handbook}とDefinitelyTypedの@<href>{http://definitelytyped.org/guides/best-practices.html,best practices}@<fn>{dt-best-practice}にしたがっておけばよいです。
本章では、そこに書かれていることや筆者の経験則などを説明していきます。

//footnote[official-handbook][@<href>{http://www.typescriptlang.org/docs/handbook/writing-declaration-files.html}]
//footnote[dt-best-practice][@<href>{http://definitelytyped.org/guides/best-practices.html}]

==={silly-go-luck} テキトーに、やろー！

一番最初にコレを書くのもどうかと思うのですが、まずは"使える"ようにするのが一番大切です。

型定義ファイルの品質の良さにこだわるあまり、完成しない、使いたいライブラリが使えない、というのがもっともよくない状態です。
型定義ファイルの良し悪しを判断する力は、TypeScript自体への理解度に大きく依存します。
TypeScriptを書き始めたばかりに作ったものは上達するにつけ後々粗が見えてくるのは避けられません。
つまり、避けられないことを気にしたってしかたないよな！
まずは"使える"状態にすることを目指しましょう。

品質や"ライブラリ全体をカバーしている"かは気になるところではあります。
しかし、まずは使いたいところが使えればいいのです。
スゴいものになると、1万行を超える型定義ファイルがあります。
また3000行程度のものはわりとごろごろしています…。
しかし、そんなにも頑張って書いてると余裕で日が暮れてしまいます@<fn>{atom-dts}。

//footnote[atom-dts][なお筆者はGitHubの作っているエディタ、Atomの型定義ファイルを3日かけて書いたことがあります。アレがジゴクだ]

まずは、使いたいところが、使える！
それでよいのです。
ドラゴン・ゲンドーソー＝先生のインストラクション・ワンを思い出しましょう。

//quote{
百発のスリケンで倒せぬ相手だからといって、一発の力に頼ってはならぬ。一千発のスリケンを投げるのだ！
//}

最初はうまくできなくても数をこなし学習を重ねれば、そのうち立派な型定義ファイルを書けるようになるでしょう。

==={done-is-better-than-perfect} 雑な型定義でやりすごす

はい。まずは気軽にスタートする方法から入っていきましょう。
何も考えずにスタートするとanyが至るところに流入してくるので少しずつ定義を育てていき、立派な安全さを得ていきましょう。

#@# 型付けなしの気軽なimport句 Untyped (implicit-any) imports in 2.1.4
まず一番簡単なのは@<code>{--noImplicitAny}オプションが指定されていない場合に限り、型定義のないモジュールをimportするとエラーにならずにanyとして参照できるというものです。
これは簡単に始められますが@<code>{--noImplicitAny}を使わないというのがそもそも厳しいため、書き捨てのスクリプトを作るときくらいしか出番が無さそうです。

#@# 雑な型定義の作成法の追加とワイルドカード（Shorthand ambient module declarations and wildcard matching in module names） in 2.0Beta
次に使える手法として、モジュールの型定義を簡略表記する方法とワイルドカードがあります。
定義の例@<list>{shorthand/module.d.ts}とそれを使う例@<list>{shorthand/usage.ts}を確認してみます。

//list[shorthand/module.d.ts][定義の例 名前を書くだけ]{
#@mapfile(../code/definition-file/shorthand/module.d.ts)
declare module "jquery";

// こういう定義と同等
declare module "jquery-alt" {
  var _temp: any;
  export = _temp;
}

// ワイルドカードも使える
declare module "json!*";
declare module "sample/*";
#@end
//}

//list[shorthand/usage.ts][利用例 とりあえず使える]{
#@mapfile(../code/definition-file/shorthand/usage.ts)
import * as $ from "jquery";

// $ はany
$.notExists();

// これらもコンパイルが通る
import * as json from "json!package.json";
import * as sampleFoo from "sample/foo";
import * as sampleFooBar from "sample/foo/bar";

export { $, json, sampleFoo, sampleFooBar }
#@end
//}

簡単ですね！
とりあえず、開発を始めることができます。

#@# @suppress ParagraphNumber SectionLength
==={interface-the-best-friend} インタフェースを活用する

#@# @suppress SentenceLength ParenthesizedSentence
インタフェースは大変使いやすいパーツです。
というのも、インタフェースには@<strong>{後から定義を拡張できる}という特性があるからです（@<list>{interface/declarationMerging.d.ts}、@<list>{interface/declarationMergingUsage.ts}）@<fn>{open-ended-class}。

//footnote[open-ended-class][ちなみに、classの定義も後から拡張可能です @<href>{https://github.com/Microsoft/TypeScript/issues/3332}]

//list[interface/declarationMerging.d.ts][定義を分割して書く]{
#@mapfile(../code/definition-file/interface/declarationMerging.d.ts)
interface Foo {
  hello(): string;
}

// 同名のインタフェースを定義すると、合成される！
interface Foo {
  bye(): string;
}
#@end
//}

//list[interface/declarationMergingUsage.ts][定義が統合される！]{
#@mapfile(../code/definition-file/interface/declarationMergingUsage.ts)
/// <reference path="./declarationMerging.d.ts" />
// ↑ 昔はこのようにreference commentを使ってファイル間の依存関係を明示していましたが、
//   最近はtsconfig.jsonに依存関係を書くため見かけることはほぼなくなりました

let foo: Foo = null as any;

foo.hello();
foo.bye();

export { }
#@end
//}

このとおり別々に定義したインタフェースがひとつに統合されています。
これを利用することで、既存の型であろうとも拡張が可能になるのです。

例をひとつ見てみましょう。
@<code>{String#trimStart}は、文字列の先頭にある空白文字を取り除く機能です。
本章執筆時点（2017年07月16日）では、この提案@<fn>{string-trimStart}はTC39のプロポーザルでstage 2@<fn>{tc39-proposal}で、TypeScriptにはまだ入ってきていません。
そのためStringインタフェースを拡張する形でコンパイルを通せるようにしてみましょう（@<list>{interface/stringTrimStart.ts}）

#@# prh:disable
//footnote[string-trimStart][@<href>{https://github.com/sebmarkbage/ecmascript-string-left-right-trim}]
//footnote[tc39-proposal][@<href>{https://tc39.github.io/process-document/}]

//list[interface/stringTrimStart.ts][String#trimStartを生やす]{
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

#@# OK mhidaka めっちゃかんたんやん！って驚く例で大変良い

==={ghost-namespace} 幽霊namespace

幽霊namespace@<fn>{ghost-module}という考え方があります。

//footnote[ghost-module][TypeScriptリファレンスでは非インスタンス化モジュールという名前で紹介しました。その後、DefinitelyTypedのbest practicesでghost moduleと表記された]

namespaceを作ったとしても、即座に実体が生成されるとは限りません。
namespaceが抱えるものがインタフェースのみの場合、実体がある扱いにはならないのです（@<list>{ghostModule/invalid.ts}）。

//list[ghostModule/invalid.ts][幽霊namespace]{
#@mapfile(../code/definition-file/ghostModule/invalid.ts)
declare namespace ghost {
  interface Test {
    str: string;
  }
}

// 型としてはしっかり存在していてアクセスできる
let test: ghost.Test;
test.str;

// 実体としては存在していない！
// invalid.ts(13,17): error TS2304: Cannot find name 'ghost'.
let notExists = ghost;

export { }
#@end
//}

これを活用して大量のインタフェースをもつようなライブラリの定義をひとまとまりにできます。

実際の例を見てみましょう。
@<list>{ghostModule/jqueryWithoutGhostModule-ignore.d.ts}はjQueryの型定義ファイルからの抜粋（および一部改変）です。

#@# TODO 最新の例に更新したいところ
//list[ghostModule/jqueryWithoutGhostModule-ignore.d.ts][実際のjQueryの型定義の例]{
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

トップレベルに複数の型がいくつも散乱してしまう点がよくありません。
それに@<code>{JQuery}というprefixが乱舞していて目を惑わせます。
ライブラリ内部でAPI同士が参照する場合でも引数や返り値にプリフィクスが必要だと面倒です。
IDE上で型注釈を手書きするときも候補がたくさんサジェストされてしまうことでしょう。

#@# @suppress ParenthesizedSentence
これを幽霊namespaceを使って書きなおしてみます（@<list>{ghostModule/jqueryWithGhostModule-ignore.d.ts}）。

//list[ghostModule/jqueryWithGhostModule-ignore.d.ts][幽霊namespaceを使ってみた]{
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
やっぱり、シンプルなほうがいいですね。

もちろん、無理に幽霊namespaceを使う必要はありません。
クラスや変数や関数などを持ち、通常の実体をもつnamespaceが存在している場合は、そのnamespaceに相乗りしてしまったほうが楽でしょう。

#@# @suppress JapaneseStyle
#@# prh:disable
どうしてDefinitelyTyped上にある型定義ファイルで幽霊namespaceを使っていないものが多いのかって？
よい質問です。
ひとつは幽霊namespaceの認知度が低いこと、もうひとつは型定義ファイルの大幅な書き換えは互換性の破壊を生み出すからです。
先で説明しましたが、インタフェースは定義の統合ができます。
この性質を利用して定義の拡張を行っているので、うかつにJQueryStaticからjquery.Staticに型名を変更するとjQueryの型定義に依存しているさまざまなライブラリの色々なところが壊れてしまいます。
特にjQueryプラグインはインタフェースを拡張する形で型定義するのでその量たるや…。

ともあれ、過去の定義との互換性を壊すことに繋がるため、途中から幽霊namespaceに切り替える選択は難しくなります。
可能であれば最初から幽霊namespaceを使うようにしましょう。
将来的には、このパターンの検出はtslintなどで機械的に行えるようにしたいところですね。

#@# @suppress JapaneseStyle ParagraphNumber SectionLength
==={interface-is-not-duct-tape} インタフェースは万能ではない

少し前の文章であんだけインタフェースを持ち上げといてこれかぁ！？
と思われたかもしれませんが、なんでもかんでも乱用すればいいってものではありません。

#@# @suppress ParenthesizedSentence
具体的にはnamespace的な構造をインタフェースを使って作ってはいけません（@<list>{interfaceAntipattern/moduleByInterfaceBad-ignore.d.ts}）。

//list[interfaceAntipattern/moduleByInterfaceBad-ignore.d.ts][namespaceを使えばいいのにインタフェースで表現してしまう。何故なのか…]{
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
素直に@<list>{interfaceAntipattern/moduleByInterfaceGood-ignore.d.ts}のように書きましょう。

//list[interfaceAntipattern/moduleByInterfaceGood-ignore.d.ts][素直にこうしよう]{
#@mapfile(../code/definition-file/interfaceAntipattern/moduleByInterfaceGood-ignore.d.ts)
// 素直にこうしよう！
declare namespace foo.bar.buzz {
  let str: string;
}
#@end
//}

通常@<list>{interfaceAntipattern/callableModuleUsage-ignore.ts}のような型定義ファイルを書こうとはしませんが、このようなコードが必要になる場合が稀にあります。
関数としても呼べるし、namespaceのようにも振る舞うオブジェクトの型定義を作成したいときです。

//list[interfaceAntipattern/callableModuleUsage-ignore.ts][関数・namespace どっちなの？]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleUsage-ignore.ts)
// assertは関数としても呼べるしnamespaceのようにも見える
assert(foo === "foo");
assert.ok(value);
#@end
//}

#@# @suppress SentenceLength
呼び出し可能で、プロパティをもつ場合、すぐに考えつく型定義は@<list>{interfaceAntipattern/callableModuleBad1-ignore.d.ts}か、@<list>{interfaceAntipattern/callableModuleBad2-ignore.d.ts}でしょう。

//list[interfaceAntipattern/callableModuleBad1-ignore.d.ts][こうしてしまいたい、気持ち]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleBad1-ignore.d.ts)
declare var assert: {
  (value: any): void;
  ok(value: any): void;
};
#@end
//}

//list[interfaceAntipattern/callableModuleBad2-ignore.d.ts][匿名型注釈よりはマシ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleBad2-ignore.d.ts)
declare var assert: Assert;

interface Assert {
  (value: any): void;
  ok(value: any): void;
}
#@end
//}

#@# @suppress ParenthesizedSentence
たしかに、この定義でも動きます。
正直なところassert関数だけの定義だとこのままでもいい気がしますが。

しかし、これには別のよいやり方があるのです（@<list>{interfaceAntipattern/callableModuleGood-ignore.d.ts}）。

//list[interfaceAntipattern/callableModuleGood-ignore.d.ts][関数とnamespace、両方やらなきゃいけないのが辛いところだ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModuleGood-ignore.d.ts)
declare function assert(value: any): void;
declare namespace assert {
  function ok(value: any): void;
}
#@end
//}

関数とnamespaceを同名で宣言できる手法です。
メリットは階層構造を素直に表現できることと、前項で説明した幽霊namespaceの書き方を併用できるところです。

#@# @suppress SentenceLength
この手法は、実際に@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/tree/master/types/power-assert,power-assertの型定義ファイル}@<fn>{power-assert-dts}でも利用されています。
@<list>{interfaceAntipattern/powerAssertAbst-ignore.d.ts}に抜粋および改変したものを示します。

//footnote[power-assert-dts][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/power-assert/}]

//list[interfaceAntipattern/powerAssertAbst-ignore.d.ts][関数+namespaceの実例]{
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

外部に公開されている関数は@<code>{assert}のみで、そこにプロパティを追加している形式です。
namespaceにOptionsインタフェースがうまく取り込まれています。
余計な名前を階層の浅いところにバラ撒かず、厳密さも損なっていません。
この書き方は、よく登場するパターンなので覚えておきましょう。

実は、この手法は型定義ファイルだけではなく通常のTypeScriptコードでも使えます（@<list>{interfaceAntipattern/callableModule.ts}）。

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

コンパイル結果の@<list>{interfaceAntipattern/callableModule.js}を見ると、なぜ関数が先でnamespaceが後、という決まりになっているかがわかります。

//list[interfaceAntipattern/callableModule.js][JSとして正しい構造だ]{
#@mapfile(../code/definition-file/interfaceAntipattern/callableModule.js)
"use strict";
function test() {
    return "test!";
}
(function (test) {
    function func() {
        return "function!";
    }
    test.func = func;
})(test || (test = {}));
#@end
//}

==={class-definitions} クラスはクラスとして定義する

クラスの型定義を行う方法を解説します。
歴史的経緯により、TypeScriptではクラスの型定義を行う時に2つの代表的なやり方が存在しています。
まずはその2つのやり方を見てみましょう（@<list>{declareClass/basic.d.ts}）。

//list[declareClass/basic.d.ts][素直にクラス定義 vs インタフェース+変数]{
#@mapfile(../code/definition-file/declareClass/basic.d.ts)
// A. クラスを定義する
declare class TestA {
}

// B. クラスの分解定義 変数 + インタフェース2つ
declare let TestB: TestBConstructor;
interface TestBConstructor {
  new(): TestB;
}
interface TestB {
}
#@end
//}

こんな感じです。
クラス定義をするほうが素直ですね。

過去にはこの2つのやり方にそれぞれメリット・デメリットがありました。
しかし、現在のTypeScriptでは大幅に制限が緩和されたためメリット・デメリットの面で考える必要はなくなってきました（@<list>{declareClass/stretch.ts}）。
よい時代になったものです。

//list[declareClass/stretch.ts][相互運用性がある！]{
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
  new(): Animal;
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
// Animalはただの変数だが問題なく継承できる！
class Cat extends Animal {
  speak() {
    return "meow";
  }
}
let cat: Cat = new Cat();
console.log(cat.speak());

export { }
#@end
//}

#@# OK REVIEW lc: s/以下のような分/以下のような文/

==={pretty-good-overload} オーバーロードをうまく使おう！

#@# @suppress JapaneseAmbiguousNounConjunction
正しいライブラリの使い方を導く気持ちを心に秘めて、@<list>{overload/useOverload.ts}を見てください。

質問：どれが一番、元々の関数の仕様がわかりやすいですか？

//list[overload/useOverload.ts][書き方あれこれ]{
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

答え：一番最初の方法。

JavaScriptのライブラリは1つの関数にさまざまな使い方をさせようとする場合がままあります。
つまり、1つの関数が複数の顔をもつということです。
その顔ひとつひとつに個別の型定義を割り振ってやるテクニックをオーバーロードと呼びます。

なおTypeScriptコードを書くときはオーバーロードをあまり使わないほうが良いスタイルです。
実装が煩雑になってしまいますからね。
素直にメソッドを分けましょう。

union typesを使うと@<list>{overload/overloadVsUnionTypes.ts}のように書くこともできます。
簡単な例だとunion typesのほうがよいと思いますが、見た目が煩雑になるケースではどっちがいいかは判断が分かれるところです。

//list[overload/overloadVsUnionTypes.ts][うーん、どっちがいいかは難しい]{
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

#@# @suppress ParenthesizedSentence
もう一例見てみます（@<list>{definition-file/overload/overloadFault-invalid.ts}）。
union typesとoverloadの両方が選択肢に入る場合、現時点ではunion typesを選んだほうがよい場合があります。

//list[definition-file/overload/overloadFault-invalid.ts][overloadとunion typesは相性がよくない]{
#@mapfile(../code/definition-file/overload/overloadFault-invalid.ts)
declare function funcA(word: string): string;
declare function funcA(num: number): string;

let obj: string | number = null as any;

// stringかnumberを渡さなければならない場合 string | number はコンパイルエラーになる
// 本来であれば、受け入れてほしいのだけど…
// error TS2345: Argument of type 'string | number'
//   is not assignable to parameter of type 'number'.
//  Type 'string' is not assignable to type 'number'.
funcA(obj);

// 元の定義がunion typesならもちろんOK
declare function funcB(word: string | number): string;
funcB(obj);
#@end
//}

#@# NOTE: https://github.com/Microsoft/TypeScript/issues/5766 で解決されるかと思ったら型定義が直されて解決された…

#@# @suppress SectionLength JapaneseAmbiguousNounConjunction
==={module-declaration-merging} モジュールの定義を統合する

#@# OK REVIEW lc: なんか「ベストプラクティス」からずれている気がする・・・
#@# vv: 心得に変えました

#@# @<strong>{利用可能になったバージョン 1.3.0}

#@# @suppress SentenceLength
あまり言及されることがないのでここで触れておきます。
モジュールの型定義はopen endedですので@<list>{externalModuleDeclarationMerging/basic.ts}と@<list>{externalModuleDeclarationMerging/usage.ts}のようなコードが書けます。
めでたい。

//list[externalModuleDeclarationMerging/basic.ts][モジュール定義を後から拡張可能]{
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

//list[externalModuleDeclarationMerging/usage.ts][統合された定義が使えます]{
#@mapfile(../code/definition-file/externalModuleDeclarationMerging/usage.ts)
import * as foo from "foo";
foo.str;
foo.num;
#@end
//}

#@# @suppress SuccessiveWord SentenceLength JapaneseAmbiguousNounConjunction
既存のライブラリに勝手にメソッドを生やす（＝型を拡張する）ようなライブラリがあります。
DefinitelyTypedではモジュールの型定義を容易に拡張するために、モジュールの型定義とそれとは独立したnamespaceを組み合わせて使うパターンがあります。
たとえば、lodashやjQueryのようなグローバルな名前空間に変数を生やすような場合に、いまだに有効です。
#@# OK mhidaka モジュールの型定義とモジュールの定義という２つの言葉を使い分けていそうだけどそうじゃない感じもする。
#@# TODO このあたりDTでのやり方の分布に変化が生じてる気もするので気が向いたら調査する
#@# https://github.com/Microsoft/types-publisher#fields-in-datadefinitionsjson kindのとこみたいな例があるはず

==={any-vs-object} どれ使う？anyと{}とObject

もしも型定義ファイルを書いていて具体的な型がわからないとき、頭を使わずにとりあえずコンパイルを通したいときは、素直に@<code>{any}を使いましょう。
こういったシチュエーションで、稀にObjectを指定する人がいます。
プロトタイプチェーンの頂点にいるObjectをとりあえず据えておこう！という考えかもしれませんがこれは悪いやり方です。

関数の引数にObjectや{}を指定するのは、どういう性質の値がほしいのかを述べていません。
本当にどのような値でも受け入れるのであれば、anyにするべきです。

関数の返り値にObjectや{}を指定しても有用なプロパティが存在しないため型アサーションでもって適切な型にキャストするしかありません。
これはanyを指定するのと同程度に危険で、なおかつanyより検出しにくいです。
素直にanyを使いましょう。

筆者は今のところ、Objectや{}が型注釈として適切な場面を見たことがありません@<fn>{primitive-object-type}。
大抵の場合は、適切な型を定義してそちらを参照するほうが優れています。

//footnote[primitive-object-type][@<chapref>{types-basic}で触れたオブジェクト限定型（object）が適切な場合は稀にある…かも？]

そしてanyを使うことに気後れするのであれば、よく調べて適切な型定義を与えるのがよいでしょう。

==={scratch-from-document} ドキュメントから書き起こす

もしライブラリにしっかりしたドキュメントがあるのであれば、実装コードから型定義ファイルを起こすのではなく、ドキュメントをベースに作成しましょう。
Visual StudioなどのIDEでは、型定義ファイル上に書かれたJSDocコメントも利用時に表示してくれる場合があります。
そのため、型定義を起こしつつ、あわせてJSDocを記述していくとよいでしょう。

#@# @suppress KatakanaSpellCheck
サンプルをテスト用コードとしてTypeScriptコードに移植し、ドキュメントどおりの記述が可能かも確かめるとよいです。
型定義ファイルは書き起こしたけれどもドキュメント中に書かれている利用例のコードをコンパイルしてみて失敗するようであれば、それは悪い型定義だといえます。
たまにドキュメントのほうが間違っているときがありますが、その場合は本家に修正のpull requestを送るチャンスです。

世の中、ドキュメントにコストをあまり掛けることのできないプロジェクトも多くあります。
そのため、この指針は絶対的なルールではありません。
この場合、コードから型定義ファイルを起こすことになるのは仕方のないことです。

==={be-careful-about-optional} コールバック関数の引数を無闇に省略可能（optional）にしない

optionalとは、値が渡されるかどうかの指標であって、コールバックを受け取った側が使うかどうかではありません。
ここを勘違いすると、"コールバックに値が渡されるが別に使わなくてもいいよ"マークとしてoptionalを使ってしまうのです。

例を見てみましょう（@<list>{callback/basic.ts}）。

//list[callback/basic.ts][optionalはもしかしたら値がないことを表す]{
#@mapfile(../code/definition-file/callback/basic.ts)
// 良い例
declare function readFileA(
  filePath: string,
  listener: (data: string) => void): void;
// 悪い例
declare function readFileB(
  filePath: string,
  listener: (data?: string) => void): void;

// 使ってみよう！
readFileA("./test.txt", data => {
  // ここでのdataは必ず実体がある
  console.log(data.toUpperCase());
});
readFileB("./test.txt", data => {
  // ここでのdataはundefinedかもしれない… チェックしなければダメ
  if (!data) {
    data = "not found";
  }
  console.log(data.toUpperCase());
});

// 引数を無視するのは自由 optionalにする理由にはならない
readFileA("./test.txt", () => {
  console.log("done");
});
readFileB("./test.txt", () => {
  console.log("done");
});
#@end
//}

#@# @suppress JapaneseStyle
両方とも、ファイルの読み取りを行うための関数を型定義として書き起こしたものです。
readFileはdataが省略不可、readFileOptはdataが省略可能（optional）になっています。
これはreadFileOptではdataがundefinedになるかもしれないことを表します。
dataがundefinedかもしれないため、if文などで中身をチェックし、undefinedだった場合の対応を入れなければなりません。
本当にundefinedになりうるのであれば省略可能にするか、union typesでundefinedを与える必要があります。
しかし、そうではなく必ずdataの値が渡されてくる場合は、無用なチェック処理が発生することになります。

間違えないよう、留意しましょう。

==={module-compat} ECMAScriptモジュールとCommonJSモジュールの互換性について

最初にまとめを書いておきます。

まとめ：@<strong>{元のJavaScriptコード中にdefaultの文字がないならimportのdefaultは使うな}。

#@# @suppress JapaneseAmbiguousNounConjunction
現在JavaScriptのモジュール仕様は過渡期にあります。
ECMAScriptでモジュールの記法や考え方は定義され、ブラウザでも実装されはじめました。
しかし、CommonJS形式のモジュールとの互換性なんてECMAScriptの仕様には含まれていません。

そのためにTypeScriptやBabelなど、各種トランスパイラ毎にECMAScriptとCommonJS間の変換方法は食い違っています。
TypeScriptが正しいのかBabelが正しいのかという議論は、そもそも仕様が不明なので成立しません。
TypeScriptもBabelもECMAScriptなモジュール記法からCommonJS形式などへの変換ルールを定めているため、我々はその特徴を知り、正しく使いこなす必要があります。

#@# @suppress ParenthesizedSentence
まずはTypeScriptで書いたコードがどのようなCommonJS形式のコードに変換されるかを見てみます（@<list>{commonJSCompat/basic/basic.ts}、@<list>{commonJSCompat/basic/basic.js}）。

//list[commonJSCompat/basic/basic.ts][関数などを素直にexportする]{
#@mapfile(../code/definition-file/commonJSCompat/basic/basic.ts)
export function hello(word = "TypeScript") {
  console.log(`Hello, ${word}`);
}

export function bye(word = "JavaScript") {
  console.log(`Bye, ${word}`);
}
#@end
//}

//list[commonJSCompat/basic/basic.js][CommonJS形式ではexports.xxx = となる]{
#@mapfile(../code/definition-file/commonJSCompat/basic/basic.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function hello(word = "TypeScript") {
    console.log(`Hello, ${word}`);
}
exports.hello = hello;
function bye(word = "JavaScript") {
    console.log(`Bye, ${word}`);
}
exports.bye = bye;
#@end
//}

単純でわかりやすいですね。

#@# @suppress SentenceLength ParenthesizedSentence
次にCommonJSでの@<code>{exports.module = ...;}形式（export.moduleへの代入）の記法を見てみます（@<list>{commonJSCompat/exportsAssignment1/util.ts}、@<list>{commonJSCompat/exportsAssignment1/util.js}）。

//list[commonJSCompat/exportsAssignment1/util.ts][export = ... と書く]{
#@mapfile(../code/definition-file/commonJSCompat/exportsAssignment1/util.ts)
function hello(word = "TypeScript") {
  console.log(`Hello, ${word}`);
}

// CommonJSの exports.module = hello; 相当
// 外からこのモジュールを参照した時のオブジェクト自体を差し替える
export = hello;
#@end
//}

//list[commonJSCompat/exportsAssignment1/util.js][exports.module = ... となる]{
#@mapfile(../code/definition-file/commonJSCompat/exportsAssignment1/util.js)
"use strict";
function hello(word = "TypeScript") {
    console.log(`Hello, ${word}`);
}
module.exports = hello;
#@end
//}

この変換は重要です。
変換結果から逆に考えるとJavaScriptで@<code>{exports.module = ...;}の形式を見たらTypeScriptでは@<code>{export = ...;}という型定義に書き起こす必要があります。

#@# @suppress JapaneseAmbiguousNounConjunction
理解を深めるためNode.jsでのCommonJSの実現方法について該当のコードを抜粋@<fn>{node-module-url}します（@<list>{node-module.js}）。

//footnote[node-module-url][@<href>{https://github.com/nodejs/node/blob/v8.1.3/lib/internal/bootstrap_node.js#L542-L549}]

//list[node-module.js][Node.jsのモジュールの実現方法]{
NativeModule.wrap = function(script) {
  return NativeModule.wrapper[0] + script + NativeModule.wrapper[1];
};

NativeModule.wrapper = [
  '(function (exports, require, module, __filename, __dirname) { ',
  '\n});'
];
//}
#@# NOTE https://github.com/nodejs/node/blob/ff3ce11894797d031956ca0cb144e600d2b36aa3/lib/internal/bootstrap_node.js#L422-L429
#@# NOTE https://github.com/nodejs/node/blob/ff3ce11894797d031956ca0cb144e600d2b36aa3/lib/module.js#L538-L541

#@# @suppress KatakanaSpellCheck JapaneseAmbiguousNounConjunction
大変シンプルなコードが出てきました。
Node.jsにおいて、モジュール固有の変数というのはモジュールのオリジナルのコードの前後に2行付け足して、evalしているだけなのです。
なので、Node.js初心者がたまにやりがちな@<code>{exports = ...;}というコードは間違いです。
単に変数の値を差し替えているだけなので当然ですね。
外部に変更を露出させるには、何かのプロパティの変更（つまり@<code>{module.exports = ...;}）でなければなりません。

互換性の話に戻ります。
この@<code>{export = ...;}の記法に対応した"正規の"importの書き方は先ほど見た@<code>{import xxx = require("...");}形式です。
これを無理やりECMAScript形式のimport文に書き直すと@<list>{commonJSCompat/exportsAssignment2/main.ts}になります。

//list[commonJSCompat/exportsAssignment2/main.ts][import モジュール全体 as 名前]{
#@mapfile(../code/definition-file/commonJSCompat/exportsAssignment2/main.ts)
// モジュール全体をutilに割当て
import * as util from "./util";

// この書き方は誤り util.ts にdefaultエクスポートはない
// error TS1192: Module '"略/util"' has no default export.
// import util from "./util";

// Hello, CommonJS と表示される
util("CommonJS");
#@end
//}

ECMAScript形式でのimportは若干良くなくて、@<code>{export =}する対象が変数ではない場合、エラーになるためワークアラウンドが必要です（@<list>{commonJSCompat/exportsAssignment2/util.ts}）。

//list[commonJSCompat/exportsAssignment2/util.ts][同名のnamespaceを被せてごまかす]{
#@mapfile(../code/definition-file/commonJSCompat/exportsAssignment2/util.ts)
function hello(word = "TypeScript") {
  console.log(`Hello, ${word}`);
}
// 呼び出し元でエラーになるのを防ぐ 同名のnamespaceを被せてごまかす
// error TS2497: Module '"略/util"' resolves to a non-module entity
//   and cannot be imported using this construct.
namespace hello { }

export = hello;
#@end
//}

いまいち優雅ではありませんね。
この場合は無理にECMAScriptのモジュール記法を使わないほうが無難かもしれません。
世間的にも意見が分かれるところです。

さて、ここで問題になる点がTypeScriptとBabelで@<code>{module.exports = ...;}形式のモジュールを利用する際、どうECMAScript形式にマッピングするかの解釈が異なる点です。
Babelの変換結果を見てみます。
@<list>{babel-before.js}をコンパイルすると@<list>{babel-after.js}（@<list>{babel-after-rewrite.js}）となります。

//list[babel-before.js][Babelで変換する前のコード]{
import util from "./util";
util();
//}

//list[babel-after.js][Babelで変換した結果のコード]{
"use strict";

var _util = require("./util");

var _util2 = _interopRequireDefault(_util);

function _interopRequireDefault(obj) {
  return obj && obj.__esModule ? obj : { default: obj };
}

(0, _util2.default)();
//}

//list[babel-after-rewrite.js][Babelで変換した結果をわかりやすく書き直す]{
"use strict";

var util = require("./util");
if (!util || !util.__esModule) {
  util = { default: util };
}

util.default();
//}

Babelは、@<code>{module.exports = ...;}形式のコードに対して特別な配慮を行い、@<code>{import util from "./util";}形式でも動作します。
TypeScriptが@<code>{import * as util from "./util";}形式しか許していないため、ここに齟齬があります。

ECMAScript形式＋BabelのコードをTypeScriptから参照したり、ECMAScript＋TypeScriptのコードをBabelから参照したりすることには大きな問題はありません。
しかし@<code>{module.exports = ...;}なコードの取り扱いには注意が必要なのです。

この話題はDefinitelyTypedでよくあるトラブルの1つで、TypeScript＋Babelの両方を組み合わせて使うユーザからこのあたりがごっちゃになったコードや修正が来ます。
レビューする側としては「いやお前の環境では動くかもしれんが大抵のビルド手順では動かんのじゃ」となり、修正してくれるまで取り込むことはありません。
TypeScriptでは@<code>{exports.default = ...}とされているコードのみ@<code>{export default ...}という型定義を与えてよいのです。
@<strong>{元のJavaScriptコード中にdefaultの文字がないならimportのdefaultは使うな}。
ということです。

==={export-and-commonjs} CommonJS形式でちょっと小難しいexport句の使い方

インタフェースやクラスのインスタンス単体をモジュールの外側に見せたい場合、@<list>{export/sample1.d.ts}のように書きます。

//list[export/sample1.d.ts][実はインタフェースBarも外から見えない]{
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

呼び出し側では@<list>{export/sample1Usage.ts}のように使います。
importした値がインタフェースFooのインスタンスになっていることがわかります。

//list[export/sample1Usage.ts][使うとき。インタフェースBarのインスタンスが得られる]{
#@mapfile(../code/definition-file/export/sample1Usage.ts)
// b は "bar" の Barのインスタンス だよ！
import * as b from "bar";
b.num;
#@end
//}

よくやりがちな誤りは@<list>{export/sample2.d.ts}のような書き方をしてしまうことです。
インタフェースのインスタンスをexportしたつもりが型がexportされてしまうのです。

//list[export/sample2.d.ts][それは値ではなくて型だけexportしているぞ！]{
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
型定義ファイルを書いたら適当なユースケースに当てはめて意図どおりコンパイルできるか確かめてみましょう。

#@# @suppress SectionLength ParagraphNumber
==={modules-and-global} グローバルに展開される型定義とモジュールの両立

#@# OK REVIEW lc: これも「ベストプラクティス」からずれている気がする・・・
#@# vv: 心得 に名称変えました

グローバルに変数が展開されるタイプとモジュールとしての利用が両立しているタイプのライブラリについて考えます。
具体的に@<kw>{UMD,Universal Module Definition}と呼ばれる形式@<fn>{umd}です。
ライブラリ内部でモジュールとしての使い方が想定されているのか、そうではないのかを判断し展開の方法を変えます。

//footnote[umd][@<href>{https://github.com/umdjs/umd}]

#@# @suppress JapaneseAmbiguousNounConjunction ParenthesizedSentence
TypeScriptではこういうパターンのときに使いやすい型定義ファイルの記述方法があります。
しかし、TypeScript 2.0.0以前は任意の場所においてある型定義ファイルを特定の名前のモジュールだと認識させる方法がなかったため、役に立ってはいませんでした。
この形式が使われているのはDefinitelyTypedの@typesパッケージシリーズ（本書執筆時点ではtypes-2.0ブランチ）だけではないでしょうか。

説明のためにstrutilとstrutil-extraという架空のライブラリについて考えてみます。
strutilはrandomizeString関数を提供します。
strutil-extraはhappy関数を提供し、strutilを拡張します。

まずは型定義ファイルを見てみましょう（@<list>{augmentGlobal/typings/strutil/index.d.ts}、@<list>{augmentGlobal/typings/strutil-extra/index.d.ts}）。
ちょっと見慣れない書き方ですね。

//list[augmentGlobal/typings/strutil/index.d.ts][typings/strutil/index.d.ts]{
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

//list[augmentGlobal/typings/strutil-extra/index.d.ts][typings/strutil-extra/index.d.ts]{
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
この形式だと、どのライブラリを拡張しているのか明示できるところが利点です。

これらを@<code>{import ... from "strutil";}するためのtsconfig.jsonを確認しておきます（@<list>{augmentGlobal/tsconfig.json}）。

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

baseUrlとpathsの指定があります。
TypeScript 2.0.0からこうして任意の場所の型定義ファイルを任意の名前に紐付けられるようになったため、ローカル環境でも利用しやすくなりました。
#@# TODO この辺typesRootを使えばもっときれいにかけるのでは？

次に前述の型定義ファイルを利用する例を見てみます。
まずはグローバルに展開される例です（@<list>{augmentGlobal/lib/bare.ts}）。

//list[augmentGlobal/lib/bare.ts][lib/bare.ts]{
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

@<code>{export as namespace ...}形式を使わないUMD形式の対応方法もありますが、importと混ぜるとエラーになるところがよいですね。

#@# @suppress JapaneseStyle
モジュール形式も見てみましょう（@<list>{augmentGlobal/lib/module.ts}）。
モジュールとしてimport句の対象にできています。

//list[augmentGlobal/lib/module.ts][lib/module.ts]{
#@mapfile(../code/definition-file/augmentGlobal/lib/module.ts)
// UMD形式のライブラリがglobalに展開されたときの動作に相当する
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
ファイル名を見ただけではどういう名前に解決されるかがわかりにくいところだけは注意が必要です。

#@# TODO /// <reference types="jquery" /> 的な記述の調査 https://github.com/Microsoft/TypeScript/issues/7156

==={check-at-last} 最終チェック！

#@# OK REVIEW lc: もはや「型定義ファイルのベストプラクティス」節であることを忘れている気がする
#@# vv: 心得に変えました

やった！型定義ファイルが書けたぞ！
出来高に満足する前に、もう少しだけやっておきたいことがあります。
それが、@<code>{--strict}をつけてのコンパイルの試運転とtslintによるチェックです。

lintとは、プログラムを静的に解析してバグになりそうな箇所や悪いコードスタイルを見つけてくるツールを指します。
TypeScriptでは@<href>{https://github.com/palantir/tslint,tslint}@<fn>{tslint-repo}というプログラムが一般的に使われています。

//footnote[tslint-repo][@<href>{https://github.com/palantir/tslint}]

tslintはコンパイルだけでは見つけきれない、悪いにおいのするコードを検出してくれます。
tslintでは頻繁に新しいルールが追加されるため、本書では詳しくは取り上げません。
その時々の最適な設定を突き詰めてみてください。

#@# @suppress SentenceLength
利用には設定ファイルを必要とします。
今のところ、TypeScriptにおける統一見解は存在していないのでtslintが使ってる@<href>{https://github.com/palantir/tslint/blob/master/tslint.json,設定ファイル}@<fn>{tslint-example-config}かTypeScript本体の@<href>{https://github.com/Microsoft/TypeScript/blob/master/tslint.json,tslint.json}@<fn>{tsc-tslint}を参照するとよいでしょう。

//footnote[tslint-example-config][@<href>{https://github.com/palantir/tslint/blob/master/tslint.json}]
//footnote[tsc-tslint][@<href>{https://github.com/Microsoft/TypeScript/blob/master/tslint.json}]

=={lets-contribute} Let's contribute!

ようこそ！@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped,DefinitelyTyped}@<fn>{dt}へ！
メンテナのvvakameです。
とかいいつつここ最近くらいは筆者はツール類のメンテ以外をサボっていて、Microsoftのメンバーがpull requestの処理を行ってくれています。

//footnote[dt][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped}]

DefinitelyTypedではさまざまな型定義ファイルを取り揃えてございます！
世界中の人々が作った型定義ファイルは集積され、@typesなどを介して広く利用されています。

貴方が作った型定義ファイルも世界中の人々に使ってほしいとは思いませんか？
もしくは、あなたがいつも使っている型定義ファイルのバグを治したい…そんな気持ちになることもあるでしょう。
その思い、すべてDefinitelyTypedにぶつけてみましょう！

本書を読んでいただいた紳士淑女の皆様は、感じのよい型定義ファイルが書けるようになっています。
品質と時間のトレードオフを考えつつ、上品な型定義ファイルを提供していただきたいです。

DefinitelyTypedはGitHub上のリポジトリなので追加、修正についてはpull requestをご利用ください。
pull requestを送る前に、@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/README.md,README.md}@<fn>{README.md}と@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/.github/PULL_REQUEST_TEMPLATE.md,PULL_REQUEST_TEMPLATE.md}@<fn>{PULL_REQUEST_TEMPLATE.md}を読んでおくとよいでしょう。

//footnote[README.md][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/README.md}]
//footnote[PULL_REQUEST_TEMPLATE.md][@<href>{https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/.github/PULL_REQUEST_TEMPLATE.md}]
#@# この節では、DefinitelyTypedに送られてきたときにどういうチェックが行われるかを簡単に解説します。
#@# あくまで、ここに書いてあることは筆者固有の観点なので、レビュワーによって別のことをいわれる場合もあるでしょう。
#@# 実際にpull requestを送ってみて、ここに書いてある以外の理由で拒否されたとしても、そこは実際のレビュワーを尊重して議論していただきたいと思います。

簡単なチェックポイントは次のとおりです。

 * 新規での型定義ファイルの作成はnpmのdts-genパッケージを使って生成されていること
 * フォルダ名がnpm上での目的のパッケージ名と一致していること
 * tscコマンドでコンパイルが通ること
 * @<code>{npm run lint パッケージ名}で問題が検出されないこと

新規型定義ファイルはDefinitelyTypedリポジトリをforkし、@<code>{npm install -g dts-gen}で入るdts-genコマンドを使い作成します。
これには推奨設定のtsconfig.jsonとtslint.jsonと、index.d.tsと簡単なテスト用ファイルも作成されます。
詳しくは@<href>{https://www.npmjs.com/package/dts-gen}を参照してください。

新規に作成する場合、DefinitelyTypedリポジトリのtypesディレクトリ配下に対象となるnpmパッケージ名と同名のディレクトリに各種ファイルを収めます。
もし対象となるパッケージがscopedパッケージの場合、たとえば@<code>{vvakame/foobar}に対して型定義ファイルを作成するのであれば@<code>{types/vvakame__fobar}ディレクトリにファイルを作成します。

現在のDefinitelyTypedでは、dtslint@<fn>{dtslint}というtslintを拡張したルールセットでチェックを行っています。
これは、@<code>{npm run lint パッケージ名}としたときに裏側で動きます。

//footnote[dtslint][@<href>{https://www.npmjs.com/package/dtslint}]

この他、人力じゃないと判別が付かないようなコードの良し悪しについてチェックします。
たとえば、幽霊モジュールを使ったほうがコードがきれいになるのでは？とかベストプラクティスにしたがっているか？などです。

逆に、ここに書かれていないことはあまり見ていません。
たとえば、ライブラリの実装全体に対する型定義ファイルのカバー率やanyの多さなどはあまり見ていません。
それらは後から別の人が補ってくれる可能性があるからです。
一人でやりきらなくてもいいよな！という発想ですね。
もちろん最初に高品質高カバー率のものが出てきたほうが「やりおる！」と感心はします。

既存の型定義の変更の場合、コードスタイルの変更や破壊的変更については取り込みは比較的慎重に行われます。
@dt-botというボットが自動的にレビューするべきであろう人にメンションしてくれるので、反応を待ちましょう。

では皆様のpull request、お待ちしています！

#@# TODO この辺レビュー復帰して空気感もう一回把握してから親切に書き直すべきかなー
#@# NOTE ababb9cd46339bf156bc96efcbf7d83a0a0e2232 より後のコミットで解説をバッサリ削ったはず。

=={publish-npm-best-practice} 自分のライブラリをnpmで公開するときのベストプラクティス

自分の作ったライブラリをnpmに公開する時のベストプラクティスについて説明します。
ここで説明する内容はTypeScriptによってコードが書かれているライブラリを前提とします。
また、npmにパッケージを公開するための基本的な説明はここでは行いません。

ポイントは.tsファイルをリリースに含めないこと、.d.tsファイルをTypeScriptコンパイラに生成させること、.d.tsファイルをTypeScriptコンパイラが自動的に見つけられるようにすることです。

まずは@<strong>{.tsファイルをリリースに含めない}理由について説明します。
これは、TypeScriptコンパイラの探索順序が.tsファイル、.tsxファイル、.d.tsファイルだからです。
.d.tsファイルも公開していたとしても、.tsファイルが存在しているとそちらが先に発見され、コンパイル処理が走ってしまいます。
TypeScriptコンパイラのバージョンが上がった時にソースコード（.ts）の修正が必要になるケースは多いですが、型定義ファイル（.d.ts）が影響を受けるケースは稀です。
つまり、自分のライブラリをより安定したものとするためには、.tsファイルをリリースに含めないほうがよいわけです。
そのために.npmignoreファイルに@<list>{.npmignore}の記述を追加します。

//list[.npmignore][.npmignoreで.tsコードを排除し.d.tsはパッケージング対象へ]{
# libディレクトリ配下でコードが管理されている場合
lib/**/*.ts
!lib/**/*.d.ts
//}

@<strong>{.d.tsファイルをTypeScriptコンパイラに生成させる}ための作業は、.tsコードをコンパイルするときに@<code>{--declaration}オプションを利用するだけなので簡単です。

次に@<strong>{.d.tsファイルをTypeScriptコンパイラが自動的に見つけられるようにする}理由ですが、これは単純に使いやすいからです。
実現するためにはTypeScriptコンパイラの検索パスに自身の型定義ファイルが入るようにします。

そのための方法はいくつかあります。

 1. パッケージのrootにindex.d.tsを置く
 2. package.jsonにtypingsプロパティを作成し、最初に参照するべき型定義ファイルの相対パスを書く
 3. package.jsonにtypesプロパティを作成し、最初に参照するべき型定義ファイルの相対パスを書く

1つ目はNode.jsが実行時にパッケージのrootにあるindex.jsを最初に読み込もうとする挙動に似せた動作です。
2つ目と3つ目はほぼおなじやり方ですが、3つ目のほうが最近追加されたやり方です。
typingsとtypesプロパティの両方が存在する場合はtypingsプロパティが優先されます。

筆者はもっぱら、1つ目の方法を使いindex.d.tsとindex.jsを手書きしています。
これはpackage.jsonに色々と書くよりも一般的なルールに従うのを良しとしているためです。

実例については筆者の@<href>{https://github.com/vvakame/typescript-formatter,typescript-formatter}@<fn>{tsfmt}リポジトリを参照してください。

//footnote[tsfmt][https://github.com/vvakame/typescript-formatter]
