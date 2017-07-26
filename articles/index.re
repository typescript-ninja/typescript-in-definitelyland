={index} Revised^2 型の国のTypeScript
#@# TODO Revised^2 にしたい(上付き) http://www.r6rs.org/ と同じ感じ

#@# TODO 気が向いたらコラム化する
#@# * トランスフォームベースのJSコード生成の話 in 2.1.1

#@# TODO ESnext系の追加構文については言及しなくていいかな… 対応された機能一覧とかあってもいいかも kangaxが更新されてないんだよな…
#@# * Object Rest/Spread Properties for ECMAScriptがstage 3になったので入ったっぽい ESNext object property spread and rest in 2.1.4
#@# * new.targetがサポートされた Support for new.target in 2.2.1
#@# * クラスの継承にObject.setPrototypeOfを使うようになった Update __extends to use Object.setPrototypeOf in 2.2.1
#@# * 非同期イテレータのサポート Asynchronous iterators in 2.3RC

#@# TODO JSX関係
#@# * 根本的に言及してない Reactはやる気ないけどSkateJSは言及したい気もする
#@# * React.createElement 以外のJSXファクトリが利用可能に New --jsxFactory in 2.1.4
#@# * JSXで子要素のspread記法がサポートされたSupport for JSX spread children in 2.2.1
#@# * JSX記法のターゲットにReact Nativeが追加されたNew --jsx react-native in 2.2.1
#@# * JSXのstateless componentsとなる関数がoverloadできなかったのが直された JSX stateless components overload resolution in 2.3RC

#@# NOTE 重箱の隅的仕様だし知らなくても別にさほど労力が減らない系の話題
#@# * 古の… 文脈依存型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#15-contextual-typing このへん？
#@# * 古の… 再帰型 https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#3.11.8 このへん？
#@# * 古の… オーバーロードの選択アルゴリズム https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#4.15.1 このへん？
#@# * 暗黙的なインデックスシグニチャ（Implicit index signatures） in 2.0Beta
#@# * リテラル型とより良い型推論 Better inference for literal types in 2.1.1
#@# * 型注釈無しの変数に対する型導出のサポート Control flow analysis for implicit any variables in 2.1.1
#@# * 型注釈無しの配列に対する型導出のサポート Control flow analysis for array construction in 2.1.1
#@# * 条件式によるnumber or stringからリテラル型への変換 Narrow string and number types in literal equality checks in 2.1.1
#@# * 部分的型注釈からの型推論 Contextual typing of partially annotated signatures in 2.1.1
#@# * nullableな値に対する演算子の適用でよいよいチェックを提供Improved checking of nullable operands in expressions in 2.2.1
#@# * stringインデックスシグニチャに対する . アクセスの挙動を改善Allow property (dotted) access for types with string index signatures in 2.2.1
#@# * 関数の返り値の型をGenericsの型パラメータの推測に利用する Infer from generic function return types in 2.4.1
#@# * Generics有りの関数の型パラメータが推論されるようになった in 2.4.1
#@# * Genericsについて共変性のチェックが行われるようになった Covariant checking for callback parameters in 2.4.1

=={about-this-book} 本書について

本書はJavaScriptにコンパイル可能な言語のTypeScriptについて解説した本です。
TypeScriptの基本的な知識（型の使い方）やコンパイラのオプション、型定義ファイルの作り方やエコシステムについて解説します。

対象読者は新しめのJavaScript、いわゆるECMAScript 2015とそれ以降@<fn>{js-primer}について理解しているユーザです。
Node.js環境やnpmの使い方、@<kw>{OOP,Object Oriented Programming}の効能や利点をある程度理解していることが望ましいです。

//footnote[js-primer][@azu_reさんと@laco0416くんが書いている入門書（書きかけ）があります @<href>{https://github.com/asciidwango/js-primer}]

#@# 128Pくらいでモダンブラウザでの動作に限定した基本的な文法の紹介と主要なツール類の解説を行ったJS本なんか出ないですかね…。

本書に登場するすべてのサンプルコードは次のTypeScriptコンパイラでコンパイルできることを確認しています。
#@mapoutput(../node_modules/.bin/tsc -v | sed -e "s/\n//")
Version 2.4.2
#@end

#@# prh:disable
本書はC87、C90で頒布した『型の国のTypeScript』の改訂（C92）版です。
あなたが読んでいるこの文章は@<href>{http://techbooster.github.io/c92/#typescript,C92で頒布されたRevised TypeScript in Definitelyland}の全文です。

前回頒布した時のTypeScriptは2.0.0でした。
TypeScriptは進歩を続け、ますます堅牢で安全で、そして最新のECMAScript仕様が使えるようになっています。
「よりよいJavaScriptコード」を書くことで型の絞込が行われ、@<code>{null}や@<code>{undefined}のチェックができます。
つまり、型に関する記法を覚え、JavaScriptとしてよいコードを書き、コンパイルできれば実行時エラーが発生する心配をほぼしなくてもよい環境が手に入ります。

今回の改訂では方針を変更し、TypeScriptについての網羅的な解説から知識の効率的な摂取にシフトすることにしました。
というのも、TypeScript本体に重箱の隅をフォローする仕様が増え、網羅的に仕様を把握・解説することが困難になり、読むのも苦痛だろうからです。
ECMAScript 2015以降の知識についての詳細は、本書では扱いません。
主に型周りの説明、TypeScriptコンパイラ自体の説明、周辺仕様を解説します。

本書の内容は@<href>{http://typescript.ninja/typescript-in-definitelyland/,Webサイト}@<fn>{wonderland}にて全文を公開しています。
誤字や内容の誤り、自分としてはコンパイルが通ると思うのに通らないパターン、深く掘り下げてほしい内容などがある場合、本書リポジトリまで@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues,Issue}@<fn>{issues}として報告していただけますと幸いです。

//footnote[wonderland][@<href>{http://typescript.ninja/typescript-in-definitelyland/}]
//footnote[issues][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

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

=={tour-of-this-book} 本書の内容

本書は@<code>{--strict}を有効にした状態を基本として解説します。
オプションの詳細については@<chapref>{tsc-options}を参照してください。

#@# prh:disable
またNode.js、ブラウザを問わずモジュールの仕組みを使います。
これはブラウザでの実行にはwebpack@<fn>{webpack}などのbundlingツールを使う前提であることを意味します。
targetの指定はInternet Explorer 11でもes5指定で十分なため@<code>{es5}以上とし、@<code>{es3}については本書では考慮しません。
また、必要があれば@<code>{--downlevelIteration}も利用します。

#@# prh:disable
//footnote[webpack][@<href>{https://webpack.github.io/}]

TypeScriptはJSXのサポートを含みますが、筆者が今のところJSXないしReactに興味がないため、本書では扱いません。
興味がある方は@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html,公式のドキュメント}@<fn>{jsx}を参照してください。
#@# TODO SkateJSとかに言及する？
//footnote[jsx][@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html}]

#@# prh:disable
@<chapref>{prepared-to-typescript}では、TypeScriptコンパイラのセットアップ方法とVisual Studio Codeの設定について言及します。

#@# prh:disable
@<chapref>{typescript-basic}では基本構文を解説し、このあとの章を読み解くための基礎知識を蓄えます。

@<chapref>{types-basic}では開発する上で理解しておきたい型についての知識を蓄えます。

@<chapref>{types-advanced}では、TypeScriptで利用可能な型のちょっと難しいところ、利用頻度は低いが知っておくと嬉しいことについて解説します。

@<chapref>{tsc-options}では、tscコマンドやtsconfig.jsonで利用できるオプションについて、重要なオプションを中心に解説します。

@<chapref>{at-types}では、@typesで用意されている型定義ファイルの使い方について解説します。

@<chapref>{definition-file}では、自分で型定義ファイルを書くための方法、さらにDefinitelyTypedへのコントリビュートの仕方について解説します。

@<chapref>{typescript-as-a-tool}では、TypeScriptのLanguage Service APIやLanguage Server Protocolの概要を紹介します。

=={why-typescript} なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript（≒JavaScript）に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しています。

TypeScriptはECMAScriptの@<kw>{superset,上位互換}であることを標榜しています。
つまり、ECMAScript＋静的型付け＝TypeScriptです。
そして、"TypeScript独自の実装として表れる仕様"を注意深く避けようとしています。

TypeScriptのリポジトリには稀に「TypeScriptにLINQを導入してほしい」などのECMAScript仕様にない独自の要望が上がってきます。
しかしながら、上記のポリシーを考えればそのような要望が取り込まれないのは明らかです。
どうしてもTypeScriptに独自の仕様を入れたい場合、まずはECMAScript本体にその仕様を入れる努力が必要でしょう。

現代的で、よいJavaScriptコードを書くことは、よいTypeScriptコードを書くことに繋がります。
そしてTypeScriptで得た知識やベストプラクティスは、JavaScriptを書く上でも正しい習慣を得るための補助輪として機能します。
どちらか片方だけでは成り立たず、両輪を理解し活用することが上達への近道です。

さらにTypeScriptを勧めたい点として、TypeScriptコンパイラーが内蔵するエディタ支援のための機能群です。
コンパイラに密に結合された（つまり更新遅延のない）エディタサポートは、強く信頼できる型推論や入力補完を提供します。
高機能で信頼できる、曖昧さが排除されたエディタを使いたい場合、TypeScriptはうってつけの選択肢です。

これらTypeScriptの利点が支持された結果として、いくつかのフレームワークや多くの人々がTypeScriptを使い始めています。
たとえばSlack@<fn>{slack-ts}、Reddit@<fn>{reddit-ts}、Google@<fn>{google-ts}、そしてTypeScript開発元であるMicrosoftなどのビッグサービスです。
乗るしかない！このビッグウェーブに！

//footnote[slack-ts][@<href>{https://slack.engineering/typescript-at-slack-a81307fa288d}]
//footnote[reddit-ts][@<href>{https://redditblog.com/2017/06/30/why-we-chose-typescript/}]
//footnote[google-ts][@<href>{http://www.publickey1.jp/blog/17/googletypescriptng-conf_2017.html}]

=={disclaimer} TypeScriptを選んだ時のデメリット

一番大きなデメリットは、JavaScriptにプラスした学習コストが発生します。
JavaScriptの書き方に加え、TypeScriptで型注釈を与える記法を学ばねばなりません。

基本的な書き方はすぐに習得できますが、既存のJavaScriptライブラリと組み合わせようとしたときにTypeScriptに対する応用的な理解が必要になります。
本書ではそのための専門知識について解説し、TypeScriptを自由自在に使いこなす手伝いをしていきます。

#@# prh:disable
懸念として「TypeScriptにロックインされてしまうのでは？」という不安を耳にすることがあります。
これについては脱出口が用意されています。
TypeScriptできれいに書いたコードは@<code>{--target esnext}で出力すると、単に型注釈を取り除いただけの素直なJavaScriptが出力されます。
簡単なコードを自分で書いてみて、それを変換して確認してみるとよいでしょう。
まずは、恐れずにTypeScriptに取り組んでみましょう。
