declare module "foo" {
    interface Foo {
        num: number;
    }

    // よくやりがちな過ち
    export = Foo;
}
