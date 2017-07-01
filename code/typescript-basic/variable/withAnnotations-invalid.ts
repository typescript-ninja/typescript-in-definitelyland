let str: string;
// 文字列は数値と互換性がない！
// error TS2322: Type '1' is not assignable to type 'string'.
str = 1;

let num: number;
// 数値は真偽値と互換性がない！
// error TS2322: Type 'true' is not assignable to type 'number'.
num = true;

let bool: boolean;
// 真偽値は文字列と互換性がない！
//  error TS2322: Type '"str"' is not assignable to type 'boolean'.
bool = "str";

export {}
