// 名前だけ定義すると全てanyでとりあえず使える
declare module "lodash";

// 必要な変数をとりあえずanyで生やす
declare let $: any;

// 特定のパッケージ配下をとりあえず全部anyで
declare module "sample/*";


// WebPackなど特殊なローダー用
declare module "json!*";

// 同上
// モジュール読んだらモジュールは文字列
declare module "*!text" {
	const _: string;
	export = _;
}
