class Sample {
    str: string;
}

// 構造的部分型！
var obj: Sample | Date = {
    str: "Hi!"
};

if (obj instanceof Sample) {
    // 型は Sample に絞られている が！ 絶対に到達しない！
    // 現在の obj はプロトタイプチェーンにSampleを持たない！
    console.log(obj.str);
}
