// 名前付き関数は定義できないので変数に入れる
var funcA = () => {
};
funcA();

// 以下の2つは等価
// アロー関数式は1ステートメントだけならカッコを省略して値をそのまま返せる
var funcB = (word = "TypeScript") => "Hello, " + word;
var funcC = (word = "TypeScript") => {
    return "Hello, " + word;
};
console.log(funcB());
console.log(funcC());
