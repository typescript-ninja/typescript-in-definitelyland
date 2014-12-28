declare module ghost {
	interface Test {
		str: string;
	}
}

// 型としては普通にアクセスできる
var test: ghost.Test;
test.str;

// 実体としては存在していない！
// invalid.ts(13,17): error TS2304: Cannot find name 'ghost'.
var notExists = ghost;
