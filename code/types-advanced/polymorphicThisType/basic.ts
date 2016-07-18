// 自分自身を型として表す時、this を利用する
class A {
	_this: this;
	a(): this {
		return this;
	}

	d(arg: this): this {
		return arg;
	}

	e() { // thisをreturnした場合暗黙的に返り値もthisとなる
		return this;
	}
}

class B extends A {
	b() {
		console.log("B");
	}
}

interface C extends A {
	c(): void;
}

// a() はクラスAのメソッドだが返り値の型はB自身だ！
new B().a().e().b();

// d() もクラスAのメソッドだが引数はBでなければならぬ
new B().d(new B()).b();

// d() はクラスAのメソッドだが、Bに生えている限りAを渡したら怒られてしまう
// error TS2345: Argument of type 'A' is not assignable to parameter of type 'B'.
//   Property 'b' is missing in type 'A'.
// new B().d(new A()).b();

// プロパティについても同様にB自身になる
new B()._this.b();

// インタフェースでもOK C自身になる
let c: C = null as any;
c.a().c();

export { }
