type FooReturns = string | number | boolean;

interface Foo {
    bar(): FooReturns;
    buzz(): FooReturns;
    barbuzz(): FooReturns;
}
