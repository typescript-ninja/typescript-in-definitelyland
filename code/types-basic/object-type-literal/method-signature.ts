var obj: {
    hello(word: string): string;
};

// 当てはまる値はこんな感じ
obj = {
    hello(word: string) {
        return "Hello, " + word;
    }
};
obj = {
    hello: (word: string) => "Hello, " + word
};
obj = {
    hello: function (word: string) {
        return "Hello, " + word;
    }
};

// プロパティシグニチャ + 関数型 の別の書き方なだけだな！
var obj2: {
    hello: (word: string) => string;
};
obj = obj2;
obj2 = obj;
