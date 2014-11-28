var array: number[] | Date;

if(array instanceof Array) {
    // Array.prototype の型は Array<any> つまりは any[] …！
    // any[] は number[] に代入可能だな！！型を狭められたに違いない！
    // error TS2339: Property 'length' does not exist on type 'number[] | Date'.
    // (つд⊂)ｺﾞｼｺﾞｼ (；ﾟ Дﾟ) !?
    array.length;
}
