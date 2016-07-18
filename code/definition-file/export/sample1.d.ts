declare module "bar" {
	interface Bar {
		num: number;
	}

	// この_は外部からは参照できない。exportしてないので。
	let _: Bar;
	export = _;
}
