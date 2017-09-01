let obj = {
	str: "string",
	num: 1,
};

// 文字列リテラルによるアクセスだと . アクセス同様の型情報が得られる
let str: string = obj["str"];
let num: number = obj["num"];
// ちなみに、存在しない要素にアクセスすると any になる
// --noImplicitAny を使うと暗黙的anyとしてちゃんと怒られる 偉い！
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let any = obj["notExists"];

// 即値じゃないとダメなので このパターンはコンパイルエラーになる
let propertyName1 = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let str2 = obj[propertyName1];

// constだと型が後述の文字列リテラル型相当なのでイケる！
const propertyName2 = "str";
let str3 = obj[propertyName2];

export { str, num, propertyName1, propertyName2, str3 }
