# C87 TypeScript本 予定地

[griflet(要権限)](https://tcb.mowa-net.jp/griflet/github/TechBooster/C87-TypeScript/)

## この本のビルドの仕方

```
$ gem install review
$ npm install -g grunt-cli
$ git clone git@github.com:TechBooster/C87-TypeScript.git
$ cd C87-TypeScript
$ npm install
# 裏で git submodule init && git submodule update が走る
$ grunt pdf
```

その他細かい準備(tex入れたり)は[FirstStepReVIEW](https://github.com/TechBooster/FirstStepReVIEW)に準じる。たぶん。

## 取らぬ狸の皮算用

前提：頒布後、リポジトリは公開する(紙の本はおまけ感) 以下のどれか1つ

* TypeScriptガイド本 (TypeScript仕様網羅
* TypeScript+AngularJS 設計手法の提案 本
* TypeScriptのこれからとECMAScriptのこれから 本
* TypeScript型定義ファイル書き方読本←よみたい

## 章立て

* TypeScriptの目指すもの
* TypeScriptの型システム
  * 1.1
    * 仕様は変わってないけどバグを直した結果breaking changeになった系
  * BCTについて
  * declaration merging for external module
  * tuple types
  * union types
  * type alias
* TypeScriptと型定義ファイル
  * 型定義ファイルの書き方
* TypeScriptとECMAScript 6
  * let, const
  * string templates
* TypeScriptとそれを取り巻く環境
  * ECMAScript 6
    * MTG..?
    * types?
  * AtScript
    * ソースコード年内にでなさそう感はんぱない
    * Traceur Compiler
    * TypeScriptとの実装上との違い
  * Flow
    * 噂しかない
* TypeScriptと開発環境
  * ここまで手がまわらないと思う…
  * WebStorm
    * File Watcherちゃんと設定しような！とかそのくらい
  * tsdとdtsm
