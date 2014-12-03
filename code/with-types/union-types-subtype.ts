class Base {
    str: string;
}
class Inherit extends Base {
    num: number;
}

// 明示的に指定しとくと Base | Inherit となる(今のところ
var obj: Base | Inherit;

// Base にも Inherit にも存在する要素には普通にアクセスできる
obj.str;
// num は Base には存在しないのでアウトー！
// error TS2339: Property 'num' does not exist on type 'Base | Inherit'.
// obj.num;

// Base | Inherit は実質単に Base なのでそのまま Base に丸められる
var base: typeof obj;
// 型が Base でも子クラスだから普通に代入できるので無問題
base = new Inherit();
