= TypeScriptの基本

#@# TODO enum
#@# TODO const enum
#@# TODO 暗黙的なインデックスシグニチャ（Implicit index signatures）

#@# prh:disable
@<chapref>{prepared-to-typescript}で述べたとおり、本書ではECMAScript 2015の文法・仕様についてすべてを解説することはしません。
ECMAScript 2015の知識はどんどん広まってきていますし、今後は基本的なJavaScriptの知識になっていくでしょう。
ECMAScriptの知識は、TypeScript固有の知識ではないですからね。

この章ではTypeScriptでの基本的な構文を解説します。
まずは、TypeScriptを使うのに必要最低限の知識を身につけていきましょう。

型の基本は@<chapref>{types-basic}を、難しいことや便利なことは@<chapref>{types-advanced}を見てください。
既存のJavaScriptな資産やライブラリを使いたい場合は@<chapref>{definition-file}を見てください。

#@# @suppress CommaNumber
また、本書は@<code>{--noImplicitAny}、@<code>{--strictNullChecks}、@<code>{--noImplicitReturns}、@<code>{--noImplicitThis}を有効にした状態を基本として解説します。
各オプションの詳細については@<chapref>{tsc-options}を参照してください。

#@# OK REVIEW lc: tsconfigの設定状態を出したほうがわかりやすい？
#@# vv: ここはこのままにしておきます…。

#@# @suppress ParagraphNumber SectionLength
== 変数

#@# @suppress JapaneseAmbiguousNounConjunction
TypeScriptの変数宣言はおおむねJavaScriptと同じです。
違うのは、@<list>{variable/withAnnotations}のように変数名の後に@<code>{: 型名}という形式でその変数がどういう型の値の入れ物になるのか指定できるところです@<fn>{suppress-warning}。
これを@<kw>{型注釈,type annotations}と呼びます。

//footnote[suppress-warning][コンパイルエラーを消すため、今後もサンプルコード中に一見意味のなさそうな export {} などが表れます]

//list[variable/withAnnotations][型注釈付きの変数]{
#@mapfile(../code/typescript-basic/variable/withAnnotations.ts)
let str: string;
let num: number;
let bool: boolean;

let func: Function;
let obj: any; // なんでも型

str = "文字列";
num = 1;
bool = true;
func = () => { };
obj = {};

export { }
#@end
//}

型注釈の何が嬉しいかというと、型に反するようなコードを書くとtscコマンドを使ってコンパイルしたときにコンパイルエラーになるのです。
たとえば@<list>{variable/withAnnotations-invalid}のように、整合性がとれていない箇所がTypeScriptによって明らかにされます。
安心安全！

//list[variable/withAnnotations-invalid][型注釈に反することをやってみる]{
#@mapfile(../code/typescript-basic/variable/withAnnotations-invalid.ts)
let str: string;
// 文字列は数値と互換性がない！
// error TS2322: Type 'number' is not assignable to type 'string'.
str = 1;

let num: number;
// 数値は真偽値と互換性がない！
// error TS2322: Type 'boolean' is not assignable to type 'number'.
num = true;

let bool: boolean;
// 真偽値は文字列と互換性がない！
// error TS2322: Type 'string' is not assignable to type 'boolean'.
bool = "str";
#@end
//}

安心安全なのはよいですが、わかりきったことを書くのは省きたいと思うのはエンジニアの性分でしょう。
そんなあなたのために、TypeScriptは型推論の機能を備えています。
@<list>{variable/withInitializer}のように、型注釈を書かずに変数定義と初期化を同時に行えます。

//list[variable/withInitializer][初期化付き変数 = 最強]{
#@mapfile(../code/typescript-basic/variable/withInitializer.ts)
let str = "string";
let num = 1;
let bool = true;

let func = () => {
};
let obj = {};

export { str, num, bool, func, obj }
#@end
//}

これで手で型注釈を与えずに済みます。
しかも、書き方がJavaScriptと全く同じになりました。
楽に書ける上に実行前にコンパイルの段階で不審な臭いのするコードを発見できるようになる、第一歩です。

=={class} クラス

#@# @suppress ParagraphNumber SectionLength
=== 普通のクラス

ECMAScript 2015より導入されたクラス構文についても各所に型注釈可能な構文が追加されています（@<list>{class/basic}）。

#@# OK REVIEW lc: ES.next的には「instance fields」と「static properties」っぽいんですが、TSでの呼称は「インスタンス変数」と「クラス変数」なんですか？ https://github.com/jeffmo/es-class-public-fields
#@# OK REVIEW lc: spec読んだら「class members」と「static class members」だった
#@# vv: ES.next的には定まった呼称はなさそう syntax的にはClassElement。若干Java方言だけどここはとりあえずこのままにしときます。

//list[class/basic][さまざまなクラス要素]{
#@mapfile(../code/typescript-basic/class/basic.ts)
class Base {
  // インスタンス変数
  numA: number;
  strA = "string";
  public numB: number;
  private numC: number;
  protected numD: number;
  regexpA?: RegExp;

  // クラス変数
  static numA: number;
  public static numB: number;
  private static numC: number;
  protected static numD: number;
  static regexpA?: RegExp;

  // コンストラクタ
  constructor(boolA: boolean,
    public boolB: boolean,
    private boolC: boolean,
    protected boolD: boolean) {
    // エラー消し 一回も使われない可能性があると怒られる
    console.log(boolA, this.numC, this.boolC, Base.numC);
  }

  // メソッド
  hello(word: string): string {
    return "Hello, " + word;
  }

  // get, setアクセサ
  // コンパイル時に --target es5 以上が必要
  /** @internal **/
  private _date: Date;
  get dateA(): Date {
    return this._date;
  }
  set dateA(value: Date) {
    this._date = value;
  }

  optional() {
    // 省略可能なプロパティは値の存在チェックが必要
    if (this.regexpA != null) {
      this.regexpA.test("Hi!");
    }
    if (Base.regexpA != null) {
      Base.regexpA.test("Hi!");
    }
  }
}

let obj = new Base(true, false, true, false);
obj.numA;
obj.strA;
obj.numB;
// obj.numC; // private   なメンバにはアクセスできない
// obj.numD; // protected なメンバにもアクセスできない
obj.boolB;
// obj.boolC; // private   なメンバにはアクセスできない
// obj.boolD; // protected なメンバにもアクセスできない
obj.hello("TypeScript");
obj.dateA = new Date();
obj.dateA;

export { }
#@end
//}

上から順に見て行きましょう。

まずはクラス変数、インスタンス変数です。
クラスそのものやインスタンスに紐づく変数です。
JavaScriptっぽくいうとプロパティですね。

#@# @suppress CommaNumber
アクセス修飾子として、private、public、protectedなどの可視性を制御するアクセス修飾子を利用できます。
何も指定していないとき、デフォルトの可視性はpublicになります。

コンパイル後のJSファイルを見るとわかりますが@<code>{any}にキャストするとそれらの要素にアクセスできてしまうので、アクセス修飾子をつけたから外部からの変更を100%防げる！と考えるのは禁物です。
そのため筆者はアクセス修飾子を使うだけではなく、privateな要素のprefixに_を使い、ドキュメントコメントに@<code>{@internal}をつけるといった工夫をしています。

#@# OK REVIEW lc: s/100/100%/

また、プロパティには省略可能（optional）かを明示する@<kw>{?}を指定できます。
コンストラクタ内の処理が終わるまでの間に値がセットされないプロパティについては、省略可能である旨を指定したほうがよいかもしれません。
#@# クラスのプロパティが省略可能かどうか指定の追加（Optional properties in classes）
#@# OK REVIEW lc: s/旨/旨を/

次はコンストラクタです。
コンストラクタ自体にも前述のprivate、protectedなどのアクセス修飾子を利用できます。

引数にアクセス修飾子をあわせて書くと、インスタンス変数としてその値が利用可能になります。
これを@<kw>{引数プロパティ宣言,parameter property declaration}と呼びます。
引数プロパティ宣言はTypeScript固有の記法です。
そもそも、JavaScriptにはアクセス修飾子がありませんからね。
@<list>{class/constructor.ts}のようなコードを書くと@<list>{class/constructor.js}のようなJavaScriptが出てきます。

//list[class/constructor.ts][引数プロパティ宣言！]{
#@mapfile(../code/typescript-basic/class/constructor.ts)
class Sample {
  constructor(public str: string) {
  }
}

let obj = new Sample("TypeScript");
// TypeScript と表示される
console.log(obj.str);

export { }
#@end
//}

//list[class/constructor.js][コンパイルするとこんなの]{
#@mapfile(../code/typescript-basic/class/constructor.js)
"use strict";
class Sample {
    constructor(str) {
        this.str = str;
    }
}
let obj = new Sample("TypeScript");
// TypeScript と表示される
console.log(obj.str);
#@end
//}

@<list>{class/basic}の解説に戻ります。
次はメソッドです。
これも特に特筆すべき要素はありませんね。普通です。

#@# @suppress SentenceLength CommaNumber ParenthesizedSentence
最後にget、setアクセサです。
これを含んだコードをコンパイルするときは、@<code>{--target es5}以上を指定します。
get、setアクセサを使うと、getterしか定義していない場合でもプログラム上は値の代入処理が記述できてしまうので、"use strict"を併用して実行時にエラーを検出するようにしましょう。

次に、クラスの継承も見て行きましょう（@<list>{class/inherit}）。
superを使い親クラスのメソッドを参照することも普通にできます。

//list[class/inherit][普通に継承もあるよ]{
#@mapfile(../code/typescript-basic/class/inherit.ts)
class Base {
  greeting(name: string) {
    return "Hi! " + name;
  }
}

class Inherit extends Base {
  greeting(name: string) {
    return super.greeting(name) + ". How are you?";
  }
}

let obj = new Inherit();
// Hi! TypeScript. How are you? と出力される
console.log(obj.greeting("TypeScript"));

export { }
#@end
//}

TypeScript以外のオブジェクト指向言語でもいえることですが、なんでもかんでも継承すればいいや！という考えはよくありません。
頑張ってオブジェクト指向に適した設計を行いましょう。

=== 抽象クラス（Abstract Class）

ECMAScriptにはない機能として、抽象クラスが作成できます。
抽象クラスは単独ではインスタンス化できません。
その代わり、抽象クラスを継承したクラスに対して、abstractで指定した要素の実装を強制できます。
例を見てみましょう（@<list>{class/abstract.ts}）。

//list[class/abstract.ts][抽象クラス]{
#@mapfile(../code/typescript-basic/class/abstract.ts)
abstract class Animal {
  abstract name: string;
  abstract get poo(): string;

  abstract speak(): string;
  sleep(): string {
    return "zzzZZZ...";
  }
}
// もちろん、abstract classはそのままではインスタンス化できない
// error TS2511: Cannot create an instance of the abstract class 'Animal'.
// new Animal();

class Cat extends Animal {
  // プロパティの実装を強制される
  name = "Cat";
  poo = "poo...";

  // メソッドの実装を強制される
  speak(): string {
    return "meow";
  }
}

new Cat();

export { }
#@end
//}

便利ですね。
privateやprotectedに比べ、よっぽど使い出があります。

コンパイル後のJavaScriptを見てみると、単なる普通のクラスに変換されていることがわかります（@<list>{class/abstract.js}）。

//list[class/abstract.js][コンパイルしてしまえばただのクラス]{
#@mapfile(../code/typescript-basic/class/abstract.js)
"use strict";
class Animal {
    get poo() { }
    sleep() {
        return "zzzZZZ...";
    }
}
// もちろん、abstract classはそのままではインスタンス化できない
// error TS2511: Cannot create an instance of the abstract class 'Animal'.
// new Animal();
class Cat extends Animal {
    constructor(...args) {
        super(...args);
        // プロパティの実装を強制される
        this.name = "Cat";
        this.poo = "poo...";
    }
    // メソッドの実装を強制される
    speak() {
        return "meow";
    }
}
new Cat();
#@end
//}

=={function} 関数

#@# NOTE クラスの後に関数定義の説明したーーーい！したくない？

=== 普通の関数

いたって普通です（@<list>{function/basic}）。
型注釈の与え方や、引数を省略可能にする方法だけがJavaScriptと違いますね。

//list[function/basic][色々な関数定義]{
#@mapfile(../code/typescript-basic/function/basic.ts)
function hello(word: string): string {
  return `Hello, ${word}`;
}
hello("TypeScript");

// 返り値の型を省略すると返り値の型から推論される。明記したほうが読みやすい場合もある
function bye(word: string) {
  return `Bye, ${word}`;
}
bye("TypeScript");

// ? をつけると呼び出しときに引数が省略可能になる
function hey(word?: string) {
  // 省略可能にした時はundefinedの時の考慮が必要！
  return `Hey, ${word || "TypeScript"}`;
}
hey();

// デフォルト値を指定することもできる (? を付けたのと同じ扱い+α)
function ahoy(word = "TypeScript") {
  return `Ahoy! ${word}`;
}
ahoy();

export { }
#@end
//}

可変長引数もあります！（@<list>{function/args}）

//list[function/args][可変長引数の例]{
#@mapfile(../code/typescript-basic/function/args.ts)
function hello(...args: string[]) {
  return `Hello, ${args.join(" & ")}`;
}
// Hello, TS & JS と表示される
console.log(hello("TS", "JS"));

export { }
#@end
//}

#@# @suppress LongKanjiChain
なお、省略可能引数の後に省略不可な引数を配置したり、可変長引数を最後以外に配置したりするのはNGです（@<list>{function/invalid}）。

//list[function/invalid][こういうパターンはNG]{
#@mapfile(../code/typescript-basic/function/invalid.ts)
// 省略可能な引数の後に省略不可な引数がきてはいけない
// error TS1016: A required parameter cannot follow an optional parameter.
function funcA(arg1?: string, arg2: string) {
  return `Hello, ${arg1}, ${arg2}`;
}

// 可変長引数は必ず最後じゃないといけない
// error TS1014: A rest parameter must be last in a parameter list.
function funcB(...args: string[], rest: string) {
  return `Hello, ${args.join(", ")} and ${rest}`;
}

export { }
#@end
//}

ここまで見てきた省略可能な引数やデフォルト値付き引数、可変長引数はクラスのコンストラクタやメソッドを記述するときも同様に利用できます。

=== アロー関数

ECMAScript 2015で導入された@<kw>{アロー関数,Arrow Functions}を見ていきましょう（@<list>{arrowFunctions/basic}）。
通常の関数とアロー関数の違いについてはECMAScript 2015の範囲であるため、本書では解説しません。

//list[arrowFunctions/basic][アロー関数 短くてかっこいい]{
#@mapfile(../code/typescript-basic/arrowFunctions/basic.ts)
// 次の2つは(thisが絡まない限り)等価！
let funcA = () => true;
let funcB = function() {
  return true;
};
funcA();
funcB();

// NOTE ここのcallbackの型注釈の意味は別の章で解説します
// 引数を1つ取って返り値無し の関数を表します
function asyncModoki(callback: (value: string) => void) {
  callback("TypeScript");
}
// ES5時代の書き方
asyncModoki(function(value: string) {
  console.log(`Hello, ${value}`);
});
// アロー関数だとさらに楽
asyncModoki(value => console.log(`Hello, ${value}`));
// アロー関数に型付をする場合
asyncModoki((value: string): void => console.log(`Hello, ${value}`));

export { }
#@end
//}

アロー関数も普通の関数同様、型注釈の与え方以外ECMAScript 2015との差分は見当たりません。
短くてかっこいいですね。

もうひとつの便利な点として、アロー関数は親スコープのthisをそのまま受け継ぎます。
この仕組みのおかげでクラスのメソッドなどでコールバック関数を使うときに無用な混乱をおこさずに済みます。
特別な理由が思いつかない限りアロー関数を使っておけばよいでしょう。

#@# TODO argumentsの取り扱いがES6準拠ではない みたいな話も仕様ちゃんと読んで書きたい

== モジュールのあれこれ

プログラムの規模が大きくなればなるほど、機能ごとに分割して統治し、見通しをよくする必要があります。
そのための武器として、ECMAScript 2015にはモジュールがあります。
1つのJSファイルを1つのモジュールと捉えます。
Node.jsで使われているCommonJS形式のモジュールと考え方は一緒です。
つまり、別ファイルになれば別モジュールと考え、モジュールから値をexportしたりimportしたりして大きなプログラムを分割し統治します。

#@# @suppress SentenceLength CommaNumber
#@# prh:disable
歴史的経緯により、TypeScriptでは先に説明した1つのJavaScriptファイルを1つのモジュールと捉えた形式のことを外部モジュール（External Modules）と呼び、関数を使って1つの名前空間を作り出す形式を内部モジュール（Internal Modules）と呼んでいました。
しかし、ECMAScript 2015で本格的に"モジュール"の概念が定義されたため、TypeScriptでは今後はモジュールといえば外部モジュールを指し、内部モジュールのことは@<code>{namespace}と呼ぶようになりました。
これにあわせて、内部モジュールの記法も旧来の@<code>{module}から@<code>{namespace}に変更されました。
未だに@<code>{module}を使うこともできますが、今後は@<code>{namespace}を使ったほうがよいでしょう。

#@# prh:disable
本書でも、これ以降は単にモジュールと書く場合は外部モジュールのことを指し、namespaceと書いた時は内部モジュールのことを指します。

#@# @suppress JapaneseAmbiguousNounConjunction
仕様としてモジュールが策定され、WHATWGでブラウザでのモジュールの動作について議論が進んでいる現状、namespaceのみを使ってプログラムを分割・構成すると将来的にはきっと負債になるでしょう。
これから新規にプロジェクトを作成する場合は実行する環境がNode.jsであれ、ブラウザであれ、モジュールを使って構成するべきでしょう。

=== モジュール

モジュールは前述のとおり、1ファイル＝1モジュールとしてプロジェクトを構成していく方式です。
@<code>{import * as foo from "./foo";}のように書くと、そのファイルから./foo.ts@<fn>{require-ext}を参照できます。
ここでは、./fooがひとつのモジュールとして扱われます。

#@# @suppress CommaNumber
TypeScriptではCommonJS、AMD、System（SystemJS）、UMD、ECMAScript 2015によるモジュールの利用に対応しています。
いずれの形式で出力するかについてはコンパイル時に@<code>{--module commonjs}などの形式で指定できます。

#@# prh:disable
本書ではNode.jsでもBrowserifyやwebpackで広く利用しやすいCommonJS形式についてのみ言及します。
対応形式の中ではAMDやSystemJSについては癖が強く、tscに与えることができるオプションの数も多いため興味がある人は自分で調べてみてください。
筆者は両形式はあまり筋がよいとは今のところ思っていませんけれど。

#@# OK REVIEW lc: s/WebPack/Webpack/

#@# @suppress SentenceLength CommaNumber
さて、実際のコード例を見てみましょう。
foo.ts（@<list>{externalModule/foo}）、bar.ts（@<list>{externalModule/bar}）、buzz.ts（@<list>{externalModule/buzz}）というファイルがあるとき、それぞれがモジュールになるので3モジュールある、という考え方になります。

//list[externalModule/foo][foo.ts]{
#@mapfile(../code/typescript-basic/externalModule/foo.ts)
// defaultをbarという名前に hello関数をそのままの名前でimport
import bar, { hello } from "./bar";
// モジュール全体をbar2に束縛
import * as bar2 from "./bar";
// ECMAScript 2015形式のモジュールでもCommonJS形式でimportできる
import bar3 = require("./bar");

// Hello, TypeScript! と表示される
console.log(hello());
// Hi!, default と表示される
console.log(bar());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar2.hello());
console.log(bar2.default());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar3.hello());
console.log(bar3.default());

// export = xxx 形式の場合モジュール全体をbuzzに束縛
import * as buzz from "./buzz";
// CommonJS形式のモジュールに対して一番素直で真っ当な書き方 in TypeScript
import buzz2 = require("./buzz");
// 両方 Good bye, TypeScript! と表示される
console.log(buzz());
console.log(buzz2());
#@end
//}

//list[externalModule/bar][bar.ts]{
#@mapfile(../code/typescript-basic/externalModule/bar.ts)
export function hello(word = "TypeScript") {
  return `Hello, ${word}`;
}

export default function(word = "default") {
  return `Hi!, ${word}`;
}
#@end
//}

//list[externalModule/buzz][buzz.ts]{
#@mapfile(../code/typescript-basic/externalModule/buzz.ts)
function bye(word = "TypeScript") {
  return `Good bye, ${word}`;
}
// foo.ts でECMAScript 2015形式でimportする際、
// 次のエラーが出るのを抑制するためのハック
// error TS2497: Module '"略/buzz"' resolves to a non-module entity
//   and cannot be imported using this construct.
namespace bye { }

// CommonJS向け ECMAScript 2015では×
export = bye;
#@end
//}

各モジュールのトップレベルでexportしたものが別のファイルからimportされたときに利用できているのがわかります。
コンパイルして結果を確かめてみましょう。
Node.jsに慣れている人なら、見覚えのある形式のコードが出力されていることが分かるでしょう。

//cmd{
$ tsc --module commonjs --target es6 foo.ts
$ cat foo.js
#@mapfile(../code/typescript-basic/externalModule/foo.js)
"use strict";
// defaultをbarという名前に hello関数をそのままの名前でimport
const bar_1 = require("./bar");
// モジュール全体をbar2に束縛
const bar2 = require("./bar");
// ECMAScript 2015形式のモジュールでもCommonJS形式でimportできる
const bar3 = require("./bar");
// Hello, TypeScript! と表示される
console.log(bar_1.hello());
// Hi!, default と表示される
console.log(bar_1.default());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar2.hello());
console.log(bar2.default());
// 上に同じく Hello, TypeScript! と Hi!, default
console.log(bar3.hello());
console.log(bar3.default());
// export = xxx 形式の場合モジュール全体をbuzzに束縛
const buzz = require("./buzz");
// CommonJS形式のモジュールに対して一番素直で真っ当な書き方 in TypeScript
const buzz2 = require("./buzz");
// 両方 Good bye, TypeScript! と表示される
console.log(buzz());
console.log(buzz2());
#@end
$ cat bar.js
#@mapfile(../code/typescript-basic/externalModule/bar.js)
"use strict";
function hello(word = "TypeScript") {
    return `Hello, ${word}`;
}
exports.hello = hello;
function default_1(word = "default") {
    return `Hi!, ${word}`;
}
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = default_1;
#@end
$ cat buzz.js
#@mapfile(../code/typescript-basic/externalModule/buzz.js)
"use strict";
function bye(word = "TypeScript") {
    return `Good bye, ${word}`;
}
module.exports = bye;
#@end
//}

#@# TODO ここじゃないほうがいいけど、型としての参照だけだと消される恐れがある旨書く。

#@# prh:disable
//footnote[require-ext][Node.js上の仕様（TypeScriptではない）について細かくいうと、require("./foo")すると最初に./foo.js が探され、次に./foo.json、./foo.nodeと検索します]

=== namespace

現実的にコードを書く時にはnamespaceを使わないほうがよいのです。
ですので、できればnamespaceについては説明したくないのですが、そうはいかない理由があります。
それが、型定義ファイルの存在です。
型定義ファイルの中ではインタフェースや関数などをきれいに取りまとめるためにnamespaceの仕組みを活用する場面がでてきます。
そのため、TypeScriptの習熟度を高めるうえでnamespaceは避けては通れないのです。

まずは簡単な例を見てみましょう（@<list>{internalModule/basic.ts}）。

//list[internalModule/basic.ts][namespaceを使ったコード]{
#@mapfile(../code/typescript-basic/internalModule/basic.ts)
namespace a {
  // export してないものは外部からは見えない
  class Sample {
    hello(word = "TypeScript") {
      return `Hello, ${word}`;
    }
  }

  export interface Hello {
    hello(word?: string): string;
  }
  export let obj: Hello = new Sample();
}
namespace a {
  export function bye(word = "JavaScript") {
    return `Bye, ${word}`;
  }

  // 定義を分けてしまうと同名のモジュールでもexportされていないものは見えない
  // error TS2304: Cannot find name 'Sample'.
  // let tmp = new Sample();
}

namespace b {
  export namespace c {
    export function hello() {
      return a.obj.hello();
    }
  }
}
namespace d.e {
  export function hello() {
    return a.obj.hello();
  }
}

// Hello, TypeScript と表示される
console.log(b.c.hello());
// Hello, TypeScript と表示される
console.log(d.e.hello());
#@end
//}

なかなかシンプルです。
namespaceの内側で定義した要素はクラスであれ、関数であれ、なんであってもexportをつけなければ外側から見えないようになります。

これをコンパイルした結果を確認してみます（@<list>{internalModule/basic.js}）。

//list[internalModule/basic.js][コンパイルすると関数を使った構文に展開される]{
#@mapfile(../code/typescript-basic/internalModule/basic.js)
var a;
(function (a) {
    // export してないものは外部からは見えない
    class Sample {
        hello(word = "TypeScript") {
            return `Hello, ${word}`;
        }
    }
    a.obj = new Sample();
})(a || (a = {}));
var a;
(function (a) {
    function bye(word = "JavaScript") {
        return `Bye, ${word}`;
    }
    a.bye = bye;
})(a || (a = {}));
var b;
(function (b) {
    var c;
    (function (c) {
        function hello() {
            return a.obj.hello();
        }
        c.hello = hello;
    })(c = b.c || (b.c = {}));
})(b || (b = {}));
var d;
(function (d) {
    var e;
    (function (e) {
        function hello() {
            return a.obj.hello();
        }
        e.hello = hello;
    })(e = d.e || (d.e = {}));
})(d || (d = {}));
// Hello, TypeScript と表示される
console.log(b.c.hello());
// Hello, TypeScript と表示される
console.log(d.e.hello());
#@end
//}

関数を使って名前空間を擬似的に作っています。
モジュールもletやconstのようなブロックスコープもなかった頃の名残ですね。

#@# @suppress JapaneseStyle
長い名前を使うのが嫌なときは@<list>{internalModule/import}のように、import句を使うこともできます。
先に説明したモジュールではこれとは異なるimport句の使い方が出てきましたが、区別しましょう。

//list[internalModule/import][import句で別名を作る]{
#@mapfile(../code/typescript-basic/internalModule/import.ts)
namespace a {
  export class Sample { }
}

namespace b {
  // 他のモジュールも普通に参照できる
  let objA: a.Sample;
  objA = new a.Sample();

  // めんどくさいなら import句 を使えばいい
  import Sample = a.Sample;
  let objB: Sample;
  objB = new Sample();

  // 別に違う名前をつけてもいい(けど混乱しちゃうかも？
  import Test = a.Sample;
  let objC: Test;
  objC = new Test();

  // 別に名前が違っても互換性が失われるわけではないのだ
  objA = new Test();
}
#@end
//}
