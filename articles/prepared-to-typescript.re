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
message TS6029: Version 1.3.0.0
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

ここでは、WebStormを使う上で便利な設定項目をいくつか解説します。

TODO
