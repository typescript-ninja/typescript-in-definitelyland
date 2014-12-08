class Sample {
	str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
	str: "Hi!"
};

if (obj !== null) {
	var str = (<Sample>obj).str;
	if (typeof str === "string") {
		console.log(str);
	}
}
