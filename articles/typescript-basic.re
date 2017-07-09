={typescript-basic} TypeScriptの基本

#@# TODO enum
#@# TODO const enum
#@# TODO 暗黙的なインデックスシグニチャ（Implicit index signatures）

#@# prh:disable
@<chapref>{prepared-to-typescript}で述べたとおり、本書ではECMAScriptの文法・仕様についてすべてを解説することはしません。
ECMAScriptの知識はどんどん広まってきていますし、最近では知っている人も多い知識になってきました。
ECMAScriptの知識は、TypeScript固有の知識ではないですからね。

この章ではTypeScriptでの基本的な構文を解説します。
まずは、TypeScriptを使うのに必要最低限の知識を身につけていきましょう。

型の基本は@<chapref>{types-basic}を、難しいことや便利なことは@<chapref>{types-advanced}を見てください。
既存のJavaScriptな資産やライブラリを使いたい場合は@<chapref>{definition-file}を見てください。

#@# @suppress CommaNumber
また、本書は@<code>{--strict}を有効にした状態を基本として解説します。
オプションの詳細については@<chapref>{tsc-options}を参照してください。

#@# OK REVIEW lc: tsconfigの設定状態を出したほうがわかりやすい？
#@# vv: ここはこのままにしておきます…。

#@# @suppress ParagraphNumber SectionLength
=={variables} 変数

#@# @suppress JapaneseAmbiguousNounConjunction
TypeScriptの変数宣言はおおむねJavaScriptと同じです。
違うのは、@<list>{variable/withAnnotations.ts}のように変数名の後に@<code>{: 型名}という形式でその変数がどういう型の値の入れ物になるのか指定できるところです。
これを@<kw>{型注釈,type annotations}と呼びます。

//list[variable/withAnnotations.ts][型注釈付きの変数]{
#@mapfile(../code/typescript-basic/variable/withAnnotations.ts)
// JSそのものの書き方
// 変数に初期値を与えると初期値の型がそのまま変数の型になる（型推論される
// 省略しても問題のない型の記述は積極的に省略してしまってよい！
{
  let str = "文字列";
  let num = 1;
  let bool = true;

  let func = () => { };
  let obj = {};

  console.log(str, num, bool, func(), obj);
}

// 型推論に頼らずに型注釈を明示的に書いてもよい
// 特別な理由がない限り、このやり方にあまり長所はない
{
  let str: string = "文字列";
  let num: number = 1;
  let bool: boolean = true;

  let func: Function = () => { };
  // any はなんでも型
  let obj: any = {};

  console.log(str, num, bool, func(), obj);
}
#@end
//}

もちろん、変数に対して初期化子を与えることで変数の型をコンパイラに考えさせる（型推論させる）こともできます。
TypeScriptはIDEやエディタとの連携が良好なため、ソースコードのある箇所がどういう型になっているかはツールチップなどで簡単に確認できます。
このため、型推論を多様しても困ることはほぼないため、安心して短く気持ちよく書きましょう。

型がつけられると何が嬉しいかというと、型に反するようなコードを書くとtscコマンドなどでコンパイルしたときにエラーになることです。
たとえば@<list>{variable/withAnnotations-invalid.ts}のように、整合性がとれていない箇所をコンパイラが見つけてくれます@<fn>{suppress-warning}。

//list[variable/withAnnotations-invalid.ts][型注釈に反することをやってみる]{
#@mapfile(../code/typescript-basic/variable/withAnnotations-invalid.ts)
let str: string;
// 文字列は数値と互換性がない！
// error TS2322: Type '1' is not assignable to type 'string'.
str = 1;

let num: number;
// 数値は真偽値と互換性がない！
// error TS2322: Type 'true' is not assignable to type 'number'.
num = true;

let bool: boolean;
// 真偽値は文字列と互換性がない！
//  error TS2322: Type '"str"' is not assignable to type 'boolean'.
bool = "str";

export {}
#@end
//}

コンパイルした段階でソースコードの整合性が保たれていない、きな臭い部分があぶり出されるのは嬉しいです。
安心安全！

//footnote[suppress-warning][コンパイルエラーを消すため、今後もサンプルコード中に一見意味のなさそうな export {} などが表れます]

=={class} クラス

クラスについて、TypeScriptではいくつかの拡張が用意されています（@<list>{class/basic.ts}）。

#@# OK REVIEW lc: ES.next的には「instance fields」と「static properties」っぽいんですが、TSでの呼称は「インスタンス変数」と「クラス変数」なんですか？ https://github.com/jeffmo/es-class-public-fields
#@# OK REVIEW lc: spec読んだら「class members」と「static class members」だった
#@# vv: ES.next的には定まった呼称はなさそう syntax的にはClassElement。若干Java方言だけどここはとりあえずこのままにしときます。

//list[class/basic.ts][一般的なクラス要素]{
#@mapfile(../code/typescript-basic/class/basic.ts)
class Base {
  // インスタンス変数
  num = 1;

  // 初期値を与えない場合は型の指定が必要
  str: string;

  // プロパティ名に ? をつけると省略可能（undefinedである可能性がある）ことを表せる
  regExpOptional?: RegExp;

  constructor(str: string) {
    // str は省略可能じゃないのでコンストラクタで初期値を設定しなければならない
    // 設定し忘れても現在のTypeScriptはエラーにしてくれないので注意が必要…
    this.str = str;
  }

  // メソッドの定義 返り値は省略してもOK
  hello(): string {
    return `Hello, ${this.str}`;
  }

  get regExp() {
    if (!this.regExpOptional) {
      return new RegExp("test");
    }

    return this.regExpOptional;
  }
}

const base = new Base("world");
console.log(base.hello());

export { };
#@end
//}

クラスのメンバーを定義する箇所にプロパティを記述していくやり方はTypeScriptの拡張で、ECMAScriptの範囲ではありません。
ECMAScriptの場合はコンストラクタ内部でプロパティの設定を行います。
コンパイルして出て来るjsコード（@<list>{class/basic.js}）との差を見てみるとわかりやすいです。

//list[class/basic.js][jsにコンパイルしたの出力]{
#@mapfile(../code/typescript-basic/class/basic.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Base {
    constructor(str) {
        // インスタンス変数
        this.num = 1;
        // str は省略可能じゃないのでコンストラクタで初期値を設定しなければならない
        // 設定し忘れても現在のTypeScriptはエラーにしてくれないので注意が必要…
        this.str = str;
    }
    // メソッドの定義 返り値は省略してもOK
    hello() {
        return `Hello, ${this.str}`;
    }
    get regExp() {
        if (!this.regExpOptional) {
            return new RegExp("test");
        }
        return this.regExpOptional;
    }
}
const base = new Base("world");
console.log(base.hello());
#@end
//}

また、プロパティには省略可能（optional）かを明示する@<kw>{?}を指定できます。
コンストラクタ内の処理が終わるまでの間に値がセットされないプロパティについては、省略可能でことを明示するようにしましょう。
#@# クラスのプロパティが省略可能かどうか指定の追加（Optional properties in classes）
#@# OK REVIEW lc: s/旨/旨を/

#@# get、setアクセサについても少し触れておきます。
#@# これを含んだコードをコンパイルするときは、@<code>{--target es5}以上を指定します。

次に、クラスの継承も見て行きましょう（@<list>{class/inherit.ts}）。
superを使い親クラスのメソッドを参照することも普通にできます。

//list[class/inherit.ts][普通に継承もあるよ]{
#@mapfile(../code/typescript-basic/class/inherit.ts)
class Base {
  greeting(name: string) {
    return `Hi! ${name}`;
  }
}

class Inherit extends Base {
  greeting(name: string) {
    return `${super.greeting(name)}. How are you?`;
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

#@# 仕様として複雑なので次のテクニックには言及しなくていいかなぁ…
#@# superを呼び出しした時コンストラクタでreturnした値がthisとなるように変更 Use returned values from super calls as 'this' in 2.1.4
#@# https://github.com/Microsoft/TypeScript/issues/13355 constructorに型付けできないと困る話

==={class-access-modifier} アクセス修飾子

#@# @suppress CommaNumber
TypeScript固有の機能として、アクセス修飾子があります。
プロパティやメソッド、コンストラクタについてprivate、public、protectedといったアクセス修飾子を利用できます（@<list>{class/modifier.ts}）。
何も指定していないとき、デフォルトの可視性はpublicになります。

//list[class/modifier.ts][アクセス修飾子の例]{
#@mapfile(../code/typescript-basic/class/modifier.ts)
class Base {
  a = "a";
  public b = "b";
  protected c = "c";
  private d = "d";

  method() {
    // privateなプロパティは利用しているコードが一箇所もないと警告してもらえる
    this.d;
  }
}

class Inherit extends Base {
  method() {
    // 子クラスから public, protected はアクセス可能
    this.a;
    this.b;
    this.c;
    // private はコンパイルエラーになる
    // this.d;
  }
}

const base = new Base();
// public は通常のアクセスが可能
base.a;
base.b;
// protected, private はコンパイルエラーになる
// base.c;
// base.d;
#@end
//}

次にコンパイル後のJSファイルを見てみます（@<list>{class/modifier.js}）。

//list[class/modifier.js][アクセス修飾子はJSコードに影響しない]{
#@mapfile(../code/typescript-basic/class/modifier.js)
"use strict";
class Base {
    constructor() {
        this.a = "a";
        this.b = "b";
        this.c = "c";
        this.d = "d";
    }
    method() {
        // privateなプロパティは利用しているコードが一箇所もないと警告してもらえる
        this.d;
    }
}
class Inherit extends Base {
    method() {
        // 子クラスから public, protected はアクセス可能
        this.a;
        this.b;
        this.c;
        // private はコンパイルエラーになる
        // this.d;
    }
}
const base = new Base();
// public は通常のアクセスが可能
base.a;
base.b;
// protected, private はコンパイルエラーになる
// base.c;
// base.d;
#@end
//}

アクセス修飾子がきれいさっぱり消えていますね。
アクセス修飾子はコンパイル時のみに影響がある機能で、@<code>{any}のような何でもあり型にキャストすると隠したはずのプロパティにアクセスできてしまいます。
アクセス修飾子をつけても外部からの変更を100%防げる！と考えることはできないということですね。
そのため筆者はアクセス修飾子を使うだけではなく、privateな要素のprefixに_を使い、ドキュメントコメントに@<code>{@internal}をつけるといった工夫をしています。

==={class-parameter-property-declaration} 引数プロパティ宣言

コンストラクタの引数にアクセス修飾子をあわせて書くと、インスタンス変数としてその値が利用可能になります（@<list>{class/constructor.ts}）。
これを@<kw>{引数プロパティ宣言,parameter property declaration}と呼びます。
引数プロパティ宣言もTypeScript固有の記法です。

//list[class/constructor.ts][引数プロパティ宣言！]{
#@mapfile(../code/typescript-basic/class/constructor.ts)
class BaseA {
  constructor(public str: string) {
  }
}

// BaseA と等価な定義
class BaseB {
  str: string;
  constructor(str: string) {
    this.str = str;
  }
}

export { BaseA, BaseB }
#@end
//}

==={abstract-class} 抽象クラス（Abstract Class）

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
privateやprotectedに比べると使い勝手がよい機能といえます。

コンパイル後のJavaScriptを見てみると、単なる普通のクラスに変換されていることがわかります（@<list>{class/abstract.js}）。

//list[class/abstract.js][コンパイルしてしまえばただのクラス]{
#@mapfile(../code/typescript-basic/class/abstract.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Animal {
    sleep() {
        return "zzzZZZ...";
    }
}
// もちろん、abstract classはそのままではインスタンス化できない
// error TS2511: Cannot create an instance of the abstract class 'Animal'.
// new Animal();
class Cat extends Animal {
    constructor() {
        super(...arguments);
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

==={standard-function} 普通の関数

いたって普通です（@<list>{function/basic.ts}）。
型注釈の書き方で通常のJavaScriptと差が出ます。
確認していきましょう。

//list[function/basic.ts][色々な関数定義]{
#@mapfile(../code/typescript-basic/function/basic.ts)
function hello(word: string): string {
  return `Hello, ${word}`;
}
hello("TypeScript");

// 返り値の型を省略すると返り値の型から推論される
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

// デフォルト値の指定で仮引数の型を省略したりもできる
function ahoy(word = "TypeScript") {
  return `Ahoy! ${word}`;
}
ahoy();

export { }
#@end
//}

可変長引数の場合は仮引数の最終的な型を書きます（@<list>{function/args.ts}）。
つまり、配列の形になりますね。

//list[function/args.ts][可変長引数の例]{
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
なお、省略可能引数の後に省略不可な引数を配置したり、可変長引数を最後以外に配置したりするのはNGです（@<list>{function/invalid.ts}）。

//list[function/invalid.ts][こういうパターンはNG]{
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

==={arrow-function} アロー関数

@<kw>{アロー関数,Arrow Functions}を見ていきましょう（@<list>{arrowFunctions/basic.ts}）。
通常の関数とアロー関数の違いについてはECMAScriptの仕様の範囲ですので省略します。

//list[arrowFunctions/basic.ts][アロー関数 短くてかっこいい]{
#@mapfile(../code/typescript-basic/arrowFunctions/basic.ts)
// NOTE ここのcallbackの型注釈の意味は別の章で解説します
// 引数を1つ取って返り値無し の関数を表します
function asyncModoki(callback: (value: string) => void) {
  callback("TypeScript");
}

// アロー関数をコールバック関数として渡す 渡す関数の型は型推論される！
asyncModoki(value => console.log(`Hello, ${value}`));

// アロー関数に明示的に型付をする場合
asyncModoki((value: string): void => console.log(`Hello, ${value}`));

export { }
#@end
//}

アロー関数も普通の関数同様、型注釈の与え方以外ECMAScriptの仕様との差分はありません。
書きやすくてよいですね。

アロー関数は親スコープのthisをそのまま受け継ぎます。
この仕組みのおかげでクラスのメソッドなどでコールバック関数を使うときに無用な混乱をおこさずに済みます。
特別な理由が思いつかない限りアロー関数を使っておけばよいでしょう。

==={async-await} Async（非同期）関数

#@# since 2.1.1

俗にasync/awaitと呼ばれる仕様です。
async/awaitの振る舞いについてはECMAScript仕様の範囲ですので概要だけ説明します。
ES2015で標準仕様に入った非同期処理APIのPromiseがあります。
これらに簡易な構文を与えたものがAsync関数です（@<list>{asyncFunction/basic.ts}）@<fn>{async-await-downpile}。

//list[asyncFunction/basic.ts][async/await 便利！]{
#@mapfile(../code/typescript-basic/asyncFunction/basic.ts)
function returnByPromise(word: string) {
  return new Promise(resolve => {
    setTimeout(() => {
      resolve(word);
    }, 100);
  });
}

// async functionの返り値の型は必ずPromiseになる
async function helloAsync(): Promise<string> {
  // この関数は実行すると A, TypeScript, B が順番に表示される

  console.log("A");
  // Promiseな値をawaitすると中身が取り出せる（ように見える）
  const word = await returnByPromise("TypeScript");
  console.log(word);
  console.log("B");

  return `Hello, ${word}`;
}

// awaitが使えるのは必ずasync functionの中
(async () => {
  const hello = await helloAsync();
  console.log(hello);
})();

// 普通にPromiseとして扱うこともできる
helloAsync().then(hello => console.log(hello));

export { }
#@end
//}

Async関数の返り値の型は常にPromiseになります。

#@# NOTE Asynchronous Iterations https://github.com/Microsoft/TypeScript/issues/11326
ちなみに、generatorの返り値の型は常に@<code>{IterableIterator}、async generatorの場合は常に@<code>{AsyncIterableIterator}とする必要があります。

//footnote[async-await-downpile][ちなみにasync/awaitのdownpileもTypeScript 2.1.1からサポートされています]

=={module-and-namespace} モジュールのあれこれ

プログラムの規模が大きくなればなるほど、機能ごとに分割して統治し、見通しをよくする必要があります。
そのための武器として、ECMAScript 2015から言語にモジュールの仕様が追加されました。
1つのJSファイルを1つのモジュールと捉えます。
つまり、別ファイルになれば別モジュールと考え、モジュールから値をexportしたりimportしたりして大きなプログラムを分割し統治します。

#@# @suppress SentenceLength CommaNumber
#@# prh:disable
歴史的経緯により、TypeScriptではモジュールの他に@<code>{namespace}という機能があります。
モジュールの考え方がまだ発達していなかった時代に、関数を使ってモジュールのようなものを涙ぐましく作っていた名残ですね。

#@# @suppress JapaneseAmbiguousNounConjunction
仕様としてモジュールが策定され、ブラウザでの実装も進んでいる今、namespaceを使ってプログラムを分割・構成するのは悪手です@<fn>{tsc-namespace}。
これから新規にプロジェクトを作成する場合は実行する環境がNode.jsであれ、ブラウザであれ、モジュールを使って構成するべきでしょう。

//footnote[tsc-namespace][なお、TypeScriptコンパイラ本体はまだnamespaceを使っている模様]

====[column] モジュールとnamespaceと外部モジュールと内部モジュール

過去の用語の使い方について参考文献としてメモしておきます。
ここに書いてあることは知らないほうがよい知識かもしれません。

歴史的経緯により、TypeScriptはモジュールのことを外部モジュール（External Modules）と呼んでいました。
また、namespaceのことを内部モジュール（Internal Modules）と呼んでいました。
内部モジュールとは、関数を使って1つの名前空間を作り出すテクニックのことで、ECMAScriptの仕様に含まれるものではありません。

ECMAScript 2015で本格的に"モジュール"の概念が定義されたため、TypeScriptでは今後はモジュールといえば外部モジュールを指し、内部モジュールのことは@<code>{namespace}と呼ぶように改めました。
これにあわせて、内部モジュールの記法も古くは@<code>{module}を使っていたのを@<code>{namespace}に変更されました。

#@# prh:disable
本書でも、単にモジュールと書く場合は外部モジュールのことを指し、namespaceと書いた時は内部モジュールのことを指しています。

====[/column]

==={module} モジュール

モジュールは前述のとおり、1ファイル＝1モジュールとしてプロジェクトを構成していく方式です。
@<code>{import * as foo from "./foo";}のように書くと、そのファイルから./foo.ts@<fn>{require-ext}を参照できます。
ここでは、./fooがひとつのモジュールとして扱われます。

#@# @suppress CommaNumber
TypeScriptではCommonJS、AMD、System（SystemJS）、UMD、ECMAScript 2015によるモジュールの利用に対応しています。
いずれの形式で出力するかについては@<code>{--module commonjs}というようにオプションで指定できます。

#@# prh:disable
本書ではNode.jsでもBrowserifyやwebpackで広く利用しやすいCommonJS形式についてのみ言及します。
rollup.jsなどの普及により、es2015形式のまま出力し別途bundlerで処理する場合もあるかもしれません。
AMDやSystemJSは正直最近廃れてきていると思いますので興味がある人は自分で調べてみてください。

#@# OK REVIEW lc: s/WebPack/Webpack/

#@# @suppress SentenceLength CommaNumber
さて、実際のコード例を見てみましょう。
foo.ts（@<list>{externalModule/foo.ts}）、bar.ts（@<list>{externalModule/bar.ts}）、buzz.ts（@<list>{externalModule/buzz.ts}）というファイルがあるとき、それぞれがモジュールになるので3モジュールある、という考え方になります。

//list[externalModule/foo.ts][foo.ts]{
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

//list[externalModule/bar.ts][bar.ts]{
#@mapfile(../code/typescript-basic/externalModule/bar.ts)
export function hello(word = "TypeScript") {
  return `Hello, ${word}`;
}

export default function(word = "default") {
  return `Hi!, ${word}`;
}
#@end
//}

//list[externalModule/buzz.ts][buzz.ts]{
#@mapfile(../code/typescript-basic/externalModule/buzz.ts)
function bye(word = "TypeScript") {
  return `Good bye, ${word}`;
}
// foo.ts でECMAScript 2015形式でimportする際、
// 次のエラーが出るのを抑制するためのハック
// error TS2497: Module '"略/buzz"' resolves to a non-module entity
//   and cannot be imported using this construct.
namespace bye { }

// CommonJS向け ECMAScript 2015では× 今後は使わなくてよし！
export = bye;
#@end
//}

各モジュールのトップレベルでexportしたものが別のファイルからimportされたときに利用できているのがわかります。
コンパイルして結果を確かめてみましょう。
Node.jsに慣れている人なら、見覚えのある形式のコードが出力されていることが分かるでしょう。

//cmd{
$ tsc --module commonjs --target es2015 foo.ts
$ cat foo.js
#@mapfile(../code/typescript-basic/externalModule/foo.js)
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
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
Object.defineProperty(exports, "__esModule", { value: true });
function hello(word = "TypeScript") {
    return `Hello, ${word}`;
}
exports.hello = hello;
function default_1(word = "default") {
    return `Hi!, ${word}`;
}
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

==={dynamic-import} 動的インポート（Dynamic Import）

TypeScript 2.4系からサポートされたECMAScriptの仕様に動的インポートがあります。
ECMAScriptの仕様上、モジュールのimport文は参照するモジュールを動的に変える余地がありませんでした。
これは、プログラムを実行しなくてもパースした時点で必要なファイルの全リストを作れるという利点があります。
この仕様は90%のユースケースを満足させるかもしれませんが、動的に必要なモジュールを決定できることにより得られる柔軟性もあります。
そのために、動的インポートの仕様が策定されています。
コード例を見てみましょう（@<list>{dynamicImport/index.ts}、@<list>{dynamicImport/sub.ts}）。

//list[dynamicImport/index.ts][実行時に動的にインポートするモジュールを決定する]{
#@mapfile(../code/typescript-basic/dynamicImport/index.ts)
async function main() {
  // 動的にモジュールをimportできる Promiseが返ってくる
  // 即値（文字列リテラル）でモジュール名を指定するとちゃんと型がついてる！
  const sub = await import("./sub");
  console.log(sub.hello());
}

function mainAnother() {
  // こういうのも普通にイケる
  import("./sub").then(sub => {
    console.log(sub.hello());
  });
}

main();
#@end
//}

//list[dynamicImport/sub.ts][なんの変哲もないimportされる側]{
#@mapfile(../code/typescript-basic/dynamicImport/sub.ts)
export function hello(word = "world") {
  return `Hello, ${word}`;
}
#@end
//}

わかりやすいですね。
TypeScript上での特徴として、importに渡す文字列が固定の場合、これは実行せずに解析できるため得られたモジュールにはしっかりと型がついています。
動的に組み立てた文字列を渡した場合、なんでもアリのanyになってしまうため、自分で独自に型注釈を与えたほうが安全に使えます。

なお、動的インポートを無変換でJSに出力したい場合、@<code>{--module esnext}が必要で、@<code>{--module es2015}ではエラーになるので注意しましょう。

==={namespace} namespace

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
"use strict";
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
(function (a) {
    function bye(word = "JavaScript") {
        return `Bye, ${word}`;
    }
    a.bye = bye;
    // 定義を分けてしまうと同名のモジュールでもexportされていないものは見えない
    // error TS2304: Cannot find name 'Sample'.
    // let tmp = new Sample();
})(a || (a = {}));
var b;
(function (b) {
    let c;
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
長い名前を使うのが嫌なときは@<list>{internalModule/import.ts}のように、import句を使うこともできます。
先に説明したモジュールではこれとは異なるimport句の使い方が出てきましたが、区別しましょう。

//list[internalModule/import.ts][import句で別名を作る]{
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
