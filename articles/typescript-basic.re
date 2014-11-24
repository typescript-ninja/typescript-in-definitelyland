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

== 内部モジュール (internal modules)

TBD

== 外部モジュール (external modules)

TBD
