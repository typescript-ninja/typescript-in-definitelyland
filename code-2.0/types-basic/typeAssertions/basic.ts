let obj: any = 1;

// お前は今後 number として生きよ…
let num = <number>obj;

let str = "string";
// 非常に外道なコードを書くことができる… 人としての良識を疑う
// stringをnumberな型の変数に代入するだなんてなんて卑劣な…！
num = <any>str;
// 後置のキャストもあります as 型名という形式
num = str as any;

export {}
