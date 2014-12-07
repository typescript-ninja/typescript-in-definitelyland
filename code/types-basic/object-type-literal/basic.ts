// オブジェクトリテラルで値を作成！
var objA = {
    x: 1,
    y: 2
};

// オブジェクト型リテラルで型を作成！(値は無し
// 上記の objA の型は型推論で objB と同一になる
var objB: {
    x: number;
    y: number;
};

// おんなじ！
objA = objB;
objB = objA;
