// 関数内部でのthisの型を偽の第一引数で指定
function testA(this: string) {
	console.log(this.toUpperCase());
}
testA.bind("TypeScript")();

function testB() {
	// --noImplicitThisオプション利用時、関数内でthisにアクセスすると怒られる
	// error TS2683: 'this' implicitly has type 'any'
	//   because it does not have a type annotation.
	console.log(this.toUpperCase());
}

export { testB }
