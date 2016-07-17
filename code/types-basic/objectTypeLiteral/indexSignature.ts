let objA: {
	[index: number]: string;
} = {};
let objB: {
	[index: string]: string;
} = {};

// どういった使い方ができるの？
let s1 = objA[1];
// --noImplicitAny 付きだとちゃんとエラーになる
// error TS7015: Element implicitly has an 'any' type because index expression is not of type 'number'.
// var s2 = objA["test"];

// インデックスの型指定が string の場合 string でも number でもOK
let s3 = objB[1];
let s4 = objB["test"];

// 当てはまる値はこんな感じ
objA = {
	0: "str",
	// オブジェクトリテラルで直接変数に代入する場合、変数の型に存在しない値があるとエラーになる
	// error TS2322: Type
	//  '{ 0: string; str: string; }'
	//     is not assignable to type '{ [index: number]: string; }'.
	//  Object literal may only specify known properties,
	//     and 'str' does not exist in type '{ [index: number]: string; }'.
	// str: "str",
};
// 変数にオブジェクトリテラル直で代入でなければ余計なパラメータがついていても許される
let tmp = {
	0: "str",
	str: "str",
};
objA = tmp;

objB = {
	0: "str",
	str: "str",
	// インデックスの型が string の場合、すべてのプロパティの型がインデックスシグニチャに
	// 反しないようにしなければならない
	// error TS2322: Type
	//  '{ 0: string; str: string; num: number; }'
	//     is not assignable to type
	//  '{ [index: string]: string; }'.
	//  Property 'num' is incompatible with index signature.
	//    Type 'number' is not assignable to type 'string'.
	// num: 1,
};

export { s1, s3, s4 }
