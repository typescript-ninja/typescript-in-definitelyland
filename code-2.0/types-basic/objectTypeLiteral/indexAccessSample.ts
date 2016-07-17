let obj = {
	str: "string",
	num: 1
};

// 文字列リテラルによるアクセスだと普通に . アクセス同様に型情報が得られる
let str: string = obj["str"];
let num: number = obj["num"];
// ちなみに、存在しない要素にアクセスすると any になる
// --noImplicitAny を使うと暗黙的anyとしてちゃんと怒られる 萌え
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let any = obj["notExists"];

// 即値じゃないとダメ！コンパイラが処理できないのです
let propertyName1 = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let str2 = obj[propertyName1];

// なお、string literal typesを使っても怒られます
let propertyName2: "str" = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
// let str3 = obj[propertyName2];

export {str,num, propertyName1, propertyName2}
