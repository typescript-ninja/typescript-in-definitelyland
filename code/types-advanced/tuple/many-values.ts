// 要素が多い分にはOKだ！
var tuple: [string, number] = ["str", 1, "test"];

// 1.3.0 では型指定されていない要素は BCT（Best Common Type) つまりここでは {} になる
// 1.4.0? では範囲外の要素の型はすべての要素のunion、つまり string | number になる。
var value = tuple[2];

// 1.3.0 では次の記述は正しい
// 1.4.0? ではダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
