// 良い例
declare function onClick(listener:(e:MouseEvent) => void):void;
// 悪い例
declare function onClickOpt(listener:(e?:MouseEvent) => void):void;

// 使ってみよう！
onClick(e => {
    // ここでのeは必ず実体がある
    console.log("clicked!" + e.srcElement.textContent);
});
onClickOpt(e => {
    // ここでのeはundefinedかもしれない… チェックしなくていいの？
    console.log("clicked!" + e.srcElement.textContent);
});

// 引数を無視するのは自由 optionalにする理由にはならぬ！
onClick(()=> {
    console.log("clicked!");
});
onClickOpt(()=> {
    console.log("clicked!");
});
