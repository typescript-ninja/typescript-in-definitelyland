let v: string | number;

v = "string";

let f = () => {
	// これはエラーになる！
	// プログラムの字面的にはstringに確定されていそうだが、関数はいつ実行されるかわからない
	// error TS2339: Property 'toUpperCase' does not exist on type 'string | number'.
	console.log(v.toUpperCase());
};
// ここではvはまだstring
f();

// ここでvがnumberに！
v = 1;
f();
