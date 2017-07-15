={typescript-as-a-tool} ツールとしてのTypeScript

TypeScriptの優位性を訴えるためにもツールとしてのTypeScriptについて簡単に紹介します。

プログラミング言語のコンパイラはコンパイルを行う。
だからといって、コンパイルのみを行うと限られているわけではありません。
プログラムをパースした結果の@<kw>{AST, Abstract Syntax Tree}、変数の型や返り値の型など、コンパイラが利用・理解しているものは余すところなく使いたいものです。
エディタを使ってコードを書く際、インスタンスにどういうメソッドが存在するかはコンパイラ自身が一番良く理解しているはずです。

実際、TypeScriptはコンパイラとしての機能やプログラムに関する情報、入力補完のためのAPIなどさまざまな使い勝手のよいAPIを備え、我々に公開してくれています。
多くのエディタやIDEでリッチな、Visual Studioと比べても遜色ない入力補完やエラー表示の機能をサポートしているのは、TypeScriptがそれらの機能をホストしているからなのです。
バックエンドに同じものを使っていれば、色々なエディタで同じように強力な機能が使えるのはある意味当然といえます。

本章ではTypeScriptのツールとしての側面をざっくりと紹介していきます。
詳しくはTypeScriptのソースを読んでください。

== Compiler API

まずはコンパイラのAPIについて触れます。
詳しくは本家Wikiのページ@<fn>{compiler-api}を参照してください。

Compiler APIはソースコードのファイルパスやソースコードを文字列として投げ込むと、ASTが得られたりコンパイル後のJSを得ることができます。

これをツールとして切り出したものとして、@<href>{https://www.npmjs.com/package/ts-node}や@<href>{https://www.npmjs.com/package/typescript-simple}があります。
筆者の@<href>{https://www.npmjs.com/package/typescript-formatter}も（undocumentedな）Compiler APIを使っているのですが、後述するLanguage Serviceを使って置き換えられるはずなので置き換えるべきですね…。

//footnote[compiler-api][@<href>{https://github.com/Microsoft/TypeScript/wiki/Using-the-Compiler-API}]

== Language Service

#@# "実装箇所へジャンプする" をLanguageServiceに追加 (Go to implementation support) in 2.1.1
#@# reference commentとかimport文での入力補完をLanguageServiceに追加 (Completions in imports and triple-slash reference paths) in 2.1.1
#@# Quick fixes をLanguageServiceに追加 (Quick fixes support in language service API) in 2.1.1
#@# Language Service経由で使えるQuick fixesの種類が増えた。More Quick fixes! in 2.2.1
#@# リファクタリングの機能をLanguage Service APIに追加 in 2.4.1

次はLanguage Serviceです。
言語の機能、エディタ向けの支援を切り出したAPIになります。
ざっくり、コンパイルエラーやソースコード上の指定した場所に当てはまる入力補完候補、シンボルのリネームで影響のある箇所の抽出、
定義の実装箇所や、メソッドなどの呼び出し元の参照、コードフォーマット、QuickFixの候補、リファクタリングの候補、などなど紹介するのも面倒な多数の機能があります。

#@# TODO LanguageServiceHostについて生成方法詰まることが多いので調べて書きたい

=== Language Service Plugin

Language Service Pluginはその名のとおり、Language Serviceを独自に拡張できる仕組みです。
Language Serviceは主にエディタから使われる機能であり、コンパイラ、つまりtscの動作には影響しません。
だから、tsconfing.jsonでpluginsを設定してもtscの動作には影響がないわけですね。

プラグインと言いますがその実現方法はなかなかダイナミックです。
エディタ上で使うべきLanguage Serviceのインスタンス、@<code>{ts.LanguageHost}がまるごとポン！と渡されるので、任意のメソッドを自分の好きな実装に差し替えます。
つまり@<code>{getQuickInfoAtPosition}の実装を差し替えればエディタ上で表示されるツールチップ（クイックインフォ）の表示を変更できる、というわけです。

プラグインの作り方については公式のガイド（@<href>{https://github.com/Microsoft/TypeScript/wiki/Writing-a-Language-Service-Plugin}）を参照するのがもっともわかりやすいでしょう。
筆者も@<href>{https://www.npmjs.com/package/@vvakame/typescript-plugin-example}にてプラグインのサンプルを公開しているので、こちらを参照してみてください。
また、中の人であるRyan Cavanaughの@<href>{https://github.com/RyanCavanaugh/sample-ts-plugin}もあります。

== Language Server Protocol

Language Server Protocol（LSP）です。
Language Service Protocolではないところに注意が必要@<fn>{decode-lsp}です。

ここまで、コンパイラがさまざまな機能をホストしていることを見てきました。
ソースコードのことを一番よく理解し、コンパイラの変更にもっとも素早く追従していくのはコンパイラ自身なのです。
エディタともっとも仲良くするべきコンポーネントはコンパイラ自身であることは明白です。

さて、ではエディタがこれらの機能にアクセスするにはどうしたらよいでしょうか？
TypeScriptはTypeScriptで書かれているため、JavaScriptの実行環境が必要です。
さらに、ソースコード全体を読み込み解釈してデータを返すには、用があるたびに毎回起動しなおしていては処理速度が低速になりすぎるため、プロセスを常駐させられたほうが都合がよいです。
VimやEmacsなどのエディタはJavaScriptを素直に実行することができません。
つまりLanguage Serviceを任意のプロセスから簡単に使えるための仕組みが必要なのです。
それが、Language Server Protocolです。

LSPはMicrosoftが提唱している仕様@<fn>{lsp-spec}です。
JSON-RPC@<fn>{json-rpc}を用いてサーバ（言語処理側）とクライアント（エディタ）間の通信を行います。
これでサーバ側の実装言語が何であるか、クライアント側の実装言語が何なのか、そしてその2つのかみ合わせをどう調整するか、という問題が解決されます。

この仕組みの素晴らしいところは、今までは各エディタでそれぞれ各言語に対応することを考えたときの問題がある程度解決することです。
このシチュエーションで、出て来るエンジニアの人種は3種類です。

 1. 言語開発者
 2. IDE・エディタ開発者
 3. 普通に開発をしたいだけの開発者

言語開発者は言語自体の開発をしたいですし、IDE・エディタの開発者も全言語の対応は非現実的なのでプラグインを開発してなんとかしてねとなりがちです。
そうすると普通に開発したいだけなのに、いつのまにかプラグインを書いたりといった余計な仕事を始めることになります。
普通に開発したいだけなのに！

言語の数×エディタの数だけ実装が必要だと地獄です。
しかし、LSPの導入により言語側のサポート、エディタ側のサポートが成立すれば、言語の数＋エディタの数の実装があれば十分です。
素晴らしい！

素晴らしいLSPですが、各言語側にLSPサポートが始まらないとお話になりません。
Rustでもrls（Rust Language Server）@<fn>{rust-lsp}の取り組みが始まっていますが、TypeScriptほど便利ではありません。
メソッドチェーンをさせた時にどういう型になるか拾えなかったりするんですね。
これは、Rust自体にTypeScriptのLanguage Service的な仕組みが設計考慮の外だったためではないかと考えられます。
よいツールを提供するためにはまずコンパイラの設計時点からこのことを念頭に置いて設計しなければならないため、開発の難易度はより難しくなるでしょう。

さて、筆者も手持ちのprhという文章構成用ツールにLSPの機能を組み込み、VSCodeで使えるようにしてみました。
興味がある人は@<href>{https://github.com/prh/prh-languageserver}と@<href>{https://github.com/prh/vscode-prh-extention}からどうぞ。
その他のLSP関連の開発を行うとき参考になるリソースは次の3つがお勧めです。

 * @<href>{https://github.com/Microsoft/vscode-tslint}
 * @<href>{https://github.com/Microsoft/vscode-languageserver-node}
 * @<href>{https://github.com/Microsoft/vscode-languageserver-node-example}

ちなみに、TypeScriptはLSPを実装していません@<fn>{ts-lsp-issue}（！？）。
TypeScriptには似た仕組みとして後述のtsserverがあるのですが、MicrosoftがLSPを提唱し始めたのはtsserverの後なのです。
仕方ないですね…。

//footnote[decode-lsp][筆者はde:code 2017の登壇概要にめっちゃLanguage Service Protocolと書いてしまいました（小声]
//footnote[lsp-spec][@<href>{https://github.com/Microsoft/language-server-protocol}]
//footnote[json-rpc][@<href>{http://www.jsonrpc.org/specification}]
//footnote[rust-lsp][@<href>{https://github.com/rust-lang-nursery/rls}]
//footnote[ts-lsp-issue][@<href>{https://github.com/Microsoft/TypeScript/issues/11274}]

=== tsserver

というわけでtsserverです。
TypeScriptはLSPをまだ実装していない代わりに、同等のJSONっぽいRPCを使ったLanguage Serviceをホストするコマンドをバンドルしています。
それがtsserverコマンドです。

本書ではtsserverについてはあまり詳しく扱いませんが、自分の好きなエディタにTypeScriptサポートを追加したい！と考えている人はこのコマンドについて調べるとよいでしょう。
公式サイトのガイド（@<href>{https://github.com/Microsoft/TypeScript/wiki/Standalone-Server-%28tsserver%29}）を参照してください。
