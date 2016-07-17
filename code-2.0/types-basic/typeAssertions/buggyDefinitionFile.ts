// こういう、型定義があるとする。実装は他のライブラリなどが与える。
declare class Base {
	str: string;
}

let obj = new Base();
// 本当は、Baseクラスがnumプロパティも持ってるんだけど…
// 型定義が不足しているので10秒で解決するにはanyで誤魔化すしかない
// キレイごとだけじゃ世の中生きていけないんじゃよ…
// でも、本当は型定義のほうを修正するのが正しいです
let num: number = (obj as any).num;

export {num}
