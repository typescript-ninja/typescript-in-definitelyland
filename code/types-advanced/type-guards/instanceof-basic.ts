class A {
    str: string;
}
class B {
    num: number;
}

var obj: A | B;
if (obj instanceof  A) {
    // ここでは A と確定されている！
    obj.str;
}else if(obj instanceof B) {
    // ここでは B と確定されている！
    obj.num;
}
