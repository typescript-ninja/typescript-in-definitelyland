// 以下2つは(thisが絡まない限り)等価！
var funcA = () => true;
var funcB = function () {
    return true;
};

// NOTE ここのcallbackの型注釈の意味は別の章で解説！
// 引数を1つ取って返り値無し の関数を表します。
function asyncModoki(callback:(value:string)=>void) {
    callback("TypeScript");
}
// 旧来の書き方
asyncModoki(function (value:string) {
    console.log("Hello, " + value);
});
// アロー関数式だと楽やで
asyncModoki(value => console.log("Hello, " + value));
