interface AStatic {
    new (): AInstance;
    prototype: AInstance;
}
interface AInstance {
    str: string;
}
declare var A: AStatic;

var obj: AInstance | {};
if (obj instanceof A) {
    // ここでは AInstance と確定されている！
    obj.str;
}
