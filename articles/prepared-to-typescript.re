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
Version 2.4.1
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
Version 2.4.1
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
Version 2.4.1
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
    /* Basic Options */
    "target": "esnext", /* Specify ECMAScript target version: 'ES3' (default), 'ES5', 'ES2015', 'ES2016', 'ES2017', or 'ESNEXT'. */
    "module": "commonjs", /* Specify module code generation: 'commonjs', 'amd', 'system', 'umd', 'es2015', or 'ESNext'. */
    "lib": [
      "dom",
      "esnext"
    ], /* Specify library files to be included in the compilation:  */
    // "allowJs": true,                       /* Allow javascript files to be compiled. */
    // "checkJs": true,                       /* Report errors in .js files. */
    // "jsx": "preserve",                     /* Specify JSX code generation: 'preserve', 'react-native', or 'react'. */
    // "declaration": true,                   /* Generates corresponding '.d.ts' file. */
    // "sourceMap": true,                     /* Generates corresponding '.map' file. */
    // "outFile": "./",                       /* Concatenate and emit output to single file. */
    // "outDir": "./",                        /* Redirect output structure to the directory. */
    // "rootDir": "./",                       /* Specify the root directory of input files. Use to control the output directory structure with --outDir. */
    // "removeComments": true,                /* Do not emit comments to output. */
    // "noEmit": true,                        /* Do not emit outputs. */
    // "importHelpers": true,                 /* Import emit helpers from 'tslib'. */
    // "downlevelIteration": true,            /* Provide full support for iterables in 'for-of', spread, and destructuring when targeting 'ES5' or 'ES3'. */
    // "isolatedModules": true,               /* Transpile each file as a separate module (similar to 'ts.transpileModule'). */

    /* Strict Type-Checking Options */
    "strict": true, /* Enable all strict type-checking options. */
    // "noImplicitAny": true,                 /* Raise error on expressions and declarations with an implied 'any' type. */
    // "strictNullChecks": true,              /* Enable strict null checks. */
    // "noImplicitThis": true,                /* Raise error on 'this' expressions with an implied 'any' type. */
    // "alwaysStrict": true,                  /* Parse in strict mode and emit "use strict" for each source file. */

    /* Additional Checks */
    "noUnusedLocals": true, /* Report errors on unused locals. */
    "noUnusedParameters": true, /* Report errors on unused parameters. */
    "noImplicitReturns": true, /* Report error when not all code paths in function return a value. */
    "noFallthroughCasesInSwitch": true, /* Report errors for fallthrough cases in switch statement. */

    /* Module Resolution Options */
    "moduleResolution": "node", /* Specify module resolution strategy: 'node' (Node.js) or 'classic' (TypeScript pre-1.6). */
    // "baseUrl": "./",                       /* Base directory to resolve non-absolute module names. */
    // "paths": {},                           /* A series of entries which re-map imports to lookup locations relative to the 'baseUrl'. */
    // "rootDirs": [],                        /* List of root folders whose combined content represents the structure of the project at runtime. */
    // "typeRoots": [],                       /* List of folders to include type definitions from. */
    "types": [
      "node"
    ], /* Type declaration files to be included in compilation. */
    // "allowSyntheticDefaultImports": true,  /* Allow default imports from modules with no default export. This does not affect code emit, just typechecking. */

    /* Source Map Options */
    // "sourceRoot": "./",                    /* Specify the location where debugger should locate TypeScript files instead of source locations. */
    // "mapRoot": "./",                       /* Specify the location where debugger should locate map files instead of generated locations. */
    // "inlineSourceMap": true,               /* Emit a single file with source maps instead of having a separate file. */
    // "inlineSources": true,                 /* Emit the source alongside the sourcemaps within a single file; requires '--inlineSourceMap' or '--sourceMap' to be set. */

    /* Experimental Options */
    "experimentalDecorators": true, /* Enables experimental support for ES7 decorators. */
    "emitDecoratorMetadata": true, /* Enables experimental support for emitting type metadata for decorators. */

    /* Other options. tsc --init で今のところ出力されないもの */
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
