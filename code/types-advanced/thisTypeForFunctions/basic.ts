// 関数内部でのthisの型を偽の第一引数で指定
function testA(this: string) {
	console.log(this.toUpperCase());
}

// こういう利用を想定しているはず
// TYPESCRIPT と表示される
testA.bind("TypeScript")();

// 普通に呼び出すとエラーになる
// error TS2684: The 'this' context of type 'void'
//   is not assignable to method's 'this' of type 'string'.
// testA();

// 1つ目の仮引数がthisの型指定だった場合、それは偽物の仮引数
// 実際に何かを渡すとエラーになってしまう
// error TS2346: Supplied parameters do not match any signature of call target.
// testA("TypeScript");

function testB() {
	// --noImplicitThisオプション利用時、関数内でthisにアクセスすると怒られる
	// error TS2683: 'this' implicitly has type 'any' 
	//   because it does not have a type annotation.
	// console.log(this.toUpperCase());
}

function testC(this: string, postfix: string) {
	console.log(`${this.toUpperCase()}${postfix}`);
}
// TYPESCRIPT! と表示される
testC.bind("TypeScript")("!");

export { testB }
