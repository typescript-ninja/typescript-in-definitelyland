// NOTE ここのcallbackの型注釈の意味は別の章で解説します
// 引数を1つ取って返り値無し の関数を表します
function asyncModoki(callback: (value: string) => void) {
	callback("TypeScript");
}

// アロー関数をコールバック関数として渡す 渡す関数の型は型推論される！
asyncModoki(value => console.log(`Hello, ${value}`));

// アロー関数に明示的に型付をする場合
asyncModoki((value: string): void => console.log(`Hello, ${value}`));

export { }
