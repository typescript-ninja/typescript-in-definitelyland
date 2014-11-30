var objA: {
    [index:number]: string;
};
var objB: {
    [index:string]: string;
};

// どういった使い方ができるの？
var s1 = objA[1];
// --noImplicitAny 付きだとちゃんとエラーになる
// error TS7017: Index signature of object type implicitly has an 'any' type.
// var s2 = objA["test"];

// インデックスの型指定が string の場合 string でも number でもOK
var s3 = objB[1];
var s4 = objB["test"];

// 当てはまる値はこんな感じ
objA = {
    0: "str",
    // インデックスの型が number の場合、数値以外のプロパティがインデックスシグニチャで取れる値に反していてもよい
    num: 1
};
objB = {
    0: "str",
    str: "str"
    // インデックスの型が string の場合、全てのプロパティの型がインデックスシグニチャに反しないようにしなければならない
    // error TS2322: Type '{ [x: string]: string | number; 0: string; str: string; num: number; }' is not assignable to type '{ [x: string]: string; }'.
    //     Index signatures are incompatible.
    //         Type 'string | number' is not assignable to type 'string'.
    //     Type 'number' is not assignable to type 'string'.
    // num: 1
};
