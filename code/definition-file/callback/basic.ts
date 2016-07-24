// 良い例
declare function readFileA(
	filePath: string,
	listener: (data: string) => void): void;
// 悪い例
declare function readFileB(
	filePath: string,
	listener: (data?: string) => void): void;

// 使ってみよう！
readFileA("./test.txt", data => {
	// ここでのdataは必ず実体がある
	console.log(data.toUpperCase());
});
readFileB("./test.txt", data => {
	// ここでのdataはundefinedかもしれない… チェックしなければダメ
	if (!data) {
		data = "not found";
	}
	console.log(data.toUpperCase());
});

// 引数を無視するのは自由 optionalにする理由にはならない
readFileA("./test.txt", () => {
	console.log("done");
});
readFileB("./test.txt", () => {
	console.log("done");
});
