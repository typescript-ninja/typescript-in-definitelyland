interface FooOptions {
	fileName?: string;
	checkBar?: boolean;
}

// 一旦別の変数を経由しても…
let obj = {
    unknownOption: 1,
};

// 共通のプロパティが1つも存在しない場合エラーにしてもらえる
// error TS2559: Type '{ unknownOption: number; }' has 
//   no properties in common with type 'FooOptions'.
let fooOpts: FooOptions = obj;
