= アドバンスド型戦略

#@# TODO 文脈依存型
#@# TODO 文字列により特殊化されたオーバーロード
#@# TODO 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.10.7
#@# TODO || 演算子あたりのだるさ
#@# TODO オーバーロードの選択アルゴリズム

型のうち、難しいけど便利な話や、あまり関わりたくないけど実用上たまにお世話になる内容を解説していきます。
タプル型（tuple types）や直和型（union types）についての解説もありますよ！
なお、普段書くコードではこの章で出てくる内容をなるべく使わずに済む設計こそ良い設計だと筆者は考えています@<fn>{bad-code}。
#@# OK REVIEW muo: ちょい文の構造をつかみにくいと感じた

TypeScriptでコードを書く中で、JavaScriptで書かれたコードを型定義ファイルを介して扱う場面があります。
#@# OK REVIEW muo: ここも多少構造がわかりにくかった。「TypeScriptコードを書く中では、」などで始めると軽減されるかも
そういった時に本章の内容が活きてきます。
ただ、本章で書かれた内容を活かさないと上手く扱えないJavaScriptコードは、元々の品質が微妙なコードだと思います。

//footnote[bad-code][本章で触れる機能を使うほうがよい場合もあります。たとえば構文木の構築・分解時などです。自分の用途に本当にそれが必要かはよくよく考えてみてください。]
#@# OK REVIEW muo: 分解など→分解時など

== 共通最適型（Best Common Type）

#@# [1.3.0での仕様](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.10)

@<strong>{導入されたバージョン 1.0.0より前}

@<strong>{消滅したバージョン 1.4.0}

後述のtuple types, 及びunion typesが導入されたので、Best Common Typeの概念がどうなったのか調べてみてびっくりしました。
現在の最新仕様ではBCTという概念そのものが消滅してますね…。
こぇー…。
本書執筆時点では1.3.0しかリリースされていないため、一応現行では生きてる仕様なのですが…。
まぁ、そりゃ、より挙動が直感的なunion typesに置き換えられてしまいますよね…。

#@# TODO 最新の記述にあわせる
#@# TODO 本校執筆時 https://github.com/Microsoft/TypeScript/blob/9a5df8585bdb46427074b53fc0e46bc4f52dd261/doc/spec.md#4.12.2

消える仕様のことを書いても詮無いことなので、ここではざっくり書くにとどめます。

Best Common Typeの名の響きどおり、複数要素の間で型の統一がされない場合、共通最適型のアルゴリズムによって型が決定されていました。
#@# OK REVIEW muo: アルゴリズムの元に→アルゴリズムによって
たとえば、@<list>{bct-basic-1.3.0}のようになります。
#@# OK REVIEW muo: 感じ→感じです

//list[bct-basic-1.3.0][基本的な例]{
#@mapfile(../code/types-advanced/bct/basic-1.3.0.ts)
// {}[] でございます。
var array = [1, true];

// No best common type exists among return expressions. と怒られます。
// つまり 返り値が {} に推論されたので怒られた。
function test() {
  if (Math.random() < 0.5) {
    return 1;
  } else {
    return true;
  }
}
#@end
//}

1つ目は配列の要素の型が一致しないため、BCTを求めた結果、共通のsuper typeがなかったので {} になっています。
2つ目は関数のreturnステートメントが2つありますが、両者で型が一致しなかったので {} になり、結果コンパイルエラーとして怒られています。

本当に共通の要素がある場合、それに収束します。
たとえば、親クラスA、その子クラスB, Cがある場合、BCTはAになります（@<list>{bct-class-1.3.0}）。

//list[bct-class-1.3.0][あまり見かけないBCTが役に立つ例]{
#@mapfile(../code/types-advanced/bct/class-1.3.0.ts)
class A {
}
class B extends A {
  str: string;
}
class C extends A {
  num: number;
}

// array: A[] だ！
var array = [new A(), new B(), new C()];
#@end
//}

以上、解散！

== 型クエリ（Type Queries）

型クエリは指定した変数（やメソッドなど）の型をコピーします。
たとえば、@<list>{type-queries/basic}のような、クラスそのものを型として指定したい場合、それ専用の書き方は用意されていません。
そういう時に、型クエリを使います。

//list[type-queries/basic][クラスそのものの型だよ！]{
#@mapfile(../code/types-advanced/type-queries/basic.ts)
class Sample {
  str: string;
}

// この書き方だとSampleのインスタンスになる Sampleクラスそのものではない
var obj: Sample;
// Sample自体の型をコピー！ つまりこれはSampleクラスそのものだ！
var clazz: typeof Sample;

// それぞれに当てはまる値は以下のとおり なるほどな！
obj = new Sample();
clazz = Sample;

obj = new clazz();

// clazz を頑張って手で書くと以下に等しい
var alterClazz: {
  new (): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;
#@end
//}

メソッドなどの値も取れますが、thisを使ったりすることはできないため少しトリッキーなコードが必要になる場合もあります。
@<list>{type-queries/cheap-trick}の例は、prototypeプロパティを使っているためJavaScript力が多少ないと思いつかないかもしれません。

//list[type-queries/cheap-trick][prototypeを参照するとメソッドの型が取れる]{
#@mapfile(../code/types-advanced/type-queries/cheap-trick.ts)
class Sample {
  hello = (word = "TypeScript") => "Hello, " + word;
  bye: typeof Sample.prototype.hello;
}

var obj = new Sample();
obj.bye = obj.hello;
#@end
//}

型クエリはわざわざインタフェースを定義するのもめんどくさいけど…という時に使える場合もあります。
@<list>{type-queries/copy-invalid}では、1つ目の引数の型を2つ目の引数や返り値の型にもコピーして使っています。

//list[type-queries/copy-invalid][ここまで複雑にするならインタフェース使って]{
#@mapfile(../code/types-advanced/type-queries/copy-invalid.ts)
// このコードは正しい
function move(p1: { x1: number; y1: number; x2: number; y2: number; },
              p2: typeof p1
             ):   typeof p1 {
  return {
    x1: p1.x1 + p2.x1,
    y1: p1.y1 + p2.y1,
    x2: p1.x2 + p2.x2,
    y2: p1.y2 + p2.y2
  };
}

var rect = move({
  x1: 1, y1: 1, z1: 1, // 無駄に多い
  x2: 2, y2: 2, z2: 2  // プロパティ
}, {
  x1: 3, y1: 3,
  x2: 4, y2: 4
});
// z1 は typeof 1 には存在しないのだ！
// error TS2339: Property 'z1' does not exist on
// type '{ x1: number; y1: number; x2: number; y2: number; }'.
rect.z1;
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

ここまで来るとさすがに読みにくくなるのでインタフェースを1つ定義したほうがいいですね。

== タプル型（Tuple Types）

#@# http://qiita.com/vvakame/items/0b5060de5566f210479b

#@# [作業場所](https://github.com/Microsoft/TypeScript/pull/428)
#@# [仕様1](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.3.3)
#@# [仕様2](https://github.com/Microsoft/TypeScript/blob/release-1.3/doc/spec.md#3.6.5)

@<strong>{導入されたバージョン 1.3.0}

tuple(タプル)は、任意の数の要素の組です。
#@# OK REVIEW muo: 他の用語は基本カタカナだけどここだけひらがななのは何か意図あります?
JavaScriptではtupleはサポートされていないため、TypeScriptでのtupleもただのArrayになります。

既存のJavaScriptの資産を使おうとした時に、配列の形で多値を返してくるライブラリが稀にあります。
タプル型はおそらくそういった時に使うためのもので、TypeScriptでコードを書く時に多用するものではないでしょう。
というのも、普通にコードを書いている限りでは型推論の結果としてタプル型が出てこないためです。

タプル型は型(TypeScript)の世界にしか登場せず、コンパイル後のJavaScriptコードでは消えてしまいます。
#@# OK REVIEW muo: ちょいわかりづらさ感じました。TS側では使われるけどJSの世界には残らんという話だけどもう少し分かりやすくならないかなー
記述方法は配列の型指定へ @<code>{[typeA, typeB]} のように配列の要素の代わりに型名を記述していくだけです。
例を見てみましょう（@<list>{tuple/basic}）。

//list[tuple/basic][基本的な例]{
#@mapfile(../code/types-advanced/tuple/basic.ts)
// まずは今までどおりの配列から
// TypeScript 1.3.0 ではこれはいままでどおりの {}[]
// TypeScript 1.4.0? ではこれは別の箇所で解説している
//   union typesとあわせて(string | number | boolean)[]
var array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
var tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
  console.log(v);
});
#@end
//}

各要素の型を指定してやると、その要素のindexでアクセスした時に適切な型で扱われるようになります。

もちろん、タプル型はGenericsと組み合わせて利用できます(@<list>{tuple/with-generics})。

//list[tuple/with-generics][Genericsでの利用も可]{
#@mapfile(../code/types-advanced/tuple/with-generics.ts)
// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
  return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
#@end
//}

Good!いいですね。

タプル型のない今まで(1.1.0-1以前)のTypeScriptでは@<list>{tuple/without-tuple}のような、数値添字で型安全 or 配列として利用可能 という究極の選択をしなければならなかったのです…。
辛かった。

//list[tuple/without-tuple][TypeScript 1.1.0-1 までの苦しい運用]{
#@mapfile(../code/types-advanced/tuple/without-tuple.ts)
var tuple: { 0: number; 1: string; 3: boolean; } = <any>[1, "str", true];

tuple[1].charAt(0); // string は charAt を持つ！

// だがしかし(型のうえでは)Arrayではないので以下は通らない
// tuple.forEach(v => {
//   console.log(v);
// });
#@end
//}

さて、タプル型の重箱の隅を見ていきましょう。
要素数が多すぎる場合、指定されていない値の型はBCT(1.3.0まで)か、union types(1.4.0以降)になります。
その例を見てみましょう(@<list>{tuple/many-values})。

//list[tuple/many-values][値の要素数が多すぎる場合]{
#@mapfile(../code/types-advanced/tuple/many-values.ts)
// 要素が多い分にはOKだ！
var tuple: [string, number] = ["str", 1, "test"];

// 1.3.0 では型指定されていない要素は BCT(Best Common Type) つまりここでは {} になる
// 1.4.0? では範囲外の要素の型はすべての要素のunion、つまり string | number になる。
var value = tuple[2];

// 1.3.0 では以下の記述は正しい
// 1.4.0? ではダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
#@end
//}

お次は要素の順序がズレた場合、どうなるかを見てみましょう(@<list>{tuple/unshift})。

//list[tuple/unshift][絶望に身をよじれ…！]{
#@mapfile(../code/types-advanced/tuple/unshift.ts)
var tuple: [string, number] = ["str", 1];

// 先頭を number に…
tuple.unshift(1);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

…悲しい結果になりました。
@<code>{[1, true]}のような配列のリテラルをタプル型に推論しないのはおそらくこのためでしょう。
#@# OK REVIEW muo: ここのtuple typesはタプル型表記のほうが自然なのでは

unshiftやpopなど、配列の要素を操作する方法は色々ありますが、後からprototypeを拡張することすら可能なJavaScriptではTypeScriptコンパイラ側ですべてをキャッチアップすることは不可能です。
タプル型を扱う場合は要素数を変更するような操作をしないほうがよいでしょう。

TypeScript 1.3.0ではもうちょっと辛いコードを書くこともできます(@<list>{tuple/unshift-1.3.0})。

//list[tuple/unshift-1.3.0][1.3.0だともっと辛い]{
#@mapfile(../code/types-advanced/tuple/unshift-1.3.0.ts)
// 1.3.0 限定！！
var tuple: [string, number] = ["str", 1];

// 先頭を boolean に…
// unshiftの定義は Array<T>.unshift(...items: T[]): number
// 1.3.0 の場合 T が {} なのでコンパイルが通ってしまう
tuple.unshift(true);

// あぁっ！実行時エラー！
// Uncaught TypeError: undefined is not a function
tuple[0].charAt(0);
#@end
//}

結論：タプル型を過信するのはやめろ繰り返すタプル型を過信するのはやめろ！

== 直和型（Union Types）

@<strong>{導入されるバージョン 1.4.0}

はい、皆様待望の機能でございます。
"名前を言ってはいけないあの界隈"がよく使う用語を使って解説しないといけないのでビクビクですね。

一番最初に書いておくけど@<strong>{TypeScriptのコード書く時に積極的に使うものじゃあないぞ！！}

じゃあ解説していきましょう。
union typesはいわゆる直和型でございます。
この変数の値の型は、アレか、コレか、ソレ！のどれか。
どれかはわからない。
みたいな感じ。

なんのために入ったのか？というと、既存JavaScriptにより良い型定義を与えるために入った…！！と言ってしまっていいでしょう。
実際、自分でTypeScriptコード書いてる時に欲しくなる機能ではあまりありません。
ECMAScriptさん、パターンマッチもないしー。

まずは簡単な例から見ていきましょう(@<list>{union-types/basic})。

//list[union-types/basic][型A | 型B → 新食感！]{
#@mapfile(../code/types-advanced/union-types/basic.ts)
var a: string | boolean;
// string | number なので以下はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean'.
// Type 'number' is not assignable to type 'boolean'.
// a = 1;

var b: boolean | number;
// a と b を合体させると string | boolean | boolean | number だ！
// 実際にはc の型は string | number | boolean となる
var c: typeof a | typeof b;
#@end
//}

ハイ、型注釈で複数の型を | で区切って書ける感じです。
既存のJavaScriptライブラリだとこういった感じの困った返り値の関数がかなりあります。
あとは普通にTypeScriptを書いている時でもSyntaxTreeとかをコードから構築する時にはあったほうが便利かもしれません。

ご覧のとおり、union types中の型の順番とかは関係ない(交換可能)し、union typesのunion typesなどは単純にまとめて1つのunion typesに統合できます。
次に見る@<list>{union-types/subtype}のように、union typesに含まれる型同士が親子関係にある場合、単に親にまとめられます。
これも実用上問題ありません。
というのも、@<hd>{type-guards}で紹介する仕組みがあるからです(後で読んでね！)。

//list[union-types/subtype][要素Bが要素Aのサブタイプの場合Aにまとまる]{
#@mapfile(../code/types-advanced/union-types/subtype.ts)
class Base {
  str: string;
}
class Inherit extends Base {
  num: number;
}

// 明示的に指定しとくと Base | Inherit となる(今のところ
var obj: Base | Inherit;

// Base にも Inherit にも存在する要素には普通にアクセスできる
obj.str;
// num は Base には存在しないのでアウトー！
// error TS2339: Property 'num' does not exist on type 'Base | Inherit'.
// obj.num;

// Base | Inherit は実質単に Base なのでそのまま Base に丸められる
var base: typeof obj;
// 型が Base でも子クラスだから普通に代入できるので無問題
base = new Inherit();
#@end
//}

この辺り、仕様書上は若干小難しく書かれているのですが、単に最も少ない要素数になるように型がまとめられていくだけです。

自然にTypeScriptを書いていて、union typesを目にする機会は3種類あります。
|| 演算子を使った時、条件(三項)演算子を使った時、配列リテラルを使った時です(@<list>{union-types/inferred})。

//list[union-types/inferred][こういう時は目にしますね]{
#@mapfile(../code/types-advanced/union-types/inferred.ts)
// and の型は string | boolean
var and = "str" || true;
// cond の型は string | number
var cond = true ? 1 : "str";
// array の型は (string | number | boolean)[]
var array = [1, true, "str"];
#@end
//}
#@# OK REVIEW muo: キャプション、ひとつ上のコピペにしてない?

一番よくお目にかかるのは配列リテラルでしょうか。
TypeScript一般のベストプラクティスとして1つの配列で複数の型の値を扱わないほうが堅牢なコードになるため、きれいなコードを書いている限りはあまり見ないかもしれません。

型注釈として関数を与える時は記法にちょっと気をつけないとコンパイルエラーになります(@<list>{union-types/syntax})。

//list[union-types/syntax][型名をカッコで囲うんです？]{
#@mapfile(../code/types-advanced/union-types/syntax.ts)
// 引数無しの返り値stringな関数 な型注釈
var func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// var a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
var b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
var c: { (): string; } | { (): boolean; };

// union typesじゃない時でも使えるけど見づらいな！
var d: (() => string);
#@end
//}

ぶっちゃけ見づらいですよね。
仕様書上でも@<href>{https://github.com/Microsoft/TypeScript/issues/1267,カッコの対応ミスってた}@<fn>{spec-example-bug}りするので、頑張って気をつけましょう。
まぁ、コンパイルすれば分かるし気にしすぎる必要はありません。

union typesな値を使う時は、一応型アサーションも使えますがなるべくなら避けてとおりましょう(@<list>{union-types/type-assertion})。
次に説明する@<hd>{type-guards}を使いましょう。話はそれからだ！

//list[union-types/type-assertion][一応使えるよ こうすれば]{
#@mapfile(../code/types-advanced/union-types/type-assertion.ts)
// 注意！ここでやってるやり方よりもtype guardsを使うんだ…！！
// 型アサーションは悪い。常に悪い。なるべく使わないこと。

var obj: string | number | Date;

// string 扱いしてみる
(<string>obj).charAt(0);

// number 扱いしてみる
(<number>obj).toFixed(2);

// Date 扱いしてみる
(<Date>obj).getTime();

// 値の集合に含まれない型にしてみると普通に怒られる
// error TS2352: Neither type 'string | number | Date'
//     nor type 'RegExp' is assignable to the other.
// (<RegExp>obj).test("test");
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

色々試してみても、期待以上に頭がよかったりはしない(@<list>{union-types/cant-inferred-invalid})。

//list[union-types/cant-inferred-invalid][こういうのは型推論できない]{
#@mapfile(../code/types-advanced/union-types/cant-inferred-invalid.ts)
function test<T>(...args: T[]): T[] {
  return args;
}
// (number | boolean)[] にはならなかった。残念。
// error TS2453: The type argument for type parameter 'T'
// cannot be inferred from the usage.
// Consider specifying the type arguments explicitly.
var v = test(1, true);
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

#@# TODO Contextual Union Types https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.4.1

#@# NOTE http://togetter.com/li/749889
#@# NOTE 代数的データ型 algebraic data type 型を組み合わせて作られる型のこと
#@# NOTE 多相性 (a: any[], b: any) => [].concat(a).concat([b]); みたいな。a, b の型に関係なく関数が操作できる
#@# NOTE 直和型 variant TypeScriptの union types っぽい OCamlのは値が取れたりしてもうちょっと強いので本当に同じかいまいちわからん https://twitter.com/omasanori/statuses/537256100915990529
#@# NOTE 直積型 ??? TypeScriptのtype aliasっぽい…？ type 線 = 点1 * 点2 みたいな たかだか一種類のコンストラクタしかもたないもの(点を2つ取るもののみとか)
#@# NOTE 小クワガタ 黒くて挟む角が2つ生えてる虫

//footnote[spec-example-bug][https://github.com/Microsoft/TypeScript/issues/1267]

=={type-guards} 型のためのガード（Type Guards）
#@# TODO 和訳が微妙…
@<strong>{導入されるバージョン 1.4.0}

type guards@<fn>{type-guards-naming}は、union typesが導入されたことで変数の型が一意ではなくなってしまったため、それを自然に解決するために導入された仕組みです。
type guardsは"変数Aが○○という条件を満たす時、変数Aの型は××である"というルールを用いて、条件チェックを行った後の変数の型を××に狭めることができます。

//footnote[type-guards-naming][guard for typesとかtype narrowing rulesとかのほうがよかったと思うんだけどなぁ…]

=== typeofによるtype guards

JavaScriptの typeof は指定した値がどういう性質のオブジェクトかを調べ、文字列で返す演算子です。
ECMAScript 5の範囲では、変換ルールは次のとおりです。

 * string の時は "string" を返す
 * boolean の時は "boolean" を返す
 * number の時は "number" を返す
 * undefined の時は "undefined" を返す
 * 関数として呼び出し可能な場合は "function" を返す
 * それ以外の場合(nullを含む！)は "object" を返す

これを利用して、変数の型を狭めます。

==== 使い方

一番簡単な使い方から見ていきましょう(@<list>{type-guards/typeof-basic})。
TypeScriptのtype guardsでは、typeofの結果がstring, boolean, numberの場合、その型に絞り込むことができます。

//list[type-guards/typeof-basic][実際の型がわからないなら調べるしかないじゃない！]{
#@mapfile(../code/types-advanced/type-guards/typeof-basic.ts)
var obj: number | string;
if (typeof obj === "string") {
  // ここでは string と確定されている！
  obj.charAt(0);
} else {
  // ここでは消去法で number と確定されている！
  obj.toFixed(2);
}
#@end
//}

TypeScript 1.4.0 以前のTypeScriptであれば、このif文のthen節の中でも変数objの型は型注釈したものから変わらずそのままでした。
#@# OK REVIEW muo: どのまま?明示したほうがいい
type guardsが導入された後は"変数objがtypeofで調べた時にstringであるという条件を満たす時、変数objの型はstringである"というルールに基づき、if文のthen節の中では変数objはstringと型付けされます。
なお、この時の比較は必ず@<code>{===}を使う必要があります。
@<code>{==}ではダメです。

もう一例見てみましょう。
#@# OK REVIEW muo: 起こる?おこなわれる?
@<list>{type-guards/typeof-invalid}では、anyと指定された変数をtype guardsでstringに絞り込んでいます。
そのため、@<code>{obj.toFixed(0)}というstringには存在しないメソッドを呼びだそうとするとコンパイルの段階でエラーにしてくれます。

//list[type-guards/typeof-invalid][1.3.0ではエラーとして検出できぬ]{
#@mapfile(../code/types-advanced/type-guards/typeof-invalid.ts)
var obj: any;
if (typeof obj === "string") {
  // ここでは string と確定されている！
  // number にしか存在しないメソッドを呼ぶとコンパイルエラー！
  // error TS2339: Property 'toFixed' does not exist on type 'string'.
  obj.toFixed(0);
}
#@end
//}

うーん、便利ですね。

ちなみに、本当に型を"狭める"だけなので、@<list>{type-guards/typeof-cant-narrow}のような互換性のない型に狭めることはできません。

//list[type-guards/typeof-cant-narrow]["狭める"だけなんだなぁ]{
#@mapfile(../code/types-advanced/type-guards/typeof-cant-narrow.ts)
var obj: number;
if (typeof obj === "string") {
  // number を string に"狭める"ことはできない…
  // コンパイル通っちゃう
  obj.toFixed(0);
}
#@end
//}

まぁ、指定した型どおりの値が入ってくるのであればなにも問題はないな！

==== 後続の型の絞込み

typeofによるtype guards特有の仕様として、後続の型の絞込みがあります(@<list>{type-guards/typeof-removes})。

//list[type-guards/typeof-removes][型の絞込み！]{
#@mapfile(../code/types-advanced/type-guards/typeof-removes.ts)
var obj: number | string | boolean;

if (typeof obj === "string") {
  // ここでは string と確定されている！
  obj.charAt(0);
} else {
  // ここでは string が引かれ number | boolean;
  obj;
}

if (typeof obj === "string") {
  // ここでは string と確定されている！
  obj.charAt(0);
} else if (typeof obj === "number") {
  // ここでは number と確定されている！
  obj.toFixed(2);
} else {
  // ここでは string, number が引かれ boolean となる！
  obj;
}
#@end
//}

最初にstringとわかったら、後続のelse句ではstringは絶対入ってこないことはわかりきっています。
親切な仕様ですね。

=== instanceofによるtype guards

primitive typesだけtype guardsが使えてもあんまり嬉しくないので、instanceofを使ったtype guardsももちろんあります！！
でも、仕様が完璧とはちょっと言い難いんですよね。

JavaScriptにおけるinstanceofは、ある値が指定した関数のインスタンスであるかを調べる演算子です。
プロトタイプチェーンも遡ってみていくので、親子関係にある場合もインスタンスかどうかを調べることができます。

動作例を確認しておきます(@<list>{type-guards/instanceof})。

//list[type-guards/instanceof][instanceof の挙動]{
#@mapfile(../code/types-advanced/type-guards/instanceof.ts)
class Base {
}

class InheritA extends Base {
}
class InheritB extends Base {
}

var obj = new InheritA();

// true と表示される
console.log(obj instanceof Base);
// true と表示される
console.log(obj instanceof InheritA);
// false と表示される
console.log(obj instanceof InheritB);

// ｵﾗｧﾝ!!! 無理矢理親を差し替える！
InheritA.prototype = new InheritB();
obj = new InheritA();
// true と表示される
console.log(obj instanceof InheritB);
#@end
//}

オブジェクトのprototypeと一致するか順繰り見ていくだけですね。

==== 使い方

instanceofで型を絞り込みます(@<list>{type-guards/instanceof-basic})。

//list[type-guards/instanceof-basic][instanceofの挙動]{
#@mapfile(../code/types-advanced/type-guards/instanceof-basic.ts)
class A {
  str: string;
}
class B {
  num: number;
}

var obj: A | B;
if (obj instanceof A) {
  // ここでは A と確定されている！
  obj.str;
} else if (obj instanceof B) {
  // ここでは B と確定されている！
  obj.num;
}
#@end
//}

typeofのtype guardsと違って、else句が自動的に絞込まれたりはしません。
個別にちゃんと調べないとダメです。
まぁ、primitiveな型の値と違って、親子関係があるので後続の型を絞ってよいと断言できないパターンがちょいちょいありますからね。
仕方ないね。

==== 自分で定義した型で使うには？

TypeScriptが標準で提供する(lib.d.tsに書いてある)型や、TypeScript上で定義したクラスだけがtype guardsの対象になる、そんなの悲しすぎ！
というわけで、それを解消する方法が用意されています。

最も簡単なのは、型定義上でも、クラスとして定義することです。
クラスはデフォルトでinstanceofによるtype guardsに対応しています。

もう一つは、Functionと互換性をもたせたうえでprototypeプロパティを生やす方法です(@<list>{type-guards/instanceof-prototype})。

//list[type-guards/instanceof-prototype][prototype の型が参照される]{
#@mapfile(../code/types-advanced/type-guards/instanceof-prototype.ts)
interface AStatic {
  new (): AInstance;
  prototype: AInstance;
}
interface AInstance {
  str: string;
}
declare var A: AStatic;

var obj: AInstance | {};
if (obj instanceof A) {
  // ここでは AInstance と確定されている！
  obj.str;
}
#@end
//}

instanceofの右側の値の、その型の、prototypeプロパティの、型！
つまり、instanceofの右側のAの型のAStaticのprototypeプロパティの、型(AInstance)！
…まわりくどい！

そもそも、公式のTypeScript Handbookの@<href>{http://www.typescriptlang.org/Handbook#writing-dts-files,Writing .d.ts files}@<fn>{writing-dts-files}の項目、クラスの分割定義について言及している箇所でも、prototypeプロパティなんかわざわざ定義してないんですよね…。
#@# OK REVIEW muo: ののののの

このため、この原稿を執筆している時点でlib.d.tsに組み込みのRegExpにprototypeプロパティが定義されておらずinstanceofによるtype guardsができないという事態がありました。
これをTypeScriptコンパイラのリポジトリに報告し、pull requestしたのが奇しくも筆者の初のコードのコントリビュートになりました@<fn>{missing-prototype-properties}。やったぜ！

だがしかし、それでは根本的な解決になっていなくて、そもそもこれだとDefinitelyTypedのほぼすべての型定義ファイルがtype guards未対応になっちゃうし、今あるものを頑張って対応したとしても今後送られてくる型定義ファイルについて全てのpull requestでprototypeプロパティを実装してください！と指摘して回るのはダルすぎるでしょ…！

というわけで、prototype propertyの代わりに、construct signatureを持っている場合はそちらの返り値を参照するのはどう？という@<href>{https://github.com/Microsoft/TypeScript/issues/1283,提案}@<fn>{type-guards-by-construct-signature}を行っています。
コレがそのまま通るかはわからないけど、1.4.0リリース時に仕様が改善されていたら俺のことめっちゃ褒めてくれてもいいと思います( ｰ`дｰ´)ｷﾘｯ

話を戻しましょう。
prototypeプロパティを持っているだけではダメで、Functionとの互換性を持たせる必要があります。
一番簡単なのは、インタフェースにconstruct signatureかcall signatureのどちらか、または両方を持たせることです。
もし、このどちらも行わず、Function型との互換性がなくなると、@<list>{type-guards/instanceof-prototype-invalid}に示すようなエラーになります。

//list[type-guards/instanceof-prototype-invalid][右側はanyかFunctionと互換性のある型にしろってさ]{
#@mapfile(../code/types-advanced/type-guards/instanceof-prototype-invalid.ts)
interface AStatic {
  // 以下のどちらかがないと Function との互換性が無い
  // (): AInstance;
  // new (): AInstance;
  prototype: AInstance;
}
interface AInstance {
  str: string;
}
declare var A: AStatic;

var obj: any;
// error TS2359: The right-hand side of an 'instanceof'
// expression must be of type 'any' or of a type assignable
// to the 'Function' interface type.
if (obj instanceof A) {
}
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

もう一つ、重要なコツを示しておきます。
それは、エラーメッセージの読み方です(@<list>{type-guards/instanceof-failed-invalid})。

//list[type-guards/instanceof-failed-invalid][んん？なんだこのエラーは？]{
#@mapfile(../code/types-advanced/type-guards/instanceof-failed-invalid.ts)
interface AStatic {
  new (): AInstance;
  // prototype: AInstance; がない！
}
interface AInstance {
  str: string;
}
declare var A: AStatic;

var obj: AInstance | Date;
if (obj instanceof A) {
  // ここでは AInstance に確定されていてほしかった…
  // error TS2339: Property 'str' does not exist on type 'Date | AInstance'.
  obj.str;
}
#@end
//}

instanceofを使ってtype guardsで型の絞込みをしたつもりのシチュエーションです。
しかし、AStaticはprototypeプロパティを持っていません。
つまり、type guardsは効力を発揮しなかったんだよ！ΩΩΩ＜な、なんだってー！

ここでのエラーの根本的な原因は、変数objの型をAInstanceに絞ることに失敗したことです。
ですが、実際に表示されるエラーメッセージは"AInstance | Dateだとstrプロパティにアクセスして安全かわかんなかったっす…"というメッセージです。
type guardsの失敗が、別のエラーとなって間接的に表れてしまっています。
union types絡みのエラーに慣れていないと、このエラーメッセージとtype guardsが実は失敗している！ということが結びつきにくいので気をつけましょう。
#@# OK REVIEW muo: 慣れて の前になんか対象物指定あったほうがよい（この周辺だいぶ飛ばした書き方になってるので、なるべくこのへんの読みやすさ改善でカバーしとくべき）

#@# TODO https://github.com/Microsoft/TypeScript/issues/1283 が解決されない限り、definition-file.re に注意書きを書き足す

//footnote[writing-dts-files][http://www.typescriptlang.org/Handbook#writing-dts-files]
//footnote[missing-prototype-properties][https://github.com/Microsoft/TypeScript/pull/1317 初pull reqest & 初merge！]
//footnote[type-guards-by-construct-signature][https://github.com/Microsoft/TypeScript/issues/1283]

==== Genericsとtype guards

さて、ちょっと前に書いた"instanceofの右側の値の、その型の、prototypeプロパティの、型！"という表現は、実はちょっと不正確です。

prototypeのプロパティの型に、Genericsが絡むと、話がややこしくなります。
話がややこしい奴代表として、Array<T>に登場してもらいましょう(@<list>{type-guards/array-declaration-invalid})。

//list[type-guards/array-declaration-invalid][lib.d.tsからArrayの定義を抜粋]{
#@mapfile(../code/types-advanced/type-guards/array-declaration-invalid.d.ts)
// lib.d.tsから抜粋 本当はもうちょっと色々ある
declare var Array: {
  new (arrayLength?: number): any[];
  (arrayLength?: number): any[];
  isArray(arg: any): boolean;
  prototype: Array<any>;
};

interface Array<T> {
  length: number;
  push(...items: T[]): number;
  pop(): T;
}
#@end
//}

instanceofでtype guardsで型を狭めた時、any[]になるのかな…？と一瞬思いますが、ことはそう簡単ではありません(@<list>{type-guards/instanceof-array-invalid}、@<list>{type-guards/instanceof-array}、@<list>{type-guards/instanceof-empty-array-invalid})。

//list[type-guards/instanceof-array-invalid][絞込み、失敗！]{
#@mapfile(../code/types-advanced/type-guards/instanceof-array-invalid.ts)
var array: number[] | Date;

if (array instanceof Array) {
  // Array.prototype の型は Array<any> つまりは any[] …！
  // any[] は number[] に代入可能だな！！型を狭められたに違いない！
  // error TS2339: Property 'length' does not exist on type 'number[] | Date'.
  // (つд⊂)ｺﾞｼｺﾞｼ (；ﾟ Дﾟ) エラーだと…！？
  array.length;
}
#@end
//}

//list[type-guards/instanceof-array][any[\] に絞り込むのはできる]{
#@mapfile(../code/types-advanced/type-guards/instanceof-array.ts)
var array: any[] | Date;

if (array instanceof Array) {
  // any[] に絞り込まれる
  array.length;
}
#@end
//}

//list[type-guards/instanceof-empty-array-invalid][{}[\] には絞り込めない…！]{
#@mapfile(../code/types-advanced/type-guards/instanceof-empty-array-invalid.ts)
var array: {}[] | Date;

if (array instanceof Array) {
  // error TS2339: Property 'length' does not exist on type 'Date | {}[]'.
  array.length;
}
#@end
//}

うーん、これもうわかんねぇな？
TypeScriptコンパイラのソースコードをざっくり読んだ感じだと、Array<any>をさらに型パラメタをインスタンス化する前に戻して、Array<T>にしてから絞り込みのチェックをしているようなのですが、その場合Array<{}>として評価されてるのかなー、と思いきや@<list>{type-guards/instanceof-empty-array-invalid}を見る限り、そうともいえなさそうなんですよね…。

この辺り、わかりやすいルールが提示されないと実用上使いにくくて困りますね。
とりあえず、Genericsが絡む場合はtype guardsに頼らず、型アサーションに頼るほうが詰まらずにすむ…という認識でよいでしょう@<fn>{type-guards-with-generics}。

#@# TODO Overloadは？状況に変化が生じていないか確認する
#@# TODO だるすぎるからなんとかして

//footnote[type-guards-with-generics][https://github.com/Microsoft/TypeScript/issues/1283 で議論されるかも？]

=== type guardsと論理演算子

&& とか || とか ? とか ! とかの論理演算子にもちゃんと対応しているよ！(@<list>{type-guards/operator})

//list[type-guards/operator][ブール代数みたいな演算に対応してる]{
#@mapfile(../code/types-advanced/type-guards/operator.ts)
var obj: number | boolean | string;

// && 演算子で絞込み
typeof obj === "string" && obj.charAt(0);
// 以下のようなコードはエラーになる！
// error TS2339: Property 'charAt' does not exist on type 'number'.
// typeof obj === "number" && obj.charAt(0);

// || 演算子でunion typesに
if (typeof obj === "string" || typeof obj === "boolean") {
  // string | boolean に絞り込まれる
} else {
  // 消去法で number ！ (typeof による type guards だけ)
}

// 三項演算子は普通にif文と一緒の挙動
typeof obj === "string" ? obj.charAt(0) : obj;
// 以下と等価
if (typeof obj === "string") {
  obj.charAt(0);
} else {
  obj;
}

// 一応、否定演算子にも対応している
if (!(typeof obj !== "string")) {
  // 否定の否定は普通にそのまんまstringだな！ちゃんと絞りこまれます
  obj.charAt(0);
}
#@end
//}

あんまり使わない気がしますね。

=== type guardsの弱点

type guardsは型システム上の仕組みだということを忘れてはいけません。
JavaScriptの実行環境とは全く関係がないのです。

TypeScriptでは、構造的部分型の仕組みにより、クラスが要求されている箇所に、互換性のある別の値を代入することができます。

その仕組みを使って、@<list>{type-guards/weakspot}のようなコードが書けてしまいます。

//list[type-guards/weakspot][構造的部分型とtype guards]{
#@mapfile(../code/types-advanced/type-guards/weakspot.ts)
class Sample {
  str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
  str: "Hi!"
};

if (obj instanceof Sample) {
  // 型は Sample に絞られている が！ 絶対に到達しない！
  // 現在の obj はプロトタイプチェーンにSampleを持たない！
  console.log(obj.str);
}
#@end
//}

objはSampleを型として持ち、その値として互換性のあるオブジェクトリテラルを持っています。
コンパイル後のJavaScriptコード(@<list>{type-guards/weakspot.js})を見ると、objの値がSampleクラスのインスタンスではないことが一目瞭然ですが、TypeScript上で見ると型を元に判別されていると勘違いしやすい、ということを頭の片隅においておきましょう。

//list[type-guards/weakspot.js][コンパイル後のJS]{
#@mapfile(../code/types-advanced/type-guards/weakspot.js)
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
var obj = {
    str: "Hi!"
};
if (obj instanceof Sample) {
    console.log(obj.str);
}
#@end
//}

これを回避する方法は2つあります。
1つ目はtype guardsに頼らず、今までどおりに処理することです(@<list>{type-guards/vs-weakspot1})。
2つ目はprivateな要素をクラスに突っ込んでしまうことです(@<list>{type-guards/vs-weakspot2-invalid})。

//list[type-guards/vs-weakspot1][type guardsに頼らず生きる]{
#@mapfile(../code/types-advanced/type-guards/vs-weakspot1.ts)
class Sample {
  str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
  str: "Hi!"
};

if (obj !== null) {
  var str = (<Sample>obj).str;
  if (typeof str === "string") {
    console.log(str);
  }
}
#@end
//}

//list[type-guards/vs-weakspot2-invalid][privateな要素があれば構造的部分型で値を偽造できない]{
#@mapfile(../code/types-advanced/type-guards/vs-weakspot2-invalid.ts)
class Sample {
  private _tmp: any;
  str: string;
}

// privateなインスタンス変数があるクラスのインスタンスは偽造できない！
// error TS2322: Type '{ _tmp: null; str: string; }' is not
//     assignable to type 'Sample'. Property '_tmp' is private
//     in type 'Sample' but not in type '{ _tmp: null; str: string; }'.
var obj: Sample = {
  _tmp: null,
  str: "Hi!"
};
#@end
//}
#@# OK REVIEW muo: これ右端あふれてます

色々書きましたが、一番の解決策はunion typesやanyを多用せず、真っ当なコードを書けるよう設計することですね。

== 型の別名（type alias）

@<strong>{導入されるバージョン 1.4.0}

最初に書いておきます。@<strong>{可能な限りtype aliasを使うな！interface使え！}
#@# OK REVIEW muo: 主観だけど、 〜書いておきます。 のほうがいいんじゃないかなぁ。そのほうが主張すっきり伝わりそう
筆者はtype aliasの乱用を恐れています！

type aliasもunion typesの扱いを便利にするために導入された機能です。
機能としてはただ単に、型をひとまとまりにして、それに名前が付けられるだけです。
そんだけ。

type aliasは仕様上、interfaceと同じように利用できる場面もあります。
ですが、基本的にtype aliasはinterfaceより機能が貧弱であるためなるべく避けるほうがよいでしょう。

代表例を見てみましょう(@<list>{type-alias/basic})。

//list[type-alias/basic][頻出するunion typesに名前をつける]{
#@mapfile(../code/types-advanced/type-alias/basic.ts)
type FooReturns = string | number | boolean;

interface Foo {
  bar(): FooReturns;
  buzz(): FooReturns;
  barbuzz(): FooReturns;
}
#@end
//}

わかりやすいですね。
1ヶ所変更すると、関連箇所がすべて更新されるのも便利です。

tuple typesに名前をつけることもできます(@<list>{type-alias/tuple})。

//list[type-alias/tuple][tuple typesに名前をつける]{
#@mapfile(../code/types-advanced/type-alias/tuple.ts)
// tuple typesに名前をつける
type Point = [number, number];
type Circle = [Point, number];

var c: Circle = [[1, 2], 3];

// でも、こっちのほうがTypeScriptとしては適切よね
module alternative {
  class Point {
    constructor(public x: number, public y: number) {
    }
  }
  class Circle {
    constructor(public p: Point, public r: number) {
    }
  }
  var c: Circle = new Circle(new Point(1, 2), 3);
}
#@end
//}

素直にクラスでやれ！って感じですね。

type aliasは型に別名をつけるだけで、コンパイルされると消えてしまう存在です。
そのため、@<list>{type-alias/do-not-have-instance-invalid}のようなコードは書くことができません。

//list[type-alias/do-not-have-instance-invalid][import句とは違うのだよ！import句とは！]{
#@mapfile(../code/types-advanced/type-alias/do-not-have-instance-invalid.ts)
// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = string[];

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
var strArray = new StringArray();
#@end
//}

TypeScriptの仕様書にのっているtype aliasの利用例について、interfaceでの書き換えができるものを示します(@<list>{type-alias/spec-example})。
union typesが絡むもの、tuple typesが絡むもの、型クエリが絡むものだけが、interfaceで置き換えることができません。

//list[type-alias/spec-example][interfaceを使うんだ！]{
#@mapfile(../code/types-advanced/type-alias/spec-example.ts)
// これらはinterfaceで表現不可 type aliasで正解
type StringOrNumber = string | number;
type TextObject = string | { text: string };
type Coord = [number, number];
type ObjectStatics = typeof Object;

// これらはinterfaceで表現可能
type HolidayLookup = Map<string, Date>;
interface AltHolidayLookup extends Map<string, Date> {
}

type Callback = (data: string) => void;
interface AltCallback {
  (date: string): void;
}

type RecFunc = () => RecFunc;
interface AltRecFunc {
  (): AltRecFunc;
}
#@end
//}

また、type aliasではGenericsを使った名前を定義することができません。
#@# OK REVIEW muo: 「〜定義することができません。」のほうが良いかな。前の段落でそのように書いてるので統一したほうが良い
つまり、@<list>{type-alias/with-type-parameters-invalid}みたいなコードは文法的に正しくないためコンパイルがとおりません。

//list[type-alias/with-type-parameters-invalid][こういうコードは書けないんじゃ]{
#@mapfile(../code/types-advanced/type-alias/with-type-parameters-invalid.ts)
// "type" Identifier "=" Type ";" という文法だからダメ
// Identifier は TypeParameters を含まない
// type-alias-with-type-parameters-invalid.ts(4,9): error TS1005: '=' expected.
type Foo<T> = Array<T> | T;

// こういうのは書けないんだ…すまんな…
var obj: Foo<number>;
obj = [1, 2];
obj = 1;
#@end
//}

こういう場合は、名前をつけずに関係各所にコピペして頑張るしかないですね…。@<fn>{type-alias-with-generics}

最後に、type aliasがいかにinterfaceに劣るかをまとめておきます。

 * interface は extends や implements とあわせて使える
 ** type aliasは無理
 * interface は定義の統合ができるので後から自由に拡張できる
 ** type aliasは無理
 * interface は Genericsが使えて型パラメータを持てる
 ** type aliasは無理
 * interface が絡んだ時のコンパイルエラーにはinterface名が表示されてわかりやすい
 ** type aliasは展開されて表示されちゃうので無理

@<strong>{interfaceでできることをtype aliasでやるな！}

//footnote[type-alias-with-generics][こういうコードをmizchi君が書きたがってた https://twitter.com/mizchi/statuses/537908273865703424]
