var func: (value: string) => string;
// 当てはまる値はこんな感じ
func = word => "Hello, " + word;
func = (word: string) => {
	return "Hello, " + word;
};
func = function(word: string) {
	return "Hello, " + word;
};

// 型に対して実装の引数の数が少ないのはOK
func = () => "Hello, TypeScript";

// 型に対して実装の引数の数が多い場合、省略可能かデフォルト値付きでなければならない
func = (v1: string, v2 = "JavaScript") => "Hello, " + v1 + " & " + v2;
