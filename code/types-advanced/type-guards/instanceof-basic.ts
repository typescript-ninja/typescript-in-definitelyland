class A {
	str: string;
}
class B {
	num: number;
}

var obj: A | B;
if (obj instanceof A) {
	// ここではAと確定されている！
	obj.str;
} else if (obj instanceof B) {
	// ここではBと確定されている！
	obj.num;
}
