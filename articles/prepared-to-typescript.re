={prepared-to-typescript} 戦闘準備だ！TypeScript！

#@# @suppress SectionLength ParagraphNumber
=={install} インストールしてみよう

Node.jsのセットアップはすでに完了しているものとします。
筆者はNode.jsのインストールとバージョン管理に@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}@<fn>{nodebrew}を利用しています。

//footnote[nodebrew][@<href>{https://github.com/hokaccha/nodebrew#nodebrew}]

#@# @suppress JapaneseStyle
TypeScriptのインストールには、Node.jsのパッケージマネージャである@<kw>{npm, Node Package Manager}を利用します。
TypeScriptをインストールすると、tscというコマンドが利用可能になります。
tscコマンドでTypeScriptコードのコンパイルを行います。

//cmd{
# -g をつけるとグローバルなコマンドとしてインストールする
$ npm install -g typescript
# 省略
$ tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.4.2
#@end
$ echo "class Sample {}" > sample.ts
$ tsc sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
}());
//}

なお、本書執筆時点では@<code>{npm install -g typescript}で導入できるTypeScriptバージョンは2.4.1です。

cutting edgeな最新版コンパイラを利用したい場合はnpmにリリースされているnextタグやrcタグを使います。

//cmd{
$ npm install -g typescript@next
# 省略
$ tsc -v
Version 2.5.0-dev.20170712
//}
#@# TODO 定期的にこの辺更新したほうがよさそう

実際にはプロジェクトごとに利用するTypeScriptのバージョンを変えたい場合がほとんどでしょう。
最新のバージョンだけを使い続ける方法は自分の管理するプロジェクトが増えれば増えるほど難しくなりますからね。

プロジェクトごとにバージョンを指定する方法を次に示します。

//cmd{
$ npm init -y
# 省略
$ ls package.json
package.json
$ npm install --save-dev typescript
$ ./node_modules/.bin/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.4.2
#@end
//}

@<code>{node_modules/.bin}にはプロジェクトローカルに導入された実行ファイルが集められています。
@<code>{npm bin}コマンドを実行するとパスが得られます。
macOSやLinux環境下では@<code>{$(npm bin)/tsc}とするとプロジェクトローカルのtscコマンドへのパスが取得できます。

さらにnpm scripts@<fn>{npm-scripts}では$PATHに@<code>{node_modules/.bin}が自動的に追加されます。
このため、npm scriptsを活用してプロジェクトのビルド環境を構築すると上手かつ自然にプロジェクトローカルなビルド環境が整えられるでしょう。

//footnote[npm-scripts][@<href>{https://docs.npmjs.com/misc/scripts}]

//cmd{
$ npm bin
$PWD/node_modules/.bin 的なパスが表示される
$ $(npm bin)/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.4.2
#@end
//}

=={prepare-tsconfig.json} tsconfig.jsonを準備する

TypeScriptではtsconfig.jsonという設定ファイルを利用します。
必要なコンパイルオプションやコンパイル対象となるファイルはすべてtsconfig.jsonに記述します。
すべてのツールやIDE・エディタ間で共通に利用できる設定ファイルになるため、大変役立ちます。

まずはtsconfig.jsonを作成してみましょう。
@<code>{tsc --init}で作成できます。

//cmd{
$ tsc --init
message TS6071: Successfully created a tsconfig.json file.
$ cat tsconfig.json
{
  // かなり長い内容のJSONが出力されます
  // もっと色々なオプションの説明が書いてあるので試してみてください
  "compilerOptions": {
    "target": "es5",
    "module": "commonjs",
    "strict": true
  }
}
//}

tsconfig.jsonはJSONですがコメントを書くことが可能@<fn>{jsoncomment}です。

//footnote[jsoncomment][大変便利なのですが一般的な規約をぶっ壊してくるのは勘弁していただきたい…]

TypeScriptが出力するデフォルトの設定は一般向けの設定です。
初心者がとりあえず使い始める足がかりとしては十分です。
@<code>{--strict}がデフォルトでtrueになっているあたりにTypeScriptの強い気持ちが感じられますね。

さらに制約を強化した本書のサンプルコード用tsconfig.jsonを@<list>{tsconfig.json}に示します。
この設定をベースに自分たちにとって都合がよい設定値へ変更したり制限を緩めたりすることをお勧めします。
具体的に"include"や"exclude"の値は一般的なフォルダ構成を対象にしたものではないので変更したほうがよいでしょう。

//list[tsconfig.json][本書サンプルコード用のtsconfig.json]{
#@mapfile(../tsconfig.json)
{
  "compilerOptions": {
    /* Basic Options */
    "target": "esnext",
    "module": "commonjs",
    "lib": [
      "dom",
      "esnext",
      "esnext.asynciterable"
    ],

    /* Strict Type-Checking Options */
    "strict": true,

    /* Additional Checks */
    "noUnusedLocals": true,
    "noUnusedParameters": true,
    "noImplicitReturns": true,
    "noFallthroughCasesInSwitch": true,

    /* Module Resolution Options */
    "moduleResolution": "node",
    "types": [
      "node"
    ],

    /* Experimental Options */
    "experimentalDecorators": true,
    "emitDecoratorMetadata": true,

    /* Other Options. tsc --init で今のところ出力されないもの */
    "noStrictGenericChecks": false,
    "forceConsistentCasingInFileNames": true,
    "traceResolution": false,
    "listFiles": false,
    "stripInternal": true,
    "skipDefaultLibCheck": true,
    "skipLibCheck": false,
    "pretty": false,
    "noEmitOnError": true
  },
  "include": [
    "code/**/*.ts"
  ],
  "exclude": [
    "node_modules",
    "code/tsc-options/helpers/",
    "code/definition-file/usage/",
    "code/definition-file/augmentGlobal/",
    "code/definition-file/issue9831/",
    "code/**/*-invalid.ts",
    "code/**/*-invalid.d.ts",
    "code/**/invalid.ts",
    "code/**/invalid.d.ts",
    "code/**/*-ignore.ts",
    "code/**/*-ignore.d.ts",
    "code/**/ignore.ts",
    "code/**/ignore.d.ts"
  ]
}
#@end
//}

より詳しい説明は@<chapref>{tsc-options}を参照してください。

=={compile} 試しにコンパイルしてみる

#@# @suppress KatakanaSpellCheck
tsconfig.jsonができたら、後は@<code>{tsc}コマンドを実行するだけです。
引数無しで実行した場合、デフォルトでtsconfig.jsonを参照してコンパイルが行われます。

本節についても詳細は@<chapref>{tsc-options}を参照してください。

#@# @suppress KatakanaSpellCheck
=={editor-and-ide} エディタ・IDEの環境を整えよう

#@# @suppress KatakanaSpellCheck CommaNumber
Visual Studio Code、Atom、WebStorm、Visual Studio、Vim、Emacs、EclipseなどTypeScript対応のIDE・エディタが数多く存在@<fn>{ts-editor}しています。

//footnote[ts-editor][@<href>{https://github.com/Microsoft/TypeScript/wiki/TypeScript-Editor-Support}]

現時点での筆者のお勧めは@<href>{https://code.visualstudio.com/,Visual Studio Code}@<fn>{vscode}です。
Visual Studio Code（略称：vscode）はMicrosoftが提供している無料のエディタです。
Visual Studioの名を冠していますがElectron@<fn>{electron}を利用したマルチプラットフォームなエディタで、Windows以外でも利用できます。
筆者はmacOSユーザですがTypeScriptを書く時はvscode一本です。
#@# OK REVIEW lc: s/Window/Windows/

//footnote[vscode][@<href>{https://code.visualstudio.com/}]
//footnote[electron][@<href>{http://electron.atom.io/}]

vscodeはプロジェクト内に配置されたtsconfig.jsonから必要な設定を読み込みます。
つまり、設定に手間をかけることなくTypeScriptコードを書き始めることができます。

#@# OK REVIEW lc: 節にしては薄すぎる気がする・・・これはエディタ・IDE節の中の項なのでは？むしろ、話の展開として先にVCSがTSサポート厚いから使う話をして、同様のサポートが他のエディタでもあります、その理由は〜という展開のほうが読みやすい気がする？
#@# vv: 項に格下げにしました
#@# OK REVIEW lc: 初めて読むひとにとってはC87の話はどうでもいいはずなので「現時点」あたりにfn打っておまけ程度で良いんじゃないでしょうか

TypeScriptにはLanguage Serviceという仕組みがあるため、IDEを作る時にTypeScriptコンパイラ本体から変数の種類やメソッドの有無などの情報を得えられます。
そのため多数のIDE・エディタでVisual Studioに勝るとも劣らないサポートを得ることができます。

#@# OK REVIEW: lc s/負けるとも劣らない/勝るとも劣らない/ (わざと？)

#@# @suppress SentenceLength
Language Serviceに興味がある場合、次の文献を参照するとよいでしょう。

 * @<href>{https://github.com/Microsoft/TypeScript/wiki/Using-the-Language-Service-API}
 * @<href>{https://github.com/Microsoft/language-server-protocol}

===[column] TypeScriptのリリースサイクルについて

TypeScriptのリリースサイクルについて解説しておきます。
以前は目玉となる機能の開発完了やビジネス上の都合（イベントの開催など）ありきのリリースサイクルでした。
このため、すでに開発完了している機能やバグフィクスがあってもなかなか使えるようにならない！という不満がありました。
最近ではこの問題が改められ、定期的にリリースする運用に変更@<fn>{ts-release-schedule}されました。

//footnote[ts-release-schedule][@<href>{https://blogs.msdn.microsoft.com/typescript/2017/03/27/typescripts-new-release-cadence/}]

変更後のリリースサイクルは次のとおりです。

 * 機能追加のフルリリースは2ヶ月毎（minorアップデート）
 * パッチまたは機能追加のリリースを1ヶ月毎（patchアップデート）

ある機能追加の開発が間に合わなかったら次のリリースに先送り。
TypeScriptのリリースはVSCodeのリリースの一週間程度前に行われる。
という感じです。
VSCodeが月イチ更新なのでエディタとの連携を視野に入れているわけです。

なお、各リリースの更新内容は公式ブログ@<fn>{official}やRoadmap@<fn>{ts-roadmap}、What's new in TypeScript@<fn>{whats-new}を確認するとわかりやすいです。

//footnote[official][@<href>{https://blogs.msdn.microsoft.com/typescript/}]
//footnote[ts-roadmap][@<href>{https://github.com/Microsoft/TypeScript/wiki/Roadmap}]
//footnote[whats-new][@<href>{https://github.com/Microsoft/TypeScript/wiki/What%27s-new-in-TypeScript}]

===[/column]
