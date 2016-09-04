// 仮引数 b は利用されていないのでエラー _c はプリフィクス_なのでエラーにならない
// error TS6133: 'b' is declared but never used.
export function foo(a: string, b: number, _c: boolean) {
	console.log(a);
}

export class Sample {
	// 仮引数 a は利用されていないのでエラー
	// error TS6133: 'a' is declared but never used.
	method(a: string) {
	}
}
