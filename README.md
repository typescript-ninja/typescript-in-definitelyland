# TypeScript in Definitelylang

この書籍はvvakame([Twitter](https://twitter.com/vvakame), [GitHub](https://github.com/vvakame))が執筆した、[TypeScript](http://www.typescriptlang.org/)についての本です。
初出はコミットマーケット87にて、[TechBoosterにて頒布](http://techbooster.github.io/c87/#typescript)したもので、その全文を公開しています。

ただし、表紙データは含まれていません。
欲しい場合は紙版をなんとか入手していただきたいところです。

## 本を読むには？

### Webサイトとして閲覧する場合

[こちら](http://typescript.ninja/typescript-in-definitelyland/)で公開予定

### 電子書籍として閲覧する場合

[griflet](https://tcb.mowa-net.jp/griflet/github/typescript-ninja/typescript-in-definitelyland/)よりダウンロード可能

### 紙版が欲しい！

未定

## 改訂履歴

* 2014/12/16 初版 確定 (C87版)

## この本のビルドの仕方

```
$ npm install -g grunt-cli
$ gem install bundler
$ git clone https://github.com/typescript-ninja/typescript-in-definitelyland.git
$ cd typescript-in-definitelyland
$ npm install
# 裏で git submodule init && git submodule update && bundle install が走る
$ grunt pdf
```

その他細かい準備(tex入れたり)は[FirstStepReVIEW](https://github.com/TechBooster/FirstStepReVIEW)に準じる。
めんどくさい場合はRe:VIEWビルド用の[dockerイメージ](https://registry.hub.docker.com/u/vvakame/review/)を用意してあるので使ってください。

## Special Thanks

[mhidaka](https://twitter.com/mhidaka)と[muo_jp](https://twitter.com/muo_jp)にはレビュー・編集・印刷の手配・当日の搬入などなどの形で大変お世話になりました(まだなってる)。
大変感謝しております :yum:

また、紙版ではイラストを[Jecy](http://jecy.main.jp/)さん、デザインを[shati](http://utata-ne.net/)さんに手がけていただき、大変可愛い装丁にしてもらいました。
大変感謝し、喜んでおります :satisfied:

私はコード書き以外は割りと無能ですからね。

## ライセンス

サンプルコードは全て[MITライセンス](http://vvakame.mit-license.org/)とします。

書籍部分については次の通りとします。

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a><br /><a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/typescript-ninja/typescript-in-definitelyland" property="cc:attributionName" rel="cc:attributionURL">vvakame</a> 作『<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">TypeScript in Definitelyland</span>』は<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/">クリエイティブ・コモンズ 表示 - 非営利 - 改変禁止 4.0 国際 ライセンス</a>で提供されています。
