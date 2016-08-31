={prepared-to-typescript} 戦闘準備だ！TypeScript！

#@# @suppress SectionLength ParagraphNumber
=={install} インストールしてみよう

Node.jsのセットアップはすでに完了しているものとします。
筆者はNode.jsのインストールとバージョン管理に@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}@<fn>{nodebrew}を利用しています。

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
Version 2.0.2
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

なお、本書執筆時点では@<code>{npm install -g typescript}で導入できるTypeScriptバージョンは1.8.10です。
2.0.0以降が使いたい場合はしばらくの間は@<code>{npm install -g typescript@beta}とする必要があります。

ともあれ、これで準備は整いました。

cutting edgeな最新版コンパイラを利用したい場合は次の手順で行います。

//cmd{
$ npm install -g typescript@next
# 省略
$ tsc -v
Version 2.1.0-dev.20160716
//}

実際にはプロジェクトごとに利用するTypeScriptのバージョンを変えたい場合がほとんどでしょう。
常に最新のバージョンだけを使い続けるのは自分の管理するプロジェクトが増えれば増えるほど難しくなりますからね。
その方法を次に示します。

//cmd{
$ npm init
# Enter連打
$ ls package.json
package.json
$ npm install --save-dev typescript
$ ./node_modules/.bin/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.0.2
#@end
//}

この、@<code>{node_modules/.bin}にはプロジェクトローカルに導入された実行ファイルが集められています。
@<code>{npm bin}コマンドを実行するとパスが得られます。
macOSやLinux環境下では@<code>{$(npm bin)/tsc}とするとプロジェクトローカルのtscコマンドへのパスが取得できます。

さらにnpm scriptsでは$PATHに@<code>{node_modules/.bin}が自動的に追加されます。
このため、npm scriptsをうまく活用してプロジェクトのビルド環境を構築すると上手かつ自然にプロジェクトローカルなビルド環境が整えられるでしょう。

//cmd{
$ npm bin
$PWD/node_modules/.bin 的なパスが表示される
$ $(npm bin)/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.0.2
#@end
//}

//footnote[nodebrew][@<href>{https://github.com/hokaccha/nodebrew#nodebrew}]
//footnote[npm-scripts][@<href>{https://docs.npmjs.com/misc/scripts}]

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
    "compilerOptions": {
        "module": "commonjs",
        "target": "es5",
        "noImplicitAny": false,
        "sourceMap": false
    },
    "exclude": [
        "node_modules"
    ]
}
//}

この設定ではTypeScriptが利用できる制約の多くをONにしていないため、完全にガバガバな設定です。
一番最初に利用する例としてはいいかもしれませんが、運用するにあたってTypeScriptが与える堅牢さのすべてを享受することはできません。

本書のサンプルコード用tsconfig.jsonを@<list>{tsconfig.json}に示します。
この設定をベースに自分たちにとって都合がよい設定値へ変更したり制限を緩めたりすることをお勧めします。
具体的に"include"や"exclude"の値は一般的なフォルダ構成を対象にしたものではないので変更したほうがよいでしょう。

//list[tsconfig.json][本書サンプルコード用のtsconfig.json]{
#@mapfile(../tsconfig.json)
{
    "compilerOptions": {
        "module": "commonjs",
        "moduleResolution": "node",
        "target": "es6",
        "declaration": true,
        "lib": [
            "dom",
            "es2017"
        ],
        "types": [
            "node"
        ],
        "noImplicitAny": true,
        "strictNullChecks": true,
        "noFallthroughCasesInSwitch": true,
        "noImplicitReturns": true,
        "noImplicitThis": true,
        "noUnusedLocals": true,
        "noUnusedParameters": true,
        "noImplicitUseStrict": false,
        "sourceMap": false,
        "emitDecoratorMetadata": true,
        "experimentalDecorators": true,
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
tsconfig.jsonができたら、後はコンパイルするだけです。
コンパイルの方法はtsconfig.jsonがあれば、どんな環境でも@<code>{--project}オプションを使うだけです。
それ以外のオプションについてはtsconfig.jsonにすべて記述されています。
具体的に、@<code>{tsc -p ./}または@<code>{tsc -p ./tsconfig.json}とします。

本節についてもより詳しい説明は@<chapref>{tsc-options}を参照してください。

#@# @suppress KatakanaSpellCheck
=={editor-and-ide} エディタ・IDEの環境を整えよう

#@# @suppress KatakanaSpellCheck CommaNumber
Atom、Eclipse、Emacs、Vim、Visual Studio、WebStormなどさまざまなTypeScript対応のIDE・エディタが存在しています@<fn>{ts-editor}。

TypeScriptにはlanguage serviceという仕組みがあるため、IDEを作る時にTypeScriptコンパイラ本体から変数の種類やメソッドの有無などの情報を得えられます。
そのため多数のIDE・エディタでVisual Studioに勝るとも劣らないサポートを得ることができます。

#@# OK REVIEW: lc s/負けるとも劣らない/勝るとも劣らない/ (わざと？)

#@# @suppress SentenceLength
language serviceに興味がある場合、@<href>{https://github.com/Microsoft/TypeScript/wiki/Using-the-Language-Service-API}や@<href>{https://github.com/Microsoft/language-server-protocol}などを参照するとよいでしょう。

//footnote[ts-editor][@<href>{https://github.com/Microsoft/TypeScript/wiki/TypeScript-Editor-Support}]

==={vscode} Visual Studio Codeを使ってみる

#@# OK REVIEW lc: 節にしては薄すぎる気がする・・・これはエディタ・IDE節の中の項なのでは？むしろ、話の展開として先にVCSがTSサポート厚いから使う話をして、同様のサポートが他のエディタでもあります、その理由は〜という展開のほうが読みやすい気がする？
#@# vv: 項に格下げにしました
#@# OK REVIEW lc: 初めて読むひとにとってはC87の話はどうでもいいはずなので「現時点」あたりにfn打っておまけ程度で良いんじゃないでしょうか

現時点での筆者のお勧めは@<href>{https://code.visualstudio.com/,Visual Studio Code}@<fn>{vscode}です。
Visual Studio Code（略称：vscode）はMicrosoftが提供している無料のエディタです。
Visual Studioの名を冠していますがElectron@<fn>{electron}を利用して組まれているマルチプラットフォームなエディタで、Windows以外でも利用できます。
#@# OK REVIEW lc: s/Window/Windows/
筆者はMac OS XユーザですがTypeScriptを書く時はvscode一本です。

tsconfig.jsonがプロジェクト内に配置されていればvscodeはそこから必要な設定を読み込みます。
つまり、設定に手間をかけることなくTypeScriptコードを書き始めることができます。

//footnote[vscode][@<href>{https://code.visualstudio.com/}]
//footnote[electron][@<href>{http://electron.atom.io/}]
