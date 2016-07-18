class Sample {
	str: string;
}

// 構造的部分型！
let obj: Sample = {
	str: "Hi!",
};

// 独自にSample型である事の判定を実装する
function isSample(s: Sample): s is Sample {
	if (!s) {
		return false;
	}
	// とりあえず、strプロパティがあって値がstringならSample型コンパチということでOK という基準にする	
	return typeof s.str === "string";
}

if (isSample(obj)) {
	console.log(obj.str);
}


export { }
