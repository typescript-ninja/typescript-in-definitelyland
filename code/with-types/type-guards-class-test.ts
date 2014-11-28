var Sample: SampleStatic;
interface SampleStatic {
    new (): SampleInstance;
    prototype: SampleInstance;
}
interface SampleInstance {
    str: string;
}
class Test {
    num: number;
}

var obj: SampleInstance | Test;
if (obj instanceof Sample) {
    obj.str;
} else {
    obj.num;
}
