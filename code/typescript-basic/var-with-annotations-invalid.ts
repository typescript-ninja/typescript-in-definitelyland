var str: string;
// 文字列は数値と互換性がない！
// error TS2322: Type 'number' is not assignable to type 'string'.
str = 1;

var num: number;
// 数値は真偽値と互換性がない！
// error TS2322: Type 'boolean' is not assignable to type 'number'.
num = true;

var bool: boolean;
// 真偽値は文字列と互換性がない！
// error TS2322: Type 'string' is not assignable to type 'boolean'.
bool = "str";
