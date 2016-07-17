class Sample {
	str: string;
}

// 構造的部分型！
var obj: Sample = {
	str: "Hi!"
};

// 独自にSample型である事の判定を実装する
function isSample(obj: Sample): obj is Sample {
	if (!obj) {
		return false;
	}
	// とりあえず、strプロパティがあって値がstringならSample型コンパチということでOK という基準にする	
	return typeof obj.str === "string";
}

if (isSample(obj)) {
	console.log(obj.str);
}


export { }
