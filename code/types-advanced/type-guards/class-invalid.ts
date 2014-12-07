// コンパイルエラーになる (コンパイラのバグ)
// https://github.com/Microsoft/TypeScript/issues/1282
var obj: RegExp | Date;

if(obj instanceof Date) {
    // ここでは Date と確定されている！
    obj.getTime();
} else if(obj instanceof RegExp) {
    // ここでは RegExp と確定されている！
    obj.test(".*");
}
