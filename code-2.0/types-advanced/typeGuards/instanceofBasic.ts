class A {
	str: string;
}
class B {
	num: number;
}
class C extends A {
	bool: boolean;
}

let obj: A | B | C = null as any;
if (obj instanceof A) {
	// ここでは A（含むC) と確定している
	obj.str;
	if (obj instanceof C) {
		// ここではCと確定している
		obj.bool;
	}
}

if (obj instanceof C) {
	// ここではCと確定している
	obj.bool;
} else {
	// ここではまだ A | B
	if (obj instanceof B) {
		// ここではBと確定している
		obj.num;
	} else {
		// ここではAと確定している
		obj.str;
	}
}

export { }
