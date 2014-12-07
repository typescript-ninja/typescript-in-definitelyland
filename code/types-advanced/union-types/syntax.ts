// 引数無しの返り値stringな関数 な型注釈
var func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// var a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
var b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
var c: { (): string; } | { (): boolean; };

// union typesじゃない時でも使えるけど見づらいな！
var d: (() => string);
