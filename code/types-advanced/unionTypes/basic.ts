let a: string | boolean | undefined;
// string | boolean なので次はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean | undefined'.
// a = 1;

// b1 と b2 を合体させてみよう
let b1: string | boolean | undefined;
let b2: boolean | number | undefined;
// c の型は string | number | boolean | undefined となる
let c: typeof b1 | typeof b2;

export { c }
