= オプションを知り己のコードを知れば百戦危うからず

本章ではtscのコマンドラインオプションについて解説していきます。
すべてを網羅することはしませんが、いくつかの重要なオプションについては知ることができるでしょう。

本章記載のオプションはtsconfig.jsonのcompilerOptionsに記載可能なプロパティ名と同一です。
tsconfig.jsonでは短縮形（@<code>{-d}や@<code>{-p}）は利用できないことに注意してください。

ここに記載されていないオプションで、知りたいものがあればIssue@<fn>{issue}にお寄せください。

//footnote[issue][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

#@# TODO 以下のオプションについてじわじわ書く
#@#   --moduleと--target 掛けあわせの話
#@#   --init
#@#   --project
#@#   --declaration
#@#   --noUnusedLocals
#@#   --noUnusedParameters
#@#   --listFiles
#@#   --forceConsistentCasingInFileNames
#@#   --moduleResolution
#@#   --noEmitOnError
#@#   --noImplicitReturns
#@#   --noFallthroughCasesInSwitch
#@#   --noImplicitThis
#@#   --pretty
#@#   --traceResolution
#@#   --types
#@#   --allowJs
#@#   --experimentalDecorators
#@#   --emitDecoratorMetadata

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
きっちりコードを書けば、この状態でも堅牢なアプリケーションを構築することはできます。
しかし、それはプログラマの努力の賜物であります。
できれば、コンパイラにしっかりチェックしてもらえたほうがコードの堅牢さがより確かなものになりますね。

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
