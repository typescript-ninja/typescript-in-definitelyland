let obj: {
	(word: string): string;
};
// 当てはまる値はこんな感じ
obj = word => `Hello, ${word}`;
obj = (word: string): string => {
	return `Hello, ${word}`;
};
obj = function(word: string): string {
	return `Hello, ${word}`;
};
// 呼び出してみよう！
let str = obj("TypeScript");
console.log(str);

export { }
