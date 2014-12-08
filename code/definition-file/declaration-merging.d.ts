interface Foo {
	hello(): string;
}

// 同名のインタフェースを定義すると、合体される！
interface Foo {
	bye(): string;
}
