interface Foo {
	hello(): string;
}

// 同名のインタフェースを定義すると、合成される！
interface Foo {
	bye(): string;
}
