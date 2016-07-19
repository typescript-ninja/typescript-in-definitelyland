let str = "TypeScript";
if (typeof str === "number") {
	// string型な変数の値がnumberだったら… ありえん！never！
	// error TS2339: Property 'toUpperCase' does not exist on type 'never'.
	str.toUpperCase();
}

function test(): never {
	// returnないし関数のおしりに到達できないので返り値の型はneverになる
	throw new Error();
}

let obj: never = test();
// ここに到達する事は…ありえん！
// error TS2339: Property 'test' does not exist on type 'never'.
obj.test();

export { }
