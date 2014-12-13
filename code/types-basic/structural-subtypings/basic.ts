// クラス Foo はstring型のstrという名前のプロパティと
// number型のnumという名前のプロパティを持つ
class Foo {
	str = "string";
	num = 1;
}

// 構造が完全に一致…！！！
// 構造が同じならもう同じってことでいいんじゃね！？
var obj: Foo = {
	str: "Hi!",
	num: 42
};
