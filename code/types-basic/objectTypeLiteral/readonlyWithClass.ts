class Foo {
	readonly str: string;

	constructor() {
		// 変更可能
		this.str = "TypeScript";
	}

	modify() {
		// readonly が変更できるのはconstructorだけ！
		// error TS2450: Left-hand side of assignment expression cannot be a constant or a read-only property.
		// this.str = "JavaScript";
	}
}

export { Foo }
