= 戦闘準備だ！TypeScript！

== まずはインストールしてみよう

Node.jsのセットアップは既に完了しているものとします。
筆者は@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}を利用しています。

TypeScriptのインストールには、Node.jsのパッケージマネージャである@<kw>{npm, Node Package Manager}を利用します。
TypeScriptをインストールすると、tscというコマンドが利用可能になり、これでTypeScriptコードのコンパイルを行います。

//cmd{
# -g をつけるとグローバルなコマンドとしてインストールする
$ npm install -g typescript
省略
$ tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
message TS6029: Version 1.3.0.0
#@end
$ echo "class Sample {}" > sample.ts
$ tsc --noImplicitAny sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
//}

これで準備は整いました。

cutting edgeな最新版コンパイラを利用したい場合は以下の手順で行います。

//cmd{
$ npm install -g Microsoft/TypeScript
省略
$ tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
message TS6029: Version 1.3.0.0
#@end
//}

未リリースのTypeScriptを常用するのは怖いので、一般的にはプロジェクトローカルで利用するのがよいでしょう。
その方法を以下に示します。

//cmd{
$ npm init
Enter連打
$ ls package.json                                                                                                                package.json
$ npm install Microsoft/TypeScript --save-dev
$ echo "class Sample {}" > sample.ts
$ ./node_modules/.bin/tsc --noImplicitAny sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
})();
//}

このやり方の場合、npm installする時に200MBを優に超えるリポジトリをまるまるcloneしてこようとするため、ものすごく処理に時間がかかります。
複数人で作業するようなプロジェクトには素直にリリースされているバージョンを利用するのがよいでしょう。

== WebStormで開発環境を整えよう

本ガイドでは開発環境としてWebStormを利用します。
Visual Studio Community Edition なども発表されたため、Windowsの方は素直にVisual Studioを使うのがよいでしょう。
筆者はMax OS XユーザなのでVisual Studioが使えないので仕方ないですね。
WebStormの他にもEclipse用TypeScriptプラグインもあるため、自分に馴染むものを探してみるのもよいでしょう。

WebStormは正式にTypeScriptに対応しています。
WebStormのTypeScript対応は独自のもので、TypeScriptコンパイラが持つIDE実装用API(俗にいうLanguageService)を利用していません。
このため、型推論の行われ方がTypeScriptコンパイラそのものほどは頭がよくありません。
しかし、実用上はあまり問題にならないと思うので我慢して使いましょう。
お願いじゃーJetBrains様ー！TypeScript関連技術にもうちょっと要員割いてくだされー！！
また、現時点ではWebStormは1.1.0-1までの対応のようで、tuple typesとprotectedは正しくハンドリングできていないはずです。

@<href>{https://youtrack.jetbrains.com/issue/WEB-14149,1.3.0対応の要望}と@<href>{https://youtrack.jetbrains.com/issue/WEB-14151,1.4.0対応の要望}は既にあげてありますがいつになることやら。
Microsoftの開発速度に対して明らかについていけてないんですよね…。

さて、とはいえIDEとして非常に完成度の高いWebStormなので使っていきましょう。

ここでは、WebStormを使う上で行うべき設定を解説します。
なお、執筆時に利用しているWebStormのバージョンは9.0.1です。

もし、WebStormの操作に困った時はShiftキーを2回ほど連続で押すと "なんでも検索" の小窓が開くので、Preferences とかで検索して開いて、さらに左上の小窓でFile WatcherとかScopeとかで検索してみてください。
WebStormはIDE内部の機能検索の機能が充実しているので、これでたいていのことはなんとかなるでしょう。

=== File Watchers の設定

WebStorm上でTypeScriptなファイルを新規作成すると、@<img>{project-open}のような表示がでます。

//image[project-open][File Watcher追加のお誘い]{
プロジェクト開いて Add watcher のバーが出てるとこ
//}

File Watcherの初期設定は@<img>{new-watcher-default}のようになります。
設定を読み取ると以下のようになります。

 * TypeScriptファイルが更新されたらコンパイル
 * 使うコンパイラはシステムデフォルトのもの
 * コンパイルオプションは --sourcemap のみ
 ** --noImplicitAny なし！必須だろ！
 ** --target es5 なし！
 ** --out なし！

//image[new-watcher-default][File Watcher初期設定]{
File Watcherの初期設定の画面
//}

不満は色々ありますが使ってみます。
変なコードを書くと@<img>{flymake}のように、ちゃんとエラーが引っかかってきます。

//image[flymake][即座にコンパイル！]{
flymakeっぽく動作してる画面
//}

WebStormは独自にTypeScriptコードの解析をしているのですが、精度が微妙に悪くて正しいコードにエラーだ！といったり、不正なコードを見逃したりします。
そんな時、File Watcherが設定してあるとtscが報告するエラーのみを画面に表示してくれるため、使いやすくなります。
WebStormで開発する場合、File Watchersは常に有効にするようにしたほうがよいでしょう。

=== 設定の改良

デフォルトの設定では色々と不満があります。

 * 使うコンパイラはシステムデフォルトのもの
 * コンパイルオプションは --sourcemap のみ
 ** --noImplicitAny なし！必須だろ！
 ** --target es5 なし！
 ** --out なし！

これを、以下のように変えたい！

 * 使うコンパイラはプロジェクトローカルのもの
 ** 具体的には ./node_modules/.bin/tsc
 * コンパイルオプションは必要なもの全部
 ** --noImplicitAny を追加
 ** --target es5 を追加
 ** コンパイルする時の対象ファイルを1ファイルに固定
 ** 必要があれば --out を設定
 * コンパイルの単位をメインの実装系とテストコード系に2分割したい

それぞれ、理由を説明していきます。

==== 使うコンパイラはプロジェクトローカルのもの

これは理由は単純明快ですね。
プロジェクト毎に更新頻度も違うので、新しいTypeScriptへの追従が遅い場合があるからです。
足並みを揃えて全部のプロジェクトのTypeScriptコンパイラのバージョンをあげるなど、不可能です。
なので、プロジェクトローカルにTypeScriptコンパイラをインストールしてやる必要があります。
プロジェクトトップで以下のコマンドを実行しましょう。

//cmd{
$ npm init
# 設問全部とりあえずEnterでOK
$ npm install typescript --save-dev
# 省略
$ ./node_modules/.bin/tsc --version
message TS6029: Version 1.3.0.0
//}

これを、File WatcherのProgram欄に指定します。
見た目上絶対パスで指定されているように見えますが、プロジェクト内部にあるファイルを指定したら内部的には以下のように保存されるので気にしないで大丈夫です。

@<code>{<option name="program" value="$PROJECT_DIR$/node_modules/.bin/tsc" />}

==== コンパイルオプションは必要なもの全部

1つずつ解説していきましょう。

 * --noImplicitAny を追加

これはTypeScript開発時の必須オプションですね。
型注釈が漏れていたり、実は型推論にミスっていた時にエラーにしてくれるオプションです。
かならず併用しましょう。

 * --target es5 を追加

get, set アクセサを使うために必要です。
古いブラウザの事をうじうじと考えるのはやめましょう。

 * コンパイルする時の対象ファイルを1ファイルに固定

これは筆者の経験則によるものです。
ファイルのコンパイル対象をデフォルトの 現在編集しているファイル($FileName$) にしていると、プロジェクト内の全てのファイルが単独でコンパイルできるようになっている必要があります。
つまり、ある1つのファイルの中に、全ての依存関係を解決するreference commentを書いてまわらないといけないのです。
筆者も最初はこの方針で作業していたのですが、管理がものすごくめんどくさいこと、循環依存が発生した時に--outオプションを併用した時の出力順制御が難しいこと、TypeScript 1.1.0-1 からコンパイルにかかる時間が大幅に改善したこと、上記4点を踏まえて、rootになるファイルを頂点にtree上に依存関係を定義するようにしました。
File Watcherやgruntなどのツールでは、このrootとして設計したファイルのみを常にコンパイルするようにします。

 * 必要があれば --out を設定

依存関係をtree上に綺麗に定義した恩恵として、ファイルの出力結果が予想しやすくなったため、後でconcatする時に順番を手動で制御する作業をもう一回やるよりはコンパイラに任せたほうが楽です。

以上を踏まえて、Arguments の所に以下を指定します。

@<code>{--sourcemap --noImplicitAny --target es5 --out lib/index.js lib/index.ts}

==== コンパイルの単位をメインの実装系とテストコード系に2分割したい

単一のrootとなるファイルを構成する方針にした場合、本チャンの実装とテストコードはさすがに分割してコンパイルする必要がありますね。
File Watcher 1つでは、1系統しか同時に面倒を見ることができないため、Scopesという機能で監視する範囲を区切り、Scope単位でFile Watcherを指定します。

==== 総括

Scopeを使ってプロジェクトを2つに区切って、File Watcherをそれに対応させて2つ作って、コンパイルオプションを変える！
こうだな！！
(@<img>{main-and-test-structure}、@<img>{scope-main}、@<img>{scope-test}、@<img>{main-watcher}、@<img>{test-watcher})

//image[main-and-test-structure][lib/ と test/ 配下にコードとテストコードを入れる]{
プロジェクト全体図
//}

//image[scope-main][lib/ に対する Scope の設定]{
//}

//image[scope-test][tests/ に対する Scope の設定]{
//}

//image[main-watcher][mainのScope に対する Watcher の設定]{
//}

//image[test-watcher][testのScope に対する Watcher の設定]{
//}
