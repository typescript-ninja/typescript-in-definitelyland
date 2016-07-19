class Base {
	str: string;
}
class InheritA extends Base {
	num: number;
}

// TはBaseを継承済の型でなければならない制約
interface Sample<T extends Base> {
	method(): T;
}

// これはOK InheritAはBaseを継承している
let objA: Sample<InheritA>;

// これはダメ RegExpはBaseを継承していない
// error TS2344: Type 'RegExp' does not satisfy the constraint 'Base'.
// Property 'str' is missing in type 'RegExp'.
// let objB: Sample<RegExp>;

// これはOK 指定したオブジェクト型リテラルはBaseクラスの要件を満たす
let objC: Sample<{ str: string; }>;

interface Service<T> {
	service(t: T): T;
}

// F-Bounded Polymorphism の例
// 制約の内容に自分自身の参照を含む 極稀に使う
function f<T extends Service<T>>(x: T) {
	return x.service(x);
}

export { objA, objC, f };
