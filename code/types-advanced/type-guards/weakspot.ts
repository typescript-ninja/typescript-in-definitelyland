class Sample {
	str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
	str: "Hi!"
};

if (obj instanceof Sample) {
	// 型はSampleに絞られている が！絶対に到達しない！
	// 現在のobjはプロトタイプチェーンにSampleを持たない！
	console.log(obj.str);
}
