// 次の2つは(thisが絡まない限り)等価！
let funcA = () => true;
let funcB = function() {
	return true;
};
funcA();
funcB();

// NOTE ここのcallbackの型注釈の意味は別の章で解説します
// 引数を1つ取って返り値無し の関数を表します
function asyncModoki(callback: (value: string) => void) {
	callback("TypeScript");
}
// ES5時代の書き方
asyncModoki(function(value: string) {
	console.log(`Hello, ${value}`);
});
// アロー関数だとさらに楽
asyncModoki(value => console.log(`Hello, ${value}`));
// アロー関数に型付をする場合
asyncModoki((value: string): void => console.log(`Hello, ${value}`));

export {}
