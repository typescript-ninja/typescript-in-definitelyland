declare class BaseA {
    str: string;
}
// ○利用時に普通に継承できる！
class Inherit extends BaseA {
    number: number;
}
// ☓ クラスはopen endedじゃないので定義の拡張ができない…
// error TS2300: Duplicate identifier 'BaseA'.
declare class BaseA {
    num: number;
}

interface FooListener {
    on(eventName: string, callback: (data: any) => void): void;
}
declare class FooListenerImpl implements FooListener {
    // インタフェースの要素全部書かないとコンパイル通らない…
    on(eventName: string, callback: (data: any) => void): void;
}
