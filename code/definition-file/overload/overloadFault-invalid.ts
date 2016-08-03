declare function funcA(word: string): string;
declare function funcA(num: number): string;

let obj: string | number = null as any;

// stringかnumberを渡さなければならない場合 string | number はコンパイルエラーになる
// 本来であれば、受け入れてほしいのだけど…
// error TS2345: Argument of type 'string | number'
//   is not assignable to parameter of type 'number'.
//  Type 'string' is not assignable to type 'number'.
funcA(obj);

// 元の定義がunion typesならもちろんOK
declare function funcB(word: string | number): string;
funcB(obj);
