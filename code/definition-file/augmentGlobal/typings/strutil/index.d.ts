// importされなかった場合、globalにstrutilという名前で展開する
export as namespace strutil;

// 普通の型定義 declare module "..." の中と同じ書き味でよい
export interface Options {
	i?: number;
}
export declare function randomizeString(str: string, opts?: Options): string;

// グローバルな要素の拡張
declare global {
	// 既存のstring型にメソッドを生やす
	interface String {
		randomizeString(opts?: Options): string;
	}
}
