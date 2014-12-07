// こういう、型定義があるとする。実装はJavaScriptが与える。
declare class Base {
    str: string;
}

var obj = new Base();
// 本当は、Baseクラスがnumプロパティも持ってるんだけど…
// 型定義が不足しているなら、anyで誤魔化せばいいじゃない！！
// キレイ事だけじゃ世の中生きていけないんじゃよ…
var num: number = (<any>obj).num;
