class Base {
}

class InheritA extends Base {
}
class InheritB extends Base {
}

var obj = new InheritA();

// true と表示される
console.log(obj instanceof Base);
// true と表示される
console.log(obj instanceof InheritA);
// false と表示される
console.log(obj instanceof InheritB);

// ｵﾗｧﾝ!!! 無理矢理親を差し替える！
InheritA.prototype = new InheritB();
obj = new InheritA();
// true と表示される
console.log(obj instanceof InheritB);
