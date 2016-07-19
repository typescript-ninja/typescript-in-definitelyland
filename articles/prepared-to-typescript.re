={prepared-to-typescript} 戦闘準備だ！TypeScript！

//comment{
--module と --target が個別に掛けあわせできるようになった話とか
//}

#@# @suppress SectionLength
== まずはインストールしてみよう

Node.jsのセットアップはすでに完了しているものとします。
筆者はNode.jsのインストールとバージョン管理に@<href>{https://github.com/hokaccha/nodebrew#nodebrew,nodebrew}@<fn>{nodebrew}を利用しています。

TypeScriptのインストールには、Node.jsのパッケージマネージャである@<kw>{npm, Node Package Manager}を利用します。
TypeScriptをインストールすると、tscというコマンドが利用可能になります。
tscコマンドでTypeScriptコードのコンパイルを行います。

#@# prh:disable
//cmd{
# -g をつけるとグローバルなコマンドとしてインストールする
$ npm install -g typescript
# 省略
$ tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.0.0
#@end
$ echo "class Sample {}" > sample.ts
$ tsc --noImplicitAny sample.ts
$ cat sample.js
var Sample = (function () {
    function Sample() {
    }
    return Sample;
}());
//}

なお、本書執筆時点では@<code>{npm install -g typescript}で導入されるTypeScriptバージョンは1.8.10です。
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
$ ls package.json                                                                                                                package.json
$ npm install --save-dev typescript
$ ./node_modules/.bin/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.0.0
#@end
//}

この、@<code>{node_modules/.bin}にはnpmの動作上色々な活用方法があります。
たとえば、@<code>{npm bin}コマンドでプロジェクトローカルな実行ファイルのパスが得られます。
このため、macOSやLinux環境下では@<code>{$(npm bin)/tsc}とするとプロジェクトローカルのtscコマンドへのパスが得られます。
さらに、npm scriptsでは$PATHに@<code>{node_modules/.bin}が自動的に追加されます。
このため、npm scriptsをうまく活用してプロジェクトのビルド環境を構築すると上手かつ自然にプロジェクトローカルなビルド環境が整えられるでしょう。

//cmd{
$ npm bin
$PWD/node_modules/.bin 的なパスが表示される
$ $(npm bin)/tsc -v
#@mapoutput(../node_modules/.bin/tsc -v)
Version 2.0.0
#@end
//}

//footnote[nodebrew][@<href>{https://github.com/hokaccha/nodebrew#nodebrew}]
//footnote[npm-scripts][@<href>{https://docs.npmjs.com/misc/scripts}]

== tsconfig.jsonを準備する

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
一番最初に利用する例としてはいいかもしれませんが、運用するにあたってこのままではTypeScriptが与える堅牢さのすべてを享受することはできません。

本書のサンプルコード用tsconfig.jsonを@<list>{tsconfig.json}に示します。
まずはこの設定をベースに自分たちにとって都合のよいように設定値を変更したり制限を緩めたりすることをお勧めします。
とりあえず、"include"や"exclude"の値は一般的なフォルダ構成ではないので変更したほうがよいでしょう。

//list[tsconfig.json][本書サンプルコード用のtsconfig.json]{
#@mapfile(../tsconfig.json)
{
    "compilerOptions": {
        "module": "commonjs",
        "moduleResolution": "node",
        "target": "es6",
        "lib": [
            "dom",
            "es2017"
        ],
        "noImplicitAny": true,
        "strictNullChecks": true,
        "noFallthroughCasesInSwitch": true,
        "noImplicitReturns": true,
        "noImplicitThis": true,
        "noUnusedLocals": true,
        "noUnusedParameters": true,
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
        "code/definition-file/augmentGlobal/",
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

#@# TODO lib.d.tsの細分化と--libプロパティによる個別指定 に言及する

== 試しにコンパイルしてみる

tsconfig.jsonの作成ができたら、後はコンパイルするだけです。
コンパイルの方法はtsconfig.jsonがあれば、どんな環境でも@<code>{--project}オプションを使うだけです。
それ以外のオプションについてはtsconfig.jsonにすべて記述されています。
具体的に、@<code>{tsc -p ./}または@<code>{tsc -p ./tsconfig.json}とします。

tsconfig.jsonではない名前のファイルを使って、プロジェクト内に複数のビルド構成を作ることもできます。
しかし、その場合エディタ・IDE側がその設定をうまくハンドリングしてくれない場合が多いため、基本的には努力して1プロジェクト1tsconfig.jsonにするようにしましょう。

gulpやgruntなどのタスクランナーを使う場合でも、tsconfig.jsonを用意し@<code>{--project}オプションのみでコンパイルが通せる環境を維持するのがよいでしょう。

#@# TODO 各オプションの細かい解説は@<chapref>{learning-tsc}を参照してください
#@# TODO allowJsについてどこかに書きたい

== エディタ・IDEの環境を整えよう

Atom, Eclipse, Emacs, Vim, Visual Studio, WebStormなどさまざまなTypeScript対応のIDE・エディタが存在しています@<fn>{ts-editor}。

TypeScriptにはlanguage serviceという仕組みがあるため、IDEを作る時にTypeScriptコンパイラ本体からどういうメソッドがあるかどうかなどさまざまな情報を得ることができます。
そのため、多くのIDE・エディタでVisual Studioに負けるとも劣らないサポートを得ることができます。

興味がある場合、@<href>{https://github.com/Microsoft/TypeScript/wiki/Using-the-Language-Service-API}や@<href>{https://github.com/Microsoft/language-server-protocol}などを参照するとよいでしょう。

//footnote[ts-editor][@<href>{https://github.com/Microsoft/TypeScript/wiki/TypeScript-Editor-Support}]

== Visual Studio Codeで開発環境を整えよう

C87版ではWebStormを利用するよう案内しましたが、現時点での筆者のお勧めは@<href>{https://code.visualstudio.com/,Visual Studio Code}@<fn>{vscode}です。
Visual Studio Code（略称：vscode）はMicrosoftが提供している無料のエディタです。
Visual Studioの名を冠していますがElectronを利用して組まれているマルチプラットフォームなエディタで、Window以外でも利用できます。
筆者はMac OS XユーザですがTypeScriptを書く時はvscode一本です。

tsconfig.jsonがプロジェクト内に配置されていればvscodeはそこから必要な設定を読み込みます。
つまり、設定に手間をかけることなくTypeScriptコードを書くことを始められます。

//footnote[vscode][@<href>{https://code.visualstudio.com/}]
