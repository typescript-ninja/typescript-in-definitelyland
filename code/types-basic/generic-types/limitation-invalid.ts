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
var objA: Sample<InheritA>;

// これはダメ RegExpはBaseを継承していない
// error TS2344: Type 'RegExp' does not satisfy the constraint 'Base'.
// Property 'str' is missing in type 'RegExp'.
var objB: Sample<RegExp>;

// これはOK 指定したオブジェクト型リテラルはBaseクラスの要件を満たす
var objC: Sample<{ str: string; }>;
