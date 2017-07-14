interface PropertyDescriptor<T> {
	configurable?: boolean;
	enumerable?: boolean;
	value?: T;
	writable?: boolean;
	get?(): T;
	set?(v: T): void;
}
function defineProperty<T, K extends keyof T>(o: T, p: K, attributes: PropertyDescriptor<T[K]>): any {
	return Object.defineProperty(o, p, attributes);
}

interface Foo {
	a?: string;
}

let foo: Foo = {};

// 正しい組み合わせ a に string
defineProperty(foo, "a", {
	enumerable: false,
	value: "a",
});

// ダメ a に number
// error TS2345: Argument of type '{ enumerable: false; value: number; }' 
//   is not assignable to parameter of type 'PropertyDescriptor<string>'.
//  Types of property 'value' are incompatible.
//    Type 'number' is not assignable to type 'string'.
defineProperty(foo, "a", {
	enumerable: false,
	value: 1,
});

// ダメ b は存在しない
// error TS2345: Argument of type '"b"' is not assignable to
//   parameter of type '"a"'.
defineProperty(foo, "b", {
	enumerable: false,
	value: "a",
});

export { }
