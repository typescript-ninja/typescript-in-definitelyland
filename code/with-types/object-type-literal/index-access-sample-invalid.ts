var obj = {
    str: "string",
    num: 1
};

// 文字列リテラルによるアクセスだと普通に . アクセス同様に型情報が得られる
var str: string = obj["str"];
var num: number = obj["num"];
// ちなみに、存在しない要素にアクセスすると any になる
// --noImplicitAny を使うと暗黙的anyとしてちゃんと怒られる 萌え
// error TS7017: Index signature of object type implicitly has an 'any' type.
var any = obj["notExists"];

// 即値じゃないとダメ！コンパイラが処理できないのです
var propertyName = "str";
// error TS7017: Index signature of object type implicitly has an 'any' type.
var str2 = obj[propertyName];
