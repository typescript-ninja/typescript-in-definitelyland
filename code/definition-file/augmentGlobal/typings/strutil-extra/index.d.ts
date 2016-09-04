// 他のモジュールの型定義を参照する
import * as strutil from "strutil";

export as namespace strutilExtra;

export declare function happy(str: string): string;

// 他のモジュールの拡張
declare module "strutil" {
	// 既存の要素を拡張できる
	interface Options {
		reverse?: boolean;
	}

	// 自分ではないモジュールに勝手に新規の変数や関数を生やしたりはできない
	// 定義の拡張のみ可能
	// error TS1038: A 'declare' modifier cannot be used
	//   in an already ambient context.
	// export declare let test: any;
}

declare global {
	interface String {
		happy(): string;
	}
}
