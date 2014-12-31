var a: string | boolean;
// string | boolean なので次はオッケー！
a = "str";
a = true;
// number はアカン。
// error TS2322: Type 'number' is not assignable to type 'string | boolean'.
// Type 'number' is not assignable to type 'boolean'.
// a = 1;

var b: boolean | number;
// a と b を合体させると string | boolean | boolean | number だ！
// 実際にはc の型は string | number | boolean となる
var c: typeof a | typeof b;
