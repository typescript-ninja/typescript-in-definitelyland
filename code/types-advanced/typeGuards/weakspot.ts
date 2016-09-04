class Sample {
	str: string;
}

// 構造的部分型！
let obj: Sample = {
	str: "Hi!",
};

if (obj instanceof Sample) {
	// 型はSampleに絞られている しかし、絶対に到達しない
	// 現在のobjはSampleを親に持たない
	console.log(obj.str);
}

export { }
