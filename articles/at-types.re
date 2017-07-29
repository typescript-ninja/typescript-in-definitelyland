={at-types} JavaScriptの資産と@types

#@# OK REVIEW muo: 6章けっこう長めなので章として切断できないかな。
#@# OK REVIEW muo: 基礎編・応用編と分けるのはむずかしいかもしれないけど、「このへんまで頑張ったら一息つける」「ここまでやってくれる人はほめてあげよう」ぐらいに分かれていると、トピック拾いやすいかもしらん
#@# 3コミケまたいで対応した

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
既存の型定義ファイルを使う場合も、この可能性を頭の片隅においておき、信じすぎないようにしましょう。

型定義ファイルのバグというと、メソッド名が間違っていたり、あるはずのプロパティが定義されていなかったり、引数の型が間違っていたりとさまざまです。
最終的に信じられるのはコンパイル後のJavaScriptファイルとデバッガ、つまりはJavaScript力です。
バグを発見したときは@<chapref>{definition-file}を参考に、型定義ファイルの総本尊、DefinitelyTypedまでpull requestをお送りください。

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
#@mapfile(../code/at-types/usage/lib/index.ts)
export function hello(word = "TypeScript") {
  return `Hello, ${word}`;
}
#@end
//}

これに対してテストコードとして@<code>{./test/indexSpec.ts}を書いてみましょう（@<list>{usage/test/indexSpec.ts}）。

//list[usage/test/indexSpec.ts][mocha+power-assertでテストを書く]{
#@mapfile(../code/at-types/usage/test/indexSpec.ts)
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
#@mapfile(../code/at-types/usageAbstract/mocha.d.ts)
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
#@mapfile(../code/at-types/usageAbstract/power-assert.d.ts)
export = assert;
export as namespace assert;

declare function assert(value: any, message?: string): void;
#@end
//}

型定義ファイルを見るとmochaとpower-assertそれぞれのAPIが表現されています。
TypeScriptコンパイラがこれらの型定義ファイルを認識できれば、矛盾なくコンパイルを通すことができそうです。
そのためのpackage.json（@<list>{usage/package.json}）とtsconfig.json（@<list>{usage/tsconfig.json}）を確認します。

//list[usage/package.json][package.json]{
#@mapfile(../code/at-types/usage/package.json)
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
#@mapfile(../code/at-types/usage/tsconfig.json)
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
power-assertについてはTypeScriptコンパイラが必要であることを判別し、型定義ファイルを探しにいってくれます。

しかしmochaはそのような機会がないため、TypeScriptコンパイラは型定義を探しにいってくれません。
このままコンパイルするとmochaがグローバルに値を展開しているdescribeやitなどが見つからないといわれてしまいます。
これを解決するためにtsconfig.jsonのtypesプロパティ中でmochaを参照するよう指定します。

あわせて型定義ファイルへの参照が意図どおり処理されずに困った場合のデバッグ方法を紹介しておきます。
コンパイルに利用したファイルをリスト表示する@<code>{--listFiles}オプションと、型定義ファイル探索の様子を表示する@<code>{--traceResolution}オプションを試してみてください。

#@# TODO できればmochaの型定義バージョンあげたい
なお、DefinitelyTypedは規約上、実装本体のパッケージと型定義ファイルのバージョンのmajorとminorが一致します。
本書の例のようにmochaの実装と型定義のメジャーバージョンすらずれているのは本来はあまりよくありません。

#@# OK mhidaka @typesをつかって使うはなしがここまでで、型定義を書くはなしがここから下で大分性質がことなるので章の分離があってもいいかもしれない。
#@# やったった
