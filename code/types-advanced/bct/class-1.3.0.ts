class A {
}
class B extends A {
	str: string;
}
class C extends A {
	num: number;
}

// array: A[] だ！
var array = [new A(), new B(), new C()];
