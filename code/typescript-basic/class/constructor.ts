class BaseA {
	constructor(public str: string) {
	}
}

// BaseA と等価な定義
class BaseB {
	str: string;
	constructor(str: string) {
		this.str = str;
	}
}

export { BaseA, BaseB }
