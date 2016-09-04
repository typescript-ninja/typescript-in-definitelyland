let obj: {
	hello(word: string): string;
};

// 当てはまる値はこんな感じ
obj = {
	hello(word: string) {
		return `Hello, ${word}`;
	},
};
obj = {
	hello: (word: string) => `Hello, ${word}`,
};
obj = {
	hello: function(word: string) {
		return `Hello, ${word}`;
	},
};

// プロパティシグニチャ + 関数型リテラル と実質同じ意味
let obj2: {
	hello: (word: string) => string;
};
obj2 = obj;
obj = obj2;

export { }
