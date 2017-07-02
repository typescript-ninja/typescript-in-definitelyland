let obj: any = 1;

// お前は今後 number として生きよ…
let num = obj as number;

let str = "string";
// 非常に外道なコードを書くことができる… 人としての良識を疑う
// stringをnumberな型の変数に代入するだなんてなんて卑劣な…！
num = str as any;

// 前置で書くやり方もあるよ
num = <any>str;

export { }
