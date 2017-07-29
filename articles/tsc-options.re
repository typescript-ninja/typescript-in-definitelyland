={tsc-options} オプションを知り己のコードを知れば百戦危うからず

本章ではtscのコマンドラインオプションについて解説していきます。
すべてを網羅することはしませんが、いくつかの重要なオプションを知ることができます。

本章記載のオプションはtsconfig.jsonのcompilerOptionsに記載可能なプロパティ名と同一です。
tsconfig.jsonでは短縮形（@<code>{-d}や@<code>{-p}）は利用できないことに注意してください。

ここに記載されていないオプションで知りたいものがあれば本書のIssue@<fn>{issue}にお寄せください。

//footnote[issue][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

#@# TODO 以下のオプションについてじわじわ書く
#@#   --declaration, --declarationDir
#@#   --listFiles
#@#   --noFallthroughCasesInSwitch
#@#   --pretty
#@#   --traceResolution
#@#   --allowJs
#@#   --experimentalDecorators
#@#   --emitDecoratorMetadata
#@#   --sourceMap, --inlineSources, --inlineSourceMap
#@#   --skipLibCheck, --skipDefaultLibCheck
#@#   --out, --outDir, --outFile
#@#   --noImplicitUseStrict
#@#   --checkJs in 2.3RC

=={init} --init

#@# tsc --init の出力が整理されてコメントとかも付くように Enhanced tsc --init output in 2.3RC
#@# tsconfig.jsonでextendsが使えるようになった (Configuration inheritance) in 2.1.1

@<code>{--init}オプションについて解説します。
このオプションを使うと、TypeScriptでコードを書き始める際に必要なtsconfig.jsonの雛形を生成します。
生成されたファイルはtscコマンドがコンパイルを行うときに読み込まれます。
後述の@<code>{--project}オプションも参照してみてください。

TypeScriptではプロジェクトのビルドに必要なコンパイルオプションや、コンパイル対象の指定などをtsconfig.jsonファイルにまとめていきます。
このファイルはすべてのツールやIDE・エディタ間で共通に利用できる設定ファイルになるため、大変役立ちます。

まずは@<code>{tsc --init}コマンドで生成されるtsconfig.jsonを見てみます（@<list>{init/default/tsconfig.json}）。

//list[init/default/tsconfig.json][生成されたtsconfig.json]{
#@mapfile(../code/tsc-options/init/default/tsconfig.json)
{
  "compilerOptions": {
    // 様々なオプションの説明が出力されるが紙面の都合でカットです！
    "target": "es5",
    "module": "commonjs",
    "strict": true // 次の4オプションを一括で有効にする
    // "noImplicitAny": true,
    // "strictNullChecks": true,
    // "noImplicitThis": true,
    // "alwaysStrict": true,
  }
}
#@end
//}

tsconfig.jsonに記述可能なプロパティは、おおむね次の5つです。

 * extends
 * compilerOptions
 * files
 * include
 * exclude

extendsには、相対パスまたは絶対パスで別のtsconfig.jsonを指定し、設定内容を引き継ぐことができます。
拡張子の@<code>{.json}は省略可能です。

compilerOptionsには、コンパイル時に利用するオプションを指定します。
コンパイルオプションの名前とcompilerOptionsに記載可能なプロパティ名は一致しています。
たとえば@<list>{init/default/tsconfig.json}は@<code>{tsc --module commonjs --target es5 --strict}という意味になります。

#@# @suppress SentenceLength CommaNumber
tsconfig.jsonで利用可能なcompilerOptionsについては、本章を読むか@<href>{http://www.typescriptlang.org/docs/handbook/tsconfig-json.html,公式ハンドブックの解説}@<fn>{handbook-tsconfig}や@<href>{http://json.schemastore.org/tsconfig,JSON Schemaの定義}@<fn>{schemastore-tsconfig}を参照してください。

//footnote[handbook-tsconfig][@<href>{http://www.typescriptlang.org/docs/handbook/tsconfig-json.html}]
//footnote[schemastore-tsconfig][@<href>{http://json.schemastore.org/tsconfig}]

残る3つはコンパイル対象ファイルを指定するためのプロパティです。
3つすべてに共通の挙動として、コンパイル対象に明示的に含めない場合でもTypeScriptコンパイラが自動的に依存関係を解決し必要なファイルを対象に含める場合があります。
この機能は歓迎すべき機能で、余計な設定の手間を減らしてくれます。

filesには、コンパイル対象にするファイルを列挙します。
時期的に後述するincludeやexcludeよりも先に実装されたのですが、現状あまりにも面倒くさいので大抵使いません。

include、excludeはコンパイル対象とするファイルやフォルダを大まかに指定します。
includeで調べるディレクトリやファイル、excludeで除外するディレクトリやファイルを指定し、この組み合わせで処理対象を限定します。
ディレクトリを指定すると、そこに含まれるすべての.tsファイルと.tsxファイルが対象になります。
簡単なワイルドカードも利用できます。

includeとexcludeをまったく指定しない場合、TypeScriptコンパイラは処理中のディレクトリやサブディレクトリ配下を調べ、すべての.tsファイルや.tsxファイルをコンパイルしようとします。
これにはnode_modulesなども含まれてしまうため、大抵は不都合です。

includeとexcludeの利用例を見てみましょう（@<list>{init/globSyntax/tsconfig.json}）。

//list[init/globSyntax/tsconfig.json][使えるワイルドカードの例]{
#@mapfile(../code/tsc-options/init/globSyntax/_tsconfig.json)
{
    "compilerOptions": {
        "listFiles": true, // コンパイルの処理対象を表示する
        "noEmit": true     // コンパイル結果の.jsファイルなどを出力しない
    },
    "include": [
        /// ディレクトリのワイルドカード
        // /**/ で全てのサブフォルダ
        "libA/**/*",
        // /*/ で直下のサブフォルダ
        "libB/*/*",

        /// 文字のワイルドカード
        // * で0文字以上にマッチする
        "libC/*.ts",
        // ? で1文字にマッチする
        "libD/?b.ts"
    ],
    "exclude": [
        "node_modules",
        // 除外でも同じようにワイルドカードが使える
        "libD/b*.ts"
    ]
}
#@end
//}

例で示したtsconfig.jsonを利用してみます。
@<list>{init/globSyntax/result}はプロジェクト内部に存在するts関連ファイルと、ファイルが処理されるかされないかを突き合わせたものです。

//list[init/globSyntax/result][ファイル一覧とマッチの結果]{
libA/a/index.ts    # 対象になる
libA/a/b/index.ts  # 対象になる
libB/index.ts      # 対象にならない
libB/a/index.ts    # 対象になる
libC/index.ts      # 対象になる
libC/index.tsx     # 対象にならない
libD/ab.ts         # 対象になる
libD/ac.ts         # 対象にならない
libD/bb.ts         # 対象にならない（exclude）
//}

なかなか素直な結果です。
単にワイルドカードであって、正規表現で記述できるわけではない点に注意しましょう。

=={project} --project

@<code>{--project}オプションについて解説します。
短縮記法の@<code>{-p}も利用できます。

通常、引数無しで@<code>{tsc}コマンドを実行するとカレントディレクトリかそれより上にあるtsconfig.jsonを探して実行してくれます。
その挙動だと困る場合にこのオプションを使います。
オプションの値としてtsconfig.jsonがあるディレクトリか、tsconfig.jsonのパスを指定します。
具体的には@<code>{tsc -p ./}または@<code>{tsc -p ./tsconfig.json}とします。

tsconfig.jsonではない名前のファイルを使って、プロジェクト内に複数のビルド構成を作ることもできます。
しかし、その場合IDE・エディタ側が設定をうまくハンドリングできないことが多いため、基本的には努力して1プロジェクトにつき1つのtsconfig.jsonとなるようにしましょう。

gulpやwebpackなどのタスクランナーやバンドラを使う場合もtsconfig.jsonを用意し@<code>{--project}オプションのみでコンパイルできる環境を維持するとよいでしょう。

=={noImplicitAny} --noImplicitAny

TypeScriptコンパイラの最重要オプション、@<code>{--noImplicitAny}について解説します。
このオプションは"暗黙的なanyを禁ずる"の名が表すとおり、型推論の結果、暗黙的に変数の型がanyになった場合、エラーとしてくれます。

@<list>{noImplicitAny/basic-invalid.ts}のようなメソッドの返り値の型を書き忘れた！という脇の甘いコードをコンパイルしてみます。

//list[noImplicitAny/basic-invalid.ts][メソッドの返り値を書き忘れた！]{
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

返り値の型を書いていなかったり、関数の仮引数の型が指定されていなかったりしたため暗黙的にanyになってしまいました。
このようなときに、それはダメだ！とコンパイラが教えてくれます。
anyが紛れ込んで型チェックが意味を成さなくなるとTypeScriptの意義が薄れてしまいます。
型定義ファイルを書くときも、通常の開発時も、常に@<code>{--noImplicitAny}を使うようにしましょう。

=={strict} --strict

#@# --strict オプションの追加 New --strict master option in 2.3RC
@<code>{tsc --init}で生成したときにデフォルトで有効になっているオプションです。
このオプションは後述の@<code>{--strictNullChecks}、@<code>{--noImplicitAny}、@<code>{--noImplicitThis}、@<code>{--alwaysStrict}の4オプションを一括で有効にします。

これがデフォルトで有効になっているということは、TypeScript開発チームは皆さんにガッツリ堅牢なコードを書いていってほしい、と思っているということですね。
新規にプロジェクトを作成する場合などは必ず有効にして冒険の旅に出るようにしましょう。

=={strictNullChecks} --strictNullChecks

@<code>{--strictNullChecks}オプションはnullやundefinedの扱いについてより厳格にし、変数の中身についての曖昧さを積極的に排除するよう振る舞います。
nullやundefinedを許容したい場合、union typesや省略可能引数を使って明示的にnullやundefinedとなる可能性を示さなければなりません。

まずはオプションありの例です（@<list>{strictNullChecks/basic.ts}）。

//list[strictNullChecks/basic.ts][危険なコードがいち早く発見される]{
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

// 非null指定演算子（!）で無理やりコンパイルを通すこともできる
objB!.getTime();

export { }
#@end
//}

nullやundefinedに対するアクセスが多くの場合未然に防がれ、"コンパイルが通ればもう安全"なコードが書きやすいことがわかります。

非null指定演算子（@<code>{!}）については@<chapref>{types-advanced}の@<hd>{types-advanced|non-null-assertion-operator}で触れました。

さて次はオプションなしの例です（@<list>{strictNullChecks/withoutStrictNullCheck-ignore.ts}）。

//list[strictNullChecks/withoutStrictNullCheck-ignore.ts][実行時にエラーになるかも]{
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
変数の中身を容易にnullやundefinedにできてしまいます。
きっちりコードを書けば、オプション無しでも堅牢なアプリケーションを構築することは不可能ではありません。
しかし、それはプログラマの不断の努力の上にしか成り立ちません。
そんな苦労をするよりは、コンパイラにしっかりチェックしてもらえたほうがコードの堅牢さが確かなものになりますね。

もちろん本書も@<code>{--strictNullChecks}オプションを常に有効にしている前提で書いています。

=={noUnusedLocals} --noUnusedLocals

@<code>{--noUnusedLocals}オプションは、使っていないローカル変数があればエラーにしてくれます。
本書のサンプルコードでも有効になっているため、エラー消しのために無意味にexportしている箇所がありました。

例を見てみます（@<list>{noUnusedLocals/basic-invalid.ts}）。

//list[noUnusedLocals/basic-invalid.ts][未使用変数はちゃんと消そう]{
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

=={noUnusedParameters} --noUnusedParameters

@<code>{--noUnusedParameters}オプションは関数やメソッドの引数に使っていないものがあるとエラーにしてくれます。
エラーにせず残しておきたい場合、変数名の頭に@<code>{_}（アンダースコア）をつけることでエラーを抑制できます。

例を見てみます（@<list>{noUnusedParameters/basic-invalid.ts}）。

//list[noUnusedParameters/basic-invalid.ts][使っていない仮引数はできれば削除したい]{
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

=={noImplicitReturns} --noImplicitReturns

@<code>{--noImplicitReturns}オプションについて解説します。
関数やメソッドの返り値について、returnで値を返す場合とreturnしない場合が混在したとき、エラーになります。

例を見てみます（@<list>{noImplicitReturns/basic-invalid.ts}）。

//list[noImplicitReturns/basic-invalid.ts][暗黙のreturnを禁じる]{
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

プログラミングのスタイルとして、elseの漏れや値の返し忘れがあるコードはミスがある可能性が高いです。
そういったコードを書くとエラーとして検出できるのは便利ですね。

=={noImplicitThis} --noImplicitThis

#@# @suppress JapaneseAmbiguousNounConjunction
@<code>{--noImplicitThis}オプションは
@<chapref>{types-advanced}の「関数のthisの型の指定」で述べたとおり、thisの型が不明瞭な関数内でthisへアクセスするとエラーになります。

例を見てみます（@<list>{noImplicitThis/basic-invalid.ts}）。

//list[noImplicitThis/basic-invalid.ts][型指定無しのthisの利用を禁じる]{
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

=={alwaysStrict} --alwaysStrict

#@# 無条件にstrict modeにするオプションの追加 (New --alwaysStrict) in 2.1.1
@<code>{"use strict";}ディレクティブを常時出力するオプションです。
ECMAScriptのモジュールを使ったときのように言語仕様上必ず@<code>{"use strict";}を出してくれる場合もありました。
このオプションを使うとそれが常時適用される状態になります。
今の時代、strictモードをあえて使わないという選択肢はないはずですのでこれも常時使うことをお勧めするオプションです。

=={target} --target

#@# es2016, 2017 をターゲットに追加 (Support for --target ES2016 and --target ES2017) in 2.1.1
#@# --target ESNext がさらに追加された Support for --target ES2016, --target ES2017 and --target ESNext in 2.1.4

@<code>{--target}オプションについて解説します。
短縮記法で@<code>{-t}も利用できます。
TypeScriptのコンパイルを行う際、どのバージョンのECMAScriptをターゲットとするかを指定します。
これは、"TypeScriptコードをどのバージョンで書くか"ではなく、"書いたTypeScriptをどのバージョンに変換するか"の指定です。
TypeScriptでは基本的に最新の記法で書き、ダウンパイル（古い書き方へ変換）します。

利用可能なオプションの値は次のとおりです。

 * es3
 * es5
 * es6 / es2015
 * es2016
 * es2017
 * esnext

基本的に、IE11などの少し古いブラウザのサポートを切らないのであればes5を選択すればよいでしょう。
es3の利用はもはやお勧めしません。

#@# --target es3 と --target es5 でもジェネレータが使えるようになった Generator support for ES3/ES5 in 2.3RC
GeneratorやIteratorをes5のコードにダウンパイルしたい場合は@<code>{--downlevelIteration}を併せて指定します。

=={module-moduleResolution} --module、--moduleResolution

@<code>{--module}オプションについて解説します。
短縮記法で@<code>{-m}も利用できます。
TypeScriptはモジュールをコンパイルする際に、どの形式に変換するかを選ぶことができます。

利用可能なオプションの値は次のとおりです。

 * none
 * commonjs
 * system（SystemJS）
 * umd
 * es6 / es2015
 * esnext

これも明確な事情がない限り、今のところはcommonjsでよいでしょう。
前述の@<code>{--target}と自由に組み合わせることができるため、@<code>{--target es5}としつつ@<code>{--module esnext}とすることもできます。

#@# @suppress JapaneseAmbiguousNounConjunction
@<code>{--moduleResolution}オプションについて少し触れておきます。
モジュールの名前解決の方法について指定できます。

利用可能なオプションの値は次のとおりです。

 * node
 * classic（TypeScript 1.6以前の形式）

基本的にはnode一択でよいでしょう。

=={lib} --lib

#@# lib.d.tsの細分化と--libプロパティによる個別指定 に言及する
#@# OK REVIEW lc: ↑めっちゃ読みたい

@<code>{--lib}オプションについて解説します。
TypeScriptのコンパイルを行う際、標準の型定義として何を使うかを個別に指定できます。
たとえ@<code>{--target es5}としてダウンパイルする場合でも、@<code>{--lib es2015}オプションで使用する型定義をes2015にできます。
最近はPromiseを使ったAPIは珍しくないですし、かつIE11でも動かしたい場合というのはザラにあります。

#@# OK mhidaka 「利用する型定義はes2015にできるのです。」利用する型定義にes2015を指定できる？「できるのです」の「の」を消したい。ののわさん！

利用可能なオプションの値は次のとおりです。
複数指定したい場合、コマンドラインオプションの場合は@<code>{,}で区切ります。
tsconfig.jsonの場合は素直に配列にしましょう。

#@# prh:disable
 * es5
 * es6 / es2015
 * es7 / es2016
 * es2017
 * esnext
 * dom
 * dom.iterable
 * webworker
 * scripthost
 * es2015.core
 * es2015.collection
 * es2015.generator
 * es2015.iterable
 * es2015.promise
 * es2015.proxy
 * es2015.reflect
 * es2015.symbol
 * es2015.symbol.wellknown
 * es2016.array.include
 * es2017.object
 * es2017.sharedmemory
 * es2017.string
 * es2017.intl
 * esnext.asynciterable

自分のプロジェクトの用途を考え、適切なものを選びましょう。
たとえばNode.jsプロジェクトであればHTMLElementなどは不要でしょうからdomは不要です。
多くのプロジェクトではes2017か、＋domの指定があれば十分でしょう。

es2017を利用する場合はes2017の型定義にes2016の参照が含まれます。
どの標準型定義ファイルが何を参照しているかが気になる場合は直接型定義ファイルを見るか、@<code>{--listFiles}オプションをつけてコンパイルしてみたりするとよいでしょう。

=={types} --types

@<code>{--types}オプションについて解説します。
TypeScriptのコンパイルを行う際、参照するべき型定義ファイルを明示的に指定します。

通常、ソースコード中でimportしたモジュールのための型定義ファイルはルールに従って発見されるため、この設定は不要です。
しかしながら、Node.jsやテスティングフレームワークのmochaなど、実行環境に初めからセットアップされているものはimportの機会がありません。
このため、何らかの方法でコンパイラに環境の情報を伝える必要があります。
そこで使うのが@<code>{--types}オプションです（@<list>{types/tsconfig.json}）。

//list[types/tsconfig.json][types指定でnodeとmochaを参照する]{
#@mapfile(../code/tsc-options/types/tsconfig.json)
{
  "compilerOptions": {
    "target": "es5",
    "module": "commonjs",
    "strict": true,
    "types": [
      "node",
      "mocha"
    ]
  }
}
#@end
//}

ここで利用する型定義ファイルは@<chapref>{at-types}で紹介する@typesからnpmで引っ張ってくるのがお手軽です。
@<code>{npm install --save-dev @types/node @types/mocha}という具合ですね。

=={forceConsistentCasingInFileNames} --forceConsistentCasingInFileNames

@<code>{--forceConsistentCasingInFileNames}オプションについて解説します。
このオプションを有効にすると、ファイル名の参照について大文字小文字の食い違いがあるとエラーにします。

macOSのような非ケースセンシティブな環境と、Linuxのようなケースセンシティブな環境が混在しているとき、macOSではエラーにならないけどLinuxではエラーになる…のようなシチュエーションを防止してくれます。
チーム内でmacOSに統一されていても、外部の人やCIサーバなどはLinuxを使っている場合などはかなり多いため、とりあえず有効にしてしまってよいでしょう。

=={noEmitOnError-noEmit} --noEmitOnError、--noEmit

@<code>{--noEmitOnError}オプションと@<code>{--noEmit}オプションについて解説します。
このオプションはコンパイルの出力となる.jsファイル、.js.mapファイル、.d.tsファイルを生成するか否かを制御します。

@<code>{--noEmitOnError}はコンパイルが成功した時のみファイルを生成します。
これはビルドのパイプラインを組む時の「コンパイル成功したつもりだが実は失敗してた。後続のタスクが続いてしまい失敗を見逃した」というパターンに有効です。
前回の生成物を削除してからコンパイルするようにすることで.jsファイルが必要なステップで処理全体が確実に落ちるようにできます。
「そんなダメタスクは作らないよ！」と思うかもしれないですが、これが案外やりがちです。
プロジェクトの健康を保つためにも、@<code>{--noEmitOnError}オプションは常に有効でよいでしょう。

#@# prh:disable
@<code>{--noEmit}オプションはコンパイルが成功しようが失敗しようが、常に成果物を何も生成しません。
@<code>{tsc}でファイルを生成する場合と違う手順でのみビルドするとき、例えばwebpackでts-loaderを使っているプロジェクトなどで有効です。
@<code>{tsc --noEmit}とすることでTypeScriptのコンパイルエラーのみをチェックできます。
これはビルドタスク全体を走らせるよりも手短で、作業ディレクトリに不要なファイルを撒き散らすこともありません。

#@# @suppress SectionLength
== --importHelpersと--noEmitHelpers

#@# ヘルパライブラリの話 in 2.1.1
TypeScriptで@<code>{--target es5}などでダウンパイルした場合、ヘルパ関数が自動生成されます。
例えば、クラスの継承を行ったときは@<code>{__extends}関数が生成されますね（@<list>{helper-__extends}）。

//list[helper-__extends][生成される__extends関数]{
var __extends = (this && this.__extends) || (function () {
  var extendStatics = Object.setPrototypeOf ||
    ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
    function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
  return function (d, b) {
    extendStatics(d, b);
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
  };
})();
//}

@<code>{--importHelpers}や@<code>{--noEmitHelpers}を併用すると、ヘルパ関数がファイル毎に生成されるのを抑制し、1つにまとめることができます。

@<code>{--importHelpers}を利用する場合、@<code>{npm install --save tslib}が必要になります。
ヘルパ関数を生成する代わりにtslibパッケージ内のヘルパ関数を使うようにする構造です（@<list>{helpers/importHelpers/basic.js}）。

//list[helpers/importHelpers/basic.js][tslibが利用される例]{
#@mapfile(../code/tsc-options/helpers/importHelpers/basic.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var tslib_1 = require("tslib");
var Base = (function () {
    function Base() {
    }
    return Base;
}());
exports.Base = Base;
var Inherit = (function (_super) {
    tslib_1.__extends(Inherit, _super);
    function Inherit() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    return Inherit;
}(Base));
exports.Inherit = Inherit;
#@end
//}

@<code>{--noEmitHelpers}を利用する場合、単純にヘルパ関数が出力されなくなります。
つまり、ヘルパ関数がグローバルな空間に定義された状態を作り、生成されたJSコードから参照できるようにしてやる必要があります（@<list>{helpers/noEmitHelpers/basic.js}）。

//list[helpers/noEmitHelpers/basic.js][唐突に参照されるヘルパ関数]{
#@mapfile(../code/tsc-options/helpers/noEmitHelpers/basic.js)
"use strict";
var Base = (function () {
    function Base() {
    }
    return Base;
}());
var Inherit = (function (_super) {
    __extends(Inherit, _super);
    function Inherit() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    return Inherit;
}(Base));
#@end
//}

現代的にはJSコードは1つにbundleし、gzipで圧縮して転送されるユースケースが多いでしょう。
つまり、tslibを使ってもさほど有利にはならないかもしれません。
TypeScriptが生成するデフォルトのヘルパ関数から実装を差し替えたい場合に使えるかもしれませんね。
そして、一見便利そうなヘルパライブラリなのですがtscの実装は若干バギー（というか不親切）です。

tslibはモジュールの形でimportされるため、moduleをcommonjsやesnextなどに指定している場合でもimport句またはexport句がない場合、tslibの参照が行われません。
@<code>{--importHelpers}と@<code>{--noEmitHelpers}を併用しているとtslibの参照が行われない上にヘルパ関数の生成も行われなくなります。
両オプションを利用する場合、安全側に倒すため、どちらか片方のオプションだけを利用するほうが安全でしょう。

なお、TypeScript 2.3.2以前でtslibを使う場合、tslibのバージョンは1.6.1を使います。

=={plugin} pluginsの設定

#@# --plugins オプションの追加 Language Service Extensibility in 2.3RC

pluginsも仕組みとして面白いので概要だけ言及しておきます。
@<chapref>{typescript-as-a-tool}でも少し扱います。

pluginsはその名の通りプラグインなのですが、現時点では効果を及ぼすことができる対象はエディタやIDE上のみです。
コンパイル時の動作には影響を及ぼすことができません。
そのため、tscのオプションとしてはpluginsは存在せず、tsconfig.jsonの設定項目としてのみ存在します。

このオプションを設定しておくと、エディタでTypeScriptの入力補完やコンパイルエラーの表示動作を拡張できます。
現時点で実際に使える（と思われる）npmパッケージを次に挙げます@<fn>{plugins-in-real-world}。

//footnote[plugins-in-real-world][@<href>{https://github.com/Microsoft/TypeScript/wiki/Writing-a-Language-Service-Plugin#real-world-plugins}]

 * @angular/language-service
 * ts-graphql-plugin
 * tslint-language-service@<fn>{tslint-plugins-issue}

#@# NOTE https://github.com/Microsoft/TypeScript/issues/15460
//footnote[tslint-plugins-issue][@<href>{https://github.com/palantir/tslint/issues/2282}]

また、筆者の作った役にも立たないプラグインを@<code>{@vvakame/typescript-plugin-example}@<fn>{vvakame-plugin-example}として公開しています。
これは入力補完候補の説明文とかクイックインフォの説明文の末尾に猫の絵文字などを出すだけのものです。
設定例と動作イメージの紹介にちょうどいいでの確認してみます。
tsconfig.jsonの内容は@<list>{plugins/tsconfig.json}で、動作例は@<img>{plugins}です。

//footnote[vvakame-plugin-example][@<href>{https://www.npmjs.com/package/@vvakame/typescript-plugin-example}]

//list[plugins/tsconfig.json][pluginsの設定例]{
#@mapfile(../code/tsc-options/plugins/tsconfig.json)
{
  "compilerOptions": {
    "target": "esnext",
    "module": "esnext",
    "strict": true,
    "plugins": [
      {
        // name は全プラグイン共通で必須
        "name": "@vvakame/typescript-plugin-example",
        // プラグイン固有の設定を行う場合がある
        "verbose": true,
        "goody": "(`_´)"
      }
    ]
  }
}
#@end
//}

//image[plugins][tsconfig.jsonの設定内容を反映して顔文字が出ている]{
にゃーん
//}

#@# OK mhidaka にゃーん

簡単にエディタの機能を拡張できるので楽しいですね。
まだまだこの仕組を使っているパッケージは少ないので、よいアイディアがあればどんどんやってみましょう。
