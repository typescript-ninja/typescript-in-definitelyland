// まずは今までどおりの配列から
// これは別の箇所で解説している union typesで表現され (number | string | boolean)[]
let array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
let tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
	console.log(v);
});

export { array }
