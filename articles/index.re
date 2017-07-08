={index} Revised^2 型の国のTypeScript
#@# TODO Revised^2 にしたい(上付き) http://www.r6rs.org/ と同じ感じ

#@# TODO トランスフォームベースのJSコード生成の話 in 2.1.1
#@# TODO async/awaitのdownpile in 2.1.1
#@# TODO ヘルパライブラリの話 in 2.1.1

#@# TODO 型推論とベストプラクティス的な話を書く 細部を取り上げてこの時はこう推論される、ということを考えるのは難しいので新しい指針が必要
#@# TODO   自然なJavaScriptを書くことでよりよい安全性を提供する という精神
#@# TODO   Better inference for literal types
#@# TODO   Control flow analysis for implicit any variables
#@# TODO   Control flow analysis for array construction
#@# TODO   Narrow string and number types in literal equality checks

#@# TODO 部分的型注釈からの型推論 Contextual typing of partially annotated signatures in 2.1.1
#@# TODO 直和型と交差型を組み合わせのノーマライズ (Normalize union/intersection type combinations in 2.1.1

#@# TODO Language Service や Language Server Protocol についての言及
#@# TODO "実装箇所へジャンプする" をLanguageServiceに追加 (Go to implementation support) in 2.1.1
#@# TODO reference commentとかimport文での入力補完をLanguageServiceに追加 (Completions in imports and triple-slash reference paths) in 2.1.1
#@# TODO Quick fixes をLanguageServiceに追加 (Quick fixes support in language service API) in 2.1.1

#@# TODO Object Rest/Spread Properties for ECMAScriptがstage 3になったので入ったっぽい ESNext object property spread and rest in 2.1.4
#@# TODO superを呼び出しした時コンストラクタでreturnした値がthisとなるように変更 Use returned values from super calls as 'this' in 2.1.4
#@# TODO React.createElement 以外のJSXファクトリが利用可能に New --jsxFactory in 2.1.4
#@# TODO 型付けなしの気軽なimport句 Untyped (implicit-any) imports in 2.1.4
#@# TODO クラスのMixinパターンのサポートMixin classes in 2.2.1
#@# TODO インタフェースが object-like typeならなんでも拡張できるようになった Allow deriving from object and intersection types in 2.2.1
#@# TODO new.targetがサポートされた Support for new.target in 2.2.1
#@# TODO nullableな値に対する演算子の適用でよいよいチェックを提供Improved checking of nullable operands in expressions in 2.2.1
#@# TODO クラスの継承にObject.setPrototypeOfを使うようになった Update __extends to use Object.setPrototypeOf in 2.2.1
#@# TODO stringインデックスシグニチャに対する . アクセスの挙動を改善Allow property (dotted) access for types with string index signatures in 2.2.1
#@# TODO JSXで子要素のspread記法がサポートされたSupport for JSX spread children in 2.2.1
#@# TODO JSX記法のターゲットにReact Nativeが追加されたNew --jsx react-native in 2.2.1
#@# TODO Language Service経由で使えるQuick fixesの種類が増えた。More Quick fixes! in 2.2.1
#@# TODO --target es3 と --target es5 でもジェネレータが使えるようになった Generator support for ES3/ES5 in 2.3RC
#@# TODO 非同期イテレータのサポート Asynchronous iterators in 2.3RC
#@# TODO Genericsの型パラメータにデフォルトの型を設定できるようになった Generic defaults in 2.3RC
#@# TODO thisの型のコントロールがより柔軟に行われるようになった Controlling this in methods of object literals through contextual type in 2.3RC
#@# TODO JSXのstateless componentsとなる関数がoverloadできなかったのが直された JSX stateless components overload resolution in 2.3RC
#@# TODO --checkJs オプションでJSコードについて型の整合性やらをチェックするようになったらしい Report errors in .js files with new --checkJs in 2.3RC

#@# TODO 関数の返り値の型をGenericsの型パラメータの推測に利用する Infer from generic function return types in 2.4.1
#@# TODO Generics有りの関数の型パラメータが推論されるようになった  in 2.4.1
#@# TODO Genericsの型の互換性チェックがより厳密になった Stricter generic signature checks in 2.4.1
#@# TODO Genericsについて共変性のチェックが行われるようになった Covariant checking for callback parameters in 2.4.1
#@# TODO enumの値にstringが使えるようになった String valued members in enums in 2.4.1
#@# TODO プロパティが全てoptionalな型について特別なサポートを与えるようにした Weak type detection in 2.4.1
#@# TODO dynamic importがstage 3になったので入った in 2.4.1
#@# TODO リファクタリングの機能をLanguage Service APIに追加 in 2.4.1
#@# TODO tsconfig.jsonについてのエラー表示がより良くなった in 2.4.1

#@# TODO DefinitelyTyped周りの変化について解説する
#@# TODO TypeScriptのリリースサイクルについて解説する

#@# TODO SlackとかRedditとかGoogleとかがTypeScript採用した話を書く

=={about-this-book} 本書について

本書はJavaScriptにコンパイル可能な言語のTypeScriptについて解説した本です。
TypeScriptの基本的な知識（型の使い方）やコンパイラのオプション、型定義ファイルの作り方やエコシステムについて解説します。

対象読者は新しめのJavaScript、いわゆるECMAScript 2015とそれ以降@<fn>{js-primer}について理解しているユーザです。
Node.js環境やnpmの使い方、@<kw>{OOP,Object Oriented Programming}についての効能や利点をある程度理解していることが望ましいです。

#@# 128Pくらいでモダンブラウザでの動作に限定した基本的な文法の紹介と主要なツール類の解説を行ったJS本なんか出ないですかね…。

本書に登場するすべてのサンプルコードは次のTypeScriptコンパイラでコンパイルできることを確認しています。
#@mapoutput(../node_modules/.bin/tsc -v | sed -e "s/\n//")
Version 2.4.1
#@end

#@# prh:disable
本書はC87、C90で頒布した『型の国のTypeScript』の改訂（C92）版です。
あなたが読んでいるこの文章は@<href>{http://techbooster.github.io/c92/#typescript,C92で頒布されたRevised TypeScript in Definitelyland}の全文です。

前回頒布した時のTypeScriptは2.0.0でした。
TypeScriptは進歩を続け、ますます堅牢で安全で、そして最新のECMAScript仕様が使えるようになっています。
「よりよいJavaScriptコード」を書くことで、型の絞込が行われたりnullやundefinedのチェックができるようになっていっています。
つまり、型周りの記法を覚え、JSとしてよいコードを書き、コンパイルが通れば実行時エラーが発生する心配をほぼしなくてもよい環境が手に入るのです。

さて、今回の改訂では本の方針を変更し、TypeScriptについての網羅的な解説からTypeScriptを使う上で知っているべき知識の効率的な摂取にシフトすることにしました。
というのも、TypeScript本体に重箱の隅をフォローする仕様が増え、網羅的に仕様を把握・解説するのが困難になり、読むのも苦痛だろうからです。
ECMAScript 2015以降の知識についての詳細は、本書では扱いません。
主に型周りの説明、TypeScriptコンパイラ自体の説明、周辺仕様の解説を行います。

本書の内容は@<href>{http://typescript.ninja/typescript-in-definitelyland/,Webサイト}@<fn>{wonderland}にて全文を公開しています。
誤字や内容の誤り、自分としてはコンパイルが通ると思うのに通らないパターン、深く掘り下げてほしい内容などがある場合、本書リポジトリまで@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues,Issue}@<fn>{issues}として報告していただけますと幸いです。

無料配布版では、表紙データなどは含まれないため可愛い表紙の冊子が欲しい場合はぜひ紙版をお買い上げください。

C87冊子版の表紙はイラスト：Jecyさん（@<href>{http://jecy.main.jp/}）、デザイン：shatiさん（@<href>{http://utata-ne.net/}）です。
この表紙のおかげで書名が決まりました。
可愛い表紙を本当にありがとうございます！

C90冊子版の表紙はイラスト：shatiさん（@<href>{http://utata-ne.net/}）、デザイン：siosioさんです。
C87版から上手に味わいを引き継いだ可愛い表紙をありがとうございます！

#@# TODO C92版の表紙について言及する

#@# prh:disable
なお、booth.pmやCOMIC ZINにて通販を行っております。
@<href>{http://shop.comiczin.jp/products/detail.php?product_id=29874,紙版の通販}、@<href>{https://techbooster.booth.pm/items/299594?utm_source=webbook&utm_medium=webbook&utm_campaign=ebook,電子版販売}があります。

//footnote[js-primer][@azu_reさんと@laco0416くんが書いている入門書（書きかけ）があります @<href>{https://github.com/asciidwango/js-primer}]
//footnote[wonderland][@<href>{http://typescript.ninja/typescript-in-definitelyland/}]
//footnote[issues][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

=={tour-of-this-book} 本書の内容

本書は@<code>{--strict}を有効にした状態を基本として解説します。
オプションの詳細については@<chapref>{tsc-options}を参照してください。

#@# prh:disable
またNode.js、ブラウザを問わずモジュールの仕組みを使います。
これはブラウザでの実行にはwebpack@<fn>{webpack}などのbundlingツールを使う前提であることを意味します。
targetの指定はInternet Explorer 11でもes5指定で十分なため@<code>{es5}以上とし、@<code>{es3}については本書では考慮しません。
また、必要があれば@<code>{--downlevelIteration}も利用します。

TypeScriptはJSXのサポートを含みますが、筆者が今のところJSXないしReactに興味がないため、本書では扱いません。
興味がある方は@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html,公式のドキュメント}@<fn>{jsx}を参照してください。
#@# TODO SkateJSとかに言及する？

#@# prh:disable
@<chapref>{prepared-to-typescript}では、TypeScriptコンパイラのセットアップ方法とVisual Studio Codeの設定について言及します。

#@# prh:disable
@<chapref>{typescript-basic}では、TypeScriptの基本構文を簡単に解説し、このあとの章を読み解くための基礎知識を蓄えます。

@<chapref>{types-basic}では、TypeScriptによる開発を行う上で理解しておきたい型についての知識を蓄えます。

@<chapref>{types-advanced}では、TypeScriptで利用可能な型のちょっと難しいところ、利用頻度は低いが知っておくと嬉しいことについて解説します。

@<chapref>{tsc-options}では、tscコマンドやtsconfig.jsonで利用可能なオプションについて、重要なオプションを中心に解説します。

@<chapref>{definition-file}では、既存のJS用資産を活かすための型定義ファイルについての解説と書き方、さらにDefinitelyTypedへのコントリビュートの仕方について解説します。

#@# prh:disable
//footnote[webpack][@<href>{https://webpack.github.io/}]
//footnote[jsx][@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html}]

=={why-typescript} なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript（JavaScript）に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しています。

TypeScriptはECMAScriptのsuperset（上位互換）であることを標榜しています。
つまり、ECMAScript＋静的型付け＝TypeScriptです。
そして、"TypeScript独自の実装として表れる仕様"を注意深く避けようとしています。

稀にTypeScriptのリポジトリに「TypeScriptにLINQを導入してほしい」などのECMAScript仕様にない独自の要望が上がってくることがあります。
しかしながら、上記のポリシーを考えればそのような要望が取り込まれないのは明らかです。
どうしてもTypeScriptに独自の仕様を入れたい場合、まずはECMAScript本体にその仕様を入れる努力が必要でしょう。

現代的で、よいJavaScriptコードを書くことは、よいTypeScriptコードを書くことに繋がります。
そしてTypeScriptで得た知識やベストプラクティスは、JavaScriptを書く上でも正しい習慣を得るための補助輪として機能します。
どちらか片方だけでは成り立たず、両輪を理解し活用することが上達への近道です。

さらにTypeScriptを勧めたい点として、TypeScriptコンパイラーが内蔵するエディタ支援のための機能群です。
コンパイラに密に結合された（つまり更新遅延のない）エディタサポートは、強く信頼できる、まさにコンパイラと同等レベルの型推論や入力補完を提供してくれます。
高機能で信頼できる、曖昧さが排除されたエディタを使いたい場合、TypeScriptはうってつけの選択肢といえます。

=={disclaimer} TypeScriptを選んだ時のデメリット

一番大きなデメリットは、JavaScriptにプラスした学習コストが発生します。
JavaScriptの書き方に加え、TypeScriptで型注釈を与える記法を学ばねばなりません。
基本的な書き方はすぐに習得できると思いますが、既存のJavaScriptライブラリと組み合わせようとしたときに少し高度なTypeScriptに対する理解が必要になります。
本書ではそのための専門知識について解説し、TypeScriptを自由自在に使いこなすお手伝いをします。

#@# prh:disable
その他の懸念として、「TypeScriptにロックインされてしまうのでは？」という不安を耳にすることがあります。
これについては脱出口が用意されています。
TypeScriptできれいに書いたコードは@<code>{--target esnext}で出力すると、単に型注釈を取り除いただけの素直なJavaScriptが出力されてきます。
簡単なコードを自分で書いてみて、それをJSに変換して確認してみるとよいでしょう。
まずは、恐れずにTypeScriptに取り組んでみましょう。
