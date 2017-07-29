declare namespace ghost {
	interface Test {
		str: string;
	}
}

// 型としてはしっかり存在していてアクセスできる
let test: ghost.Test;
test.str;

// 実体としては存在していない！
// invalid.ts(13,17): error TS2304: Cannot find name 'ghost'.
let notExists = ghost;

export { }
