interface Foo {
    bar: FooBar;
}

interface FooBar {
    buzz: FooBarBuzz;
}

interface FooBarBuzz {
    str: string;
}

declare var foo: Foo;

// foo.bar.buzz.str という使い方ができる。わかりにくくてユーザは死ぬ。
