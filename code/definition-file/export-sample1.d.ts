declare module "foo" {
    interface Foo {
        num: number;
    }

    // この _ は外部からは参照できない。exportしてないので。
    var _:Foo;
    export = _;
}
