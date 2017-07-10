// object という型があると、解決できる問題がある

function receiveObject(obj: object) {
	// number, string, boolean, symbol, null, undefined は弾きたい！
	// Object.create のようにobjectとnull以外を渡すと実行時エラーになるAPIがある
	Object.create(obj);
}


// OK!
receiveObject({});

// NG! 値がprimitiveな型なので…
receiveObject(1);
receiveObject("string");
receiveObject(true);
receiveObject(1);
receiveObject(Symbol.for("symol"));
receiveObject(null);
receiveObject(undefined);
