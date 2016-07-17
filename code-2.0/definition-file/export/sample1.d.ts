declare module "bar" {
	interface Bar {
		num: number;
	}

	// この_は外部からは参照できない。exportしてないので。
	var _: Bar;
	export = _;
}
