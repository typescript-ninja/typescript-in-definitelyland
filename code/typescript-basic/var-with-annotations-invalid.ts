var str: string;
// var-with-annotations-invalid.ts(3,1): error TS2322: Type 'number' is not assignable to type 'string'.
str = 1;

var num: number;
// var-with-annotations-invalid.ts(7,1): error TS2322: Type 'boolean' is not assignable to type 'number'.
num = true;

var bool: boolean;
// var-with-annotations-invalid.ts(11,1): error TS2322: Type 'string' is not assignable to type 'boolean'.
bool = "str";
