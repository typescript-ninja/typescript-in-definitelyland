// OK！変数の型に対して、過不足なし
let obj: { name: string; } = {
	name: "TypeScript",
};

// NG… 変数の型に対してプロパティが多すぎる
// error TS2322: Type '{ name: string; version: string; }'
//    is not assignable to type '{ name: string; }'.
//    Object literal may only specify known properties,
//    and 'version' does not exist in type '{ name: string; }'.
obj = {
	name: "JavaScript",
	version: "2016",
};

// オブジェクトリテラルの直接代入じゃなければOK 互換性はあるのだ
let tmp = {
	name: "JavaScript",
	version: "2016",
};
obj = tmp;

// この制約はオプション名のtypoの検出に役立つ
interface FooOptions {
	fileName?: string;
	checkBar?: boolean;
}
declare function foo(opts: FooOptions): void;

// fileName の大文字小文字を間違えている！
// Object literal may only specify known properties,
// and 'filename' does not exist in type 'FooOptions'.
foo({
	filename: "vvakame.txt",
	checkBar: true,
});

export { }
