let obj: number | boolean | string = null as any;

// &&演算子で絞込み
typeof obj === "string" && obj.charAt(0);
// 次のようなコードはエラーになる！
// error TS2339: Property 'charAt' does not exist on type 'number'.
// typeof obj === "number" && obj.charAt(0);

// ||演算子でunion typesに
if (typeof obj === "string" || typeof obj === "boolean") {
	// string | boolean に絞り込まれる
} else {
	// 消去法でnumber！
}

// 三項演算子はif文と一緒の挙動
typeof obj === "string" ? obj.charAt(0) : obj;
// 次と等価
if (typeof obj === "string") {
	obj.charAt(0);
} else {
	obj;
}

// 一応、否定演算子にも対応している
if (!(typeof obj !== "string")) {
	// 否定の否定はそのまんまstringだな！ちゃんと絞り込まれます
	obj.charAt(0);
}

export { }
