// 要素が多い分にはOKだ！
let tuple: [string, number] = ["str", 1, "test"];

// 範囲外の要素の型はすべての要素のunion、つまり string | number になる。
let value = tuple[2];

// 以下はダメ。true は string | number ではないため。
// tuple = ["str", 1, true];
