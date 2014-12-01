var clazz: {
    new (): any;
};

// 当てはまる値はこんな感じ
class Sample {
}
clazz = Sample;
var obj = new clazz();

function Hello(){
}
// 返り値が void な関数は new できるけど…
new Hello();
// 型としての互換性はない
// error TS2322: Type '() => void' is not assignable to type 'new () => any'.
// clazz = Hello;
