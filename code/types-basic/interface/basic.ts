// 一般的な用法
interface A {
	str: string;
}
// クラスに特定の実装を強制する
class AImpl implements A {
	str: string;
}
var objA: A = new AImpl();

// インタフェースは他のインタフェースを拡張できる
interface B1 {
	str: string;
}
interface B2 extends B1 {
	num: number;
}
// 代入する値は実装したクラスとかじゃなくてもええんじゃ！
var objB: B2 = {
	str: "string",
	num: 42
};

// interfaceはクラスすら拡張する！(実装はなかったことになる
class FooClass {
	constructor(public num: number) {
	}
}
interface C extends FooClass {
	str: string;
}
var objC: C = {
	num: 42,
	str: "string"
};
