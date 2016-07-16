namespace a {
	export class Sample { }
}

namespace b {
	// 他のモジュールも普通に参照できる
	let objA: a.Sample;
	objA = new a.Sample();

	// めんどくさいなら import句 を使えばいい
	import Sample = a.Sample;
	let objB: Sample;
	objB = new Sample;

	// 別に違う名前をつけてもいい(けど混乱しちゃうかも？
	import Test = a.Sample;
	let objC: Test;
	objC = new Test();

	// 別に名前が違っても互換性が失われるわけではないのだ
	objA = new Test();
}
