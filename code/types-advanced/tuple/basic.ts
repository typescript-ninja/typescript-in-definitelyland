// まずは今までどおりの配列から
// TypeScript 1.3.0 ではこれはいままでどおりの {}[]
// TypeScript 1.4.0? ではこれは別の箇所で解説している
//   union typesとあわせて(string | number | boolean)[]
var array = [1, "str", true];

// {} は charAt を持たないので下記はコンパイルエラーになる
// array[1].charAt(0);

// tuple! 明示的な型の指定が必要
var tuple: [number, string, boolean] = [1, "str", true];

// string は charAt を持つ！
tuple[1].charAt(0);

// TypeScriptのtuple typesは普通にArrayでもあるのだ
tuple.forEach(v => {
	console.log(v);
});
