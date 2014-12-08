var obj: any;
if (typeof obj === "string") {
	// ここでは string と確定されている！
	// number にしか存在しないメソッドを呼ぶとコンパイルエラー！
	// error TS2339: Property 'toFixed' does not exist on type 'string'.
	obj.toFixed(0);
}
