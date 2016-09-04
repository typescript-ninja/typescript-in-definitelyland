let objA: any;
if (typeof objA === "string") {
	// ここでは string と確定されている！
	// number にしか存在しないメソッドを呼ぶとコンパイルエラー！
	// error TS2339: Property 'toFixed' does not exist on type 'string'.
	objA.toFixed(0);
}

let objB: number = 1;
if (typeof objB === "string") {
	// "ありえない" パターンだとnever型になり怒られる
	// error TS2339: Property 'toFixed' does not exist on type 'never'.
	objB.toFixed(0);
}
