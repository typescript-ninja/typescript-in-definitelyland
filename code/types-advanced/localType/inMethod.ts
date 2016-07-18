// 現実的な活用例
class Foo {
	method() {
		// メソッド内でのみ使えるtype alias！
		type Data = string | number;
		let obj: Data = 1;

		console.log(obj);
	}
}

export { Foo }
