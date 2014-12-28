= 戦闘準備だ！TypeScript！

== まずはインストールしてみよう

Node.jsのセットアップはすでに完了しているものとします。
筆者はNode.jsのインストールとバージョン管理に@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}@<fn>{nodebrew}を利用しています。

TypeScriptのインストールには、Node.jsのパッケージマネージャである@<kw>{npm, Node Package Manager}を利用します。
TypeScriptをインストールすると、tscというコマンドが利用可能になります。
tscコマンドでTypeScriptコードのコンパイルを行います。

//cmd{
# -g をつけるとグローバルなコマンドとしてインストールする
$ npm install -g typescript
# 省略
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

cutting edgeな最新版コンパイラを利用したい場合は次の手順で行います。

//cmd{
$ npm install -g Microsoft/TypeScript
# 省略
$ tsc -v
#@mapoutput(node ../typescript/bin/tsc.js -v)
message TS6029: Version 1.4.0.0
#@end
//}

未リリースのTypeScriptを常用するのは怖いので、一般的にはプロジェクトローカルで利用するのがよいでしょう。
その方法を次に示します。

//cmd{
$ npm init
# Enter連打
$ ls package.json                                                                                                                package.json
$ npm install Microsoft/TypeScript --save-dev
$ ./node_modules/.bin/tsc -v
#@mapoutput(node ../typescript/bin/tsc.js -v)
message TS6029: Version 1.4.0.0
#@end
//}

このやり方の場合、npm installするときに100MB近くもあるリポジトリをまるまるcloneしてこようとするため、ものすごく処理に時間がかかります。
複数人で作業するようなプロジェクトでは素直にリリースされているバージョンを利用するのがよいでしょう。

//footnote[nodebrew][@<href>{https://github.com/hokaccha/nodebrew#nodebrew}]

== WebStormで開発環境を整えよう

Windowsの方は素直にVisual Studioを使うのがよいでしょう。
11月13日、Visual Studio Community Editionが発表されましたので、そちらがよいかもしれません。
しかし、筆者はMac OS XユーザなのでVisual Studioが選択肢から外れてしまいます。
そのため、本ガイドでは開発環境としてWebStormを紹介していきます。
WebStormの他にもEclipse用TypeScriptプラグインもあるため、自分に馴染むものを探してみるのがよいでしょう。

WebStormは正式にTypeScriptに対応しています。
WebStormのTypeScript対応は独自のもので、TypeScriptコンパイラが持つIDE実装用API（俗にいうLanguageService）を利用していません。

このため、型推論の行われ方がTypeScriptコンパイラそのものほどは頭がよくありません。
しかし、実用上はあまり問題にならないと思うので我慢できる範囲です。
最近ではTypeScript側の開発が早すぎて、WebStorm側の対応が後手に回っている状態です。
TypeScript関連技術にもうちょっと要員割いてください！お願いですJetBrains様！！

@<href>{https://youtrack.jetbrains.com/issue/WEB-14149,1.3.0対応の要望}@<fn>{webstorm-support-1.3.0}は執筆時には実装が行われ、リリース待ちになっています。
@<href>{https://youtrack.jetbrains.com/issue/WEB-14151,1.4.0対応の要望}@<fn>{webstorm-support-1.4.0}はすでにあげてありますが対応はいつになるかな…？
という感じです。

とはいえIDEとして非常に完成度の高いWebStormなので是非使っていきましょう。

ここでは、WebStormを使う上で行うべき設定を解説します。
執筆時に利用しているWebStormのバージョンは9.0.1です。

もし、WebStormの操作に困ったときはShiftキーを2回ほど連続で押すと"なんでも検索"の小窓が開くので、Preferencesとかで検索して開いて、さらに左上の小窓でFile WatcherとかScopeとかで検索してみてください。
WebStormはIDE内部の機能検索の機能が充実しているので、これでたいていのことはなんとかなるでしょう。

//footnote[webstorm-support-1.3.0][@<href>{https://youtrack.jetbrains.com/issue/WEB-14149}]
//footnote[webstorm-support-1.4.0][@<href>{https://youtrack.jetbrains.com/issue/WEB-14151}]

=== File Watchersの設定

WebStorm上でTypeScriptなファイルを新規作成すると、@<img>{project-open}のような表示がでます。

//image[project-open][File Watcher追加のお誘い]{
プロジェクト開いて Add watcher のバーが出てるとこ
//}

File Watcherの初期設定は@<img>{new-watcher-default}のようになります。
設定を読み取ると次のようになります。

 * TypeScriptファイルが更新されたらコンパイル
 * 使うコンパイラはシステムデフォルトのもの
 * コンパイルオプションは --sourcemap のみ
 ** --noImplicitAny なし！（必須でしょ！）
 ** --target es5 なし！
 ** --out なし！

//image[new-watcher-default][File Watcher初期設定]{
File Watcherの初期設定の画面
//}

初期値に不満はあります。
とはいえ、変なコードを書くと@<img>{flymake}のようにちゃんとエラーが引っかかってきます。
便利です。

//image[flymake][即座にコンパイル！]{
flymakeっぽく動作してる画面
//}

WebStormは独自にTypeScriptコードの解析をしているのですが、精度が微妙に悪くて正しいコードにエラーだ！といったり、不正なコードを見逃したりします。
そんなとき、File Watcherが設定してあるとtscが報告するエラーのみを画面に表示してくれるため、使いやすくなります。
WebStormで開発する場合、File Watchersは常に有効にするようにしたほうがよいでしょう。

=== 設定の改良

デフォルトの設定には色々と不満があります。
設定を次のように変えたいところです。
#@# OK REVIEW muo: 前の節に同じのがあるので、これ構成ミスった?と見える。「前述のように」とかでつなぐのがいい

 * 使うコンパイラはプロジェクトローカルのもの
 ** 具体的には ./node_modules/.bin/tsc
 * コンパイルオプションは必要なもの全部盛り
 ** --noImplicitAny を追加
 ** --target es5 を追加
 ** コンパイルするときの対象ファイルを1ファイルに固定
 ** 必要があれば --out を設定
 * コンパイルの単位をメインの実装系とテストコード系に2分割したい

それぞれ、理由を説明していきます。

==== 使うコンパイラはプロジェクトローカルのもの

この理由は単純明快ですね。
プロジェクト毎に開発速度や更新頻度も違います。
ゆえに、新しいTypeScriptコンパイラがリリースされたとしてもそれへの対応にはばらつきが出ます。
足並みを揃えて全部のプロジェクトのTypeScriptコンパイラのバージョンを一気にあげるなど、不可能です。
これを解決するため、プロジェクトローカルにTypeScriptコンパイラをインストールする必要があります。
プロジェクトトップで次のコマンドを実行しましょう。

//cmd{
$ npm init
# 設問全部とりあえずEnterでOK
$ npm install typescript --save-dev
# 省略
$ ./node_modules/.bin/tsc --version
#@mapoutput(../node_modules/.bin/tsc -v)
message TS6029: Version 1.3.0.0
#@end
//}
#@# OK REVIEW muo: コマンド実行へのコメント、前のほうは#無しだったけどここでは付いてる。揃えたほうがいい

これを、File WatcherのProgram欄に指定します。
見た目上絶対パスで指定されているように見えますが、プロジェクト内部にあるファイルを指定したら内部的には次のように保存されるので気にする必要はありません。

#@# OK REVIEW muo: 「大丈夫です。」はなんとかしたいところ。口語的に行き過ぎ感

@<code>{<option name="program" value="$PROJECT_DIR$/node_modules/.bin/tsc" />}

==== コンパイルオプションは必要なもの全部盛り

1つずつ解説していきましょう。

 * --noImplicitAny を追加

これはTypeScript開発時の必須オプションですね。
型注釈が漏れていたり、実は型推論にミスっていたときにエラーにしてくれるオプションです。
かならず利用しましょう。

 * --target es5 を追加

get, setアクセサを使うために必要です。
古いブラウザのことをうじうじと考えるのはやめましょう。

 * コンパイルするときの対象ファイルを1ファイルに固定

これは筆者の経験則によるものです。
ファイルのコンパイル対象をデフォルトの"現在編集しているファイル（$FileName$）"にするためには、プロジェクト内のすべてのファイルが単独でコンパイルできるようになっている必要があります。
つまり、ある1つのファイルの中にすべての依存関係を解決するreference commentを書いてまわらないといけないのです。
筆者も最初はこの方針で作業していたのですが、管理がものすごくめんどくさいこと、循環依存が発生したときに--outオプションを併用した時の出力順制御が難しいこと、TypeScript 1.1.0-1からコンパイル速度が大幅に改善されたのでコンパイル対象が大きくても気にならなくなったこと、上記3点を踏まえて、rootになるファイルを1つ定め、それを頂点としてtree上に依存関係を定義するようにしました。
File Watcherやgruntなどのツールでは、このrootとして設計したファイルのみを常にコンパイルするようにします。

 * 必要があれば --out を設定

依存関係をtree状にきれいに定義すると、結果としてファイルの出力を予想しやすくなります。
#@# OK REVIEW muo: 「場合として」とは
なので、コンパイラに結合処理を任せてしまったほうが、あとで順番を手動で制御する作業をもう一回やるよりも楽です。
#@# OK REVIEW muo: concatは「結合作業」とか「結合」のほうがわかりよいのでは

以上を踏まえて、Argumentsの所に次を指定します。

@<code>{--sourcemap --noImplicitAny --target es5 --out lib/index.js lib/index.ts}

==== コンパイルの単位をメインの実装系とテストコード系に2分割したい

単一のrootとなるファイルを構成する方針にした場合、本チャンの実装とテストコードはさすがに分割してコンパイルする必要がありますね。
File Watcher1つでは、1系統しか同時に面倒を見ることができないため、Scopesという機能で監視する範囲を区切り、Scope単位でFile Watcherを指定します。

==== 総括

Scopeを使ってプロジェクトを2つに区切って、File Watcherをそれに対応させて2つ作って、コンパイルオプションを変えよう！
（@<img>{main-and-test-structure}、@<img>{scope-main}、@<img>{scope-test}、@<img>{main-watcher}、@<img>{test-watcher}）

//image[main-and-test-structure][lib/ と test/ 配下にコードとテストコードを入れる]{
プロジェクト全体図
//}

//image[scope-main][lib/ に対するScopeの設定]{
//}

//image[scope-test][tests/ に対するScopeの設定]{
//}

//image[main-watcher][mainのScopeに対するWatcherの設定]{
//}

//image[test-watcher][testのScopeに対するWatcherの設定]{
//}
