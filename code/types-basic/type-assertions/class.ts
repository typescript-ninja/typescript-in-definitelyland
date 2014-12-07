class Base {
    str: string;
}

class InheritA extends Base {
    num: number;
}
class InheritB extends Base {
    bool: boolean;
}

// とりあえず型は親クラスとして…
var obj: Base;

// 中身が何か調べてから安全にダウンキャストする
if (obj instanceof InheritA) {
    var a = <InheritA>obj;
    a.num;
} else if(obj instanceof InheritB) {
    var b = <InheritB>obj;
    b.bool;
}
