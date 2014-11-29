= TypeScriptの基本

JavaScriptの(TypeScriptではなく)仕様まで踏み込んだ解説については、拙著TypeScriptリファレンス(@<href>{http://www.amazon.co.jp/gp/product/484433588X?tag=damenako-22,Amazon}、@<href>{http://tatsu-zine.com/books/typescript-reference,達人出版会})を参照してください。
TypeScriptリファレンスをお持ちの場合(TypeScript 1.3.0現在)、この章は読まなくて大丈夫です。

TypeScript 1.0.0 対応の書籍ですが、基本的な部分は変わっていないです。
型システム周りはだいぶ強化されているので、@<chapref>{with-types}を熟読してもらえるとよいでしょう。

この章ではTypeScriptの基本的な構文を解説していきます。
TypeScriptの懐は広く、巨大で、ともすれば沼に落ち込みそうになります。
まずは、TypeScriptを使える必要最低限の知識を身につけていきましょう。

難しいこととか便利なことは@<chapref>{with-types}を見てください。
既存のJavaScriptの資産やライブラリを使いたい場合は@<chapref>{definition-file}の最初のほうを見てください。

#@# TODO enum と const enum どっか

== 変数

TypeScriptの変数宣言は概ねJavaScriptと一緒です。
違うのは、@<list>{var-with-annotations}のように変数名の後に@<code>{: 型名}という形式でその変数がどういう型の値の入れ物になるのか指定できるところです。
これをtype annotations、直訳して型注釈と呼びます。

//list[var-with-annotations][型注釈付きの変数]{
#@mapfile(../code/typescript-basic/var-with-annotations.ts)
var str: string;
var num: number;
var bool: boolean;

var func: Function;
// とりあえずなんか型
var obj: {};
#@end
//}

これの何が嬉しいかというと、@<list>{var-with-annotations-invalid}のように、型に反するようなコードを書くと、tscコマンドを使ってコンパイルした時にコンパイルエラーとして整合性がとれていない箇所が明らかになります。
安心安全！

//list[var-with-annotations-invalid][型注釈に反することをやってみる]{
#@mapfile(../code/typescript-basic/var-with-annotations-invalid.ts)
var str: string;
// var-with-annotations-invalid.ts(3,1): error TS2322: Type 'number' is not assignable to type 'string'.
str = 1;

var num: number;
// var-with-annotations-invalid.ts(7,1): error TS2322: Type 'boolean' is not assignable to type 'number'.
num = true;

var bool: boolean;
// var-with-annotations-invalid.ts(11,1): error TS2322: Type 'string' is not assignable to type 'boolean'.
bool = "str";
#@end
//}

いや、安心・安全を得るために型注釈いちいち全部に書くの？ダルすぎない？というあなたのために、TypeScriptは型推論の機能を備えています。
例えば@<list>{var-with-initializer}のように、型注釈を書かずに、変数定義と初期化を同時に行うようにします。

//list[var-with-initializer][初期化付き変数 = 最強]{
#@mapfile(../code/typescript-basic/var-with-initializer.ts)
var str = "string";
var num = 1;
var bool = true;

var func = function () {
};
var obj = {};
#@end
//}

これで手で型注釈を与えずに済むぞ！しかも、書き方がJavaScriptと全く同じになりました。
楽 + コンパイルによる型チェック = TypeScriptサイキョー。
お分かりいただけただろうか？

== クラス

TypeScriptには一般的な構文でのクラスの定義が備わっています@<list>{class-basic}。

//list[class-basic][クラスの要素様々]{
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
    constructor(boolA: boolean, public boolB: boolean, private boolC: boolean, protected boolD: boolean) {
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
// obj.numC; // private はダメ
// obj.numD; // protected もダメ
obj.boolB;
// obj.boolC; // private はダメ
// obj.boolD; // protected もダメ
obj.hello("TypeScript");
obj.dateA = new Date();
obj.dateA;
#@end
//}

上から順に見て行きましょう。

まずはクラス変数、インスタンス変数です。
クラスそのものやインスタンスに紐づく変数です。JavaScriptっぽく言うとプロパティですね。

アクセス修飾子として、private, public, protected(TypeScript 1.3.0より)などの可視性を制御するアクセス修飾子が利用可能です。
何も指定していない時のデフォルトの可視性はpublicになります。
しかしまぁコンパイル後のJSを見るとわかりますが、@<code>{<any>}とかを使うと簡単にそれら要素にアクセスできてしまうので過信は禁物です。
筆者はアクセス修飾子を使わず、アクセスされたくない要素はprefixに _ を使うなどの(JavaScriptでもよく見られた)運用をしています。
privateやprotectedは使わないほうが良いと思うんだよなぁ…。

次はコンストラクタです。
引数にアクセス修飾子をあわせて書くと、インスタンス変数としてその値が利用可能になります。
これを@<kw>{引数プロパティ宣言,parameter property declaration}と呼びます。
@<list>{class-constructor.ts}のようなコードを書くと@<list>{class-constructor.js}のようなJavaScriptが出てきます。

//list[class-constructor.ts][引数プロパティ宣言！]{
#@mapfile(../code/typescript-basic/class-constructor.ts)
class Sample {
    constructor(public str:string) {
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

@<list>{class-basic}に戻りまして。
次はメソッドです。
これも特に特筆すべき要素はありませんね。

最後に、get, setアクセサです。
これはtscでコンパイルする時に@<code>{--target es5}が必要になるやつです。
なかなかめんどくさいJavaScriptコードが生成されるようになりますが、ECMAScript 6で仕様に盛り込まれています。
#@# TODO 仕様に盛り込まれてるよな！？(不安)
これを使うと、getterしか定義してなくてもプログラム上値の代入もできてしまうので、"use strict"を併用して実行時に検出できるようにしましょう。

これら構文はECMAScript 6の文法を概ね踏襲しており、将来的にJavaScriptでもこれと同様の記法でクラス定義ができるようになります。

次に、クラスの継承も見て行きましょう。
継承も普通にできます@<list>{class-inherit}。
superを使っての親クラスのメソッドの参照も一応普通に使えます。

//list[class-inherit][普通に継承もあるよ]{
#@mapfile(../code/typescript-basic/class-inherit.ts)
class Base {
    greeting(name:string) {
        return "Hi! " + name;
    }
}

class Inherit extends Base {
    greeting(name:string) {
        return super.greeting(name) + ". How are you?";
    }
}

var obj = new Inherit();
// Hi! TypeScript. How are you? と出力される
console.log(obj.greeting("TypeScript"));
#@end
//}

TypeScript以外のオブジェクト指向の世界でも言えることですが、なんでもかんでも継承すればいいや！という考えはよくありません。
頑張ってオブジェクト指向に適した設計を行いましょう。

#@# TODO 引数プロパティ宣言はES6に入らないよなぁ？

== 関数

#@# NOTE クラスの後に関数定義の説明したーーーい！したくない？

=== 普通の関数

おう普通だよ(@<list>{function})。
型注釈の与え方や、引数をオプショナルにする方法だけJavaScriptと違いますね。
デフォルト値付き引数はECMAScript 6で入ります。

//list[function][色々な関数定義]{
#@mapfile(../code/typescript-basic/function.ts)
function hello(word:string):string {
    return "Hello, " + word;
}
hello("TypeScript");

// 返り値の型を省略すると返り値の型から推論される。明記したほうが読みやすい場合もある。
function bye(word:string) {
    return "Bye, " + word;
}
bye("TypeScript");

// ? をつけると呼び出し時に引数が省略可能になる
function hey(word?:string) {
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

可変長引数もあるよ(@<list>{function-args})！

//list[function-args][可変長引数もあるよ]{
#@mapfile(../code/typescript-basic/function-args.ts)
function hello(...args:string[]) {
    return "Hello, " + args.join(" & ");
}
// Hello, TS & JS と表示される
console.log(hello("TS", "JS"));
#@end
//}

なお、省略可能引数の後に省略不可な引数を配置したり、可変長引数を最後以外に配置するのはNGです(@<list>{function-invalid})。

//list[function-invalid][こういうのはアカン]{
#@mapfile(../code/typescript-basic/function-invalid.ts)
// オプショナルな引数の後に省略不可な引数がきてはいけない
// error TS1016: A required parameter cannot follow an optional parameter.
function funcA(arg1?:string, arg2:string) {
    return "Hello, " + arg1 + ", " + arg2;
}

// 可変長引数は必ず最後じゃないといけない
// error TS1014: A rest parameter must be last in a parameter list.
function funcB(...args:string, rest:string) {
    return "Hello, " + args.join(", ") + " and " + rest;
}
#@end
//}

ここまで見てきたオプショナルな引数とかデフォルト値付き引数とか可変長引数とかは全てクラスのコンストラクタやメソッドでもそのまま使えます。

=== アロー関数式

ECMAScript 6で導入される予定の@<kw>{アロー関数式,arrow function expression}を見ていきましょう(@<list>{arrow-function-expression})。

//list[arrow-function-expression][アロー関数式 短くてかっこいい]{
#@mapfile(../code/typescript-basic/arrow-function-expression.ts)
// 以下2つは(thisが絡まない限り)等価！
var funcA = () => true;
var funcB = function () {
    return true;
};

// NOTE ここのcallbackの型注釈の意味は別の章で解説！
// 引数を1つ取って返り値無し の関数を表します。
function asyncModoki(callback:(value:string)=>void) {
    callback("TypeScript");
}
// 旧来の書き方
asyncModoki(function (value:string) {
    console.log("Hello, " + value);
});
// アロー関数式だと楽やで
asyncModoki(value => console.log("Hello, " + value));
#@end
//}

短くてかっこいいですね。
将来のJavaScriptでは、アロー関数式による記述が主流になることは間違いないでしょう(なんせ、楽だし)。
早くNode.jsでも使えるようになって、Gruntfile.jsとかで使わせてほしいものです。

アロー関数式は1つの文しか持たない時、その文の値を返り値として使ってくれます(@<list>{arrow-function-expression-short})。

//list[arrow-function-expression-short][1つの文しか持たない時の便利な振る舞い]{
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
この仕組のおかげでクラスのメソッドなどでコールバック関数を使う時に、不要な混乱をおこさずに済みます。
例を見てみましょう(@<list>{arrow-function-expression-this})。

//list[arrow-function-expression-this][受け継がれるthisの値…！]{
#@mapfile(../code/typescript-basic/arrow-function-expression-this.ts)
"use strict";

class Sample {
    test():void {
        var funcA = () => {
            // ここでの this は元のまま(Sampleのインスタンス)
            console.log(typeof this);
        };
        var funcB = function () {
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

うーん、アロー関数式は期待通りの挙動ですね。
旧来の関数では値が undefined になっています。
JavaScriptに慣れている人も、慣れていない人も、特別に理由がない限りアロー関数式使っとけばいいんだよｵﾗｧﾝ！

== モジュール

プログラムの複雑さというのはクラスや関数だけではいかんともしがたく立ち向かいにくいものです。
プログラムの規模が大きくなればなるほど、機能ごとに分割して統治し、見通しをよくする必要があります。

そのための武器として、モジュールがあります。
概ね、JavaのpackageやC#のnamespaceと類似のものと考えてよいでしょう。

TypeScriptでは2種類のモジュールがあり、それぞれ内部モジュールと外部モジュールと呼ばれています。
これにはJavaScript実行環境が色々とあるためです。

1つは、ブラウザです。
ブラウザ上では複数のJavaScriptのファイルが連結され、まるで1つのファイルであるかのように解釈され実行されます。
これに対して階層構造を与えるための仕組みが内部モジュールです。

もう1つは、Node.jsです。
Node.jsでは、1つのファイルを1つの独立した構造と見なす仕組みが備わっています。
この、1ファイル = 1モジュール と見なした仕組みが外部モジュールです。

内部モジュール・外部モジュール、どっちの形式でプロジェクトを管理するかを決めるのは、非常に、非常に重要な決め事です。
内部モジュールと外部モジュールを混在させてプロジェクトを管理するのはあまり良い決断とはいえません。
なおかつ、プロジェクトの途中で内部モジュールと外部モジュールを切り替えるのは大変な苦痛を伴うため、最初によくよく調査・検証するようにしましょう。

マジだからな！！

=== 内部モジュール (internal modules)

まずは簡単な例を見てみましょう(@<list>{internal-module/basic})。

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

うーん、簡単だな！！
内部モジュール内部で定義した要素はクラスであれ関数であれなんであれ、最初に export をつけなければ外側から見えないようになります。

長い名前を使うのが嫌な時は@<list>{internal-module/import}のように、import句を使うこともできます。
このimport句の使い方は内部モジュールに限った話ではありません。
しかし、外部モジュールではまた別のimport句の使い方が出てくるので区別するようにしましょう。

//list[internal-module/import][import句で別名を作る]{
#@mapfile(../code/typescript-basic/internal-module/import.ts)
module a {
    export class Sample {}
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

単にファイルを分けて、何も工夫しないとJavaScriptやCoffeeScriptならともかく、プログラム全体をあわせた時の整合性のチェックを型でおこなうTypeScriptでは型が追えなくて困ってしまう場合があります。
そういう時のために、TypeScriptにはソースコード同士の関係性を記述するreference commentという仕組みがあります。

例えば、reference-a.ts(@<list>{internal-module/reference-a})とreference-b.ts(@<list>{internal-module/reference-b})があったとします。
reference-b.tsはreference-a.tsで定義している関数を呼び出しています。
つまり、reference-b.tsはreference-a.tsに依存しているわけです。
そこで、reference commentを使って@<code>{/// <reference path="./reference-a.ts" />}と記述します。
パスは相対パスでも絶対パスでも大丈夫ですが、一般的に相対パスを利用します。

//list[internal-module/reference-a][reference-a.ts]{
#@mapfile(../code/typescript-basic/internal-module/reference-a.ts)
module a {
    export function hello(word = "TypeScript") {
        return "Hello, " + word;
    }
}
#@end
//}

//list[internal-module/reference-b][reference-b.ts]{
#@mapfile(../code/typescript-basic/internal-module/reference-b.ts)
/// <reference path="./reference-a.ts" />

module b {
    console.log(a.hello("internal module"));
}
#@end
//}

これをコンパイルする時、reference-b.tsだけコンパイルすればreference-a.tsも自動的に一緒にコンパイルされます。
通常、何も指定しない場合reference-a.jsとreference-b.jsが生成されますが、--out オプションを併用することで1ファイルにまとめることも可能です。

//cmd{
$ tsc --out reference.js reference-b.ts
$ cat reference.js
#@mapoutput(../node_modules/.bin/tsc --out ../code/typescript-basic/internal-module/reference.js ../code/typescript-basic/internal-module/reference-b.ts && cat ../code/typescript-basic/internal-module/reference.js)
var a;
(function (a) {
    function hello(word) {
        if (word === void 0) { word = "TypeScript"; }
        return "Hello, " + word;
    }
    a.hello = hello;
})(a || (a = {}));
/// <reference path="./reference-a.ts" />
var b;
(function (b) {
    console.log(a.hello("internal module"));
})(b || (b = {}));
#@end
//}

ひとまとまりのファイルとして出力されていますね。

=== 外部モジュール (external modules)

TBD
