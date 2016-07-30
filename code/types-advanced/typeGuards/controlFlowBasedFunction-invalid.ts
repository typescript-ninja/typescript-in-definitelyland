let v1: string | number;
if (typeof v1 === "string") {
	let f = () => {
		// これはエラーになる！
		// プログラムの字面的にはstringに確定されていそう…
		// しかし、関数はいつ実行されるかわからない
		// error TS2339: Property 'toUpperCase'
		//   does not exist on type 'string | number'.
		console.log(v1.toUpperCase());
	};
	// ここではvはまだstring
	f();

	// ここでvがnumberに！
	v1 = 1;
	f();
}

// letではなくてconstを使うと…
const v2: string | number = null as any;
if (typeof v2 === "string") {
	let f = () => {
		// v2の中身が入れ替えられる可能性はないのでエラーにならない
		console.log(v2.toUpperCase());
	};
	f();

	// constなので再代入しようとするとエラーになる
	// error TS2450: Left-hand side of assignment expression
	//   cannot be a constant or a read-only property.
	v2 = 1;
}
