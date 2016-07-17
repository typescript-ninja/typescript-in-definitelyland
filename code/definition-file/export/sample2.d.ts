declare module "buzz" {
	interface Buzz {
		num: number;
	}

	// よくやりがちな過ち
	export = Buzz;
}
