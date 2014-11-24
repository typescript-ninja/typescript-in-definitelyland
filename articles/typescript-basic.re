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

== 関数定義

#@# TODO クラスの後に関数定義の説明したーーーい！したくない？
TBD

== 内部モジュール (internal modules)

TBD

== 外部モジュール (external modules)

TBD
