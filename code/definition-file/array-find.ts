// TypeScript 1.3.0 では Array.prototype.find の定義はまだ存在していない
interface Array<T> {
	find(callback: (e: T, idx: number, array: T[]) => boolean, thisArg?: any): T;
}

var array = [1, 2, 3];

// 最初に見つかる奇数を得る
array.find(v => v % 2 === 1);
