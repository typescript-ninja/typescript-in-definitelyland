// 引数無しの返り値stringな関数 な型注釈
let func: () => string;

// 素直に考えるとこう書けてもいいっしょ！でもダメ！
// let a: () => string | () => boolean;

// 型名をカッコでくくる必要がある。これならOK
let b: (() => string) | (() => boolean);
// もしくはオブジェクト型リテラル使う
let c: { (): string; } | { (): boolean; };

// union typesじゃないときでも使えるけど見づらいな！
let d: (() => string);

export {func, b, c, d}
