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
let obj: Base = new InheritA();

// objの中身はInheritAだ！人間はそれを知っているので無理やりダウンキャスト
(obj as InheritA).num;

// なお、instanceofで調べると勝手に対象の型にダウンキャストされる（型のnarrowing）
// もちろん、キャストするよりこっちのやり方のほうが安全
if (obj instanceof InheritA) {
	obj.num;
} else if (obj instanceof InheritB) {
	obj.bool;
} else {
	obj.str;
}

export { }
