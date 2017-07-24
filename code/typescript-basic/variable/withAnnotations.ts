// JavaScriptそのものの書き方
// 変数に初期値を与えると初期値の型がそのまま変数の型になる（型推論される）
// 省略しても問題のない型の記述は積極的に省略してしまってよい！
{
	let str = "文字列";
	let num = 1;
	let bool = true;

	let func = () => { };
	let obj = {};

	console.log(str, num, bool, func(), obj);
}

// 型推論に頼らずに型注釈を明示的に書いてもよい
// 特別な理由がない限り、このやり方に長所はない
{
	let str: string = "文字列";
	let num: number = 1;
	let bool: boolean = true;

	let func: Function = () => { };
	// any はなんでも型
	let obj: any = {};

	console.log(str, num, bool, func(), obj);
}
