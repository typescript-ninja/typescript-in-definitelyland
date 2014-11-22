var BaseA: BaseAStatic;
interface BaseAStatic {
    new (): BaseA;
}
interface BaseA {
    str: string;
}
// ☓ 利用時に継承できない…(しょせんインタフェース
// error TS2311: A class may only extend another class.
class Inherit extends BaseA {
    number: number;
}
// ○ インタフェースは定義の拡張ができる！！
interface BaseA {
    num: number;
}

interface FooListener {
    on(eventName: string, callback: (data: any) => void): void;
}
var FooListenerImpl: FooListenerImplStatic;
interface FooListenerImplStatic {
    new (): FooListenerImpl;
}
interface FooListenerImpl extends FooListener {
    // FooListenerの要素何も転記しなくて済む！
}
