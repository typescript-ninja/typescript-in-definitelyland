= TypeScriptの基本

JavaScriptの（TypeScriptではなく）仕様まで踏み込んだ解説については、拙著TypeScriptリファレンス（@<href>{http://www.amazon.co.jp/gp/product/484433588X?tag=damenako-22,Amazon}@<fn>{ts-reference-amazon}、@<href>{http://tatsu-zine.com/books/typescript-reference,達人出版会}@<fn>{ts-reference-tatsujin}）を参照してください。
TypeScriptリファレンスをお持ちの場合（TypeScript 1.3.0現在）、この章は読まなくても問題ありません。
#@# OK REVIEW muo: 大丈夫ですか

TypeScriptリファレンスはTypeScript 1.0.0対応の書籍です。
しかし、TypeScriptの基本的な部分は変わっていないため、今でも役に立ちます。
TypeScript 1.0.0の時代に比べて、型システム周りは強化されてきつつあるので@<chapref>{types-advanced}を熟読してください。

この章ではTypeScriptの基本的な構文を解説します。
TypeScriptの懐は広く、巨大で、ともすれば沼に落ち込みそうになります。
まずは、TypeScriptを使える必要最低限の知識を身につけていきましょう。

型周りの基本は@<chapref>{types-basic}を、難しいこととか便利なことは@<chapref>{types-advanced}を見てください。
既存のJavaScriptの資産やライブラリを使いたい場合は@<chapref>{definition-file}の最初のほうを見てください。

#@# TODO enum と const enum どっか

//footnote[ts-reference-amazon][http://www.amazon.co.jp/gp/product/484433588X?tag=damenako-22]
//footnote[ts-reference-tatsujin][http://tatsu-zine.com/books/typescript-reference]

== 変数

TypeScriptの変数宣言は概ねJavaScriptと同じです。
違うのは、@<list>{var-with-annotations}のように変数名の後に@<code>{: 型名}という形式でその変数がどういう型の値の入れ物になるのか指定できるところです。
これを@<kw>{型注釈,type annotations}と呼びます。

//list[var-with-annotations][型注釈付きの変数]{
#@mapfile(../code/typescript-basic/var-with-annotations.ts)
var str: string;
var num: number;
var bool: boolean;

var func: Function;
var obj: any; // なんでも型
#@end
//}

これの何が嬉しいかというと、型に反するようなコードを書くとtscコマンドを使ってコンパイルしたときにコンパイルエラーになるのです。
たとえば@<list>{var-with-annotations-invalid}のように、整合性がとれていない箇所がTypeScriptによって明らかにされます。
安心安全！

//list[var-with-annotations-invalid][型注釈に反することをやってみる]{
#@mapfile(../code/typescript-basic/var-with-annotations-invalid.ts)
var str: string;
// 文字列は数値と互換性がない！
// error TS2322: Type 'number' is not assignable to type 'string'.
str = 1;

var num: number;
// 数値は真偽値と互換性がない！
// error TS2322: Type 'boolean' is not assignable to type 'number'.
num = true;

var bool: boolean;
// 真偽値は文字列と互換性がない！
// error TS2322: Type 'string' is not assignable to type 'boolean'.
bool = "str";
#@end
//}

おう、安心・安全なのはわかった。
わかったけど、そのために型注釈をいちいち全部に書くの？ダルすぎない？というあなたのために、TypeScriptは型推論の機能を備えています。
@<list>{var-with-initializer}のように、型注釈を書かずに変数定義と初期化を同時に行うようにします。

//list[var-with-initializer][初期化付き変数 = 最強]{
#@mapfile(../code/typescript-basic/var-with-initializer.ts)
var str = "string";
var num = 1;
var bool = true;

var func = function() {
};
var obj = {};
#@end
//}

これで手で型注釈を与えずに済むぞ！しかも、書き方がJavaScriptと全く同じになりました。
楽 + コンパイルによる型チェック = TypeScriptサイキョー。
ということがご納得いただけたでしょう。
#@# OK REVIEW muo: 「いただけたと思います」よりは「いただけたでしょう」あたりかな

== クラス

TypeScriptには一般的な構文でのクラスの定義が備わっています（@<list>{class-basic}）。

//list[class-basic][さまざまなクラス要素]{
#@mapfile(../code/typescript-basic/class-basic.ts)
class Base {
  // インスタンス変数
  numA: number;
  strA = "string";

  public numB: number;
  private numC: number;
  // TypeScript 1.3.0 から protected 利用可
  protected numD: number;

  // クラス変数
  static numA: number;
  public static numB: number;
  private static numC: number;
  protected static numD: number;

  // コンストラクタ
  constructor(boolA: boolean,
    public boolB: boolean,
    private boolC: boolean,
    protected boolD: boolean) {
  }

  // メソッド
  hello(word: string): string {
    return "Hello, " + word;
  }

  // get, setアクセサ
  // コンパイル時に --target es5 以上が必要
  _date: Date;
  get dateA(): Date {
    return this._date;
  }
  set dateA(value: Date) {
    this._date = value;
  }
}

var obj = new Base(true, false, true, false);
obj.numA;
obj.strA;
obj.numB;
// obj.numC; // private   なメンバにはアクセスできぬ
// obj.numD; // protected なメンバにもアクセスできぬ
obj.boolB;
// obj.boolC; // private   なメンバにはアクセスできぬ
// obj.boolD; // protected なメンバにもアクセスできぬ
obj.hello("TypeScript");
obj.dateA = new Date();
obj.dateA;
#@end
//}

上から順に見て行きましょう。

まずはクラス変数、インスタンス変数です。
クラスそのものやインスタンスに紐づく変数です。JavaScriptっぽく言うとプロパティですね。

アクセス修飾子として、private, public, protected（TypeScript 1.3.0より）などの可視性を制御するアクセス修飾子を利用できます。
何も指定していないときのデフォルトの可視性はpublicになります。
しかし、コンパイル後のJSを見るとわかりますが@<code>{<any>}などを使うと簡単にそれらの要素にアクセスできてしまうので過信は禁物です。
そのため筆者はアクセス修飾子を使わず、アクセスされたくない要素はprefixに _ を使うなどの（JavaScriptでもよく見られた）運用をしています。

次はコンストラクタです。
引数にアクセス修飾子をあわせて書くと、インスタンス変数としてその値が利用可能になります。
これを@<kw>{引数プロパティ宣言,parameter property declaration}と呼びます。
@<list>{class-constructor.ts}のようなコードを書くと@<list>{class-constructor.js}のようなJavaScriptが出てきます。

//list[class-constructor.ts][引数プロパティ宣言！]{
#@mapfile(../code/typescript-basic/class-constructor.ts)
class Sample {
  constructor(public str: string) {
  }
}

var obj = new Sample("TypeScript");
// TypeScript と表示される
console.log(obj.str);
#@end
//}

//list[class-constructor.js][コンパイルするとこんなの]{
#@mapfile(../code/typescript-basic/class-constructor.js)
var Sample = (function () {
    function Sample(str) {
        this.str = str;
    }
    return Sample;
})();
var obj = new Sample("TypeScript");
console.log(obj.str);
#@end
//}

@<list>{class-basic}に戻ります。
次はメソッドです。
これも特に特筆すべき要素はありませんね。普通です。

最後に、get, setアクセサです。
これを含んだコードをコンパイルするときは、@<code>{--target es5}オプションが必要です。
なかなかめんどくさいJavaScriptコードが生成されるようになりますが、便利です。
これを使うと、getterしか定義してなくてもプログラム上は値の代入もできてしまうので、"use strict"を併用して実行時に検出できるようにしましょう。
なお、この文法のget, setアクセサはECMAScript 5のオブジェクト初期化子としてFirefox上では利用可能だったのですが、ECMAScript 6で取り除かれ@<fn>{getter-setter}るという不遇な運命をたどっており、@<code>{--target es6}でどういう扱いになるかドキドキしています（今のところ問題なく使えるようですが…）。

これらの構文はECMAScript 6の文法を概ね踏襲しており、将来的にJavaScriptでもこれと同様の記法でクラスを定義できるようになります。

次に、クラスの継承も見て行きましょう。
継承も普通にできます@<list>{class-inherit}。
superを使った親クラスのメソッドの参照も一応普通に使えます。

//list[class-inherit][普通に継承もあるよ]{
#@mapfile(../code/typescript-basic/class-inherit.ts)
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

var obj = new Inherit();
// Hi! TypeScript. How are you? と出力される
console.log(obj.greeting("TypeScript"));
#@end
//}

TypeScript以外のオブジェクト指向の世界でもいえることですが、なんでもかんでも継承すればいいや！という考えはよくありません。
頑張ってオブジェクト指向に適した設計を行いましょう。

#@# TODO 引数プロパティ宣言はES6に入らないよなぁ？

//footnote[getter-setter][@<href>{https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Working_with_Objects#Defining_getters_and_setters}]

== 関数

#@# NOTE クラスの後に関数定義の説明したーーーい！したくない？

=== 普通の関数

至って普通です（@<list>{function}）。
型注釈の与え方や、引数をオプショナルにする方法だけがJavaScriptと違いますね。
デフォルト値付き引数はECMAScript 6で入る予定です。

//list[function][色々な関数定義]{
#@mapfile(../code/typescript-basic/function.ts)
function hello(word: string): string {
  return "Hello, " + word;
}
hello("TypeScript");

// 返り値の型を省略すると返り値の型から推論される。明記したほうが読みやすい場合もある。
function bye(word: string) {
  return "Bye, " + word;
}
bye("TypeScript");

// ? をつけると呼び出しときに引数が省略可能になる
function hey(word?: string) {
  return "Hey, " + (word || "TypeScript");
}
hey();

// デフォルト値を指定することもできる (? を付けたのと同じ扱い+α)
function ahoy(word = "TypeScript") {
  return "Ahoy! " + word;
}
ahoy();
#@end
//}

可変長引数もあります！（@<list>{function-args}）

//list[function-args][可変長引数もあるよ]{
#@mapfile(../code/typescript-basic/function-args.ts)
function hello(...args: string[]) {
  return "Hello, " + args.join(" & ");
}
// Hello, TS & JS と表示される
console.log(hello("TS", "JS"));
#@end
//}

なお、省略可能引数の後に省略不可な引数を配置したり、可変長引数を最後以外に配置するのはNGです（@<list>{function-invalid}）。

//list[function-invalid][こういうのはアカン]{
#@mapfile(../code/typescript-basic/function-invalid.ts)
// オプショナルな引数の後に省略不可な引数がきてはいけない
// error TS1016: A required parameter cannot follow an optional parameter.
function funcA(arg1?: string, arg2: string) {
  return "Hello, " + arg1 + ", " + arg2;
}

// 可変長引数は必ず最後じゃないといけない
// error TS1014: A rest parameter must be last in a parameter list.
function funcB(...args: string, rest: string) {
  return "Hello, " + args.join(", ") + " and " + rest;
}
#@end
//}

ここまで見てきたオプショナルな引数やデフォルト値付き引数、可変長引数はクラスのコンストラクタやメソッドの記述のときも同様に利用できます。

=== アロー関数式

ECMAScript 6で導入される予定の@<kw>{アロー関数式,arrow function expression}を見ていきましょう（@<list>{arrow-function-expression}）。

//list[arrow-function-expression][アロー関数式 短くてかっこいい]{
#@mapfile(../code/typescript-basic/arrow-function-expression.ts)
// 以下2つは(thisが絡まない限り)等価！
var funcA = () => true;
var funcB = function() {
  return true;
};

// NOTE ここのcallbackの型注釈の意味は別の章で解説！
// 引数を1つ取って返り値無し の関数を表します。
function asyncModoki(callback: (value: string) => void) {
  callback("TypeScript");
}
// 旧来の書き方
asyncModoki(function(value: string) {
  console.log("Hello, " + value);
});
// アロー関数式だと楽やで
asyncModoki(value => console.log("Hello, " + value));
#@end
//}

短くてかっこいいですね。
将来のJavaScriptでは、アロー関数式による記述が主流になることは間違いないでしょう（なんせ、楽だし）。
早くNode.jsでも使えるようになって、Gruntfile.jsとかで使わせてほしいものです。

アロー関数式は1つの文しか持たないとき、その文の値を返り値として使ってくれます（@<list>{arrow-function-expression-short}）。

//list[arrow-function-expression-short][1つの文しか持たないときの便利な振る舞い]{
#@mapfile(../code/typescript-basic/arrow-function-expression-short.ts)
// 名前付き関数は定義できないので変数に入れる
var funcA = () => {
};
funcA();

// 以下の2つは等価
// アロー関数式は1ステートメントだけならカッコを省略して値をそのまま返せる
var funcB = (word = "TypeScript") => "Hello, " + word;
var funcC = (word = "TypeScript") => {
  return "Hello, " + word;
};
console.log(funcB());
console.log(funcC());
#@end
//}

もう一つの便利な点として、アロー関数式は親スコープのthisをそのまま受け継ぎます。
この仕組みのおかげでクラスのメソッドなどでコールバック関数を使うときに不要な混乱をおこさずに済みます。
例を見てみましょう（@<list>{arrow-function-expression-this}）。

//list[arrow-function-expression-this][受け継がれるthisの値…！]{
#@mapfile(../code/typescript-basic/arrow-function-expression-this.ts)
"use strict";

class Sample {
  test(): void {
    var funcA = () => {
      // ここでの this は元のまま(Sampleのインスタンス)
      console.log(typeof this);
    };
    var funcB = function() {
      // ここでの this は undefined (ECMAScriptの仕様)
      console.log(typeof this);
    };
    // object と表示される
    funcA();
    // undefined と表示される
    funcB();
  }
}

new Sample().test();
#@end
//}

うーん、アロー関数式は期待どおりの挙動ですね。
旧来の関数では値がundefinedになっています。
JavaScriptに慣れている人も、慣れていない人も、特別に理由がない限りアロー関数式を使っとけばいいと思います。

#@# TODO argumentsの取り扱いがES6準拠ではない みたいな話も仕様ちゃんと読んで書きたい

== モジュール

プログラムの複雑さというのはクラスや関数だけではいかんともしがたく立ち向かいにくいものです。
プログラムの規模が大きくなればなるほど、機能ごとに分割して統治し、見通しをよくする必要があります。

そのための武器として、TypeScriptにはモジュールがあります。
概ね、JavaのpackageやC#のnamespaceと類似のものと考えてよいでしょう。

TypeScriptでは2種類のモジュールがあり、それぞれ内部モジュールと外部モジュールと呼ばれています。
複数種類必要なのは、JavaScript実行環境自体が複数あるためです。

1つは、ブラウザです。
ブラウザ上では複数のJavaScriptのファイルが連結され、まるで1つのファイルであるかのように解釈され実行されます。
これに対して階層構造を与えるための仕組みが内部モジュールです。

もう1つは、Node.jsです。
Node.jsでは、1つのファイルを1つの独立した構造と見なす仕組みが備わっています。
この、1ファイル = 1モジュール と見なした仕組みが外部モジュールです。
階層構造は普通のファイルシステムのようにディレクトリで構成します。

内部モジュール・外部モジュール、どちらの形式でプロジェクトを管理するかを決めるのは、非常に、非常に重要な決めごとです。
しかも、プロジェクトの途中で内部モジュールと外部モジュールを切り替えるのは大変な苦痛を伴うため、最初によくよく調査・検証する必要があります。
そして、内部モジュールと外部モジュールを混在させてプロジェクトを管理するのはあまりよい決断とはいえません。
ブラウザ上で使う場合はbrowserifyを使うのか、require.jsを使うのか、concatして使うのか、それ以外か。
SourceMapを使ってのデバッグが必須要件になるかどうか。
などなど、プロジェクト毎に求める解は違ってくるでしょう。

念を押しますが、最初によくよく調査して、どうプロジェクトを構成するべきか決定するべきです。
最初によくよく調査して、どうプロジェクトを構成するべきか決定するべきです。
本当に大事なことなので二回言いました。

=== 内部モジュール （Internal Modules）

まずは簡単な例を見てみましょう（@<list>{internal-module/basic}）。

//list[internal-module/basic][内部モジュール！]{
#@mapfile(../code/typescript-basic/internal-module/basic.ts)
module a {
  // export してないものは外部からは見えない
  class Sample {
    hello(word = "TypeScript") {
      return "Hello, " + word;
    }
  }

  export var obj = new Sample();
}
module a {
  export function bye(word = "JavaScript") {
    return "Bye, " + word;
  }

  // 定義を分けてしまうと同名のモジュールでもexportされていないものは見えない
  // error TS2304: Cannot find name 'Sample'.
  // var tmp = new Sample();
}

module b {
  export module c {
    export function hello() {
      return a.obj.hello();
    }
  }
}
module d.e {
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

うーん、簡単ですね！！
内部モジュールの内側で定義した要素はクラスであれ関数であれなんであれ、exportをつけなければ外側から見えないようになります。
#@# OK REVIEW muo: 「内部モジュール内部」は少々読みづらいので別の表現が良い感じ

長い名前を使うのが嫌なときは@<list>{internal-module/import}のように、import句を使うこともできます。
外部モジュールではまた別のimport句の使い方が出てくるので区別するようにしましょう。

//list[internal-module/import][import句で別名を作る]{
#@mapfile(../code/typescript-basic/internal-module/import.ts)
module a {
  export class Sample { }
}

module b {
  // 他のモジュールも普通に参照できる
  var objA: a.Sample;
  objA = new a.Sample();

  // めんどくさいなら import句 を使えばいい
  import Sample = a.Sample;
  var objB: Sample;
  objB = new Sample;

  // 別に違う名前をつけてもいい(けど混乱しちゃうかも？
  import Test = a.Sample;
  var objC: Test;
  objC = new Test();

  // 別に名前が違っても互換性が失われるわけではないのだ
  objA = new Test();
}
#@end
//}

さて、内部モジュールでプログラムを組むといっても、1つのソースファイルにだらだらと全部書いていくのは現実的ではありません。
そのため、ファイルを分割する必要があります。

単にファイルを分けて何も工夫せずにいると、型の整合性を気にしないJavaScriptやCoffeeScriptならともかく、TypeScriptでは型が追えなくなって困ってしまう場合があります。
そういうときのために、TypeScriptにはソースコード同士の関係性を記述する@<kw>{リファレンスコメント,reference comments}という仕組みがあります。

たとえば、a.ts（@<list>{internal-module/a}）とb.ts（@<list>{internal-module/b}）があったとします。
b.tsはa.tsで定義している関数を呼び出しています。
つまり、b.tsはa.tsに依存しているわけです。
そこで、reference commentを使って@<code>{/// <reference path="./a.ts" />}と記述します。
パスは相対パスでも絶対パスでも問題ありませんが、一般的に相対パスを利用します。
#@# OK REVIEW muo: 大丈夫ですか

//list[internal-module/a][a.ts]{
#@mapfile(../code/typescript-basic/internal-module/a.ts)
module a {
  export function hello(word = "TypeScript") {
    return "Hello, " + word;
  }
}
#@end
//}

//list[internal-module/b][b.ts]{
#@mapfile(../code/typescript-basic/internal-module/b.ts)
/// <reference path="./a.ts" />

module b {
  console.log(a.hello("internal module"));
}
#@end
//}

これをコンパイルするとき、b.tsだけコンパイルすればa.tsも自動的に一緒にコンパイルされます。
通常、何も指定しない場合はa.jsとb.jsが生成されますが、--out オプションを併用すると1ファイルにまとめられます。

//cmd{
$ tsc --out main.js b.ts
$ cat main.js
#@mapoutput(../node_modules/.bin/tsc --out ../code/typescript-basic/internal-module/main.js ../code/typescript-basic/internal-module/b.ts && cat ../code/typescript-basic/internal-module/main.js)
var a;
(function (a) {
    function hello(word) {
        if (word === void 0) { word = "TypeScript"; }
        return "Hello, " + word;
    }
    a.hello = hello;
})(a || (a = {}));
/// <reference path="./a.ts" />
var b;
(function (b) {
    console.log(a.hello("internal module"));
})(b || (b = {}));
#@end
//}

ひとまとまりのファイルとして出力されていますね。

=== 外部モジュール （External Modules）

外部モジュールは前述のとおり、1ファイル = 1モジュール としてプロジェクトを構成していく方式です。
@<code>{import foo = require("./foo")}のように書くと、そのファイルから ./foo.ts@<fn>{require-ext} を参照することができます。
ここでは、./foo が1つのモジュールとして扱われます。

外部モジュールはTypeScriptでは2つの方式に対応していて、その両方の形式ともTypeScript上では同じ文法で書けます。
コンパイル時に --module commonjs とするか --module amd とするかだけの違いです。

なので、ここでは細かいことは解説しません。
1つ目のCommonJSはNode.jsが採用している仕組みです。
2つ目のAMDはブラウザ上で外部モジュールを利用するための仕組みです。

さて、実際のコード例を見てみましょう。
foo.ts（@<list>{external-module/foo}）、bar.ts（@<list>{external-module/bar}）、buzz.ts（@<list>{external-module/buzz}）というファイルがあるとき、それぞれがモジュールになるので3モジュールある、という考え方になります。

//list[external-module/foo][foo.ts]{
#@mapfile(../code/typescript-basic/external-module/foo.ts)
import bar = require("./bar");

// Hello, TypeScript! と表示される
console.log(bar.hello());

// Good bye, TypeScript! と表示される
import bye = require("./buzz");
console.log(bye());
#@end
//}

//list[external-module/bar][bar.ts]{
#@mapfile(../code/typescript-basic/external-module/bar.ts)
export function hello(word = "TypeScript") {
  return "Hello, " + word;
}
#@end
//}

//list[external-module/buzz][buzz.ts]{
#@mapfile(../code/typescript-basic/external-module/buzz.ts)
function bye(word = "TypeScript") {
  return "Good bye, " + word;
}

export = bye;
#@end
//}

トップレベルの定義でexportしたものが別のファイルから参照されたときに公開されています。
コンパイルして結果を確かめてみましょう。
Node.jsに慣れている人なら、見覚えのある形式のコードが出力されていることが分かるでしょう。

//cmd{
$ tsc --module commonjs foo.ts
$ cat foo.js
#@mapfile(../code/typescript-basic/external-module/foo.js)
var bar = require("./bar");
console.log(bar.hello());
var bye = require("./buzz");
console.log(bye());
#@end
$ cat bar.js
#@mapfile(../code/typescript-basic/external-module/bar.js)
function hello(word) {
    if (word === void 0) { word = "TypeScript"; }
    return "Hello, " + word;
}
exports.hello = hello;
#@end
$ cat buzz.js
#@mapfile(../code/typescript-basic/external-module/buzz.js)
function bye(word) {
    if (word === void 0) { word = "TypeScript"; }
    return "Good bye, " + word;
}
module.exports = bye;
#@end
//}

Node.jsに慣れている人に不可解な仕様を1つ紹介しておきます。
通常、Node.jsでは@<code>{require("./sub/")}とすると自動的に ./sub/index が参照されますが、TypeScriptではそうならないため、明示的に@<code>{require("./sub/index")}としてやる必要があります（@<list>{external-module/reference-sub}）。

//list[external-module/reference-sub][ディレクトリを指定してもindex.tsを見てくれない]{
#@mapfile(../code/typescript-basic/external-module/reference-sub.ts)
// Node.jsだと sub/ で自動的に sub/index と同じ扱いになるのだが…
// import sub = require("sub/");
// TypeScript上では index が省略不可になっている
import sub = require("sub/index");

console.log(sub.hello());
#@end
//}

#@# TODO ここじゃないほうがいいけど、型としての参照だけだと消される恐れがある旨書く。

//footnote[require-ext][Node.js上の仕様（TypeScriptではない）について細かく言うと、require("./foo")すると最初に ./foo.js が探され、次に ./foo.json、./foo.node と検索されます]
