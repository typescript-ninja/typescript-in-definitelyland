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

== 関数定義

TBD

== クラス

TBD
TODO super

== 内部モジュール (internal modules)

TBD

== 外部モジュール (external modules)

TBD
