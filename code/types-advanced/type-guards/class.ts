class A {
	str: string;
}
class B {
	num: number;
}
class C {
	bool: boolean;
}

var obj: A | B | C;

if (obj instanceof A) {
	// A 確定！
	obj.str;
} else if (obj instanceof B) {
	// B 確定！
	obj.num;
} else if (obj instanceof C) {
	// 消去法にはならない 世知辛い
	obj.bool;
}
