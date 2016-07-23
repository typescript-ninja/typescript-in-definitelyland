= オプションを知り己のコードを知れば百戦危うからず

本章ではtscのコマンドラインオプションについて解説していきます。
すべてを網羅することはしませんが、いくつかの重要なオプションについては知ることができるでしょう。

本章記載のオプションはtsconfig.jsonのcompilerOptionsに記載可能なプロパティ名と同一です。
tsconfig.jsonでは短縮形（@<code>{-d}や@<code>{-p}）は利用できないことに注意してください。

ここに記載されていないオプションで、知りたいものがあればIssue@<fn>{issue}にお寄せください。

//footnote[issue][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

#@# TODO 以下のオプションについてじわじわ書く
#@#   --init
#@#   --project
#@#   --declaration, --declarationDir
#@#   --listFiles
#@#   --noEmitOnError
#@#   --noFallthroughCasesInSwitch
#@#   --pretty
#@#   --traceResolution
#@#   --types
#@#   --allowJs
#@#   --experimentalDecorators
#@#   --emitDecoratorMetadata
#@#   --sourceMap, --inlineSources, --inlineSourceMap
#@#   --skipLibCheck, --skipDefaultLibCheck
#@#   --out, --outDir, --outFile
#@#   --noImplicitUseStrict
#@# TODO 短縮形の紹介

==== --noImplicitAny

TypeScriptコンパイラの最重要オプション、--noImplicitAnyについて解説します。
このオプションは"暗黙的なanyを禁ずる"の名が表すとおり、型推論の結果、暗黙的に変数の型がanyになった場合、エラーとしてくれます。

@<list>{noImplicitAny/basic-invalid}のような、メソッドの返り値の型を書き忘れた！という脇の甘いコードをコンパイルしてみます。

//list[noImplicitAny/basic-invalid][メソッドの返り値を書き忘れた！]{
#@mapfile(../code/tsc-options/noImplicitAny/basic-invalid.ts)
declare class Sample {
  // 返り値の型を指定し忘れている！
  // error TS7010: 'method', which lacks return-type annotation,
  //               implicitly has an 'any' return type.
  method();
}

// 仮引数wordに型注釈がない
// error TS7006: Parameter 'word' implicitly has an 'any' type.
function hi(word) {
  word = word || "TypeScript";
  console.log(`Hello, ${word}`);
}

export { }
#@end
//}

//cmd{
$ tsc --noImplicitAny definition.d.ts
definition.d.ts(3,5): error TS7010: 'method', which lacks return-type
    annotation, implicitly has an 'any' return type.
//}

返り値の型を書いていなかったり、関数の仮引数の型が指定されていないため暗黙的にanyになってしまいました。
これに対して、それはあかん！とコンパイラが教えてくれます。
anyが紛れ込んで、型チェックが意味を成さなくなるとTypeScriptコードの意義が薄れてしまいます。
型定義ファイルを書くときも、通常の開発時も、常に--noImplicitAnyを使うようにしましょう。

== --strictNullChecks

@<code>{--strictNullChecks}オプションについて解説します。
このオプションはnullやundefinedの扱いについてより厳格にし、変数の中身についての曖昧さを積極的に排除するよう振る舞います。
nullやundefinedを許容させたい場合、union typesや省略可能引数を使って明示的にnullやundefinedである可能性を示さなければなりません。

本書では、@<code>{--strictNullChecks}オプションは常に有効にしている前提で書いています。
有効にしている時の挙動は本書のサンプルすべてが該当しますので、この節ではこのオプションを使わないときの挙動について確認します。

まずは、本書標準であるオプションありの例です（@<list>{strictNullChecks/basic}）。

//list[strictNullChecks/basic][危険なコードがいち早く発見される]{
#@mapfile(../code/tsc-options/strictNullChecks/basic.ts)
// --strictNullChecks無しとの比較
// 無しの場合に等しい表現は…
let objA: Date;
// コレです
let objB: Date | null | undefined;

objA = new Date();
// Date単体の型注釈の場合、エラーとなる
// error TS2322: Type 'null' is not assignable to type 'Date'.
// objA = null;

// …しかし、一回anyを経由すればごまかせてしまう 他のサンプルコードでもたまにやってます
objA = null as any;

// objB は null も undefined も許容するため、ゆるゆる
objB = new Date();
objB = null;
objB = void 0; // undefined

// 処理フロー的にundefinedが確定しているのでエラーとなる
// error TS2532: Object is possibly 'undefined'.
// error TS2339: Property 'getTime' does not exist on type 'never'.
// objB.getTime();

export { }
#@end
//}

nullやundefinedに対するアクセスが多くの場合未然に防がれ、"コンパイルが通ればもう安全"であるコードが書きやすいことがわかります。

さて、次はオプションなしの例です（@<list>{strictNullChecks/withoutStrictNullCheck-ignore}）。

//list[strictNullChecks/withoutStrictNullCheck-ignore][実行時にエラーになるかも]{
#@mapfile(../code/tsc-options/strictNullChecks/withoutStrictNullCheck-ignore.ts)
// --strictNullChecks無しだと大変ゆるい
let obj: Date;
// 全部OK！
obj = new Date();
obj = null;
obj = void 0; // undefined

// 処理フロー的にはundefinedだけど怒られない
obj.getTime();

export { }
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
ゆるゆるですね。
変数の中身を容易にnullやundefinedにすることができてしまいます。
きっちりコードを書けば、オプション無しでも堅牢なアプリケーションを構築することは不可能ではありません。
しかし、それはプログラマの不断の努力の上にしか成り立ちません。
そんな苦労をするよりは、コンパイラにしっかりチェックしてもらえたほうがコードの堅牢さがより確かなものになりますね。

== --noUnusedLocals

@<code>{--noUnusedLocals}オプションについて解説します。
その名のとおり、使っていないローカル変数があったらエラーにしてくれます。
本書のサンプルコードでも有効になっているため、エラー消しのために無意味にexportしてみたりしている箇所がありました。

例を見てみます（@<list>{noUnusedLocals/basic-invalid}）。

//list[noUnusedLocals/basic-invalid][未使用変数はちゃんと消そう]{
#@mapfile(../code/tsc-options/noUnusedLocals/basic-invalid.ts)
// importした後、一回も使わないのはエラー
// error TS6133: 'readFile' is declared but never used.
import { readFile } from "fs";

// 1回も参照されていないとエラーになる
// error TS6133: 'objA' is declared but never used.
let objA = {};

// どこかで参照されていればOK
let objB = {};
export { objB }

// exportしていればどこかで使われるかもしれないからOK
export let objC = {};
#@end
//}

未使用の変数があるとエラーになります。
まるでGo言語のようですね。
エラーを削っていくと、import文自体を削減できるパターンもあるでしょう。
コードをきれいに保とう！

== --noUnusedParameters

@<code>{--noUnusedParameters}オプションについて解説します。
関数やメソッドの引数に使っていないものがあるとエラーにしてくれます。
エラーにせず残しておきたい場合、変数名の頭に@<code>{_}（アンダースコア）をつけることでエラーを抑制することができます。

例を見てみます（@<list>{noUnusedParameters/basic-invalid}）。

//list[noUnusedParameters/basic-invalid][使っていない仮引数はできれば削除したい]{
#@mapfile(../code/tsc-options/noUnusedParameters/basic-invalid.ts)
// 仮引数 b は利用されていないのでエラー _c はプリフィクス_なのでエラーにならない
// error TS6133: 'b' is declared but never used.
export function foo(a: string, b: number, _c: boolean) {
  console.log(a);
}

export class Sample {
  // 仮引数 a は利用されていないのでエラー
  // error TS6133: 'a' is declared but never used.
  method(a: string) {
  }
}
#@end
//}

#@# @suppress JapaneseAmbiguousNounConjunction
未使用の仮引数があるとエラーになります。
関数の引数の数や型を後から変更するのはめんどくさいので、なるべく早めに検出し修正してしまいたいものです。

== --noImplicitReturns

@<code>{--noImplicitReturns}オプションについて解説します。
関数やメソッドの返り値について、returnで値を返すパターンとreturnしないパターンがある場合、エラーにしてくれます。

例を見てみます（@<list>{noImplicitReturns/basic-invalid}）。

//list[noImplicitReturns/basic-invalid][暗黙のreturnを禁じる]{
#@mapfile(../code/tsc-options/noImplicitReturns/basic-invalid.ts)
// returnがない（暗黙的にundefinedが返る）パターンを検出してくれる
// error TS7030: Not all code paths return a value.
function a(v: number) {
  if (v < 0) {
    return "negative";
  } else if (0 < v) {
    return "positive";
  }

  // return がない！
}

function b() {
  // そもそも常にreturnがないならOK
}

export { }
#@end
//}

プログラミングのスタイルとして、elseの漏れや値の返し忘れがあるコードはミスである可能性が高いです。
そういったコードを書くとエラーになるのは便利ですね。

== --noImplicitThis

#@# @suppress JapaneseAmbiguousNounConjunction
@<code>{--noImplicitThis}オプションについておさらいします。
@<chapref>{types-advanced}の「関数のthisの型の指定」で述べたとおり、このオプションを利用すると、thisの型指定がない関数内でthisへアクセスするとエラーになります。

例を見てみます（@<list>{noImplicitThis/basic-invalid}）。

//list[noImplicitThis/basic-invalid][型指定無しのthisの利用を禁じる]{
#@mapfile(../code/tsc-options/noImplicitThis/basic-invalid.ts)
// 関数内部でのthisの型を偽の第一引数で指定
function testA(this: string) {
  console.log(this.toUpperCase());
}
testA.bind("TypeScript")();

function testB() {
  // --noImplicitThisオプション利用時、関数内でthisにアクセスすると怒られる
  // error TS2683: 'this' implicitly has type 'any'
  //   because it does not have a type annotation.
  console.log(this.toUpperCase());
}

export { testB }
#@end
//}

== --target

@<code>{--target}オプションについて解説します。
TypeScriptのコンパイルを行う際、ECMAScript 3（超古い！）、ECMAScript 5（古い！）、ECMAScript 2015（最近）のどのバージョンをターゲットとするかを指定します。
これは、"TypeScriptコードをどのバージョンで書くか"ではなく、"書いたTypeScriptをどのバージョンに変換するか"の指定です。
TypeScriptでは基本的に最新の記法で書き、ダウンパイル（古い書き方へ変換）します。

利用可能なオプションの値は次のとおりです。

 * es3
 * es5
 * es6 / es2015

基本的に、IE11などの少し古いブラウザのサポートを切らないのであればes5を選択すればよいでしょう。
es3の利用はもはやお勧めしません。

一部、Generatorやasync/awaitなどの記法はダウンパイルできません。
これらは2.1.0でサポートされる予定なので、延期されないようにみんなで祈りましょう。

== --module, --moduleResolution

@<code>{--module}オプションについて解説します。
TypeScriptはモジュールをコンパイルする際に、どの形式に変換するかを選ぶことができます。

利用可能なオプションの値は次のとおりです。

 * none
 * commonjs
 * system（SystemJS）
 * umd
 * es6 / es2015

これも明確な事情がない限り、今のところはcommonjsでよいでしょう。

#@# @suppress JapaneseAmbiguousNounConjunction
@<code>{--moduleResolution}オプションについて少し触れておきます。
モジュールの名前解決の方法について指定できます。

利用可能なオプションの値は次のとおりです。

 * node
 * classic（TypeScript 1.6以前の形式）

基本としてnode一択でよいでしょう。

前述の@<code>{--target}と自由に組み合わせることができるため、@<code>{--target es5}としつつ@<code>{--module es6}とすることもできます。
この組み合わせが可能になったのはTypeScript 2.0.0からなので、Rollup.js@<fn>{rollup.js}との組み合わせの運用などの検討はまだ未知数です。
TypeScript+Rollup.jsをプロジェクトに導入してみてブログ記事などにしてみると話題になるかもしれません。
お待ちしています！

//footnote[rollup.js][@<href>{http://rollupjs.org/}]

== --lib

#@# lib.d.tsの細分化と--libプロパティによる個別指定 に言及する
#@# OK REVIEW lc: ↑めっちゃ読みたい

@<code>{--lib}オプションについて解説します。
TypeScriptのコンパイルを行う際、標準の型定義として何を使うかを個別に指定できます。
たとえ、@<code>{--target es5}としてダウンパイルする場合でも、利用する型定義はes2015にできるのです。
最近はPromiseを使ったAPIは珍しくないですし、かつIE11でも動かしたい場合というのはザラにあります。

利用可能なオプションの値は次のとおりです。
複数指定したい場合、コマンドラインオプションの場合は@<code>{,}で区切ります。
tsconfig.jsonの場合は素直に配列にしましょう。

#@# prh:disable
 * dom
 * webworker
 * es5
 * es6 / es2015
 * es2015.core
 * es2015.collection
 * es2015.iterable
 * es2015.promise
 * es2015.proxy
 * es2015.reflect
 * es2015.generator
 * es2015.symbol
 * es2015.symbol.wellknown
 * es2016
 * es2016.array.include
 * es2017
 * es2017.object
 * es2017.sharedmemory
 * scripthost

自分のプロジェクトが何用途なのかを考え、適切なものを選びましょう。
たとえば、Node.jsなプロジェクトであればHTMLElementなどは不要でしょうからdomはいらないです。
多くのプロジェクトではes2017か、+domの指定があれば十分でしょう。

es2017を利用する場合はes2017の型定義にes2016の参照が含まれます。
どの標準型定義ファイルが何を参照しているかが気になる場合は直接型定義ファイルを見るか、@<code>{--listFiles}オプションをつけてコンパイルしてみたりするとよいでしょう。

== --forceConsistentCasingInFileNames

@<code>{--forceConsistentCasingInFileNames}オプションについて解説します。
このオプションを有効にすると、ファイル名の参照について、大文字小文字の食い違いがあるとエラーにします。
macOSのような非ケースセンシティブな環境と、Linuxのようなケースセンシティブな環境が混在しているとき、macOSではエラーにならないけどLinuxではエラーになる…のようなシチュエーションを防止してくれます。
チーム内でmacOSに統一されていても、外部の人やCIサーバなどはLinuxを使っている場合などはかなり多いため、とりあえず有効にしてしまってよいでしょう。
