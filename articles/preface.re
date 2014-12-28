= 型の国のTypeScript

== 本書について

本書の内容は全文をインターネット上で公開する予定です。
予定地は @<href>{http://typescript.ninja/typescript-in-definitelyland/} こちら。
しかし、表紙データなどは含まれない予定のため可愛い表紙の冊子が欲しい場合はぜひお買い上げください。
C87冊子版の表紙はイラスト：Jecyさん（@<href>{http://jecy.main.jp/}）、デザイン：shatiさん（@<href>{http://utata-ne.net/}）です。
この表紙のおかげで書名が決まりました。
可愛い表紙を本当にありがとうございます！

本書は、ECMAScript 5レベルのJavaScriptの言語仕様と、JavaScriptによるOOPのいろはを理解している人を対象にしています。
また、解説するTypeScriptの内容には執筆時点（1.3.0）ではまだ未リリースの構文についての解説も含みます。

本書のすべてのサンプルコードは公式にリリースされている最新のTypeScriptコンパイラか、TypeScriptリポジトリの次に示したコミットハッシュの時点での@<kw>{LKG, Last Known Good}のコンパイラを使ってコンパイルの確認をしています。

現時点で最新のTypeScriptコンパイラは次のとおり。
#@mapoutput(../node_modules/.bin/tsc -v | sed -e "s/message TS[0-9]*: //")
Version 1.3.0.0
#@end

現時点でTypeScriptリポジトリのmaster/HEADは次のとおり。
#@mapoutput(git --git-dir=../typescript/.git --work-tree=../typescript rev-parse master)
5237ed7bed24695178a3386e4a9399e3614ed650
#@end

== 本書の内容

@<chapref>{prepared-to-typescript}では、TypeScriptコンパイラのセットアップ方法と、WebStormの設定について言及します。Visual StudioやNuGetの扱いについては言及しませんのでご了承ください。

@<chapref>{typescript-basic}では、TypeScriptの基本構文を簡単に解説し、このあとの章を読み解くための基礎知識を蓄えます。

@<chapref>{types-basic}では、TypeScriptによる開発を行う上で理解しておきたい型についての知識を蓄えます。

@<chapref>{types-advanced}では、TypeScriptで利用可能な型のちょっと難しいところ、利用頻度は低いが知っておくと嬉しいことについて解説します。

@<chapref>{definition-file}では、既存のJS用資産を活かすための型定義ファイルについての解説とその書き方と、ついでにDefinitelyTypedへのコントリビュートの仕方について解説します。

#@# TBD @<chapref>{es6}では、TypeScriptのOOPの基礎であるECMAScript 6の概要と、TypeScriptに取り込まれつつあるstring templatesなどの仕様について簡単に解説します。

#@# TBD @<chapref>{another-transpiler}では、TypeScript以外のTypeScript処理系について、現状わかっている範囲で解説を行います。

JavaScriptの（TypeScriptではなく）仕様まで踏み込んだ解説については、拙著TypeScriptリファレンス（@<href>{http://www.amazon.co.jp/gp/product/484433588X?tag=damenako-22,Amazon}@<fn>{ts-reference-amazon}、@<href>{http://tatsu-zine.com/books/typescript-reference,達人出版会}@<fn>{ts-reference-tatsujin}）を参照してください。

TypeScriptリファレンスはTypeScript 1.0.0対応の書籍です。
しかし、TypeScriptの基本的な部分は変わっていないため、今でも役に立ちます。

//footnote[ts-reference-amazon][@<href>{http://www.amazon.co.jp/gp/product/484433588X?tag=damenako-22}]
//footnote[ts-reference-tatsujin][@<href>{http://tatsu-zine.com/books/typescript-reference}]

== なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript（JavaScript）に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しているようです。

実際に、TypeScriptはECMAScriptのsuper set（上位互換）であることを標榜しています。
つまり、ECMAScript+静的型付け=TypeScriptです。
そして、"TypeScript独自の実装として表れる仕様"を注意深く避けてきています。

他のaltJS、たとえばCoffeeScriptやDartは、JavaScriptとは全く違う独自の記法で記述します。
これは記述はしやすいですが、変換結果のJavaScriptは見づらく、人間に優しい出力とはいえません。
つまりこれらは独自路線に行くことで、JavaScriptの辛さを軽減しようとしています。

一方、TypeScriptは先に述べたようにJavaScriptを踏襲します。
@<href>{http://mozaic.fm/post/96461640633/8-altjs,mozaic.fm出張版 #8}@<fn>{mozaic.fm}でもあんどうやすしさんと一緒に考えましたが、そこでの結論も「JSの完全な置換えを目指すDartは夢、TypeScriptは現実」というものでした。

この方針ではECMAScriptのクソな仕様の数々は（勝手に闇に葬るわけにはいかないので）残ってしまいます。
ですが、その代わりにTypeScriptは将来"正式な"JavaScriptになる可能性があります。
稀に、TypeScriptのリポジトリに「TypeScriptにLINQを導入してほしい」という要望が上がってくることがありますが、上記の事情を鑑みればそのような要望が取り込まれないのは明らかです。
LINQや拡張メソッドは確かに便利ですし、作者がC#と同じアンダース・ヘルスバーグ氏なのでそういう期待が湧くのは理解できます。
しかし、将来のECMAScriptに入るとは現時点では考えにくいでしょう。

さて、「TypeScriptが将来のJavaScriptそのものになる？妄想乙」と思われた読者諸兄もおられるでしょう。
あながちそれが誇大妄想ではないのではないか？という証跡が最近出てきています。

GoogleからAngularJS 2.0のために開発する新言語AtScriptと、Facebookからも新言語Flowが公表されました。
これら2つの言語は、両方ともTypeScriptの上位互換または互換性を考慮して設計されるそうです。
MicrosoftとGoogle, Facebookの3巨人がTypeScriptに賭けると言っているのです。
将来的にTypeScriptが時代の本流になる、という考えを妄想と切って捨てることはできないでしょう@<fn>{advent-calendar}。

というわけで、Dartが完全にJSを置き換える可能性はありますが、今のところ私はTypeScriptに賭けていこうと思います。

//footnote[mozaic.fm][@<href>{http://mozaic.fm/post/96461640633/8-altjs}]
//footnote[advent-calendar][詳しくは@<href>{http://qiita.com/vvakame/items/bf4d1e339d5815026fbb}にまとめました]
