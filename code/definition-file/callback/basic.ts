// 良い例
declare function readFile(filePath: string, listener: (data: string) => void): void;
// 悪い例
declare function readFileOpt(filePath: string, listener: (data?: string) => void): void;

// 使ってみよう！
readFile("./test.txt", data => {
	// ここでのdataは必ず実体がある
	console.log(data.toUpperCase());
});
readFileOpt("./test.txt", data => {
	// ここでのdataはundefinedかもしれない… チェックしなければダメ
	if (!data) {
		data = "not found";
	}
	console.log(data.toUpperCase());
});

// 引数を無視するのは自由 optionalにする理由にはならない
readFile("./test.txt", () => {
	console.log("done");
});
readFileOpt("./test.txt", () => {
	console.log("done");
});
