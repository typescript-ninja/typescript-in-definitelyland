= 公正なる『型付け』はプログラム自身を人間的に生長させてくれる

== 本書について

本書は、ECMAScript 3レベルのJavaScriptの言語仕様と、JavaScriptによるOOPのいろはを理解している人を対象にしています。
また、解説するTypeScriptの内容には執筆時点(1.1.0-1)ではまだ導入されていないもろもろの構文についての解説も含みます。

内容は以下の通りです。

 1. @<chapref>{about}
 2. @<chapref>{with-types}
 3. @<chapref>{definition-file}
 4. @<chapref>{es6}
 5. @<chapref>{another-transpiler}

@<chap>{about}では、コマンドライン上でのTypeScriptコンパイラの使い方と、TypeScriptの基本構文を簡単に解説し、この後の章を読み解くための基礎知識を蓄えます。
@<chapref>{with-types}では、TypeScriptの型の使われ方について解説し、コンパイルを通し、実際に開発を行うための基礎知識を蓄えます。1.1.0-1時点では導入されていないtuple types, union types, type aliasについての解説を含みます。
@<chapref>{definition-file}では、既存のJS用資産を活かすための型定義ファイルについての解説と、その書き方、ついでにDefinitelyTypedへのコントリビュートの仕方について解説します。
@<chapref>{es6}では、TypeScriptのOOPの基礎であるECMAScript 6の概要と、TypeScriptに取り込まれつつあるstring templatesなどの仕様について簡単に解説します。
@<chapref>{another-transpiler}では、TypeScript以外のTypeScript処理系について、現状わかっている範囲で解説を行います。

なお、本解説ではMac OS X + WebStorm での利用を前提に解説します。
Visual StudioやNuGetの扱いについては言及しませんのでご了承ください。
#@# NOTE 誰かが書いてpull requestしてくれてもいいけどその部分の文責は負わない＞＜

== なぜTypeScriptを選ぶべきなのか

TypeScriptはMicrosoftが主導となって開発している言語で、ECMAScript(JavaScript)に静的な型付けによる検証を導入したものです。
現実を見据えた言語仕様で、"未来のJavaScriptそのもの"になることを目指しているようです。

実際に、TypeScriptはECMAScriptのsuper set(上位互換)であることを標榜しています。
つまり、ECMAScript+静的型付け=TypeScriptです。
そして、"TypeScript独自の仕様"というものを避けて通っています。

他のaltJS、例えばCoffeeScriptやDartは、JavaScriptとは全く違う独自の記法で記述し、変換結果のJavaScriptも見づらく、人間に優しいとは言えません。
これらは独自路線に行くことで、JavaScriptの辛さを軽減しようとしています。
一方、TypeScriptは先に述べたようにJavaScriptを踏襲します。
@<href>{http://mozaic.fm/post/96461640633/8-altjs,mozaic.fm出張版 #8}でもあんどうやすしさんと一緒に考えましたが、そこでの結論も「JSの完全な置換えを目指すDartは夢、TypeScriptは現実」というものでした。

この方針ではECMAScriptのクソな仕様の数々は(勝手に闇に葬れないので)残ってしまいます。
ですが、その代わりにTypeScriptは将来"正式な"JavaScriptになる可能性があります。
稀に、TypeScriptのリポジトリに「TypeScriptにLINQを導入してほしい」という要望が上がってくることがありますが、上記の事情を鑑みればそのような要望が取り込まれないのは明らかです。
LINQや拡張メソッドは確かに便利ですし、作者がC#と同じアンダース・ヘルスバーグ氏なのでそういう期待が湧くのは、理解できますが、将来のECMAScriptに入ることは現時点では考えにくいでしょう。

さて、「TypeScriptが将来のJavaScriptそのものになる？妄想乙」と思われた読者諸兄もおられるでしょう。
あながちそれが誇大妄想ではないのではないか？という証跡が最近出てきましたのでここで紹介しておきます。

GoogleがAngularJS 2.0のために開発する新言語AtScriptと、Facebookからも新言語Flowの存在が公表されました。
これら2つの言語は、両方ともTypeScriptのsuper set(上位互換)として設計されるそうです。
MicrosoftとGoogle, Facebookの3巨人がTypeScriptに賭けると言っているのです。
将来的にTypeScriptが時代の本流になる、という考えを妄想と切って捨てることはできないでしょう。

#@# TODO ソース探してきて貼る

というわけで、Dartが完全にJSを置き換える可能性はありますが、今のところ私はTypeScriptに賭けていこうと思います。
