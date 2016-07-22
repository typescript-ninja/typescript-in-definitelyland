= オプションを知り己のコードを知れば百戦危うからず

本章ではtscのコマンドラインオプションについて解説していきます。
すべてを網羅することはしませんが、いくつかの重要なオプションについては知ることができるでしょう。

ここに記載されていないオプションで、知りたいものがあればIssue@<fn>{issue}にお寄せください。

//footnote[issue][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

#@# TODO --noImplicitAny
#@# TODO --moduleと--target
#@# TODO --noUnusedLocals
#@# TODO --noUnusedParameters
#@# TODO --listFiles

== --strictNullChecks

@<code>{--strictNullChecks}オプションについて説明します。
このオプションはnullやundefinedの扱いについてより厳格にし、変数の中身についての曖昧さを積極的に排除するよう振る舞います。
nullやundefinedを許容させたい場合、union typesや省略可能引数を使って明示的にnullやundefinedである可能性を示さなければなりません。

本書では、@<code>{--strictNullChecks}オプションは常に有効にしている前提で書いています。
有効にしている時の挙動は本書のサンプルすべてが該当しますので、この節ではこのオプションを使わないときの挙動について確認します。

まずは、本書標準のオプションありのときの例です（@<list>{strictNullChecks/basic}）。

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
