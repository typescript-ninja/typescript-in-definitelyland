class Base {
	str: string;
}
class InheritA extends Base {
	num: number;
}

interface Sample<T extends Base> {
	method(): T;
}

// これはOK
let objA: Sample<InheritA>;

// これはダメ RegExpはBaseを継承していない
// error TS2344: Type 'RegExp' does not satisfy the constraint 'Base'.
// Property 'str' is missing in type 'RegExp'.
// let objB: Sample<RegExp>;

// これはOK 指定したオブジェクト型リテラルはBaseクラスの要件を満たす
let objC: Sample<{ str: string; }>;

export { objA, objC };
