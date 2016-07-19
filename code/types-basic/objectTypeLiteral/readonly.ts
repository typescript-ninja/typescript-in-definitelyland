interface Foo {
	// readonly を使うと直接は書き換えできなくなる
	readonly str: string;
}

let objA: Foo = {
	str: "TypeScript",
};
// 上書きはできない！
// error TS2450: Left-hand side of assignment expression cannot be a constant or a read-only property.
// objA.str = "JavaScript";

// 別にconstではないので迂回路から変更できてしまう
let objB = {
	str: "Mutable",
};
objA = objB;
// objB経由でobjA.strを書き換える
objB.str = "Modified!";
// Modified! と表示される
console.log(objA.str);

export { }
