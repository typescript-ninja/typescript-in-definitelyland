declare module "foo" {
	interface Foo {
		num: number;
	}

	// この_は外部からは参照できない。exportしてないので。
	var _: Foo;
	export = _;
}
