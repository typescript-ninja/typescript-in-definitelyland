// "type" Identifier "=" Type ";" という文法だからダメ
// Identifier は TypeParameters を含まない
// type-alias-with-type-parameters-invalid.ts(4,9): error TS1005: '=' expected.
type Foo<T> = Array<T> | T;

// こういうのは書けないんだ…すまんな…
var obj: Foo<number>;
obj = [1, 2];
obj = 1;
