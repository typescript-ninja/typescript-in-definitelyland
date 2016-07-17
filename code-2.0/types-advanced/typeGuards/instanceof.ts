class Base {
}

class InheritA extends Base {
}
class InheritB extends Base {
}

let obj = new InheritA();

// trueと表示される
console.log(obj instanceof Base);
// trueと表示される
console.log(obj instanceof InheritA);
// falseと表示される
console.log(obj instanceof InheritB);

// 無理矢理親を差し替える！
InheritA.prototype = new InheritB();
obj = new InheritA();
// trueと表示される
console.log(obj instanceof InheritB);

export { }
