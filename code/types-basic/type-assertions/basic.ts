var obj: any = 1;

// お前は今後 number として生きよ…
var num = <number>obj;

var str = "string";
// 非常に外道なコードを書くことができる… 人としての良識を疑う
// やめて！私をanyにしてnumberに打ち込む気なんでしょう！？anyみたいに！
num = <any>str;
