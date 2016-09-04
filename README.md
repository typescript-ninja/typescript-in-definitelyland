# TypeScript in Definitelyland [![Circle CI](https://circleci.com/gh/typescript-ninja/typescript-in-definitelyland.svg?style=svg)](https://circleci.com/gh/typescript-ninja/typescript-in-definitelyland)

この書籍はvvakame([Twitter](https://twitter.com/vvakame), [GitHub](https://github.com/vvakame))が執筆した、[TypeScript](http://www.typescriptlang.org/)についての本です。

初出はコミットマーケット87にて、[TechBoosterにて頒布](http://techbooster.github.io/c87/#typescript)したものです。
現在はコミックマーケット90にて[頒布](http://techbooster.github.io/c90/#typescript)した内容が反映されています。

ただし、表紙データは含まれていません。
欲しい場合は[booth.pm](https://booth.pm/ja/items/299594)（電子版）や[COMIC ZIN](http://shop.comiczin.jp/products/detail.php?product_id=29874)（紙版）から購入してください。

## 本を読むには？

### Webサイトとして閲覧する場合

[こちら](http://typescript.ninja/typescript-in-definitelyland/)で公開中。

### 電子書籍として閲覧する場合

[booth.pm](https://booth.pm/ja/items/299594)（電子版）から入手できます。

### 紙版が欲しい！

在庫があれば[COMIC ZIN](http://shop.comiczin.jp/products/detail.php?product_id=29874)（紙版）から入手できます。

## 改訂履歴

* 2016/08/04 第2版 確定 (C90版)
* 2014/12/16 初版 確定 (C87版)

## この本のビルドの仕方

```
$ git clone https://github.com/typescript-ninja/typescript-in-definitelyland.git
$ cd typescript-in-definitelyland
$ npm run global
# gem install bundler 相当の処理が走る
$ npm install
# bundle install 相当の処理も走る
$ npm run pdf
```

その他細かい準備(tex入れたり)は[FirstStepReVIEW v2](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)に準じる。
めんどくさい場合はRe:VIEWビルド用の[dockerイメージ](https://registry.hub.docker.com/u/vvakame/review/)を用意してあるので使ってください。

## Special Thanks

### 第2版 Revised TypeScript in Definitelyland

[mhidaka](https://twitter.com/mhidaka)と[muo_jp](https://twitter.com/muo_jp)と[laco](https://twitter.com/laco0416)にはレビュー・編集・印刷の手配・当日の搬入などなどの形で大変お世話になりました(まだなってる)。
大変感謝しております :yum:

また、紙版ではイラストを[shati](http://utata-ne.net/)さん、デザインをsiosioさんに手がけていただき、第1版の世界観を踏襲した良い装丁にしてもらいました。
大変感謝し、喜んでおります :satisfied:

### 第1版 TypeScript in Definitelyland

[mhidaka](https://twitter.com/mhidaka)と[muo_jp](https://twitter.com/muo_jp)にはレビュー・編集・印刷の手配・当日の搬入などなどの形で大変お世話になりました(まだなってる)。
大変感謝しております :yum:

また、紙版ではイラストを[Jecy](http://jecy.main.jp/)さん、デザインを[shati](http://utata-ne.net/)さんに手がけていただき、大変可愛い装丁にしてもらいました。
大変感謝し、喜んでおります :satisfied:

私はコード書き以外は割りと無能ですからね。

## ライセンス

サンプルコードは全て[MITライセンス](http://vvakame.mit-license.org/)とします。

書籍部分については次の通りとします。

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a><br /><a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/typescript-ninja/typescript-in-definitelyland" property="cc:attributionName" rel="cc:attributionURL">vvakame</a> 作『<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">TypeScript in Definitelyland</span>』は<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/">クリエイティブ・コモンズ 表示 - 非営利 - 改変禁止 4.0 国際 ライセンス</a>で提供されています。
