={index} Revised 型の国のTypeScript

=={about-this-book} 本書について

#@# prh:disable
本書はC87で頒布した『型の国のTypeScript』の改訂（C90）版です。
あなたが読んでいるこの文章は@<href>{http://techbooster.github.io/c90/#typescript,C90で頒布されたRevised TypeScript in Definitelyland}の全文です。

前回頒布した時はTypeScript 1.3.0が安定版で、ちらほら1.4.1の形が見えている…という時期だったと記憶しています。
そこから、本書の扱うTypeScript 2.0.0までの間にいくつもの魅力的な変更がありました。
本書を改訂するにあたり、さまざまな「こういう制限に注意するように」や「この方法は使うべきではない」といった文章を削りました。
こうして振り返ってみるとTypeScriptチームの大きな働き、コードの積み重ねに感謝するばかりです。

本書の内容は@<href>{http://typescript.ninja/typescript-in-definitelyland/,Webサイト}@<fn>{wonderland}にて全文を公開しています。
誤字や内容の誤り、深く掘り下げてほしい内容などがある場合、本書リポジトリまで@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues,Issue}@<fn>{issues}として報告していただけますと幸いです。

無料配布版では、表紙データなどは含まれないため可愛い表紙の冊子が欲しい場合はぜひ紙版をお買い上げください。

C87冊子版の表紙はイラスト：Jecyさん（@<href>{http://jecy.main.jp/}）、デザイン：shatiさん（@<href>{http://utata-ne.net/}）です。
この表紙のおかげで書名が決まりました。
可愛い表紙を本当にありがとうございます！

C90冊子版の表紙はイラスト：shatiさん（@<href>{http://utata-ne.net/}）、デザイン：siosioさんです。
C87版から上手に味わいを引き継いだ可愛い表紙をありがとうございます！

//footnote[wonderland][@<href>{http://typescript.ninja/typescript-in-definitelyland/}]
//footnote[issues][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

=={main-target} 対象読者

本書はECMAScript 2015について理解しているユーザを対象にしています。
@<kw>{OOP,Object Oriented Programming}についても効能や利点をある程度理解していることが望ましいです。
もし、これらの理解に不安がある場合、azuさんとlacoが執筆中のECMAScript 2015時代のためのJavaScript入門書『@<href>{https://github.com/asciidwango/js-primer,js-primer}』@<fn>{js-primer}などを参考にしてください。
今のところまだ書き終わっていないようですが2016年中くらいには書き終わるのではないでしょうか。

また解説するTypeScriptの内容は2.0.0（Beta）に準拠するため、現時点での最新の安定版である1.8.10では動作しない内容が多く含まれます。
本書に登場するすべてのサンプルコードは@<code>{npm install typescript@beta}でインストールできるTypeScript 2.0.0 Betaでコンパイルの確認をしています。

本書で利用しているTypeScriptコンパイラは次のものです。
#@mapoutput(../node_modules/.bin/tsc -v | sed -e "s/\n//")
Version 2.0.0
#@end

//footnote[js-primer][@<href>{https://github.com/asciidwango/js-primer}]

=={tour-of-this-book} 本書の内容

本書は@<code>{--noImplicitAny}、@<code>{--strictNullChecks}、@<code>{--noImplicitReturns}、@<code>{--noImplicitThis}を有効にした状態を基本として解説します。
各オプションの詳細については@<chapref>{tsc-options}を参照してください。

#@# prh:disable
またNode.js、ブラウザを問わずモジュールの仕組みを使います。
これはブラウザでの実行にはBrowserify@<fn>{browserify}なりwebpack@<fn>{webpack}なりのツールを使う前提があることを意味します。
targetの指定はInternet Explorer 11でもes5指定で十分なため@<code>{es5}以上とし、@<code>{es3}については本書では考慮しません。

TypeScriptはJSXのサポートを含みますが、筆者が今のところJSXないしReactに興味がないため、本書では扱いません。
興味がある方は@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html,公式のドキュメント}@<fn>{jsx}を参照してください。

#@# prh:disable
@<chapref>{prepared-to-typescript}では、TypeScriptコンパイラのセットアップ方法とVisual Studio Codeの設定について言及します。

#@# prh:disable
@<chapref>{typescript-basic}では、TypeScriptの基本構文を簡単に解説し、このあとの章を読み解くための基礎知識を蓄えます。

@<chapref>{types-basic}では、TypeScriptによる開発を行う上で理解しておきたい型についての知識を蓄えます。

@<chapref>{types-advanced}では、TypeScriptで利用可能な型のちょっと難しいところ、利用頻度は低いが知っておくと嬉しいことについて解説します。

@<chapref>{tsc-options}では、tscコマンドやtsconfig.jsonで利用可能なオプションについて、重要なオプションを中心に解説します。

@<chapref>{definition-file}では、既存のJS用資産を活かすための型定義ファイルについての解説と書き方、さらにDefinitelyTypedへのコントリビュートの仕方について解説します。

//footnote[browserify][@<href>{http://browserify.org/}]
#@# prh:disable
//footnote[webpack][@<href>{https://webpack.github.io/}]
//footnote[jsx][@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html}]

=={why-typescript} なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript（JavaScript）に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しています。

TypeScriptはECMAScriptのsuperset（上位互換）であることを標榜しています。
つまり、ECMAScript＋静的型付け＝TypeScriptです。
そして、"TypeScript独自の実装として表れる仕様"を注意深く避けてきています。

この方針ではECMAScriptのクソな仕様の数々は（勝手に闇に葬るわけにはいかないので）残ってしまうため、自力で避ける必要があります。
with構文やtry-catch構文などは、TypeScript的に使いにくい仕様になっているためそれと分かるでしょう。

ですが、その代わりにTypeScriptは将来"正式な"JavaScriptになる可能性があります@<fn>{jonathandturner-left}。
…あるんじゃないかな！

稀にTypeScriptのリポジトリに「TypeScriptにLINQを導入してほしい」などというECMAScript仕様にない独自の要望が上がってくることがありますが、上記のポリシーを考えればそのような要望が取り込まれないのは明らかです。
どうしてもTypeScriptに独自の仕様を入れたい場合、まずはECMAScript本体に入れる努力が必要でしょう。

//footnote[jonathandturner-left][過去にTypesの正式な仕様化について@<href>{https://github.com/rwaldron/tc39-notes/blob/master/es6/2014-09/sept-25.md#types,TC39ミーティングで話された}ことがあったがJonathan TurnerがMicrosoftを離れたため以後の進捗はよくない]

=={disclaimer} TypeScriptを選んだ時のデメリット

一番大きなデメリットは学習コストが追加で発生します。
JavaScriptの書き方に加え、TypeScriptで型注釈を与える記法を学ばねばなりません。
基本的な書き方はすぐに習得できると思いますが、既存のJavaScriptライブラリと組み合わせようとしたときに専門知識が必要になります。
本書ではそのための専門知識について解説し、TypeScriptを自由自在に使いこなすお手伝いをします。

#@# prh:disable
その他の懸念としてTypeScriptにロックインされてしまうのでは？という不安を耳にすることがあります。
これについては脱出口が用意されています。
TypeScriptできれいに書いたコードは@<code>{--target es6}で出力すると、単に型注釈を取り除いただけの素直なJavaScriptが出力されてきます。
簡単なコードを自分で書いてみて、それをECMAScript 2015なJSに変換して確認してみるとよいでしょう。
まずは、恐れずにTypeScriptに挑戦してみましょう。
