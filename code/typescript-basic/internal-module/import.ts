module a {
	export class Sample { }
}

module b {
	// 他のモジュールも普通に参照できる
	var objA: a.Sample;
	objA = new a.Sample();

	// めんどくさいなら import句 を使えばいい
	import Sample = a.Sample;
	var objB: Sample;
	objB = new Sample;

	// 別に違う名前をつけてもいい(けど混乱しちゃうかも？
	import Test = a.Sample;
	var objC: Test;
	objC = new Test();

	// 別に名前が違っても互換性が失われるわけではないのだ
	objA = new Test();
}
