interface Foo {
	value: number | string;
}

let foo: Foo = {
	value: "TypeScript",
};

// number | string では toUpperCase があるか確定できない
// error TS2339: Property 'toUpperCase' does not exist on type 'number | string'.
// foo.value.toUpperCase();

// 変数直だけではなくて、変数のプロパティでもtype guardsが使える
if (typeof foo.value === "string") {
	// ここでは foo.value は string に絞りこまれている！一時変数いらない！
	foo.value.toUpperCase();
}

export { }
