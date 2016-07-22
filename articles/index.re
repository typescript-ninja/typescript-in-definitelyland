={index} Revised 型の国のTypeScript

== 本書について

#@# TODO 書き直す
#@# @suppress SentenceLength ParenthesizedSentence
#@# prh:disable
本書はC87で頒布された型の国のTypeScriptの改訂版（C90版）です。
あなたが読んでいるこの文章は@<href>{http://techbooster.github.io/c87/#typescript,C87で頒布されたTypeScript in Definitelyland}の全文です。
内容は（筆者のやる気次第で）随時更新されていく可能性があります。
誤字や内容の誤り、深く掘り下げてほしい内容などがある場合、@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues,Issue}@<fn>{issues}として報告していただけますと幸いです。

Webサイトとして閲覧したい場合は@<href>{http://typescript.ninja/typescript-in-definitelyland/}を参照してください。
PDF, epubとして入手したい場合は@<href>{https://tcb.mowa-net.jp/griflet/github/typescript-ninja/typescript-in-definitelyland/}を参照してください。

無料配布版では、表紙データなどは含まれないため可愛い表紙の冊子が欲しい場合はぜひ紙版をお買い上げください。
C87冊子版の表紙はイラスト：Jecyさん（@<href>{http://jecy.main.jp/}）、デザイン：shatiさん（@<href>{http://utata-ne.net/}）です。
この表紙のおかげで書名が決まりました。
可愛い表紙を本当にありがとうございます！

//footnote[issues][@<href>{https://github.com/typescript-ninja/typescript-in-definitelyland/issues}]

== 対象読者

本書はECMAScript 2015について理解しているユーザを対象にしています。
また、@<kw>{OOP,Object Oriented Programming}についても効能や利点をある程度理解していることが望ましいです。
もし、これらの理解に不安がある場合、azuさんとlacoが執筆中のECMAScript 2015時代のためのJavaScript入門書、@<href>{js-primer,https://github.com/asciidwango/js-primer}@<fn>{js-primer}などを参考にしてください。
今のところまだ書き終わっていないようですが2016年中くらいには書き終わるのではないでしょうか。

また、解説するTypeScriptの内容は2.0.0（Beta）に準拠するため、現時点での最新の安定版である1.8.10では動作しない内容が多く含まれます。
本書に登場するすべてのサンプルコードは@<code>{npm install typescript@beta}でインストールできるTypeScript 2.0.0 Betaでコンパイルの確認をしています。

本書で利用しているTypeScriptコンパイラは次のものです。
#@mapoutput(../node_modules/.bin/tsc -v | sed -e "s/\n//")
Version 2.0.0
#@end

//footnote[js-primer][@<href>{https://github.com/asciidwango/js-primer}]

== 本書の内容

本書は@<code>{--noImplicitAny}, @<code>{--strictNullChecks}, @<code>{--noImplicitReturns}, @<code>{--noImplicitThis}を有効にした状態を基本として解説します。
また、Node.js、ブラウザを問わずECMAScript 2015のモジュールの仕組みか、TypeScript独自のCommonJS向けの文法を使います。
これはブラウザでの実行にはBrowserifyなりWebPackなりの仕組みを使うことを前提とすることになります。
また、targetの指定はInternet Explorer 11でもes5指定で十分なため@<code>{es5}以上とし、@<code>{es3}については本書では考慮しません。

また、TypeScriptはJSXのサポートを含みますが、筆者が今のところJSXないしReactに興味がないため、本書では扱いません。
興味がある方は@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html,公式のドキュメント}@<fn>{jsx}を参照してください。

#@# prh:disable
@<chapref>{prepared-to-typescript}では、TypeScriptコンパイラのセットアップ方法と、Visual Studio Codeの設定について言及します。

#@# prh:disable
@<chapref>{typescript-basic}では、TypeScriptの基本構文を簡単に解説し、このあとの章を読み解くための基礎知識を蓄えます。

@<chapref>{types-basic}では、TypeScriptによる開発を行う上で理解しておきたい型についての知識を蓄えます。

@<chapref>{types-advanced}では、TypeScriptで利用可能な型のちょっと難しいところ、利用頻度は低いが知っておくと嬉しいことについて解説します。

@<chapref>{tsc-options}では、tscコマンドやtsconfig.jsonで利用可能なオプションについて、重要なオプションを中心に解説します。

@<chapref>{definition-file}では、既存のJS用資産を活かすための型定義ファイルについての解説とその書き方と、ついでにDefinitelyTypedへのコントリビュートの仕方について解説します。

//footnote[jsx][@<href>{http://www.typescriptlang.org/docs/handbook/jsx.html}]

== なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript（JavaScript）に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しているようです。

TypeScriptはECMAScriptのsuperset（上位互換）であることを標榜しています。
つまり、ECMAScript+静的型付け=TypeScriptです。
そして、"TypeScript独自の実装として表れる仕様"を注意深く避けてきています。

この方針ではECMAScriptのクソな仕様の数々は（勝手に闇に葬るわけにはいかないので）残ってしまうため、自力で避ける必要があります。
with構文やtry-catch構文などは、TypeScript的に使いにくい仕様になっているためそれと分かるでしょう。

ですが、その代わりにTypeScriptは将来"正式な"JavaScriptになる可能性があります。
…あるんじゃないかな@<fn>{jonathandturner-left}！

稀に、TypeScriptのリポジトリに「TypeScriptにLINQを導入してほしい」などというECMAScript仕様にない独自の要望が上がってくることがありますが、上記のポリシーを考えればそのような要望が取り込まれないのは明らかです。
どうしてもTypeScriptに独自の仕様を入れたい場合、まずはECMAScript本体に入れるよう努力する必要があるでしょう。

//footnote[jonathandturner-left][過去にTypesの正式な仕様化について@<href>{https://github.com/rwaldron/tc39-notes/blob/master/es6/2014-09/sept-25.md#types,TC39ミーティングで話された}ことがあったがJonathand TurnerがMicrosoftを離れたため以後の進捗はよくない。]

== TypeScriptを選んだ時のデメリット

一番大きなデメリットは学習コストが追加で必要になることでしょう。
JavaScriptの書き方に加え、TypeScriptで型注釈を与える記法を学ばねばなりません。
基本的な書き方はすぐに習得できると思いますが、既存のJavaScriptライブラリと組み合わせようとしたときに専用の知識が必要になります。
本書ではそのための専用の知識について解説し、TypeScriptを自由自在に使いこなすためのお手伝いをします。

その他の懸念としてTypeScriptにロックインされてしまうのでは？という不安を耳にすることがあります。
これについては脱出口が用意されています。
TypeScriptできれいに書いたコードは@<code>{--target es6}で出力すると、単に型注釈を取り除いただけの素直なJavaScriptが出力されてきます。
まずは簡単なコードを自分で書いてみて、それをECMAScript 2015なJSに変換して確認してみるとよいでしょう。
まずは、恐れずにTypeScriptに挑戦してみましょう。
